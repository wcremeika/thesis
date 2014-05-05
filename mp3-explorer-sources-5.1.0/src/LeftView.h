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
 

#if !defined(AFX_LEFTVIEW_H__6097C6CE_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_LEFTVIEW_H__6097C6CE_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMp3ExpDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CMp3ExpDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bFound;
	BOOL m_bAterLastFound;
	HTREEITEM m_hLastFound;
	CString m_strFindText;
	void StartFind( BOOL bFirst );
	BOOL Find( HTREEITEM hItem, CTreeCtrl& tree, CString strFindText );

	void SelectOldCurrentItem( HTREEITEM hItem, CTreeCtrl& tree );
	void RestoreCurrentItem();
	void SaveCurrentItem();
	DWORD m_nExpandTimer;
	void OnDropMp3();
	UINT m_nStateBeforeDrag;
	void DragEnter();
	void DragOverEnd();
	HTREEITEM m_hTreeDropHilited;
	HTREEITEM m_hTreeBeforeDrag;
	void DragOverUpdate( CPoint point );
	BOOL m_bResetInProgress;
	int m_nViewType;
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CListDropTarget m_OleDropTarget;

// Generated message map functions
protected:
	CString m_strItemText;
	void SetStatusInfos( CString strItemText, CMp3FileList* pMp3List);
	void FillTree( CTreeCtrl& tree );
	CMyImageList m_ImageList;
	HTREEITEM m_hitemRoot;
	//{{AFX_MSG(CLeftView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileExportAll();
	afx_msg void OnUpdateFileExportAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg LRESULT OnFilePlayAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileEnqueueAll(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilePlaylistAll();
	afx_msg void OnFindfirst();
	afx_msg void OnUpdateFindfirst(CCmdUI* pCmdUI);
	afx_msg void OnFindnext();
	afx_msg void OnUpdateFindnext(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CMp3ExpDoc* CLeftView::GetDocument()
   { return (CMp3ExpDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__6097C6CE_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
