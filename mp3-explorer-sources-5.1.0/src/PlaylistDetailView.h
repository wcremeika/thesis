// This file is part of [ mp3 - explorer ]
// Copyright (C) 1998-2004  Pierre LEVY
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 


#if !defined(AFX_PLAYLISTDETAILVIEW_H__7F7F8DA6_065C_11D3_BEDD_00400556855E__INCLUDED_)
#define AFX_PLAYLISTDETAILVIEW_H__7F7F8DA6_065C_11D3_BEDD_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlaylistDetailView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDetailView view

#include "Mp3ExpView.h"

class CPlaylistDetailView : public CMp3View
{
protected:
	CPlaylistDetailView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPlaylistDetailView)

// Attributes
public:

// Operations
public:
	BOOL m_bInitialUpdateDone;
	UpdateList();
	void UpdatePlaylistDetailList();
	UINT m_nStateBeforeDrag;
	void DragEnter();
	void DragOverEnd();
	int m_nItemDropHilited;
	int m_nItemBeforeDrag;
	void DragOverUpdate( CPoint point );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaylistDetailView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlaylistDetailView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlaylistDetailView)
	afx_msg void OnDetailDown();
	afx_msg void OnDetailUp();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDetailDelete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTDETAILVIEW_H__7F7F8DA6_065C_11D3_BEDD_00400556855E__INCLUDED_)
