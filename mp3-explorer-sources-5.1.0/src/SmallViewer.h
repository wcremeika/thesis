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
 

#if !defined(AFX_SMALLVIEWER_H__D7760901_3BB6_11D3_BEDE_00400556855E__INCLUDED_)
#define AFX_SMALLVIEWER_H__D7760901_3BB6_11D3_BEDE_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmallViewer.h : header file
//

#include "util/TransBitmap.h"

#define NB_VIEWER_BMP 5

/////////////////////////////////////////////////////////////////////////////
// CSmallViewer dialog

class CSmallViewer : public CDialog
{
// Construction
protected:
	int m_nFontWidth;
	CFont m_font;
	CTransBitmap m_bmIcons[NB_VIEWER_BMP];
	int m_nMode;
	int m_nFontHeight;

public:
	void Initialize();
	void DoViewerAction( int nAction );
	void FillPlaylist( CPlaylistList* pList );
	int m_nType;
	void FillCategory(CMapStringToOb *pMap, int nType );
	CBitmapButton m_buttonClose;
	CBitmapButton m_buttonMinimize;
	CSmallViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmallViewer)
	enum { IDD = IDD_SMALL_VIEWER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmallViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolBar		m_wndToolBar;

	// Generated message map functions
	//{{AFX_MSG(CSmallViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnViewerAlbum();
	afx_msg void OnUpdateViewerAlbum(CCmdUI* pCmdUI);
	afx_msg void OnViewerArtist();
	afx_msg void OnUpdateViewerArtist(CCmdUI* pCmdUI);
	afx_msg void OnViewerGenre();
	afx_msg void OnUpdateViewerGenre(CCmdUI* pCmdUI);
	afx_msg void OnViewerPlaylist();
	afx_msg void OnUpdateViewerPlaylist(CCmdUI* pCmdUI);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDblclkViewerList();
	afx_msg void OnViewerLargeMode();
	afx_msg void OnViewerPlay();
	afx_msg void OnUpdateViewerPlay(CCmdUI* pCmdUI);
	afx_msg void OnViewerEnqueue();
	afx_msg void OnUpdateViewerEnqueue(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMALLVIEWER_H__D7760901_3BB6_11D3_BEDE_00400556855E__INCLUDED_)
