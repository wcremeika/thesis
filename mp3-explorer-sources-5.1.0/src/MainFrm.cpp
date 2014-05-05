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
 

#include "stdafx.h"
#include "Mp3Exp.h"

#include "LeftView.h"
#include "MainFrm.h"
#include "util/ListVwEx.h"
#include "Mp3ExpView.h"
#include "Mp3ExpDoc.h"
#include "OptionSheet.h"
#include "HtmlFileView.h"

#include "PlaylistView.h"
#include "PlaylistDetailView.h"
#include "Splash.h"
#include "ID3v2Sheet.h"
#include "SearchDlg.h"
#include "util/HyperLink.h"
#include "HtmlMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NB_BUTTONS 14
#define NB_VIEW_STYLES 4

#define NO_PANE_LEFT		0
#define NO_PANE_RIGHT		1
#define NO_PANE_VIEW		2
#define NO_PANE_FILEINFO	3 
#define NO_PANE_SIZE		4
#define NO_PANE_TIME		5 


#define ID_PANE_LEFT		0
#define ID_PANE_RIGHT		1
#define ID_PANE_VIEW		2
#define ID_PANE_FILEINFO	3 
#define ID_PANE_SIZE		4
#define ID_PANE_TIME		5
 
#define VIEW_STYLE_SMALLICON	0
#define VIEW_STYLE_LARGEICON	1
#define VIEW_STYLE_LIST			2
#define VIEW_STYLE_DETAILS   	3

#define MENU_SIZE 4


static DYNAMIC_MENU _gMenuList[MENU_SIZE] =
{ 
	{ MF_STRING , ID_VIEW_LARGEICON , "LargeIcons" },
	{ MF_STRING , ID_VIEW_SMALLICON , "SmallIcons" },
	{ MF_STRING , ID_VIEW_LIST , "List" },
	{ MF_STRING , ID_VIEW_DETAILS , "Details" }

};


extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_DISPLAY_ARTIST, OnViewArtist)
	ON_COMMAND(ID_DISPLAY_GENRE, OnViewGenre)
	ON_COMMAND(ID_VIEW_ADRESSBAR, OnViewAdressbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_LABELS, OnViewToolbarLabels)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_ARTIST, OnUpdateViewArtist)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_GENRE, OnUpdateViewGenre)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ADRESSBAR, OnUpdateViewAdressbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_LABELS, OnUpdateViewToolbarLabels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_COMMAND(ID_VIEW_OPTIONS, OnOptions)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_DISPLAY_ALBUM, OnViewAlbum)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_ALBUM, OnUpdateViewAlbum)
	ON_COMMAND(ID_FILE_UPDATE_SNAPSHOT, OnFileUpdateSnapshot)
	ON_UPDATE_COMMAND_UI(ID_FILE_UPDATE_SNAPSHOT, OnUpdateFileUpdateSnapshot)
	ON_MESSAGE(WM_SYSTRAY_NOTIFY, OnSystrayNotify)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_BROWSE_ADDRESS, OnFileBrowseAddress)
	ON_COMMAND(ID_FILE_PLAY, OnFilePlay)
	ON_COMMAND(ID_FILE_ENQUEUE, OnFileEnqueue)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_COMMAND(ID_VIEW_SMALL_VIEWER, OnViewSmallViewer)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMALL_VIEWER, OnUpdateViewSmallViewer)
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_COMMAND(ID_VIEW_PLAYLIST, OnViewPlaylist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAYLIST, OnUpdateViewPlaylist)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDOK, OnNewAddressEnter)
	ON_CBN_SELENDOK(AFX_IDW_TOOLBAR + 1, OnNewAddress)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
	ON_COMMAND(ID_LISTVIEW_DROPDOWN, DoNothing)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
};

static int g_view_mode[NB_VIEW_STYLES] = { 
	ID_VIEW_SMALLICON, 
	ID_VIEW_LARGEICON, 
	ID_VIEW_LIST,
	ID_VIEW_DETAILS
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// CG: The following block was inserted by 'Status Bar' component.
	{
		m_nStatusPane1Width = -1;
		m_bMenuSelect = FALSE;
	}
	

	m_bShowToolbar = TRUE;
	m_bShowLabels = (BOOL) theApp.m_nShowLabels;
	m_bShowAdress = TRUE;
	m_bUpdateStatusIcon = TRUE;
	m_nViewType = theApp.m_nViewType;

	m_pTray = NULL ;
	m_pTrayIcon[0] = theApp.LoadIcon ( MAKEINTRESOURCE(IDI_SYSTRAY) ) ;

	m_bHidden = FALSE;
	m_bShowSmallViewer = theApp.m_bSmallViewer;
	m_bViewerCreated = FALSE;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Création de la Barre d'adresse

	if (!m_wndAddress.Create(CBS_DROPDOWN | WS_CHILD, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}

	m_imagelistComboAddress.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelistComboAddress.SetOverlayImage( 1, 1 );

	m_wndAddress.SetImageList( &m_imagelistComboAddress );
	for( int i = 0 ; i < MAX_ADDRESS ; i++ )
	{
		if( theApp.m_strAddress[i].GetLength() > 0 )
		{
			InsertAddress(theApp.m_strAddress[i]);
		}
	}
	InsertAddress(theApp.m_strFilePath);
	m_wndViews[ADDRESS_VIEW] = &m_wndAddress;

	///////////////////////////////////////////////////////////////////////////
	// Création de l'icône animé

	if( !m_wndAnimate.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 50, 50), this, AFX_IDW_TOOLBAR + 2))
	{
		TRACE0("Failed to create Animate Ctrl\n");
		return -1;      // fail to create
	}

	CString strAviFilename;
	strAviFilename.LoadString( IDS_PLUGIN_AVI );
	CString strAviPath = theApp.m_strPluginPath + strAviFilename;
	CFileFind ff;
	if( (strAviFilename != "NONE") && ff.FindFile( strAviPath ))
	{
		m_wndAnimate.Open( strAviPath );
	}
	else
	{
		m_wndAnimate.Open(IDR_AVI1);
	}


	///////////////////////////////////////////////////////////////////////////
	// Création de la ReBar

	m_bmpReBar.LoadBitmap( IDB_REBARBITMAP );
	if (!m_wndReBar.Create(this))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	// add the toolbar, animation, and address bar to the rebar
	int nBand =	m_wndReBar.AddBar(&m_wndToolBar);
	CString strAddressLabel = theApp.Translate( "AddressBar" , "Address" );
	nBand =	m_wndReBar.AddBar(&m_wndAddress, strAddressLabel, NULL, RBBS_FIXEDBMP | RBBS_BREAK);
	nBand =	m_wndReBar.AddBar(&m_wndAnimate, NULL, &m_bmpReBar, RBBS_FIXEDSIZE | RBBS_FIXEDBMP);

	// set up toolbar properties
	CMyImageList img;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	img.Create( 22, 22, IDB_HOTTOOLBAR );
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();
	img.Create( 22, 22, IDB_COLDTOOLBAR );
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
//	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);

	if( theApp.m_bPlayer )
	{
		m_wndToolBar.SetButtons( NULL, NB_BUTTONS );
	}
	else
	{
		m_wndToolBar.SetButtons( NULL, NB_BUTTONS - 1 );
	}

	// set up each toolbar button
	m_wndToolBar.SetButtonInfo(0, ID_FILE_PLAY, TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_FILE_ENQUEUE, TBSTYLE_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_FILE_EDIT_TAG, TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(3, ID_FILE_DELETE, TBSTYLE_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(4, ID_EDIT_COPY, TBSTYLE_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(5, ID_SEARCH, TBSTYLE_BUTTON, 5);
	m_wndToolBar.SetButtonInfo(6, ID_DISPLAY_ARTIST, TBSTYLE_BUTTON, 6);
	m_wndToolBar.SetButtonInfo(7, ID_DISPLAY_GENRE, TBSTYLE_BUTTON, 7);
	m_wndToolBar.SetButtonInfo(8, ID_DISPLAY_ALBUM, TBSTYLE_BUTTON, 8);
	m_wndToolBar.SetButtonInfo(9, ID_DISPLAY_ALL, TBSTYLE_BUTTON, 9);
	m_wndToolBar.SetButtonInfo(10, ID_VIEW_PLAYLIST, TBSTYLE_BUTTON, 10);
	if( theApp.m_bPlayer )
	{
		m_wndToolBar.SetButtonInfo(11, ID_TOOLS_PLAYER, TBSTYLE_BUTTON, 11);
		m_wndToolBar.SetButtonInfo(12, ID_LISTVIEW_DROPDOWN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 12);
		m_wndToolBar.SetButtonInfo(13, ID_VIEW_SMALL_VIEWER, TBSTYLE_BUTTON, 13);
	}
	else
	{
		m_wndToolBar.SetButtonInfo(11, ID_LISTVIEW_DROPDOWN, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, 12);
		m_wndToolBar.SetButtonInfo(12, ID_VIEW_SMALL_VIEWER, TBSTYLE_BUTTON, 13);
	}

	m_wndToolBar.SetButtonText(0, theApp.Translate( "Toolbar", "Play" ));
	m_wndToolBar.SetButtonText(1, theApp.Translate( "Toolbar", "Enqueue" ));
	m_wndToolBar.SetButtonText(2, theApp.Translate( "Toolbar", "Properties" ));
	m_wndToolBar.SetButtonText(3, theApp.Translate( "Toolbar", "Delete" ));
	m_wndToolBar.SetButtonText(4, theApp.Translate( "Toolbar", "Copy" ));
	m_wndToolBar.SetButtonText(5, theApp.Translate( "Toolbar", "Search" ));
	m_wndToolBar.SetButtonText(6, theApp.Translate( "Toolbar", "Artist" ));
	m_wndToolBar.SetButtonText(7, theApp.Translate( "Toolbar", "Genre" ));
	m_wndToolBar.SetButtonText(8, theApp.Translate( "Toolbar", "Album" ));
	m_wndToolBar.SetButtonText(9, theApp.Translate( "Toolbar", "AllCols" ));
	m_wndToolBar.SetButtonText(10, theApp.Translate( "Toolbar", "Playlists" ));

	if( theApp.m_bPlayer )
	{
		m_wndToolBar.SetButtonText(11, theApp.Translate( "Toolbar", "Player" ));
		m_wndToolBar.SetButtonText(12, theApp.Translate( "Toolbar", "ListView" ));
		m_wndToolBar.SetButtonText(13, theApp.Translate( "Toolbar", "SmallViewer" ));
	}
	else
	{
		m_wndToolBar.SetButtonText(11, theApp.Translate( "Toolbar", "ListView" ));
		m_wndToolBar.SetButtonText(12, theApp.Translate( "Toolbar", "SmallViewer" ));
	}

	

	// set up toolbar button sizes

	m_wndToolBar.GetItemRect(0, &m_rectToolBar);
	m_wndToolBar.SetSizes( m_rectToolBar.Size(), CSize(22,22));

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_SIZE_FIXED);

	UpdateReBar();

	///////////////////////////////////////////////////////////////////////////
	// Création de la barre de statut

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, 
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_imagelistStatusBar.Create( 16, 16, IDB_SMALL_ICONS );

	m_wndStatusBar.SetPaneInfo( NO_PANE_LEFT,	  ID_PANE_LEFT,		SBPS_NORMAL,  170 );
	m_wndStatusBar.SetPaneInfo( NO_PANE_RIGHT,	  ID_PANE_RIGHT,	SBPS_STRETCH, 100 );
	m_wndStatusBar.SetPaneInfo( NO_PANE_VIEW,	  ID_PANE_VIEW,		SBPS_NORMAL,  16 );
	m_wndStatusBar.SetPaneInfo( NO_PANE_FILEINFO, ID_PANE_FILEINFO,	SBPS_NORMAL,  150 );
	m_wndStatusBar.SetPaneInfo( NO_PANE_SIZE,	  ID_PANE_SIZE,		SBPS_NORMAL,  65 );
	m_wndStatusBar.SetPaneInfo( NO_PANE_TIME,	  ID_PANE_TIME,		SBPS_NORMAL,  65 );


	///////////////////////////////////////////////////////////////////////////
	// Gestion du system tray
	InitSystray();

	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
	///////////////////////////////////////////////////////////////////////////
	// Création du splitter

	if (!m_wndSplitter.CreateStatic(this, 2, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize( theApp.m_nCx1, theApp.m_nCy1), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMp3ExpView), CSize(theApp.m_nCx2, theApp.m_nCy1), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CHtmlMenu), CSize(theApp.m_nCx1, theApp.m_nCy2), pContext) ||
		!m_wndSplitter.CreateView(1, 1, RUNTIME_CLASS(CHtmlFileView), CSize(theApp.m_nCx2, theApp.m_nCy2), pContext)  )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	if( theApp.m_bDisplayPlaylists )
	{
		theApp.m_bDisplayPlaylists = !theApp.m_bDisplayPlaylists;
		OnViewPlaylist();
	}


	m_wndViews[LEFT_VIEW] = m_wndSplitter.GetPane(0, 0);
	m_wndViews[RIGHT_VIEW] = m_wndSplitter.GetPane(0, 1);

//	m_wndSplitter.RecalcLayout();


	// Options spécifiques de création :
	//		- Vue détails par défaut	
	OnViewStyle( ID_VIEW_DETAILS );

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = MP3EXPLORER_CLASSNAME;

	// Ancienne position par défaut
    theApp.GetInitialPosition( cs.cx, cs.cy, cs.x, cs.y ); 

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CMp3ExpView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CMp3ExpView* pView = DYNAMIC_DOWNCAST(CMp3ExpView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;

}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CMp3ExpView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CMp3ExpView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CMp3ExpView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;
		CListCtrl& refListCtrl = pView->GetListCtrl();
		refListCtrl.Arrange(LVA_DEFAULT);

		switch (nCommandID)
		{
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT | LVS_SHOWSELALWAYS;
			m_nViewStyle = VIEW_STYLE_DETAILS;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON | LVS_SHOWSELALWAYS;
			m_nViewStyle = VIEW_STYLE_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON | LVS_SHOWSELALWAYS;
			m_nViewStyle = VIEW_STYLE_LARGEICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST | LVS_SHOWSELALWAYS;
			m_nViewStyle = VIEW_STYLE_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}


void CMainFrame::OnNewAddressEnter()
{

	// Récupération de la chaîne se trouvant dans la Combo box
	CString strNewPath;
	m_wndAddress.GetEditCtrl()->GetWindowText(strNewPath);
	if( ScanNewAddress( strNewPath ))
	{
		InsertAddress(strNewPath);
	}
	else
	{
		::MessageBeep( MB_OK );
	}

	
}

void CMainFrame::OnNewAddress()
{

	// Récupération de la chaîne se trouvant dans la Combo box
	CString strNewPath;
	int nIndex = m_wndAddress.GetCurSel();
	m_wndAddress.GetLBText( nIndex, strNewPath);
	if( ScanNewAddress( strNewPath ))
	{	
		m_wndAddress.GetEditCtrl()->SetWindowText(strNewPath);
	}
	else
	{
		// Si l'adresse est invalide il faut la supprimer de la liste
		m_wndAddress.DeleteItem( nIndex );
		::MessageBeep( MB_OK );
	}
	
}
void CMainFrame::InsertAddress(CString strNewPath)
{
	// Pour une unité disque ajouter un antislash
	int nImage = ICON_FOLDER;  // Répertoire

	CString strFavorite = theApp.LoadTranslatedString( "IdsFavoriteAddresses" );

	if( (strNewPath.GetLength() < 4) || (strNewPath == strFavorite) ) 
	{
		if( (strNewPath != "*") && (strNewPath != strFavorite))
		{
			if( strNewPath.Right(1) != "\\" )
				strNewPath += "\\";
			nImage = ICON_DRIVE;  // Disque
		}
		else
		{
				nImage = ICON_FAVORITE;  // Favorite
				strNewPath = theApp.LoadTranslatedString( "IdsFavoriteAddresses" );
		}
	}
	// Ajoute le nouveau chemin à la Combo s'il n'existe pas déjà 
	int nIndex = m_wndAddress.FindStringExact( -1 , strNewPath );
	if( nIndex == -1 )
	{
		COMBOBOXEXITEM cbeItem;

	    cbeItem.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		cbeItem.iItem = 0;
		static _TCHAR buffer[MAX_PATH];
		strcpy( buffer , strNewPath );
	    cbeItem.pszText = buffer;
		cbeItem.cchTextMax = MAX_PATH - 1;
	    cbeItem.iImage = nImage;
		cbeItem.iSelectedImage = nImage;
		nIndex = m_wndAddress.InsertItem( &cbeItem );
	}
	m_wndAddress.SetCurSel( nIndex );

}

BOOL CMainFrame::ScanNewAddress( CString strNewPath )
{
	StartAnimation();

	// Modifie le chemin courant du document
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
	BOOL bRet = pDoc->OnNewPath( strNewPath );

	StopAnimation();
	return bRet;
}


void CMainFrame::OnUpdateViewArtist(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio( (m_nViewType == VIEW_ARTIST ? 1 : 0 ));
	
}

void CMainFrame::OnUpdateViewGenre(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio( (m_nViewType == VIEW_GENRE ? 1 : 0 ));
	
}

void CMainFrame::OnUpdateViewAlbum(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio( (m_nViewType == VIEW_ALBUM ? 1 : 0 ));
	
}

void CMainFrame::OnViewArtist() 
{
	if( m_nViewType != VIEW_ARTIST )
	{
		m_nViewType = VIEW_ARTIST;
		m_bUpdateStatusIcon = TRUE;

		// Modifie les vues du document

		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
		pDoc->UpdateAllViews ( NULL , UPDATE_RESET_TREEVIEW , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_SET_VIEW_ARTIST , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_FILL_TREEVIEW , NULL );
	}
	
}

void CMainFrame::OnViewGenre() 
{
	if( m_nViewType != VIEW_GENRE )
	{
		m_nViewType = VIEW_GENRE;
		m_bUpdateStatusIcon = TRUE;

		// Modifie les vues du document

		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
		pDoc->UpdateAllViews ( NULL , UPDATE_RESET_TREEVIEW , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_SET_VIEW_GENRE , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_FILL_TREEVIEW , NULL );
	}
}

void CMainFrame::OnViewAlbum()
{
	if( m_nViewType != VIEW_ALBUM )
	{
		m_nViewType = VIEW_ALBUM;
		m_bUpdateStatusIcon = TRUE;

		// Modifie les vues du document

		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
		pDoc->UpdateAllViews ( NULL , UPDATE_RESET_TREEVIEW , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_SET_VIEW_ALBUM , NULL );
		pDoc->UpdateAllViews ( NULL , UPDATE_FILL_TREEVIEW , NULL );
	}

}

///////////////////////////////////////////////////////////////////////////
// Affichage des barres d'outils, adresse, ...

// Mises à jour de l'interface

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bShowToolbar );	
	
}

void CMainFrame::OnUpdateViewAdressbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bShowAdress );	
	
}

void CMainFrame::OnUpdateViewToolbarLabels(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bShowLabels );	
}


// Commandes de l'interface

void CMainFrame::OnViewToolbar() 
{
	m_bShowToolbar =! m_bShowToolbar;
	UpdateReBar();	
}


void CMainFrame::OnViewAdressbar() 
{
	m_bShowAdress = !m_bShowAdress;
	UpdateReBar();

}

void CMainFrame::OnViewToolbarLabels() 
{
	m_bShowLabels = !m_bShowLabels;
	UpdateReBar();
	
}

UINT CMainFrame::GetBandNumber( HWND hwnd )
{
	CReBarCtrl& rebarCtrl = m_wndReBar.GetReBarCtrl();

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof( REBARBANDINFO );
	rbbi.fMask = RBBIM_CHILD;

	for( UINT i = 0 ; i < (UINT) rebarCtrl.GetBandCount() ; i++ )
	{
		rebarCtrl.GetBandInfo( i , &rbbi );
		if( rbbi.hwndChild == hwnd )
			return i;

	}
	return 0;
	
}

void CMainFrame::UpdateReBar()
{

	// Définition des dimensions de la barre d'outils en fonction de l'affichage des étiquettes
	CRect rectToolBar;
	if( m_bShowLabels )
	{
		// Initialiser rectToolBar avec la variable membre contenant la taille avec labels
		rectToolBar = m_rectToolBar;
		m_wndToolBar.SetSizes( rectToolBar.Size(), CSize(22,22));
	}
	else
	{
		// Définir la taille minimale des boutons pour calculer rectToolBar
		m_wndToolBar.SetSizes( CSize( 32, 30 ), CSize(22, 22));
		m_wndToolBar.GetItemRect(0, &rectToolBar);
	}

	// Récupération du control de la ReBar
	CReBarCtrl& rebarCtrl = m_wndReBar.GetReBarCtrl();
	UINT nBand;

	// Définition des paramètres (tailles minimale, ideale ) des bandes de la ReBar 

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);

	// Définition des paramètres de l'icône animé

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	rbbi.cxMinChild = 40;
	rbbi.cyMinChild = 40;
	nBand = GetBandNumber( m_wndAnimate );
	m_wndReBar.GetReBarCtrl().SetBandInfo( nBand, &rbbi);

	// Définition des paramètres de la barre d'outils

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
	rbbi.cxMinChild = rectToolBar.Width();
	rbbi.cyMinChild = rectToolBar.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * NB_BUTTONS;

	nBand = GetBandNumber( m_wndToolBar );
	m_wndReBar.GetReBarCtrl().SetBandInfo( nBand, &rbbi);

	// Définition des paramètres de la barre d'adresse

	CRect rectAddress;

	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	m_wndAddress.GetEditCtrl()->GetWindowRect(&rectAddress);
	rbbi.cyMinChild = rectAddress.Height() + 10;
	rbbi.cxIdeal = 200;

	nBand = GetBandNumber( m_wndAddress );
	m_wndReBar.GetReBarCtrl().SetBandInfo( nBand, &rbbi);
	
	
	// Affichage ou non de la barre d'outils
	nBand = GetBandNumber( m_wndToolBar );
	rebarCtrl.ShowBand( nBand , m_bShowToolbar );
	
	// Affichage ou non de la barre d'adresse
	nBand = GetBandNumber( m_wndAddress );
	rebarCtrl.ShowBand( nBand , m_bShowAdress );

}

///////////////////////////////////////////////////////////////////////////
// Destruction de la fenêtre

BOOL CMainFrame::DestroyWindow() 
{
	// Sauvegarde l'affichage des textes des boutons de la barre d'outils
	theApp.WriteProfileInt( SECT_USR, ENTR_SHOWLABELS, (int) m_bShowLabels);
	theApp.WriteProfileInt( SECT_USR, ENTR_VIEWTYPE, (int) m_nViewType);
	theApp.WriteProfileInt( SECT_USR, ENTR_ALL_COLS, (int) GetRightPane()->m_bDisplayAll );
	theApp.WriteProfileInt( SECT_USR, ENTR_SORTINGCOL, theApp.m_nSortingCol );
	theApp.WriteProfileInt( SECT_USR, ENTR_SECONDARYSORTINGCOL, theApp.m_nSecondSortingCol );
	theApp.WriteProfileInt( SECT_USR, ENTR_PLAYLISTSORTINGCOL, theApp.m_nPlaylistSortingCol );
	theApp.WriteProfileInt( SECT_USR, ENTR_SHOWVIEWER, m_bShowSmallViewer );
	theApp.WriteProfileInt( SECT_USR, ENTR_DISPLAY_PLAYLISTS, theApp.m_bDisplayPlaylists );

	// Sauvegarde la taille des colonnes
	if( m_nViewStyle == VIEW_STYLE_DETAILS )
	{
		GetRightPane()->SaveColumnsSize();
	}

	// Sauvegarde les adresses
	static _TCHAR szEntry[MAX_BUFFER];
	CString strAddress;
	int nAddressCount = m_wndAddress.GetCount();
	for( int i = 0 ; i < nAddressCount ; i++ )
	{
		m_wndAddress.GetLBText(i, strAddress);
		wsprintf( szEntry, ENTR_ADDRESS, (i % MAX_ADDRESS) );
		theApp.WriteProfileString( SECT_USR, szEntry, strAddress );
	}

	// Sauvegarde la position et les dimensions de la fenêtre
	WINDOWPLACEMENT wndpl;

	GetWindowPlacement( &wndpl );
	theApp.WriteProfileInt( SECT_USR, ENTR_CX, wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left );
	theApp.WriteProfileInt( SECT_USR, ENTR_CY, wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top );
	theApp.WriteProfileInt( SECT_USR, ENTR_X, wndpl.rcNormalPosition.left );
	theApp.WriteProfileInt( SECT_USR, ENTR_Y, wndpl.rcNormalPosition.top );

	int nCx1, nCx2, nCy1, nCy2, nCmin;
	m_wndSplitter.GetColumnInfo( 0 , nCx1 , nCmin );
	m_wndSplitter.GetColumnInfo( 1 , nCx2 , nCmin );
	m_wndSplitter.GetRowInfo( 0 , nCy1 , nCmin );
	m_wndSplitter.GetRowInfo( 1 , nCy2 , nCmin );

	theApp.WriteProfileInt( SECT_USR, ENTR_CX1, nCx1 );
	theApp.WriteProfileInt( SECT_USR, ENTR_CX2, nCx2 );
	theApp.WriteProfileInt( SECT_USR, ENTR_CY1, nCy1 );
	theApp.WriteProfileInt( SECT_USR, ENTR_CY2, nCy2 );

	BOOL bMaximized = IsZoomed();
	theApp.WriteProfileInt( SECT_USR, ENTR_MAXIMIZED, bMaximized );

	
	if ( m_pTray )	// remove icon from taskbar tray
	{
		delete m_pTray ;
		m_pTray = NULL ;
	}


	return CFrameWnd::DestroyWindow();
}


void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
  	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// Restore first pane of the statusbar?
		if (nFlags == 0xFFFF && hSysMenu == 0 && m_nStatusPane1Width != -1)
		{
			m_bMenuSelect = FALSE;
			m_wndStatusBar.SetPaneInfo(0, 
				m_nStatusPane1ID, m_nStatusPane1Style, m_nStatusPane1Width);
			m_wndStatusBar.SetPaneText(0, m_strStatusPane1Text );
			m_nStatusPane1Width = -1;   // Set it to illegal value
		}
		else 
		{
			m_bMenuSelect = TRUE;
		}
	}

}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
  	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
  
	// CG: The following block was inserted by 'Status Bar' component.
//*	m_bMenuSelect = TRUE;
	{
		// store width of first pane and its style
		if (m_nStatusPane1Width == -1 && m_bMenuSelect)
		{
			m_wndStatusBar.GetPaneInfo(0, m_nStatusPane1ID, 
				m_nStatusPane1Style, m_nStatusPane1Width);
			m_wndStatusBar.SetPaneInfo(0, m_nStatusPane1ID, 
				SBPS_NOBORDERS|SBPS_STRETCH, 16384);
//*			m_wndStatusBar.GetPaneText(0, m_strStatusPane1Text );

		}
	}

}



void CMainFrame::SetStatusText(CString strLeftPane, CString strRightPane, CString strFileInfo, CString strSize, CString strTime )
{
	CStatusBarCtrl& ctrlStatus = m_wndStatusBar.GetStatusBarCtrl();

//*	if( m_bMenuSelect )
//*		return;

	static CString strOldLeftPane;
	// Left Pane : Drive / Artist / Genre
	if( strLeftPane  != "" ) 
	{
		m_wndStatusBar.SetPaneText( NO_PANE_LEFT , strLeftPane );
		strOldLeftPane = strLeftPane;
	}
	else
	{
		m_wndStatusBar.SetPaneText( NO_PANE_LEFT , strOldLeftPane );
	}


	// Right Pane
	if( strRightPane != "" )
	{
		m_wndStatusBar.SetPaneText( NO_PANE_RIGHT , strRightPane );
		ctrlStatus.SetIcon( NO_PANE_RIGHT , m_imagelistStatusBar.ExtractIcon( ICON_TITLE ));
	}
	else
	{
		m_wndStatusBar.SetPaneText( 1 , strRightPane );
		ctrlStatus.SetIcon( NO_PANE_RIGHT , NULL );
	}
	// View pane
	if(	m_bUpdateStatusIcon )
	{
		int nImage;
		switch( m_nViewType )
		{
		case VIEW_ARTIST:
			nImage = ICON_ARTIST;
			break;
		case VIEW_GENRE:
			nImage = ICON_GENRE;
			break;
		case VIEW_ALBUM:
			nImage = ICON_ALBUM;
			break;
		default:
			nImage = NULL;
		}
		ctrlStatus.SetIcon( NO_PANE_VIEW , m_imagelistStatusBar.ExtractIcon( nImage ));
		m_bUpdateStatusIcon = FALSE;
	}

	// File info pane
	if( strFileInfo  != "" ) m_wndStatusBar.SetPaneText( NO_PANE_FILEINFO , strFileInfo );

	// Size pane
	if( strSize  != "" ) m_wndStatusBar.SetPaneText( NO_PANE_SIZE , strSize );

	// Time pane
	if( strTime  != "" ) m_wndStatusBar.SetPaneText( NO_PANE_TIME , strTime );

}



void CMainFrame::StartAnimation()
{
	m_wndAnimate.Play(0, -1, -1);
}

void CMainFrame::StopAnimation()
{
	m_wndAnimate.Stop();
	m_wndAnimate.Seek(0);

}

void CMainFrame::OnOptions()
{
	COptionSheet optionSheet;

	optionSheet.m_Page1.m_bFullRowSelection = theApp.m_bFullRowSelection;
	optionSheet.m_Page1.m_bActionPlay = ( theApp.m_nDoubleClickAction == PLAY );
	optionSheet.m_Page1.m_bActionEnqueue = ( theApp.m_nDoubleClickAction == ENQUEUE );
	optionSheet.m_Page1.m_bActionProperties = ( theApp.m_nDoubleClickAction == PROPERTIES );
	optionSheet.m_Page1.m_nMaxRow = theApp.m_nMaxRow;

	CObArray& columns = GetRightPane()->GetColumns();
	optionSheet.m_Page2.GetColumnsInfos( columns );

	optionSheet.m_Page3.m_strFavoriteAddresses = theApp.m_strFavoriteAddresses;

	optionSheet.m_Page4.m_strCmdPlay = theApp.m_strCmdPlay;
	optionSheet.m_Page4.m_strCmdEnqueue = theApp.m_strCmdEnqueue;
	optionSheet.m_Page4.m_bUseCachedCatalog = theApp.m_bUseCachedCatalog;
	optionSheet.m_Page4.m_bDisableConnection = theApp.m_bDisableConnection;

	optionSheet.m_Page5.LoadFormats();
	theApp.m_nDefaultFormat = theApp.GetProfileInt( SECT_USR, ENTR_FORMAT_DEFAULT, 0 );

	optionSheet.m_Page6.m_strExportDefName = theApp.m_strExportDefName;
	optionSheet.m_Page6.m_strExportDefFile = theApp.m_strExportDefFile;
	optionSheet.m_Page6.m_strExportFile = theApp.m_strExportFile;
	optionSheet.m_Page6.m_bChooseExportFile = theApp.m_bChooseExportFile;
	optionSheet.m_Page6.m_bDefaultExport = theApp.m_bDefaultExport;
	optionSheet.m_Page6.m_bExportReinit = theApp.m_bExportReinit;

	optionSheet.m_Page7.m_bScanMp3 = theApp.m_bScanFileMp3;
	optionSheet.m_Page7.m_bScanM3u = theApp.m_bScanPlaylistM3u;
	optionSheet.m_Page7.m_bScanPls = theApp.m_bScanPlaylistPls;

	// Plugins
	CString strPluginDll;
	if( theApp.m_strPluginDll != "" )
	{
		strPluginDll = theApp.m_strPluginDll;
	}
	else
	{
		strPluginDll = theApp.LoadTranslatedString( "IdsNone" );
	}
	strPluginDll.MakeUpper();
	optionSheet.m_Page8.m_strCurrentDll = strPluginDll;

	// IntelliTag, Normalization
	optionSheet.m_Page9.m_bValidate = theApp.m_bIntelliTagValidate;
	optionSheet.m_Page9.m_bNormalize = theApp.m_bNormalizeTag;
	optionSheet.m_Page9.m_profile = theApp.m_NormeProfile;
		
	// Options page 10 : ID3
	optionSheet.m_Page10.m_bWriteId3v1Tags = theApp.m_bWriteId3v1Tags;
	optionSheet.m_Page10.m_bUseId3v2 = theApp.m_bUseId3v2Tags;
	optionSheet.m_Page10.m_bUseId3v1Only = !theApp.m_bUseId3v2Tags;

	// Options page 11 : Language
	optionSheet.m_Page11.m_strCurrentLanguage = theApp.m_strLanguageFile;

	// Cache le bouton Apply de la fenêtre Properties
	optionSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	if( optionSheet.DoModal() == IDOK )
	{
		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
		// Options de la page1
		theApp.m_bFullRowSelection = optionSheet.m_Page1.m_bFullRowSelection;
		theApp.WriteProfileInt( SECT_USR, ENTR_FULLROWSEL, theApp.m_bFullRowSelection );

		if( optionSheet.m_Page1.m_bActionPlay )
			theApp.m_nDoubleClickAction = PLAY;
		if( optionSheet.m_Page1.m_bActionEnqueue )
			theApp.m_nDoubleClickAction = ENQUEUE;
		if( optionSheet.m_Page1.m_bActionProperties )
			theApp.m_nDoubleClickAction = PROPERTIES;
		theApp.WriteProfileInt( SECT_USR, ENTR_DBLECLICK, theApp.m_nDoubleClickAction );
		theApp.m_nMaxRow = optionSheet.m_Page1.m_nMaxRow;
		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_MAX_ROW, theApp.m_nMaxRow );

		// Options de la page2
		optionSheet.m_Page2.SetColumnsInfos( columns );
		GetRightPane()->SaveColumnsInfos();
		optionSheet.m_Page5.SaveFormats();
		optionSheet.m_Page5.FreeFormats();
		theApp.WriteProfileInt( SECT_USR, ENTR_FORMAT_DEFAULT, theApp.m_nDefaultFormat );

		// Options de la page3
		theApp.m_strFavoriteAddresses = optionSheet.m_Page3.m_strFavoriteAddresses;
		theApp.WriteProfileString( SECT_USR, ENTR_FAVORITE_ADRESSES, theApp.m_strFavoriteAddresses );

		// Options de la page4
		theApp.m_strCmdPlay = optionSheet.m_Page4.m_strCmdPlay;
		theApp.m_strCmdEnqueue = optionSheet.m_Page4.m_strCmdEnqueue;
		theApp.m_bUseCachedCatalog = optionSheet.m_Page4.m_bUseCachedCatalog;
		theApp.m_bDisableConnection = optionSheet.m_Page4.m_bDisableConnection;

		theApp.WriteProfileString( SECT_ADVANCED, ENTR_CMD_PLAY, theApp.m_strCmdPlay );
		theApp.WriteProfileString( SECT_ADVANCED, ENTR_CMD_ENQUEUE, theApp.m_strCmdEnqueue );
		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_CACHED_CATALOG, theApp.m_bUseCachedCatalog );
		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_DISABLE_CONNECTION, theApp.m_bDisableConnection );



		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_SYSTRAY, theApp.m_bSystray );
		InitSystray();


		theApp.m_strExportDefName = optionSheet.m_Page6.m_strExportDefName;
		theApp.m_strExportDefFile = optionSheet.m_Page6.m_strExportDefFile;
		theApp.m_strExportFile = optionSheet.m_Page6.m_strExportFile;
		theApp.m_bChooseExportFile = optionSheet.m_Page6.m_bChooseExportFile;
		theApp.m_bDefaultExport = optionSheet.m_Page6.m_bDefaultExport;
		theApp.m_bExportReinit = optionSheet.m_Page6.m_bExportReinit;
		theApp.WriteProfileString( SECT_EXPORT, ENTR_EXP_DEFINITION_NAME,  theApp.m_strExportDefName );
		theApp.WriteProfileString( SECT_EXPORT, ENTR_EXP_DEFINITION_FILE,  theApp.m_strExportDefFile );
		theApp.WriteProfileString( SECT_EXPORT, ENTR_EXP_FILENAME, theApp.m_strExportFile );
		theApp.WriteProfileInt( SECT_EXPORT, ENTR_EXP_CHOOSEFILE, theApp.m_bChooseExportFile );
		theApp.WriteProfileInt( SECT_EXPORT, ENTR_EXP_DEFAULT_FORMAT, theApp.m_bDefaultExport );
		theApp.WriteProfileInt( SECT_EXPORT, ENTR_EXP_REINIT, theApp.m_bExportReinit );

		BOOL bReScan = FALSE;
		if( theApp.m_bScanFileMp3 != optionSheet.m_Page7.m_bScanMp3 )
		{
			theApp.m_bScanFileMp3 = optionSheet.m_Page7.m_bScanMp3;
			theApp.WriteProfileInt( SECT_FILE, ENTR_FILE_SCAN_MP3, theApp.m_bScanFileMp3 );
			bReScan = TRUE;
		}
		if( theApp.m_bScanPlaylistM3u != optionSheet.m_Page7.m_bScanM3u )
		{
			theApp.m_bScanPlaylistM3u = optionSheet.m_Page7.m_bScanM3u;
			theApp.WriteProfileInt( SECT_PLAYLIST, ENTR_PL_SCAN_M3U, theApp.m_bScanPlaylistM3u );
			bReScan = TRUE;
		}
		if( theApp.m_bScanPlaylistPls != optionSheet.m_Page7.m_bScanPls )
		{
			theApp.m_bScanPlaylistPls = optionSheet.m_Page7.m_bScanPls;
			theApp.WriteProfileInt( SECT_PLAYLIST, ENTR_PL_SCAN_PLS, theApp.m_bScanPlaylistPls );
			bReScan = TRUE;
		}

		// Plugin
		CString strNewPluginDll = optionSheet.m_Page8.m_strCurrentDll;
		strNewPluginDll.MakeUpper();
		if( strNewPluginDll != strPluginDll )
		{
			theApp.m_strPluginDll = optionSheet.m_Page8.m_strCurrentDll;
			theApp.WriteProfileString(  SECT_SETUP , ENTR_RESDLL , theApp.m_strPluginDll );
			// The application should be restarted 
			CString strMessage = theApp.LoadTranslatedString( "IdsRestart" );
			if( MessageBox( strMessage, NULL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
			{
				PostMessage( WM_COMMAND , ID_APP_EXIT , 0 );
				::ShellExecute( m_hWnd , NULL, MP3E_EXE, "", theApp.m_strInstallPath, SW_SHOW );
			}
		}

		// Options de la page 9 : Intellitag
		theApp.m_bIntelliTagValidate = optionSheet.m_Page9.m_bValidate;
		theApp.WriteProfileInt( SECT_TAG, ENTR_INTELLITAG_VALIDATE, theApp.m_bIntelliTagValidate );
		theApp.m_bNormalizeTag = optionSheet.m_Page9.m_bNormalize;
		theApp.WriteProfileInt( SECT_TAG, ENTR_NORMALIZE_TAG, theApp.m_bNormalizeTag );


		theApp.m_NormeProfile =	optionSheet.m_Page9.m_profile;
		theApp.m_NormeProfile.Save();

		// Options page 10 : ID3
		theApp.m_bWriteId3v1Tags = optionSheet.m_Page10.m_bWriteId3v1Tags;
		theApp.m_bUseId3v2Tags = optionSheet.m_Page10.m_bUseId3v2;
		CAudioFile::bWriteId3v1Tag = theApp.m_bWriteId3v1Tags;
		CAudioFile::bReadId3v2Tag = theApp.m_bUseId3v2Tags;
		CAudioFile::bWriteId3v2Tag = theApp.m_bUseId3v2Tags;
		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_WRITE_ID3V1_TAGS, theApp.m_bWriteId3v1Tags );
		theApp.WriteProfileInt( SECT_ADVANCED, ENTR_USE_ID3V2_TAGS, theApp.m_bUseId3v2Tags );

		// Options page 11 : Language
		if( theApp.m_strLanguageFile != optionSheet.m_Page11.m_strCurrentLanguage )
		{
			theApp.m_strLanguageFile = optionSheet.m_Page11.m_strCurrentLanguage;
			theApp.WriteProfileString( SECT_SETUP, ENTR_LANGUAGE , theApp.m_strLanguageFile );
			CString strMessage = theApp.LoadTranslatedString( "IdsRestart" );
			if( MessageBox( strMessage, NULL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
			{
				PostMessage( WM_COMMAND , ID_APP_EXIT , 0 );
				::ShellExecute( m_hWnd , NULL, MP3E_EXE, "", theApp.m_strInstallPath, SW_SHOW );
			}
		}

		pDoc->UpdateAllViews ( NULL , UPDATE_FILE_LIST , NULL );
		if( bReScan )
		{
			pDoc->RebuildSnapshot();
		}

	}
}



void CMainFrame::OnEditSelectAll() 
{
	SetActiveView( (CView*) m_wndViews[RIGHT_VIEW] );
	GetRightPane()->SelectAllRows();
	
}



void CMainFrame::OnResetTreeViewBegin()
{
	StartAnimation();
	GetRightPane()->m_bItemChangedHandling = FALSE;
}

void CMainFrame::OnResetTreeViewEnd()
{
	GetRightPane()->m_bItemChangedHandling = TRUE;
	if( m_bViewerCreated )
	{
		m_dlgViewer.Initialize();
	}
	StopAnimation();
}

void CMainFrame::OnFileUpdateSnapshot() 
{
	// Reconstruit le snapshot
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
	pDoc->RebuildSnapshot();

	
}

void CMainFrame::OnUpdateFileUpdateSnapshot(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( theApp.m_bUseCachedCatalog );
	
}

LONG CMainFrame::OnSystrayNotify ( WPARAM wParam, LPARAM lParam ) 
{
	switch ( lParam )
	{
		case WM_RBUTTONDOWN:
		{
			break ;
		}
		case WM_LBUTTONDOWN:
			m_bHidden = !m_bHidden;
			ShowExplorerWindows();
			break ;
	}
	return ( 0 ) ;


}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if( (theApp.m_bSystray ) && IsIconic())
	{
		ShowWindow( SW_HIDE );
		m_bHidden = TRUE;
	}
	
}

void CMainFrame::InitSystray()
{
	if( (theApp.m_bSystray ) && (m_pTray == NULL ))
	{
		CString strAppName;
		strAppName.LoadString(AFX_IDS_APP_TITLE);
		m_pTray = new CTrayNot ( this, WM_SYSTRAY_NOTIFY, strAppName, m_pTrayIcon ) ;
		m_pTray->SetState ( 0 ) ;
	}

	if( (!theApp.m_bSystray ) && (m_pTray != NULL ))
	{
		delete m_pTray;
		m_pTray = NULL;
	}
}

void CMainFrame::DoNothing()
{
	m_nViewStyle = ( m_nViewStyle + 1 ) % NB_VIEW_STYLES;
	OnViewStyle( g_view_mode[m_nViewStyle]);
	
}

void CMainFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
	if(pNMToolBar->iItem == ID_LISTVIEW_DROPDOWN)
	{
		CMenu menu;
		menu.CreatePopupMenu();
		theApp.CreateDynamicMenu( &menu , _gMenuList , MENU_SIZE , "ListMenu" ); 
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	*pResult = TBDDRET_DEFAULT;
}



void CMainFrame::OnFileBrowseAddress() 
{
	BROWSEINFO bi;
	
	CString strTitle = theApp.LoadTranslatedString( "IdsBrowseFolder" );

	_TCHAR szBuffer[MAX_PATH];
	bi.hwndOwner = m_hWnd; 
    bi.pidlRoot = NULL; 
    bi.pszDisplayName = szBuffer; 
    bi.lpszTitle = strTitle; 
    bi.ulFlags = BIF_RETURNONLYFSDIRS; 
    bi.lpfn = NULL; 
    bi.lParam = NULL; 
    bi.iImage = 0; 

	LPITEMIDLIST pidl;

	if( (pidl = ::SHBrowseForFolder( &bi )) != NULL)
	{
		CString strNewPath;
		if( ::SHGetPathFromIDList( pidl, strNewPath.GetBuffer( MAX_PATH )))
		{	
			strNewPath.ReleaseBuffer();
			if( ScanNewAddress( strNewPath ))
			{
				InsertAddress(strNewPath);
			}
		}
	}
	if ( m_nStatusPane1Width != -1)
	{
		m_bMenuSelect = FALSE;
		m_wndStatusBar.SetPaneInfo(0, 
		m_nStatusPane1ID, m_nStatusPane1Style, m_nStatusPane1Width);
		m_wndStatusBar.SetPaneText(0, m_strStatusPane1Text );
		m_nStatusPane1Width = -1;   // Set it to illegal value
	}
	
}

void CMainFrame::OnFilePlay() 
{
	CView* pView = GetActiveView();
	pView->PostMessage( WM_PLAY );
	
}


void CMainFrame::OnFileEnqueue() 
{
	CView* pView = GetActiveView();
	pView->PostMessage( WM_ENQUEUE );
	
}


void CMainFrame::OnFileDelete() 
{
	CView* pView = GetActiveView();
	pView->PostMessage( WM_DELETE );
	
}


void CMainFrame::OnViewSmallViewer() 
{
	m_bShowSmallViewer = TRUE;
	ShowExplorerWindows();
}

void CMainFrame::OnUpdateViewSmallViewer(CCmdUI* pCmdUI) 
{
	
	
}

void CMainFrame::ShowExplorerWindows()
{

	if( !m_bHidden )
	{
		if( m_bShowSmallViewer )
		{
			ShowWindow( SW_HIDE );
			if( !m_bViewerCreated )
			{
				m_dlgViewer.Create( IDD_SMALL_VIEWER );
				m_bViewerCreated = TRUE;
			}
			int nCmd = m_dlgViewer.IsIconic() ? SW_RESTORE : SW_SHOW;
			m_dlgViewer.ShowWindow( nCmd );
			m_dlgViewer.SetForegroundWindow();
		}
		else
		{
			if( m_bViewerCreated )
			{
				m_dlgViewer.ShowWindow( SW_HIDE );
			}
			int nCmd = IsIconic() ? SW_RESTORE : SW_SHOW;
			ShowWindow( nCmd );
			SetForegroundWindow();
		}
	}
	else
	{
		ShowWindow( SW_HIDE );
		if( m_bViewerCreated )
		{
			m_dlgViewer.ShowWindow( SW_HIDE );
		}
	}
}


void CMainFrame::OnSearch() 
{
	CSearchDlg dlg;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
	dlg.m_pList = pDoc->GetFileList();
	dlg.m_pMapGenre = &pDoc->m_mapGenre;
	dlg.DoModal();
	
}

void CMainFrame::OnViewPlaylist() 
{
	theApp.m_bDisplayPlaylists = !theApp.m_bDisplayPlaylists;

	theApp.BeginWaitCursor();
	LockWindowUpdate();

	// Active une vue permanente avant le remplacement
	SetActiveView( (CView*) m_wndSplitter.GetPane( 0 , 1 ));

	if( theApp.m_bDisplayPlaylists )
	{
		m_wndSplitter.ReplaceView( 1, 0, RUNTIME_CLASS(CPlaylistView), CSize(theApp.m_nCx1, theApp.m_nCy2) );
		m_wndSplitter.ReplaceView( 1, 1, RUNTIME_CLASS(CPlaylistDetailView), CSize(theApp.m_nCx2, theApp.m_nCy2));

	}
	else
	{
		m_wndSplitter.ReplaceView( 1 , 0, RUNTIME_CLASS(CHtmlMenu), CSize(theApp.m_nCx1, theApp.m_nCy2) );
		m_wndSplitter.ReplaceView( 1 , 1, RUNTIME_CLASS(CHtmlFileView), CSize(theApp.m_nCx2, theApp.m_nCy2) );
	}
	m_wndSplitter.RecalcLayout();

	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetActiveDocument();
	pDoc->UpdateLowerPanesViews();

	UnlockWindowUpdate();
	theApp.EndWaitCursor();

}

void CMainFrame::OnUpdateViewPlaylist(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( theApp.m_bDisplayPlaylists );
}


void CMainFrame::UpdateStatusBar(CPlaylist *pPlaylist)
{
	int nItemCount = 0;
	LONG lSize = 0;
	int nTime = 0;

	for( int i = 0 ; i < pPlaylist->m_mp3List.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = pPlaylist->m_mp3List.GetAt(i);
		lSize += pMp3->GetSize();
		nTime += pMp3->GetTime();
		nItemCount++;
	}

	CString strSize = CAudioFile::FormatSize( lSize );
	CString strTime = CAudioFile::FormatTime( nTime );

	CString strTitle;
	
	strTitle = theApp.LoadTranslatedString( "IdsStatusPlaylistPath" ) + pPlaylist->GetFullPath();
	static _TCHAR szStatusText[MAX_STATUS];
	CString strFormat = theApp.LoadTranslatedString( "IdsStatusPlaylistContents" );
	wsprintf( szStatusText, strFormat , nItemCount );
	CString strFileInfo = szStatusText;
	
	SetStatusText( pPlaylist->m_strFilename , strTitle, strFileInfo, strSize, strTime );

}


void CMainFrame::SetFileFound(CString strFileFoundCount)
{
/*	CStatusBarCtrl& ctrlStatus = m_wndStatusBar.GetStatusBarCtrl();

	if( ::IsWindow(ctrlStatus.m_hWnd))
	{
		m_wndStatusBar.SetPaneText( NO_PANE_LEFT , strFileFoundCount );
		m_wndStatusBar.UpdateWindow();
	}
*/
/*
	m_bMenuSelect = FALSE;
	m_wndStatusBar.SetPaneInfo(0, m_nStatusPane1ID, m_nStatusPane1Style, m_nStatusPane1Width);
	m_wndStatusBar.SetPaneText(0, m_strStatusPane1Text );
	m_nStatusPane1Width = -1;   // Set it to illegal value
*/
	SetStatusText( strFileFoundCount , "", "", "", "" );

}
