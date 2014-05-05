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
 

#if !defined(AFX_MP3ExpVIEW_H__6097C6CC_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_MP3ExpVIEW_H__6097C6CC_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mp3View.h"


class CMp3ExpView : public CMp3View
{
protected: // create from serialization only
	CMp3ExpView();
	DECLARE_DYNCREATE(CMp3ExpView)
	DWORD GetViewType();
	BOOL SetViewType(DWORD dwViewType);


// Attributes
public:
	CMp3ExpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3ExpView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetAllFiles( CMp3FileList* pMp3List );
	void FillList( CMp3FileList *pMp3List);
	UpdateList();
	void ModifyTagInfo(CString strArtist, CString strAlbum, CString strGenre);


	void SaveColumnsSize();
	void SaveColumnsInfos();
	void SelectAllRows();
	CObArray& GetColumns();
	void UpdateRowSelectionMode();

	virtual ~CMp3ExpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	void DefaultAction();

	//{{AFX_MSG(CMp3ExpView)
	afx_msg void OnUpdateFileEditTag(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileRename();
	afx_msg void OnUpdateFileRename(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileAddToPlaylist();
	afx_msg void OnUpdateFileAddToPlaylist(CCmdUI* pCmdUI);
	afx_msg LRESULT OnFileDelete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFileProperties();
	afx_msg void OnFileIntellitag();
	afx_msg void OnUpdateFileIntellitag(CCmdUI* pCmdUI);
	afx_msg void OnFileNormalize();
	afx_msg void OnUpdateFileNormalize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mp3ExpView.cpp
inline CMp3ExpDoc* CMp3ExpView::GetDocument()
   { return (CMp3ExpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3ExpVIEW_H__6097C6CC_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
