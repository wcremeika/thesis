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
#include "mp3exp.h"
#include "mp3/mp3file.h"
#include "mainfrm.h"
#include "mp3expdoc.h"
#include "SmallViewer.h"
#include "util/ListVwEx.h"
#include "mp3view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;


#define VIEWER_ARTIST	0
#define VIEWER_GENRE	1
#define VIEWER_ALBUM	2
#define VIEWER_PLAYLIST	3
#define VIEWER_FILE		4

#define VIEWER_MODE_FILE		0
#define VIEWER_MODE_CATEGORY	1
#define VIEWER_MODE_PLAYLIST	2

#define VIEWER_NB_BUTTONS  7
#define VIEWER_BUTTON_SIZE 26
/////////////////////////////////////////////////////////////////////////////
// CSmallViewer dialog


CSmallViewer::CSmallViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CSmallViewer::IDD, pParent)
{
    // Load the font we want to use.
    m_font.CreateStockObject(ANSI_VAR_FONT);
    // Get the metrics of the font.    
	CDC dc;    
	dc.CreateCompatibleDC(NULL);
    CFont* pfntOld = (CFont*) dc.SelectObject(&m_font);    
	TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);    
	dc.SelectObject(pfntOld);
    m_nFontHeight = tm.tmHeight;    
	m_nFontWidth = tm.tmMaxCharWidth;
    // Load the bitmap we want.    

	CMyImageList img;
	img.Create( 16, 16, IDB_SMALL_ICONS );

	m_bmIcons[ VIEWER_ARTIST ].LoadBitmap( IDB_ARTIST );
	m_bmIcons[ VIEWER_ALBUM ].LoadBitmap( IDB_ALBUM );
	m_bmIcons[ VIEWER_GENRE ].LoadBitmap( IDB_GENRE );
	m_bmIcons[ VIEWER_PLAYLIST ].LoadBitmap( IDB_PLAYLIST );
	m_bmIcons[ VIEWER_FILE ].LoadBitmap( IDB_FILE );

	m_nType = VIEWER_ARTIST;
	m_nMode = VIEWER_MODE_CATEGORY;

	img.Detach();


	//{{AFX_DATA_INIT(CSmallViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSmallViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmallViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmallViewer, CDialog)
	//{{AFX_MSG_MAP(CSmallViewer)
	ON_COMMAND(ID_VIEWER_ALBUM, OnViewerAlbum)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_ALBUM, OnUpdateViewerAlbum)
	ON_COMMAND(ID_VIEWER_ARTIST, OnViewerArtist)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_ARTIST, OnUpdateViewerArtist)
	ON_COMMAND(ID_VIEWER_GENRE, OnViewerGenre)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_GENRE, OnUpdateViewerGenre)
	ON_COMMAND(ID_VIEWER_PLAYLIST, OnViewerPlaylist)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_PLAYLIST, OnUpdateViewerPlaylist)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_LBN_DBLCLK(IDC_VIEWER_LIST, OnDblclkViewerList)
	ON_COMMAND(ID_VIEWER_LARGE_MODE, OnViewerLargeMode)
	ON_COMMAND(ID_VIEWER_PLAY, OnViewerPlay)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_PLAY, OnUpdateViewerPlay)
	ON_COMMAND(ID_VIEWER_ENQUEUE, OnViewerEnqueue)
	ON_UPDATE_COMMAND_UI(ID_VIEWER_ENQUEUE, OnUpdateViewerEnqueue)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmallViewer message handlers


BOOL CSmallViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();


	if (!m_wndToolBar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	// set up toolbar properties
	CMyImageList img;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	img.Create( 18, 18, IDB_HOTVIEWER );
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();
	img.Create( 18, 18, IDB_COLDVIEWER );
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
//	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons( NULL, VIEWER_NB_BUTTONS );

	// set up each toolbar button
	m_wndToolBar.SetButtonInfo(0, ID_VIEWER_PLAY , TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_VIEWER_ENQUEUE , TBSTYLE_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(2, ID_VIEWER_ARTIST , TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(3, ID_VIEWER_GENRE , TBSTYLE_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(4, ID_VIEWER_ALBUM, TBSTYLE_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(5, ID_VIEWER_PLAYLIST , TBSTYLE_BUTTON, 5);
	m_wndToolBar.SetButtonInfo(6, ID_VIEWER_LARGE_MODE , TBSTYLE_BUTTON, 6);

	// set up toolbar button sizes
	CRect rc;
	GetWindowRect( rc );
	CRect rcDesktop;
	SystemParametersInfo( SPI_GETWORKAREA, 0, rcDesktop, 0 ); 
	int cx = rc.right - rc.left;
	int cy = rc.bottom - rc.top;
	SetWindowPos( NULL , rcDesktop.right - cx, rcDesktop.bottom - cy , 0, 0 , SWP_SHOWWINDOW | SWP_NOSIZE); 

	int xToolbar = (( cx - ( VIEWER_NB_BUTTONS * VIEWER_BUTTON_SIZE )) / 2);
	int yToolbar = 3 ;
	m_wndToolBar.SetWindowPos( NULL, xToolbar , yToolbar , cx - 8, VIEWER_BUTTON_SIZE, SWP_SHOWWINDOW );
	m_wndToolBar.SetSizes( CSize(26,26), CSize(18,18));

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_SIZE_FIXED);

	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSmallViewer::OnViewerAlbum() 
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc *) pFrame->GetActiveDocument();

	FillCategory( &pDoc->m_mapAlbum, VIEWER_ALBUM );
	
}

void CSmallViewer::OnUpdateViewerAlbum(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSmallViewer::OnViewerArtist() 
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc *) pFrame->GetActiveDocument();

	FillCategory( &pDoc->m_mapArtist, VIEWER_ARTIST );
}

void CSmallViewer::OnUpdateViewerArtist(CCmdUI* pCmdUI) 
{
	
}

void CSmallViewer::OnViewerGenre() 
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc *) pFrame->GetActiveDocument();

	FillCategory( &pDoc->m_mapGenre, VIEWER_GENRE );
	
}

void CSmallViewer::OnUpdateViewerGenre(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSmallViewer::OnViewerPlaylist() 
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc *) pFrame->GetActiveDocument();

	FillPlaylist( pDoc->GetPlaylistList() );
}

void CSmallViewer::OnUpdateViewerPlaylist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSmallViewer::FillCategory(CMapStringToOb *pMap, int nType)
{
	CListBox *pListBox = (CListBox *) GetDlgItem( IDC_VIEWER_LIST );


	m_nType = nType;
	m_nMode = VIEWER_MODE_CATEGORY;

	pListBox->ResetContent();

	POSITION pos = pMap->GetStartPosition();
	while( pos != NULL )
	{
		CCategory *pCategory;
		CString strCategory;
		pMap->GetNextAssoc( pos, strCategory, (CObject *&) pCategory );
		int nIndex = pListBox->AddString( pCategory->GetName());
		pListBox->SetItemData( nIndex, (DWORD) pCategory );
	}
	pListBox->SetCurSel( 0 );

}

void CSmallViewer::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if( nIDCtl == IDC_VIEWER_LIST )
	{

		lpMeasureItemStruct->itemHeight = max(m_nFontHeight,
                                          m_bmIcons[ m_nType ].GetHeight());	
	}
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CSmallViewer::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( nIDCtl == IDC_VIEWER_LIST )
	{

	    HFONT hfntOld;
	    CRect rcRow;
	    CRect rcText;

		rcRow = lpDrawItemStruct->rcItem; 
		rcText = lpDrawItemStruct->rcItem; 
		rcText.left = lpDrawItemStruct->rcItem.left + m_bmIcons[ m_nType ].GetWidth() + 2;

		CString strItemText;
		CCategory* pCategory;
		CPlaylist* pPlaylist;
		CAudioFile* pAudioFile;

		switch( m_nMode )
		{
		case VIEWER_MODE_CATEGORY:
			pCategory = (CCategory*) lpDrawItemStruct->itemData;
			if( pCategory == NULL )
			{
				CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
				return;
			}
			strItemText = pCategory->GetName();
			break;

		case VIEWER_MODE_PLAYLIST:
			pPlaylist = (CPlaylist*) lpDrawItemStruct->itemData;
			if( pPlaylist == NULL )
			{
				CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
				return;
			}
			strItemText = pPlaylist->GetName();
			break;

		case VIEWER_MODE_FILE:
			pAudioFile = (CAudioFile*) lpDrawItemStruct->itemData;
			if( pAudioFile == NULL )
			{
				CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
				return;
			}
			strItemText = pAudioFile->GetShortInfo();
			break;
		}


		switch (lpDrawItemStruct->itemAction) 
		{
		case ODA_DRAWENTIRE:        // Draw the whole line of information.
			// Check if we need to show selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
	        // Set up the font we want to use.
		    hfntOld = (HFONT) ::SelectObject(lpDrawItemStruct->hDC, m_font.m_hObject);
			// Erase the entire area.
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			// Draw the bitmap in place.
			m_bmIcons[ m_nType ].DrawTrans(lpDrawItemStruct->hDC, rcRow.left, rcRow.top);
	        // Check if we need to show focus state.
		    if (lpDrawItemStruct->itemState & ODS_FOCUS) 
			{
				::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			}
			// Move the text over to just beyond the bitmap.
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, strItemText, -1,  &rcText, DT_LEFT | DT_VCENTER);
			::SelectObject(lpDrawItemStruct->hDC, hfntOld);        
			break;    

		case ODA_FOCUS:
		    // Toggle the focus state.
			::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			break;

		case ODA_SELECT:        // Toggle the selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, strItemText, -1,  &rcText, DT_LEFT | DT_VCENTER);
			break;    
	
		default:
		    break;    
		}
	}
	else
	{
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

void CSmallViewer::OnDblclkViewerList() 
{

	CListBox *pListBox = (CListBox *) GetDlgItem( IDC_VIEWER_LIST );
	int nIndex = pListBox->GetCurSel();
	if( m_nMode == VIEWER_MODE_CATEGORY )
	{
		CCategory* pCategory = (CCategory *) pListBox->GetItemData( nIndex );
		m_nType = VIEWER_FILE;
		m_nMode = VIEWER_MODE_FILE;

		pListBox->ResetContent();
		for( int i = 0 ; i < pCategory->GetFileList()->GetSize() ; i++ )
		{
			CAudioFile *pAudioFile = (CAudioFile *) pCategory->GetFileList()->GetAt(i);
			int nIndex = pListBox->AddString( pAudioFile->GetShortInfo());
			pListBox->SetItemData( nIndex, (DWORD) pAudioFile );
		}
		pListBox->SetCurSel( 0 );
	} 
	else if ( m_nMode == VIEWER_MODE_FILE )
	{
		CAudioFile* pAudioFile = (CAudioFile *) pListBox->GetItemData( nIndex );
		int nAction = ( theApp.m_nDoubleClickAction == ENQUEUE ) ? ENQUEUE : PLAY;
		CMp3FileList filelist;
		filelist.Add( pAudioFile );
		CMp3View::DoFileAction( this , nAction , &filelist ); 

	}
	else if ( m_nMode == VIEWER_MODE_PLAYLIST )
	{
		CPlaylist* pPlaylist = (CPlaylist *) pListBox->GetItemData( nIndex );
		m_nType = VIEWER_FILE;
		m_nMode = VIEWER_MODE_FILE;

		pListBox->ResetContent();
		for( int i = 0 ; i < pPlaylist->m_mp3List.GetSize() ; i++ )
		{
			CAudioFile *pAudioFile = (CAudioFile *) pPlaylist->m_mp3List.GetAt(i);
			int nIndex = pListBox->AddString( pAudioFile->GetShortInfo() );
			pListBox->SetItemData( nIndex, (DWORD) pAudioFile );
		}
		pListBox->SetCurSel( 0 );

	}


}

void CSmallViewer::FillPlaylist(CPlaylistList *pList)
{
	CListBox *pListBox = (CListBox *) GetDlgItem( IDC_VIEWER_LIST );


	m_nType = VIEWER_PLAYLIST;
	m_nMode = VIEWER_MODE_PLAYLIST;

	pListBox->ResetContent();
	for( int i = 0 ; i < pList->GetSize() ; i++ )
	{
		CPlaylist *pPlaylist = (CPlaylist *) pList->GetAt(i);
		int nIndex = pListBox->AddString( pPlaylist->GetName());
		pListBox->SetItemData( nIndex, (DWORD) pPlaylist );
	}
	pListBox->SetCurSel( 0 );

}


void CSmallViewer::OnViewerLargeMode() 
{
		CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
		pFrame->m_bShowSmallViewer = FALSE;
		pFrame->ShowExplorerWindows();
	
}



void CSmallViewer::OnViewerPlay() 
{
	DoViewerAction(PLAY);
	
}

void CSmallViewer::OnUpdateViewerPlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSmallViewer::OnViewerEnqueue() 
{
	DoViewerAction(ENQUEUE);
}

void CSmallViewer::OnUpdateViewerEnqueue(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSmallViewer::DoViewerAction(int nAction)
{
	CListBox *pListBox = (CListBox *) GetDlgItem( IDC_VIEWER_LIST );
	int nIndex = pListBox->GetCurSel();

	if( nIndex == LB_ERR )
	{
		return;
	}
	CCategory*		pCategory;
	CPlaylist*		pPlaylist;
	CAudioFile*		pAudioFile;
	CMp3FileList*	pAudioFileList;
	CMp3FileList	filelist;

	switch( m_nMode )
	{
	case VIEWER_MODE_CATEGORY:
		pCategory = (CCategory*) pListBox->GetItemData( nIndex );
		pAudioFileList = pCategory->GetFileList();
		break;

	case VIEWER_MODE_PLAYLIST:
		pPlaylist = (CPlaylist*) pListBox->GetItemData( nIndex );
		pAudioFileList = &pPlaylist->m_mp3List;
		break;

	case VIEWER_MODE_FILE:
		pAudioFile = (CAudioFile*) pListBox->GetItemData( nIndex );
		filelist.Add( pAudioFile );
		pAudioFileList = &filelist;
		break;
	}

	CMp3View::DoFileAction( this , nAction , pAudioFileList );

}

void CSmallViewer::OnClose() 
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	pFrame->PostMessage( WM_COMMAND , ID_APP_EXIT , 0 );
	
	CDialog::OnClose();
}


void CSmallViewer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( (theApp.m_bSystray ) && IsIconic())
	{
		ShowWindow( SW_HIDE );
		CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
		pFrame->m_bHidden = TRUE;
	}
	
}


BOOL CSmallViewer::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if( wParam == IDOK )
	{
		OnDblclkViewerList();
		return TRUE;
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CSmallViewer::Initialize()
{
	CMainFrame* pFrame = (CMainFrame *) theApp.m_pMainWnd;
	switch( pFrame->GetViewType() )
	{
	case VIEW_ARTIST:
		OnViewerArtist();
		break;
	case VIEW_GENRE:
		OnViewerGenre();
		break;
	case VIEW_ALBUM:
		OnViewerAlbum();
		break;
	}


}
