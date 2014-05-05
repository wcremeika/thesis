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
 
#if !defined(AFX_PLAYLISTVIEW_H__7F7F8DA5_065C_11D3_BEDD_00400556855E__INCLUDED_)
#define AFX_PLAYLISTVIEW_H__7F7F8DA5_065C_11D3_BEDD_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlaylistView.h : header file
// Copyright (c) 1998-2000 Pierre LEVY
//

/////////////////////////////////////////////////////////////////////////////
// CPlaylistView view

class CPlaylistView : public CListViewEx
{
protected:
	CPlaylistView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPlaylistView)

// Attributes
public:

protected:
	CMyImageList m_LargeImageList;
	CMyImageList m_SmallImageList;

// Operations
public:
	BOOL m_bInitialUpdateDone;
	int m_nSortedCol;
	BOOL m_bItemChangedHandling;
	void SelectPlaylist( CPlaylist* pPlaylist );
	void UpdateList();
	BOOL m_bValidList;
	BOOL IsSelected();
	void InitColumns();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaylistView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlaylistView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlaylistView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPlaylistSave();
	afx_msg void OnUpdatePlaylistSave(CCmdUI* pCmdUI);
	afx_msg void OnPlaylistSaveas();
	afx_msg void OnUpdatePlaylistSaveas(CCmdUI* pCmdUI);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPlaylistPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlaylistEnqueue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlaylistDelete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPlaylistShuffle();
	afx_msg void OnPlaylistReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTVIEW_H__7F7F8DA5_065C_11D3_BEDD_00400556855E__INCLUDED_)
