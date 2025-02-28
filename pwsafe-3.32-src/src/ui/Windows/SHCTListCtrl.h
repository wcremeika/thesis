/*
* Copyright (c) 2003-2013 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#pragma once

#include "SHCTHotKey.h"
#include "MenuShortcuts.h"

class COptionsShortcuts;

// Subitem indices
#define SHCT_SHORTCUTKEYS  0
#define SHCT_MENUITEMTEXT  1
#define SHCT_NUM_COLUMNS   2

class CSHCTListCtrl : public CListCtrl
{
public:
  CSHCTListCtrl();
  ~CSHCTListCtrl();

  void Init(COptionsShortcuts *pParent);

  void SaveHotKey();
  bool IsHotKeyActive() {return m_bHotKeyActive;}

  void OnMenuShortcutKillFocus(const WORD wVirtualKeyCode, const WORD wModifiers);

protected:
  INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;

  //{{AFX_MSG(CSHCTListCtrl)
  afx_msg BOOL OnToolTipText(UINT id, NMHDR *pNotifyStruct, LRESULT *pLResult);
  afx_msg void OnCustomDraw(NMHDR *pNotifyStruct, LRESULT *pLResult);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

private:
  COptionsShortcuts *m_pParent;

  CSHCTHotKey *m_pHotKey;
  WCHAR *m_pwchTip;
  int m_item;
  UINT m_id;
  bool m_bHotKeyActive;
  COLORREF m_crWindowText, m_crRedText;
};
