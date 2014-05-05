/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#pragma once

// CFontsDialog

enum {PWFONT, TLFONT, VKFONT};

extern LOGFONT dfltTreeListFont;

class CFontsDialog : public CFontDialog
{
  DECLARE_DYNAMIC(CFontsDialog)

public:
  CFontsDialog(LPLOGFONT lplfInitial = NULL,
                 DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
                 CDC* pdcPrinter = NULL,
                 CWnd* pParentWnd = NULL,
                 int iType = PWFONT);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
  CFontsDialog(const CHARFORMAT& charformat,
                 DWORD dwFlags = CF_SCREENFONTS,
                 CDC* pdcPrinter = NULL,
                 CWnd* pParentWnd = NULL,
                 int iType = PWFONT);
#endif
  virtual ~CFontsDialog();

  // Following override to reset idle timeout on any event
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

  // Following override to stop accelerators interfering
  virtual INT_PTR DoModal();

  CString m_sampletext, m_title;
  int m_iType;
  LOGFONT m_dfltVKBDFont;
  bool m_bReset;

  // Dialog Data
  //{{AFX_DATA(CXFontDialog)
  //enum { IDD = IDD_PWFONTDIALOG };
  //}}AFX_DATA

protected:
  DECLARE_MESSAGE_MAP()
};
