/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

// FontsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "resource3.h"
#include "ThisMfcApp.h"
#include "DboxMain.h"
#include "Fonts.h"
#include "FontsDialog.h"
#include "SampleTextDlg.h"
#include "dlgs.h"

extern const wchar_t *EYE_CATCHER;

#ifndef _WIN32_WCE // CFontDialog is not supported for Windows CE.

// CFontsDialog

IMPLEMENT_DYNAMIC(CFontsDialog, CFontDialog)

static UINT_PTR CALLBACK CFHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static CFontsDialog *pwfd_self(NULL);

CFontsDialog::CFontsDialog(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter,
                               CWnd* pParentWnd, int iType)
  : CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd), m_iType(iType)
{
  m_cf.Flags |= CF_ENABLETEMPLATE | CF_ENABLEHOOK | CF_APPLY;
  m_cf.Flags &= ~(CF_EFFECTS | CF_SHOWHELP);
  m_cf.hInstance = AfxGetResourceHandle();
  m_cf.lpTemplateName = MAKEINTRESOURCE(IDD_PWFONTDIALOG);
  m_cf.hwndOwner = m_hWnd;
  m_cf.lpfnHook = CFHookProc;

  pwfd_self = this;
  m_sampletext.LoadString(IDS_SAMPLETEXT);
  UINT uiID(0);
  switch (iType) {
    case PWFONT:
      uiID = IDS_PSWDFONT;
      break;
    case TLFONT:
      uiID = IDS_TREEFONT;
      break;
    case VKFONT:
      uiID = IDS_VKBDFONT;
      // Set up default font, which is NONE
      memcpy(&m_dfltVKBDFont, lplfInitial, sizeof(LOGFONT));
      SecureZeroMemory(m_dfltVKBDFont.lfFaceName, sizeof(m_dfltVKBDFont.lfFaceName));
      break;
    default:
      ASSERT(0);
  }
  m_title.LoadString(uiID);
  m_bReset = false;
}

CFontsDialog::~CFontsDialog()
{
  pwfd_self = NULL;
}

BEGIN_MESSAGE_MAP(CFontsDialog, CFontDialog)
END_MESSAGE_MAP()

// CFontsDialog message handlers

static UINT_PTR CALLBACK CFHookProc(HWND hdlg, UINT uiMsg, 
                                    WPARAM wParam, LPARAM /* lParam */)
{
  // lParam = m_cf.lCustData when uiMsg == WM_INITDIALOG
  // but we don't need it as 'pwfd_self' must be set before calling
  // since we cannot be sure WM_INITDIALOG is the first call.
  if (uiMsg == WM_INITDIALOG) {
    ASSERT(pwfd_self);

    ::SetWindowText(hdlg, pwfd_self->m_title);

    if (pwfd_self->m_iType == VKFONT) {
      // Disable things we don't allow changed
      EnableWindow(GetDlgItem(hdlg, cmb2), FALSE); // style
      EnableWindow(GetDlgItem(hdlg, stc2), FALSE); // style
#if 0
      EnableWindow(GetDlgItem(hdlg, cmb3), FALSE); // size
      EnableWindow(GetDlgItem(hdlg, stc3), FALSE); // size
      // Make Edit in Size ComboBox R-O
      SendMessage(GetDlgItem(GetDlgItem(hdlg, cmb3), 1001), 
                     EM_SETREADONLY, TRUE, 0);
#endif
      EnableWindow(GetDlgItem(hdlg, cmb5), FALSE); // script
      ShowWindow(GetDlgItem(hdlg, cmb5), SW_HIDE); // script
      ShowWindow(GetDlgItem(hdlg, stc7), SW_HIDE); // script
    }
    return TRUE;
  }
  if (uiMsg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED) {
    if (LOWORD(wParam) == IDC_SETSAMPLETEXT) {
      CSampleTextDlg stDlg(NULL, pwfd_self->m_sampletext);

      INT_PTR rc = stDlg.DoModal();

      if (rc == IDOK) {
        pwfd_self->m_sampletext = stDlg.m_sampletext;
        ::SetDlgItemText(hdlg, stc5, pwfd_self->m_sampletext);
        ::InvalidateRect(hdlg, NULL, TRUE);
      }
      return TRUE;  // We processed message
    }
    if (LOWORD(wParam) == IDC_RESETFONT) {
      if (pwfd_self->m_iType == VKFONT) {
        pwfd_self->m_bReset = true;
        pwfd_self->PostMessage(WM_COMMAND, IDABORT, 0);
        return TRUE;  // We processed message
      }
      LOGFONT dfltFont;
      wchar_t wc_pt[4] = {0, 0, 0, 0};
      // Due to a documentation bug in WM_CHOOSEFONT_SETLOGFONT - instead of just this:
      //   pwfd_self->SendMessage(WM_CHOOSEFONT_SETLOGFONT, 0, (LPARAM)&dfltFont);
      // Need to do:
      switch (pwfd_self->m_iType) {
        case PWFONT:
          Fonts::GetInstance()->GetDefaultPasswordFont(dfltFont);
          break;
        case TLFONT:
          memcpy(&dfltFont, &dfltTreeListFont, sizeof(LOGFONT));
          break;
        default:
          ASSERT(0);
          return FALSE;
      }
      // First get point_size = (height - Internal Leading) * 72 / LOGPIXELSY
      // Assume "Internal Leading" == 0
      CClientDC dc((CWnd *)pwfd_self);
      int pt = -MulDiv(dfltFont.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
      _snwprintf_s(wc_pt, 4, L"%d", pt);
      SendMessage(GetDlgItem(hdlg, cmb1), CB_SELECTSTRING, (WPARAM)(-1),
                  (LPARAM)dfltFont.lfFaceName);
      SendMessage(GetDlgItem(hdlg, cmb3), CB_SELECTSTRING, (WPARAM)(-1),
                  (LPARAM)wc_pt);
      SendMessage(hdlg, WM_COMMAND, MAKEWPARAM(cmb3, CBN_SELCHANGE),
                  (LPARAM)GetDlgItem(hdlg, cmb3));
      pwfd_self->SendMessage(WM_CHOOSEFONT_SETLOGFONT, 0, (LPARAM)&dfltFont);
      return TRUE;  // We processed message
    }
  }

  // For some reason, we keep having to change the sample text!
  ::SetDlgItemText(hdlg, stc5, pwfd_self->m_sampletext);

  return FALSE; // We didn't process message
}

INT_PTR CFontsDialog::DoModal()
{
  bool bAccEn = app.IsAcceleratorEnabled();
  if (bAccEn)
    app.DisableAccelerator();

  CPWDialog::GetDialogTracker()->AddOpenDialog(this);
  INT_PTR rc = CFontDialog::DoModal();
  CPWDialog::GetDialogTracker()->RemoveOpenDialog(this);

  if (bAccEn)
    app.EnableAccelerator();

  return rc;
}

LRESULT CFontsDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  CWnd *pParent = GetParent();
  while (pParent != NULL) {
    DboxMain *pDbx = dynamic_cast<DboxMain *>(pParent);
    if (pDbx != NULL && pDbx->m_eye_catcher != NULL &&
        wcscmp(pDbx->m_eye_catcher, EYE_CATCHER) == 0) {
      pDbx->ResetIdleLockCounter(message);
      break;
    } else
      pParent = pParent->GetParent();
  }
  if (pParent == NULL)
    pws_os::Trace(L"CFontsDialog::WindowProc - couldn't find DboxMain ancestor\n");
  return CFontDialog::WindowProc(message, wParam, lParam);
}

#endif // !_WIN32_WCE
