/*
* Copyright (c) 2003-2011 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#include <iomanip>
#include <sstream>
#include "stdafx.h"
#include "PKBaseDlg.h"
#include "Fonts.h"
#include "resource.h"
#include "os/env.h"

#include "core/pwsprefs.h"
#include "core/core.h" // for IDSC_UNKNOWN_ERROR
#include "VirtualKeyboard/VKeyBoardDlg.h"

using namespace std;

const wchar_t CPKBaseDlg::PSSWDCHAR = L'*';
bool CPKBaseDlg::s_yubiDetected = false;

CPKBaseDlg::CPKBaseDlg(int id, CWnd *pParent)
: CPWDialog(id, pParent), m_passkey(L""), m_pctlPasskey(new CSecEditExtn),
  m_pVKeyBoardDlg(NULL), m_pending(false)
{
  if (pws_os::getenv("PWS_PW_MODE", false) == L"NORMAL")
    m_pctlPasskey->SetSecure(false);
  m_present = !IsYubiInserted(); // lie to trigger correct actions in timer event
}

CPKBaseDlg::~CPKBaseDlg()
{
  delete m_pctlPasskey;
  if (m_pVKeyBoardDlg != NULL && m_pVKeyBoardDlg->SaveKLID()) {
    // Save Last Used Keyboard
    UINT uiKLID = m_pVKeyBoardDlg->GetKLID();
    std::wostringstream os;
    os.fill(L'0');
    os << std::nouppercase << std::hex << std::setw(8) << uiKLID;
    StringX cs_KLID = os.str().c_str();
    PWSprefs::GetInstance()->SetPref(PWSprefs::LastUsedKeyboard, cs_KLID);

    m_pVKeyBoardDlg->DestroyWindow();
    delete m_pVKeyBoardDlg;
  }
}

void CPKBaseDlg::OnDestroy()
{
  CPWDialog::OnDestroy();
}

void CPKBaseDlg::DoDataExchange(CDataExchange* pDX)
{
  CPWDialog::DoDataExchange(pDX);

  // Can't use DDX_Text for CSecEditExtn
  m_pctlPasskey->DoDDX(pDX, m_passkey);
  DDX_Control(pDX, IDC_PASSKEY, *m_pctlPasskey);
  DDX_Control(pDX, IDC_YUBI_PROGRESS, m_yubi_timeout);
  DDX_Control(pDX, IDC_YUBI_STATUS, m_yubi_status);
}

bool CPKBaseDlg::IsYubiInserted() const
{
  if (m_pending)
    return true; // can't check in the middle of a request
  else {
    CSingleLock singeLock(&m_mutex);
    singeLock.Lock();
    return (m_yk.enumPorts() == 1);
  }
}

BOOL CPKBaseDlg::OnInitDialog(void)
{
  CPWDialog::OnInitDialog();
  // Setup a timer to poll the key every 250 ms
  SetTimer(1, 250, 0);
  Fonts::GetInstance()->ApplyPasswordFont(GetDlgItem(IDC_PASSKEY));

  m_pctlPasskey->SetPasswordChar(PSSWDCHAR);

  m_yubiLogo.LoadBitmap(IDB_YUBI_LOGO);
  m_yubiLogoDisabled.LoadBitmap(IDB_YUBI_LOGO_DIS);
  CWnd *ybn = GetDlgItem(IDC_YUBIKEY_BTN);

  if (YubiExists()) {
    ybn->ShowWindow(SW_SHOW);
    m_yubi_status.ShowWindow(SW_SHOW);
  } else {
    ybn->ShowWindow(SW_HIDE);
    m_yubi_status.ShowWindow(SW_HIDE);
  }
  m_yubi_timeout.ShowWindow(SW_HIDE);
  m_yubi_timeout.SetRange(0, 15);
  bool yubiInserted = IsYubiInserted();
  // MFC has ancient bug: can't render disabled version of bitmap,
  // so instead of showing drek, we roll our own, and leave enabled.
  ybn->EnableWindow(TRUE);

  if (yubiInserted) {
    ((CButton*)ybn)->SetBitmap(m_yubiLogo);
    m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_CLICK_PROMPT)));
  } else {
    ((CButton*)ybn)->SetBitmap(m_yubiLogoDisabled);
    m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_INSERT_PROMPT)));
  }
  return TRUE;
}


void CPKBaseDlg::yubiInserted(void)
{
  CButton *ybn = (CButton*)GetDlgItem(IDC_YUBIKEY_BTN);
  ybn->SetBitmap(m_yubiLogo);
  ybn->ShowWindow(SW_SHOW);
  m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_CLICK_PROMPT)));
  m_yubi_status.ShowWindow(SW_SHOW);
}

void CPKBaseDlg::yubiRemoved(void)
{
  ((CButton*)GetDlgItem(IDC_YUBIKEY_BTN))->SetBitmap(m_yubiLogoDisabled);
  m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_INSERT_PROMPT)));
}

static StringX Bin2Hex(const unsigned char *buf, int len)
{
  wostringstream os;
  os << setw(2);
  os << setfill(L'0');
  for (int i = 0; i < len; i++) {
    os << hex << setw(2) << int(buf[i]);
  }
  return StringX(os.str().c_str());
}

void CPKBaseDlg::yubiCheckCompleted()
{
  // We now wait for a response with the HMAC-SHA1 digest
  BYTE respBuf[SHA1_DIGEST_SIZE];
  unsigned short timer;
  CSingleLock singeLock(&m_mutex);
  singeLock.Lock();
  YKLIB_RC rc = m_yk.waitForCompletion(YKLIB_NO_WAIT,
                                       respBuf, sizeof(respBuf), &timer);
  switch (rc) {
  case YKLIB_OK:
    m_yubi_status.ShowWindow(SW_SHOW);
    m_yubi_timeout.ShowWindow(SW_HIDE);
    m_yubi_timeout.SetPos(0);
    m_yubi_status.SetWindowText(_T(""));
    TRACE(_T("yubiCheckCompleted: YKLIB_OK"));
    m_pending = false;
    m_yk.closeKey();
    m_passkey = Bin2Hex(respBuf, SHA1_DIGEST_SIZE);
    // The returned hash is the passkey
    ProcessPhrase();
    // If we returned from above, reset status:
    m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_CLICK_PROMPT)));
    break;
  case YKLIB_PROCESSING:  // Still processing or waiting for the result
    break;
  case YKLIB_TIMER_WAIT:  // A given number of seconds remain 
    m_yubi_timeout.SetPos(timer);
    break;

  case YKLIB_INVALID_RESPONSE:  // Invalid or no response
    m_pending = false;
    m_yubi_timeout.ShowWindow(SW_HIDE);
    m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDS_YUBI_TIMEOUT)));
    m_yubi_status.ShowWindow(SW_SHOW);
    m_yk.closeKey();
    YubiFailed(); // allow subclass to do something useful
    break;

  default:                // A non-recoverable error has occured
    m_pending = false;
    m_yubi_timeout.ShowWindow(SW_HIDE);
    m_yubi_status.ShowWindow(SW_SHOW);
    m_yk.closeKey();
    // Generic error message
    TRACE(_T("yubiCompleted(%d)\n"), rc);
    m_yubi_status.SetWindowText(CString(MAKEINTRESOURCE(IDSC_UNKNOWN_ERROR)));
    break;
  }
}

void CPKBaseDlg::yubiRequestHMACSha1()
{
  if (m_pending) {
    // no-op if a request's already in the air
  } else {
    CSingleLock singeLock(&m_mutex);
    singeLock.Lock();
    // open key
    // if zero or >1 key, we'll fail
    if (m_yk.openKey() != YKLIB_OK) {
      return;
    }

    // Prepare the HMAC-SHA1 challenge here

    BYTE chalBuf[SHA1_MAX_BLOCK_SIZE];
    BYTE chalLength = BYTE(m_passkey.GetLength()*sizeof(TCHAR));
    memset(chalBuf, 0, SHA1_MAX_BLOCK_SIZE);
    if (chalLength > SHA1_MAX_BLOCK_SIZE)
      chalLength = SHA1_MAX_BLOCK_SIZE;

    memcpy(chalBuf, m_passkey, chalLength);

    // Initiate HMAC-SHA1 operation now

    if (m_yk.writeChallengeBegin(YKLIB_SECOND_SLOT, YKLIB_CHAL_HMAC,
                                 chalBuf, chalLength) != YKLIB_OK) {
      TRACE(_T("m_yk.writeChallengeBegin() failed"));
      return;
    }
    // request's in the air, setup GUI to wait for reply
    m_pending = true;
    m_yubi_status.ShowWindow(SW_HIDE);
    m_yubi_status.SetWindowText(_T(""));
    m_yubi_timeout.ShowWindow(SW_SHOW);
    m_yubi_timeout.SetPos(15);
  }
}

void CPKBaseDlg::OnTimer(UINT_PTR )
{
  // If an operation is pending, check if it has completed

  if (m_pending) {
    yubiCheckCompleted();
  } else {
    // No HMAC operation is pending - check if one and only one key is present
    bool inserted = IsYubiInserted();
    // call relevant callback if something's changed
    if (inserted != m_present) {
      m_present = inserted;
      if (m_present) {
        SetYubiExists(); // proof that user has a yubikey!
        yubiInserted();
      } else
        yubiRemoved();
    }
  }
}
