/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

// PWResizeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PWResizeDialog.h"

// CPWResizeDialog dialog

IMPLEMENT_DYNAMIC(CPWResizeDialog, CPWDialog)

CPWResizeDialog::CPWResizeDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
  : CPWDialog(nIDTemplate, pParent),
  m_bInitDone(false), m_bStatusBarOK(false),
  m_DialogMaxHeight(-1), m_DialogMaxWidth(-1),
  m_iMainCtrl(0), m_numbtns(-1), m_numsbpanes(-1), m_pstatustext(NULL)
{
}

CPWResizeDialog::~CPWResizeDialog()
{
  delete m_pstatustext;
}

void CPWResizeDialog::AddBtnsCtrlIDs(std::vector<UINT> viBottomButtons, int iFocus)
{
  m_viBottomButtons = viBottomButtons;
  m_numbtns = (int)m_viBottomButtons.size();
  if (iFocus < 0 || iFocus > m_numbtns - 1)
    m_iFocus = 0;
  else
    m_iFocus = iFocus;
}

void CPWResizeDialog::SetStatusBar(const UINT *pstatustext, int nIDCount, bool bTextVisible)
{
  ASSERT(nIDCount > 0);  // Must be at least 1
  m_numsbpanes = nIDCount;
  m_pstatustext = new UINT[nIDCount];
  memcpy((void *)m_pstatustext, (void *)pstatustext, nIDCount * sizeof(UINT));
  m_bTextVisible = bTextVisible;

  return;
}

void CPWResizeDialog::SetMaxHeightWidth(int iMaxHeight, int iMaxWidth)
{
  m_DialogMaxHeight = iMaxHeight; 
  m_DialogMaxWidth = iMaxWidth;

  if (m_DialogMaxWidth < m_DialogMinWidth)
    m_DialogMaxWidth = m_DialogMinWidth;
  if (m_DialogMaxHeight < m_DialogMinHeight)
    m_DialogMaxHeight = m_DialogMinHeight;
}

BEGIN_MESSAGE_MAP(CPWResizeDialog, CPWDialog)
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// CPWResizeDialog message handlers

BOOL CPWResizeDialog::OnInitDialog()
{
  CPWDialog::OnInitDialog();

  // MUST add main control ID, button IDs and status bar info first
  // This verify is to warn developers to do this!  No impact
  // for users once set up!
  if (m_iMainCtrl == 0 || m_numbtns < 1 || m_numsbpanes < 0)
    VERIFY(0);

  // Add the status bar
  if (m_statusBar.CreateEx(this, SBARS_SIZEGRIP)) {
    m_statusBar.SetIndicators(m_pstatustext, m_numsbpanes);
    UINT style;
    if (m_bTextVisible)
      style = SBPS_STRETCH;
    else
      style = SBPS_DISABLED | SBPS_NOBORDERS;

    m_statusBar.SetPaneInfo(0, m_statusBar.GetItemID(0), style, NULL);
    m_statusBar.UpdateWindow();

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    m_bStatusBarOK = true;
  }

  m_pMainCtrl = GetDlgItem(m_iMainCtrl);

  // Arrange all the controls - needed for resizeable dialog
  CRect sbRect, mainCtrlRect, btnRect, dlgRect;
  int xleft, ytop;

  GetClientRect(&dlgRect);
  m_DialogMinWidth = dlgRect.Width();
  m_DialogMinHeight = dlgRect.Height();

  m_statusBar.GetWindowRect(&sbRect);
  ScreenToClient(&sbRect);

  m_pMainCtrl->GetWindowRect(&mainCtrlRect);
  ScreenToClient(&mainCtrlRect);

  GetDlgItem(m_viBottomButtons[0])->GetWindowRect(&btnRect);
  ScreenToClient(&btnRect);
  m_ybuttondiff = btnRect.top - mainCtrlRect.bottom;

  m_cxBSpace = dlgRect.Size().cx - mainCtrlRect.Size().cx;
  m_cyBSpace = dlgRect.Size().cy - mainCtrlRect.Size().cy;
  m_cySBar = sbRect.Size().cy;

  m_pMainCtrl->SetWindowPos(NULL, NULL, NULL,
                          dlgRect.Size().cx - (2 * mainCtrlRect.TopLeft().x),
                          dlgRect.Size().cy - m_cyBSpace,
                          SWP_NOMOVE | SWP_NOZORDER);

  m_pMainCtrl->GetWindowRect(&mainCtrlRect);
  ScreenToClient(&mainCtrlRect);

  ytop = mainCtrlRect.bottom + m_ybuttondiff;

  CWnd *pwnd;
  for (int i = 1; i <= m_numbtns; i++) {
    pwnd = GetDlgItem(m_viBottomButtons[i - 1]);
    pwnd->GetWindowRect(&btnRect);
    xleft = (i * m_DialogMinWidth / (m_numbtns + 1)) - (btnRect.Width() / 2);
    pwnd->SetWindowPos(NULL, xleft, ytop, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
  }

  GetDlgItem(m_viBottomButtons[m_iFocus])->SetFocus();

  m_bInitDone = true;

  return FALSE;
}

void CPWResizeDialog::OnSize(UINT nType, int cx, int cy)
{
  CPWDialog::OnSize(nType, cx, cy);

  if (!m_bInitDone || !m_bStatusBarOK)
    return;

  SetControls(cx, cy);
}

void CPWResizeDialog::SetControls(int cx, int cy)
{
  if (!m_bInitDone || !m_bStatusBarOK)
    return;

  if (m_pMainCtrl == NULL || !IsWindow(m_pMainCtrl->GetSafeHwnd()))
    return;

  CRect sbRect, mainCtrlRect, btnRect, dlgRect;
  CPoint pt_top;

  GetWindowRect(&dlgRect);

  // Allow ListCtrl to grow/shrink but leave room for the buttons underneath!
  m_pMainCtrl->GetWindowRect(&mainCtrlRect);

  pt_top.x = mainCtrlRect.left;
  pt_top.y = mainCtrlRect.top;
  ScreenToClient(&pt_top);

  m_pMainCtrl->MoveWindow(pt_top.x, pt_top.y,
                          cx - (2 * pt_top.x), cy - m_cyBSpace, TRUE);

  // Keep buttons in the bottom area
  int xleft, ytop;

  m_pMainCtrl->GetWindowRect(&mainCtrlRect);
  ScreenToClient(&mainCtrlRect);

  ytop = mainCtrlRect.bottom + m_ybuttondiff;

  CWnd *pwnd;
  for (int i = 1; i <= m_numbtns; i++) {
    pwnd = GetDlgItem(m_viBottomButtons[i - 1]);
    pwnd->GetWindowRect(&btnRect);
    xleft = (i * cx / (m_numbtns + 1)) - (btnRect.Width() / 2);
    pwnd->SetWindowPos(NULL, xleft, ytop, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);
  }

  // Now move the status bar
  m_statusBar.GetWindowRect(&sbRect);
  pt_top.x = sbRect.left;
  pt_top.y = sbRect.top;
  ScreenToClient(&pt_top);

  m_statusBar.MoveWindow(pt_top.x, cy - sbRect.Height(),
                         cx - (2 * pt_top.x),
                         sbRect.Height(), TRUE);

  GetDlgItem(m_viBottomButtons[m_iFocus])->SetFocus();
}

void CPWResizeDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
  if (m_bInitDone) {
    lpMMI->ptMinTrackSize = CPoint(m_DialogMinWidth, m_DialogMinHeight);
    if (m_DialogMaxHeight > 0)
      lpMMI->ptMaxTrackSize = CPoint(m_DialogMaxWidth, m_DialogMaxHeight);
  } else
    CPWDialog::OnGetMinMaxInfo(lpMMI);
}
