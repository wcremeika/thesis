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
#include "mp3Exp.h"

#include "Mp3ExpDoc.h"
#include "util/ListVwEx.h"
#include "mp3ExpView.h"
#include "mp3/mp3file.h"
#include "MainFrm.h"
#include "Format.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpView

IMPLEMENT_DYNCREATE(CMp3ExpView, CMp3View)

BEGIN_MESSAGE_MAP(CMp3ExpView, CMp3View)
	//{{AFX_MSG_MAP(CMp3ExpView)
	ON_UPDATE_COMMAND_UI(ID_FILE_EDIT_TAG, OnUpdateFileEditTag)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENAME, OnUpdateFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_COMMAND(ID_FILE_ADD_TO_PLAYLIST, OnFileAddToPlaylist)
	ON_UPDATE_COMMAND_UI(ID_FILE_ADD_TO_PLAYLIST, OnUpdateFileAddToPlaylist)
	ON_MESSAGE( WM_DELETE, OnFileDelete)
	ON_COMMAND(ID_FILE_EDIT_TAG, OnFileProperties)
	ON_COMMAND(ID_FILE_INTELLITAG, OnFileIntellitag)
	ON_UPDATE_COMMAND_UI(ID_FILE_INTELLITAG, OnUpdateFileIntellitag)
	ON_COMMAND(ID_FILE_NORMALIZE, OnFileNormalize)
	ON_UPDATE_COMMAND_UI(ID_FILE_NORMALIZE, OnUpdateFileNormalize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define MENU_SIZE 14


static DYNAMIC_MENU _gMenuFile[MENU_SIZE] =
{ 
	{ MF_STRING , ID_FILE_PLAY , "Play" },
	{ MF_STRING , ID_FILE_ENQUEUE , "Enqueue" },
	{ MF_STRING , ID_FILE_ADD_TO_PLAYLIST , "Playlist" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_INTELLITAG , "IntelliTag" },
	{ MF_STRING , ID_FILE_NORMALIZE , "Normalize" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_EDIT_COPY , "Copy" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_DELETE , "Delete" },
	{ MF_STRING , ID_FILE_RENAME , "Rename" },
	{ MF_STRING , ID_FILE_EXPORT , "Export" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_EDIT_TAG , "Properties" }

};


/////////////////////////////////////////////////////////////////////////////
// CMp3ExpView construction/destruction


CMp3ExpView::CMp3ExpView()
{
	m_menu.CreatePopupMenu();
	theApp.CreateDynamicMenu( &m_menu , _gMenuFile , MENU_SIZE , "FileMenu" );

	// Drag and Drop
	m_ImageFormat = ::RegisterClipboardFormat( CF_MP3_EXPLORER );
}

CMp3ExpView::~CMp3ExpView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMp3ExpView drawing

void CMp3ExpView::OnDraw(CDC* pDC)
{
	CMp3ExpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CMp3ExpView::OnInitialUpdate()
{
	CMp3View::OnInitialUpdate();

	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	ASSERT_VALID(pDoc);

	m_pList = &(pDoc->m_mp3List);
	OnUpdate( NULL, UPDATE_FILE_LIST, NULL );

}



void CMp3ExpView::UpdateRowSelectionMode()
{
	SetFullRowSel( theApp.m_bFullRowSelection );
}


void CMp3ExpView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	switch ( lHint )
	{
	case UPDATE_SET_VIEW_ARTIST :
		m_nLeftViewType = VIEW_ARTIST;
		break;

	case UPDATE_SET_VIEW_GENRE :
		m_nLeftViewType = VIEW_GENRE;
		break;

	case UPDATE_FILE_LIST :
		UpdateRowSelectionMode();
		DefineDisplayedColumns();
		SetDisplayedColumns();
		UpdateList();
		break;

	}
}



/////////////////////////////////////////////////////////////////////////////
// CMp3ExpView diagnostics

#ifdef _DEBUG
void CMp3ExpView::AssertValid() const
{
	CListView::AssertValid();
}

void CMp3ExpView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMp3ExpDoc* CMp3ExpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMp3ExpDoc)));
	return (CMp3ExpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpView message handlers


BOOL CMp3ExpView::SetViewType(DWORD dwViewType)
{
	return(ModifyStyle(LVS_TYPEMASK,dwViewType & LVS_TYPEMASK));

}

DWORD CMp3ExpView::GetViewType()
{
	return(GetStyle() & LVS_TYPEMASK);
}


/////////////////////////////////////////////////////////////////////
// Gestion de l'apparence des boutons

void CMp3ExpView::OnUpdateFileEditTag(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());

}

void CMp3ExpView::OnUpdateFileRename(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}


void CMp3ExpView::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}

void CMp3ExpView::OnUpdateFileAddToPlaylist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}

void CMp3ExpView::OnUpdateFileIntellitag(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}
void CMp3ExpView::OnUpdateFileNormalize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}
/////////////////////////////////////////////////////////////////////
// Gestion des actions sur les fichiers :
//		- AddToPlaylist
//		- Properties
//		- Rename
//		- Delete

void CMp3ExpView::OnFileAddToPlaylist() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFilePlaylist( &mp3list );
	
}

void CMp3ExpView::OnFileProperties() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileProperties( &mp3list );
	if( bUpdateList )
		UpdateList();
	
}


void CMp3ExpView::OnFileRename() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileRename( &mp3list );
	if( bUpdateList )
		UpdateList();
	
}


LRESULT CMp3ExpView::OnFileDelete(WPARAM wParam, LPARAM lParam) 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileDelete( &mp3list );
	return (LRESULT) 0;
}


void CMp3ExpView::OnFileIntellitag() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileIntelliTag( &mp3list );
	if( bUpdateList )
		UpdateList();
	
}


void CMp3ExpView::OnFileNormalize() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileNormalize( &mp3list );
	if( bUpdateList )
		UpdateList();
	
	
}

////////////////////////////////////////////////////////////////////
// Gestion des actions de la souris :
//		- Double-click = Play
//		- Bouton droit = Menu Contextuel

void CMp3ExpView::DefaultAction()
{
	switch( theApp.m_nDoubleClickAction )
	{
	case PLAY :
		OnFilePlay( 0 , 0 );
		break;
	case ENQUEUE :
		OnFileEnqueue( 0 , 0 );
		break;
	case PROPERTIES :
		OnFileProperties();
		break;
	}
}

void CMp3ExpView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	DefaultAction();

	CListView::OnLButtonDblClk(nFlags, point);
}

void CMp3ExpView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar == VK_RETURN )
		DefaultAction();

	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMp3ExpView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nNewSortedCol;

	CColumn* pCol = (CColumn*) m_colsDisplayed.GetAt( pNMListView->iSubItem );
	nNewSortedCol = pCol->m_nInfoIndex;

	if( (nNewSortedCol == m_nSortedCol) || (nNewSortedCol == - m_nSortedCol))
	{
		m_nSortedCol = -m_nSortedCol;
	}
	else
	{
		m_nPrevSortedCol = m_nSortedCol;
		m_nSortedCol = nNewSortedCol;
	}

	CListCtrl& list = GetListCtrl();
	list.SortItems( CompareMp3 , m_nSortedCol );
	theApp.m_nSortingCol = m_nSortedCol;
	theApp.m_nSecondSortingCol = m_nPrevSortedCol;
	*pResult = 0;
}


CObArray& CMp3ExpView::GetColumns()
{
	return m_columns;
}

void CMp3ExpView::SelectAllRows()
{
	CListCtrl& list = GetListCtrl();
	int nIndex = -1;
	m_bItemChangedHandling = FALSE;
	while( (nIndex = list.GetNextItem( nIndex , LVNI_ALL )) != -1 )
	{
		list.SetItemState( nIndex, LVIS_SELECTED , LVIS_SELECTED );
	}
	m_bItemChangedHandling = TRUE;

}


void CMp3ExpView::SaveColumnsInfos()
{
	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn* pCol = (CColumn*) m_columns.GetAt(i);
		static _TCHAR szSection[MAX_PATH];
		wsprintf( szSection , SECT_COLUMNS , pCol->m_nInfoIndex );
		theApp.WriteProfileInt( szSection, ENTR_COL_ALIGN, pCol->m_nAlignemnt);
		theApp.WriteProfileInt( szSection, ENTR_COL_DISPLAY, pCol->m_nDisplayed);
		theApp.WriteProfileInt( szSection, ENTR_COL_ORDER, pCol->m_nInfoOrder);
	}
	theApp.WriteProfileInt( SECT_USR, ENTR_COL_SETTINGS, TRUE );

}


void CMp3ExpView::SaveColumnsSize()
{
	CListCtrl& list = GetListCtrl();

	for( int i = 0 ; i < m_colsDisplayed.GetSize() ; i++ )
	{
		CColumn* pCol = (CColumn*) m_colsDisplayed.GetAt(i);
		pCol->m_nWidth = list.GetColumnWidth( i );

		static _TCHAR szSection[MAX_PATH];
		wsprintf( szSection , SECT_COLUMNS , pCol->m_nInfoIndex );

		theApp.WriteProfileInt( szSection, ENTR_COL_WIDTH, pCol->m_nWidth );
	}
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

void CMp3ExpView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
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

	// Create a dummy object for internal Drag and Drop interne
	CString strCF_PRIVATE = "MP3 Explorer";
    HGLOBAL hgCF_PRIVATEData = GlobalAlloc(GPTR, strCF_PRIVATE.GetLength() + 1);   
    LPSTR   lpCF_PRIVATEData = (LPSTR)GlobalLock(hgCF_PRIVATEData);
                                                 
	lstrcpy(lpCF_PRIVATEData, strCF_PRIVATE);
	GlobalUnlock(hgCF_PRIVATEData);                                                 

	pDataSource->CacheGlobalData( m_ImageFormat, hgCF_PRIVATEData );

	((COleDataSource*)pDataSource)->DoDragDrop( DROPEFFECT_COPY | DROPEFFECT_MOVE, NULL, NULL );
	delete (COleDataSource*)pDataSource;

	*pResult = 0;
}




void CMp3ExpView::ModifyTagInfo(CString strArtist, CString strAlbum, CString strGenre)
{
	CListCtrl& list = GetListCtrl();
	int nIndex = -1;

	CFormat format;
	int nFormat = theApp.GetDefaultFormat();
	BOOL bRename = FALSE;
	if( theApp.m_bAutoRename && nFormat )
	{
		format.Load( nFormat );
		bRename = TRUE;
	}

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CAudioFile *pMp3 = (CAudioFile*) list.GetItemData( nIndex );
		pMp3->ModifyTagInfo( strArtist, strAlbum, strGenre );
		if( bRename )
			pMp3->Rename( format.GetFilename( pMp3 ));
	}
}

CMp3ExpView::UpdateList()
{
	CListCtrl& list = GetListCtrl();
	m_bItemChangedHandling = FALSE;
	CMp3View::UpdateList();
	list.SortItems( CompareMp3 , m_nPrevSortedCol );
	list.SortItems( CompareMp3 , m_nSortedCol );
	m_bItemChangedHandling = TRUE;
}

void CMp3ExpView::FillList(CMp3FileList *pMp3List)
{
		ASSERT(pMp3List->IsKindOf(RUNTIME_CLASS(CMp3FileList)));
		m_pList = pMp3List;
		UpdateList();

}

int CMp3ExpView::GetAllFiles(CMp3FileList *pMp3List)
{
	CListCtrl& list = GetListCtrl();
	
	int nCount = 0;
	int nIndex = -1;
	while( (nIndex = list.GetNextItem( nIndex , LVNI_ALL )) != -1 )
	{
		CAudioFile *pMp3 = (CAudioFile *) list.GetItemData( nIndex );
		pMp3List->Add( pMp3 );
		nCount++;
	}
	return nCount;

}



