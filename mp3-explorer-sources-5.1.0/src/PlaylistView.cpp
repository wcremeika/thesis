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
#include "util/ListVwEx.h"
#include "Mp3View.h"
#include "PlaylistView.h"
#include "MainFrm.h"
#include "mp3ExpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_STRING 100
#define NUM_PLAYLIST_INFO	2
#define NUM_ITEMS	1

#define MENU_SIZE 9

typedef struct playlist {
	_TCHAR szColTitle[MAX_STRING];
	_TCHAR szTitleResName[MAX_STRING];
	int nInfoOrder;
	int nInfoIndex;
	int nAlignemnt;
	int nWidth;

} PL_COLUMNINFO;


static PL_COLUMNINFO g_playlist_columns[]= {
	{ "", "IdsPlaylistFile",			1,	FILENAME,	LVCFMT_LEFT,	150 },
	{ "", "IdsLocation",		2,	LOCATION,	LVCFMT_LEFT,	150 },
};

static DYNAMIC_MENU _gMenuPlaylist[MENU_SIZE] =
{ 
	{ MF_STRING , ID_FILE_PLAY , "Play" },
	{ MF_STRING , ID_FILE_ENQUEUE , "Enqueue" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_PLAYLIST_RESET , "Reset" },
	{ MF_STRING , ID_PLAYLIST_SHUFFLE , "Shuffle" },
	{ MF_STRING , ID_FILE_DELETE , "Delete" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_PLAYLIST_SAVE , "Save" },
	{ MF_STRING , ID_PLAYLIST_SAVEAS , "SaveAs" }

};

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlaylistView

IMPLEMENT_DYNCREATE(CPlaylistView, CListViewEx)

CPlaylistView::CPlaylistView()
{
	m_bFullRowSel = TRUE;
	m_nSortedCol = theApp.m_nPlaylistSortingCol;
	m_bInitialUpdateDone = FALSE;

	SetColors( theApp.m_colorViewText, 
		theApp.m_colorViewTextBk, 
		theApp.m_colorViewBackground, 
		theApp.m_colorViewSelectBackground,
		theApp.m_colorViewSelectText);

}

CPlaylistView::~CPlaylistView()
{
}


BEGIN_MESSAGE_MAP(CPlaylistView, CListViewEx)
	//{{AFX_MSG_MAP(CPlaylistView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PLAYLIST_SAVE, OnPlaylistSave)
	ON_UPDATE_COMMAND_UI(ID_PLAYLIST_SAVE, OnUpdatePlaylistSave)
	ON_COMMAND(ID_PLAYLIST_SAVEAS, OnPlaylistSaveas)
	ON_UPDATE_COMMAND_UI(ID_PLAYLIST_SAVEAS, OnUpdatePlaylistSaveas)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_PLAY, OnPlaylistPlay)
	ON_MESSAGE(WM_ENQUEUE, OnPlaylistEnqueue)
	ON_MESSAGE( WM_DELETE, OnPlaylistDelete)
	ON_COMMAND(ID_PLAYLIST_SHUFFLE, OnPlaylistShuffle)
	ON_COMMAND(ID_PLAYLIST_RESET, OnPlaylistReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaylistView drawing

void CPlaylistView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPlaylistView diagnostics

#ifdef _DEBUG
void CPlaylistView::AssertValid() const
{
	CListView::AssertValid();
}

void CPlaylistView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlaylistView message handlers

void CPlaylistView::OnInitialUpdate() 
{
	if( m_bInitialUpdateDone )
		return;
	
	CListView::OnInitialUpdate();
	
//	ModifyStyle( 0, LVS_SHOWSELALWAYS | LVS_REPORT /*| LVS_SORTASCENDING*/ | LVS_SINGLESEL );
		// Récupère la référence du LV control associé à la view
	CListCtrl& list = GetListCtrl();

	// Définit les ImageLists associées
	m_LargeImageList.Create( 32, 32, IDB_LARGEICONS );
	m_SmallImageList.Create( 16, 16, IDB_SMALL_ICONS );

	m_LargeImageList.SetOverlayImage(NUM_ITEMS, 1);
	m_SmallImageList.SetOverlayImage(NUM_ITEMS, 1);

	list.SetImageList(&m_LargeImageList, LVSIL_NORMAL);
	list.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	m_bInitialUpdateDone = TRUE;

}

void CPlaylistView::InitColumns()
{
	// Récupère la référence du LV control associé à la view
	CListCtrl& list = GetListCtrl();

	// Insert les colonnes
	LV_COLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	for( int i = 0 ; i < NUM_PLAYLIST_INFO ; i++ )
	{
		lstrcpy( g_playlist_columns[i].szColTitle , theApp.LoadTranslatedString( g_playlist_columns[i].szTitleResName ));
		lvc.iSubItem = i;
		lvc.pszText = g_playlist_columns[i].szColTitle;
		lvc.cx = g_playlist_columns[i].nWidth;
		lvc.fmt = g_playlist_columns[i].nAlignemnt;
		list.InsertColumn(i,&lvc);
	}
		
}

void CPlaylistView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();


	if( lHint == UPDATE_PLAYLIST_VIEW )
	{

		InitColumns();
		UpdateList();
		m_bValidList = TRUE;
		SelectPlaylist( pDoc->m_pPlaylistSelected );
	}
}


void CPlaylistView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.CreatePopupMenu();
	theApp.CreateDynamicMenu( &menu , _gMenuPlaylist , MENU_SIZE , "PlaylistMenu" );
		
	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, theApp.m_pMainWnd, NULL ); 
	
}

LRESULT CPlaylistView::OnPlaylistEnqueue(WPARAM wParam, LPARAM lParam) 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	CMp3View::DoFileAction( this, ENQUEUE , &(pDoc->m_pPlaylistSelected->m_mp3List) );
	return (LRESULT) 0;
	
}


LRESULT CPlaylistView::OnPlaylistPlay(WPARAM wParam, LPARAM lParam) 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	CMp3View::DoFileAction( this, PLAY , &(pDoc->m_pPlaylistSelected->m_mp3List));
	return (LRESULT) 0;
}


void CPlaylistView::OnPlaylistSave() 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->SavePlaylist( FALSE );
}

void CPlaylistView::OnUpdatePlaylistSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( IsSelected() );
	
}

void CPlaylistView::OnPlaylistSaveas() 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->SavePlaylist( TRUE );
}

void CPlaylistView::OnUpdatePlaylistSaveas(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( IsSelected() );
	
}

BOOL CPlaylistView::IsSelected()
{
	CListCtrl& list = GetListCtrl();
	return( list.GetSelectedCount() > 0 );
}

void CPlaylistView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( !m_bValidList || !m_bItemChangedHandling )
	{
		return;
	}


	CListCtrl& list = GetListCtrl();
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	int nIndex = -1;
	if( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		pDoc->UpdatePlaylistSelection( (CPlaylist*) list.GetItemData( nIndex ));
	}


	*pResult = 0;
}

BOOL CPlaylistView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SINGLESEL;
	
	return CListViewEx::PreCreateWindow(cs);
}

LRESULT CPlaylistView::OnPlaylistDelete(WPARAM wParam, LPARAM lParam) 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->PlaylistDelete();
	return (LRESULT) 0;
	
}


void CPlaylistView::UpdateList()
{
		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
		CListCtrl& list = GetListCtrl();
		m_bItemChangedHandling = FALSE;

		list.DeleteAllItems();

		LV_ITEM lvi;
		int i = 0;

		//Paramètres
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iImage = ICON_PLAYLIST;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		static _TCHAR buffer[MAX_STRING];

		lvi.iSubItem = 0;
		lvi.pszText = buffer;
		list.InsertItem( &lvi );
		list.SetItemData( i , (DWORD) &pDoc->m_playlistMemory );
		list.SetItemText( i , 0 , pDoc->m_playlistMemory.m_strFilename );
		list.SetItemText( i , 1 , pDoc->m_playlistMemory.m_strLocation );

		int nPlaylistCount = pDoc->GetPlaylistCount();

		for( i = 0 ; i < nPlaylistCount ; i++ )
		{
			//Paramètres
			lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			lvi.iItem = i + 1;
			lvi.iImage = ICON_PLAYLIST;
			lvi.stateMask = LVIS_STATEIMAGEMASK;
			lvi.state = INDEXTOSTATEIMAGEMASK(1);

			static _TCHAR buffer[MAX_STRING];
			CPlaylist *pPlaylist = (CPlaylist *) pDoc->GetPlaylist(i);

			lvi.iSubItem = 0;
			lvi.pszText = buffer;
			list.InsertItem( &lvi );
			list.SetItemData( i + 1 , (DWORD) pPlaylist );
			list.SetItemText( i + 1 , 0 , pPlaylist->m_strFilename );
			list.SetItemText( i + 1 , 1 , pPlaylist->m_strLocation );
		}
		list.SortItems( ComparePlaylist, m_nSortedCol );
		m_bItemChangedHandling = TRUE;

}

void CPlaylistView::SelectPlaylist( CPlaylist* pPlaylistToSelect )
{
	CListCtrl& list = GetListCtrl();
	int nIndex = -1;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_ALL )) != -1 )
	{
		CPlaylist* pPlaylist = (CPlaylist*) list.GetItemData( nIndex );
		if( pPlaylist == pPlaylistToSelect )
		{
			list.SetItemState( nIndex , LVNI_SELECTED , LVNI_SELECTED );
			return;
		}
	}

}

void CPlaylistView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnPlaylistPlay( 0 , 0 );
	CListView::OnLButtonDblClk(nFlags, point);
}

void CPlaylistView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nNewSortedCol;

	if( pNMListView->iSubItem != 0 )
	{
		nNewSortedCol = LOCATION;
	}
	else
	{
		nNewSortedCol = FILENAME;
	}

	if( (nNewSortedCol == m_nSortedCol) || (nNewSortedCol == - m_nSortedCol))
	{
		m_nSortedCol = -m_nSortedCol;
	}
	else
	{
		m_nSortedCol = nNewSortedCol;
	}

	CListCtrl& list = GetListCtrl();
	list.SortItems( ComparePlaylist , m_nSortedCol );
	theApp.m_nPlaylistSortingCol = m_nSortedCol;
	*pResult = 0;
}


void CPlaylistView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_RETURN )
	{
		OnPlaylistPlay( 0 , 0 );
	}
	
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags);

}


void CPlaylistView::OnPlaylistShuffle() 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->PlaylistShuffle();
	
}

void CPlaylistView::OnPlaylistReset() 
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->PlaylistReset();
	
}

