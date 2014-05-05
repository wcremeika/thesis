/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

/*
*  This is nearly an exact copy of CInfoDisplay class in Asynch Explorer by
*  Joseph M. Newcomer [MVP]; http://www.flounder.com
*  Only minor formatting and naming changes have been made to fit in with this
*  project.
*/

#pragma once

// CInfoDisplay

class CInfoDisplay : public CWnd
{
  DECLARE_DYNAMIC(CInfoDisplay)

public:
  CInfoDisplay();
  virtual ~CInfoDisplay();
  BOOL Create(int x, int y, LPCWSTR caption, CWnd * parent);

protected:
  DECLARE_MESSAGE_MAP()
  HFONT m_font;

  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg LRESULT OnSetFont(WPARAM, LPARAM);
  afx_msg LRESULT OnGetFont(WPARAM, LPARAM);

  virtual void PostNcDestroy();
};
