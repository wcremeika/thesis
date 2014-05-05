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
 

#if !defined(AFX_MAINFRM_H__6097C6C8_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_MAINFRM_H__6097C6C8_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "util/TrayNot.h"
#include "SmallViewer.h"
#include "Splitter.h"
#include "util/ComboBoxAuto.h"

#define ADDRESS_VIEW	0
#define LEFT_VIEW		1
#define RIGHT_VIEW		2
#define NB_VIEWS		3

class CMp3ExpView;
class CLeftView;
class CPlaylistDetailView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitter m_wndSplitter; //extends CSplitterWnd
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFileFound( CString strFileFoundCount );
	void UpdateStatusBar( CPlaylist *pPlaylist );
	BOOL m_bViewerCreated;
	void ShowExplorerWindows();
	BOOL m_bShowSmallViewer;
	BOOL m_bImageHidden;
	int m_nViewStyle;
	void InitSystray();
	BOOL m_bHidden;
	void OnResetTreeViewBegin();
	void OnResetTreeViewEnd();
	CWnd* m_wndViews[NB_VIEWS];
	void StopAnimation();
	void StartAnimation();
	CMyImageList m_imagelistStatusBar;
	void InsertAddress( CString strPath );
	BOOL ScanNewAddress( CString strNewPath );
	CString m_strStatusPane1Text;
	void SetStatusText( CString strLeftPane, CString strRightPane, CString strFileInfo, CString strSize, CString strTime );
	virtual ~CMainFrame();
	CMp3ExpView* GetRightPane();
	CLeftView* GetLeftPane();
	int GetViewType() { return m_nViewType; };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	// ReBar
	CReBar			m_wndReBar;
	CBitmap			m_bmpReBar;
	// Barre d'outils
	CToolBar		m_wndToolBar;
	CRect			m_rectToolBar;
	BOOL			m_bShowToolbar;
	BOOL			m_bShowLabels;
	// Barre d'adresse
	CComboBoxAuto	m_wndAddress;
	CMyImageList		m_imagelistComboAddress;
	BOOL			m_bShowAdress;
	// Icone animé
	CAnimateCtrl	m_wndAnimate;
	// Barre de statut
	CStatusBar		m_wndStatusBar;
	// Systray
	CTrayNot*		m_pTray;
	HICON			m_pTrayIcon[1];
	// Small Viewer
	CSmallViewer	m_dlgViewer;


	// Gestion des Views
	int m_nViewType;
	
	UINT GetBandNumber( HWND hwndBar );
	void UpdateReBar();

// Generated message map functions
protected:
	BOOL m_bUpdateStatusIcon;
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewArtist();
	afx_msg void OnViewGenre();
	afx_msg void OnViewAdressbar();
	afx_msg void OnViewToolbarLabels();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewArtist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGenre(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewAdressbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbarLabels(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnEditSelectAll();
	afx_msg void OnViewAlbum();
	afx_msg void OnUpdateViewAlbum(CCmdUI* pCmdUI);
	afx_msg void OnFileUpdateSnapshot();
	afx_msg void OnUpdateFileUpdateSnapshot(CCmdUI* pCmdUI);
	afx_msg LONG OnSystrayNotify(UINT,LONG);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileBrowseAddress();
	afx_msg void OnFilePlay();
	afx_msg void OnFileEnqueue();
	afx_msg void OnFileDelete();
	afx_msg void OnViewSmallViewer();
	afx_msg void OnUpdateViewSmallViewer(CCmdUI* pCmdUI);
	afx_msg void OnSearch();
	afx_msg void OnViewPlaylist();
	afx_msg void OnUpdateViewPlaylist(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnNewAddress();
	afx_msg void OnNewAddressEnter();
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void DoNothing();
	DECLARE_MESSAGE_MAP()
private:
	UINT m_nStatusPane1ID;
private:
	UINT m_nStatusPane1Style;
private:
	INT  m_nStatusPane1Width;
private:
	BOOL InitStatusBar(UINT *pIndicators, int nSize, int nSeconds);
	BOOL m_bMenuSelect;
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6097C6C8_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
