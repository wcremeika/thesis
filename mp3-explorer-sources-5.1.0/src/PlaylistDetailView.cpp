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
#include "MainFrm.h"
#include "Mp3ExpDoc.h"
#include "util/ListVwEx.h"
#include "Mp3ExpView.h"
#include "PlaylistDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENU_SIZE 11


static DYNAMIC_MENU _gMenuDetail[MENU_SIZE] =
{ 
	{ MF_STRING , ID_FILE_PLAY , "Play" },
	{ MF_STRING , ID_FILE_ENQUEUE , "Enqueue" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_DELETE , "Delete" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_DETAIL_UP , "MoveUp" },
	{ MF_STRING , ID_DETAIL_DOWN , "MoveDown" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_EDIT_COPY , "Copy" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_EXPORT , "Export" }

};

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDetailView

IMPLEMENT_DYNCREATE(CPlaylistDetailView, CMp3View)

CPlaylistDetailView::CPlaylistDetailView()
{
	m_bInitialUpdateDone = FALSE;

	m_menu.CreatePopupMenu();
	theApp.CreateDynamicMenu( &m_menu, _gMenuDetail , MENU_SIZE , "PlayListDetailMenu" );
}

CPlaylistDetailView::~CPlaylistDetailView()
{
}


BEGIN_MESSAGE_MAP(CPlaylistDetailView, CMp3View)
	//{{AFX_MSG_MAP(CPlaylistDetailView)
	ON_COMMAND(ID_DETAIL_DOWN, OnDetailDown)
	ON_COMMAND(ID_DETAIL_UP, OnDetailUp)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_MESSAGE(WM_DELETE, OnDetailDelete)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDetailView drawing

void CPlaylistDetailView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDetailView diagnostics

#ifdef _DEBUG
void CPlaylistDetailView::AssertValid() const
{
	CListView::AssertValid();
}

void CPlaylistDetailView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlaylistDetailView message handlers


void CPlaylistDetailView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if( lHint == UPDATE_DETAIL_VIEW )
	{
		DefineDisplayedColumns();
		SetDisplayedColumns();
		UpdatePlaylistDetailList();
	}

}

void CPlaylistDetailView::UpdatePlaylistDetailList()
{
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();

	if( pDoc->m_pPlaylistSelected != NULL )
	{
		m_pList = &(pDoc->m_pPlaylistSelected->m_mp3List);
		UpdateList();
	}
}

void CPlaylistDetailView::DragOverUpdate( CPoint point )
{
 	CListCtrl& list = GetListCtrl();
	UINT nFlags;
 	int nItem = list.HitTest( point , &nFlags );
 	if( (nItem != -1 ) && (nItem != m_nItemDropHilited))
 	{
		if( m_nItemDropHilited != -1 )
		{
			list.SetItemState( m_nItemDropHilited, 0, LVIS_DROPHILITED);
		}
		m_nItemDropHilited = nItem;
		int nPrevItem = list.GetNextItem( nItem , LVNI_ABOVE );
		if( nPrevItem != -1 )
		{
			list.EnsureVisible( nPrevItem , FALSE );
		}
		int nNextItem = list.GetNextItem( nItem , LVNI_BELOW );
		if( nNextItem != -1 )
		{
			list.EnsureVisible( nNextItem , FALSE );
		}
		list.SetItemState( nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
 	}

}

void CPlaylistDetailView::DragOverEnd()
{

 	CListCtrl& list = GetListCtrl();
	list.SetItemState( m_nItemDropHilited, FALSE, LVIS_DROPHILITED);
	if( m_nItemDropHilited != NULL )
	{
		list.SetItemState( m_nItemDropHilited, 0, LVIS_DROPHILITED);
	}
	m_nItemDropHilited = NULL;

}

void CPlaylistDetailView::DragEnter()
{
 	CListCtrl& list = GetListCtrl();
	m_nItemDropHilited = NULL;

}


LRESULT CPlaylistDetailView::OnDetailDelete(WPARAM wParam, LPARAM lParam) 
{
 	CListCtrl& list = GetListCtrl();
	int nIndex = -1;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CMp3File *pMp3 = (CMp3File*) list.GetItemData( nIndex );
		m_pList->DeleteFile( pMp3 );
	}
	UpdateList();
	return (LRESULT) 0;
}

void CPlaylistDetailView::OnDetailDown() 
{
 	CListCtrl& list = GetListCtrl();
	int nIndex = -1;
	int nFirstIndex = -1;
	BOOL bMove;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CMp3File *pMp3 = (CMp3File*) list.GetItemData( nIndex );
		bMove = m_pList->MoveDown( pMp3 );
		if( !bMove )
		{
			return;
		}
		if( nFirstIndex == -1 )
		{
			nFirstIndex = nIndex + 1;
		}
	}
	UpdateList();
	list.SetItemState( nFirstIndex , LVNI_SELECTED , LVNI_SELECTED );
	list.EnsureVisible( nFirstIndex , TRUE );
	
}

void CPlaylistDetailView::OnDetailUp() 
{
 	CListCtrl& list = GetListCtrl();
	int nIndex = -1;
	int nFirstIndex = -1;
	BOOL bMove;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CMp3File *pMp3 = (CMp3File*) list.GetItemData( nIndex );
		bMove = m_pList->MoveUp( pMp3 );
		if( !bMove )
		{
			return;
		}
		if( nFirstIndex == -1 )
		{
			nFirstIndex = nIndex - 1;
		}
	}
	UpdateList();
	list.SetItemState( nFirstIndex , LVNI_SELECTED , LVNI_SELECTED );
	list.EnsureVisible( nFirstIndex , TRUE );
	
}

CPlaylistDetailView::UpdateList()
{
	m_bItemChangedHandling = FALSE;
	CMp3View::UpdateList();
	m_bItemChangedHandling = TRUE;
}


void CPlaylistDetailView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_DELETE )
	{
		OnDetailDelete( 0 , 0 );
	}
	
	CMp3View::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CPlaylistDetailView::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
 	pCmdUI->Enable(IsSelected());	
}

void CPlaylistDetailView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnFilePlay( 0 , 0 );	
	CMp3View::OnLButtonDblClk(nFlags, point);
}



void CPlaylistDetailView::OnInitialUpdate() 
{
	if( m_bInitialUpdateDone )
		return;

	CMp3View::OnInitialUpdate();
	
	// Drag and Drop
	m_ImageFormat = ::RegisterClipboardFormat( CF_MP3_EXPLORER );
	// Drag and Drop
	m_OleDropTarget.Register(this);
	
	m_bInitialUpdateDone = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Drag and Drop message handlers

/////////////////////////////////////////////////////////////////////////////
// OnBeginDrag will be called in response to the LVN_BEGINDRAG message.
// This is the same place that a common control would normally handle 
// beginning a drag drop operation.  Basically what we do here is load 
// up a COleDataSource and post a message for the drag drop operation 
// to be handled.  Posting the message insures that the common control 
// can "normalize" before the Drag Drop operation is started.

void CPlaylistDetailView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// allocate our data object
	COleDataSource* pDataSource;
	pDataSource = new COleDataSource;

	// Create a CF_TEXT global object 
	HGLOBAL hgCF_TEXTData = GetHTextData();
	pDataSource->CacheGlobalData( CF_TEXT, hgCF_TEXTData );

	// Create an  HDROP object
	HGLOBAL hgCF_HDROPData = GetHDropData();
	pDataSource->CacheGlobalData( CF_HDROP, hgCF_HDROPData );

	((COleDataSource*)pDataSource)->DoDragDrop( DROPEFFECT_COPY | DROPEFFECT_MOVE, NULL, NULL );
	delete (COleDataSource*)pDataSource;

	*pResult = 0;
}

void CPlaylistDetailView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CColumn* pCol = (CColumn*) m_colsDisplayed.GetAt( pNMListView->iSubItem );
	int nSortInfo = pCol->m_nInfoIndex;

	CListCtrl& list = GetListCtrl();
	list.SortItems( CompareMp3 , nSortInfo );
	*pResult = 0;
}
