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
#include "LeftView.h"
#include "DlgFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MENU_SIZE 5


static DYNAMIC_MENU _gMenuTree[MENU_SIZE] =
{ 
	{ MF_STRING , ID_FILE_PLAY , "PlayAll" },
	{ MF_STRING , ID_FILE_ENQUEUE , "EnqueueAll" },
	{ MF_STRING , ID_FILE_PLAYLIST_ALL , "Playlist" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_EXPORT , "ExportAll" }

};

extern CMp3ExpApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_FILE_EXPORT_ALL, OnFileExportAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_ALL, OnUpdateFileExportAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_MESSAGE( WM_PLAY, OnFilePlayAll)
	ON_MESSAGE( WM_ENQUEUE, OnFileEnqueueAll)
	ON_COMMAND(ID_FILE_PLAYLIST_ALL, OnFilePlaylistAll)
	ON_COMMAND(ID_FINDFIRST, OnFindfirst)
	ON_UPDATE_COMMAND_UI(ID_FINDFIRST, OnUpdateFindfirst)
	ON_COMMAND(ID_FINDNEXT, OnFindnext)
	ON_UPDATE_COMMAND_UI(ID_FINDNEXT, OnUpdateFindnext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	m_nViewType = theApp.m_nViewType;
	m_bResetInProgress = FALSE;
	m_hLastFound = NULL;
	m_strFindText = theApp.m_strLastFindString;

}

CLeftView::~CLeftView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CMp3ExpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}


void CLeftView::OnInitialUpdate()
{
	CTreeCtrl& tree = GetTreeCtrl();

	tree.ModifyStyle( 0 , TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS );
	m_ImageList.Create( 16, 16, IDB_SMALL_ICONS );
	tree.SetImageList( &m_ImageList, LVSIL_NORMAL);

	// Drag and Drop
	m_OleDropTarget.Register(this);

	// Setup colors
	tree.SetBkColor( theApp.m_colorTreeBackground );
	tree.SetTextColor( theApp.m_colorTreeText );
	tree.SetInsertMarkColor( theApp.m_colorViewSelectBackground );

//	CTreeView::OnInitialUpdate(); // Appel à OnUpdate inutile

}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CMp3ExpDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMp3ExpDoc)));
	return (CMp3ExpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::SetStatusInfos( CString strItemText , CMp3FileList* pMp3List )
{
	// Mise à jour de la StatusBar de la MainFrame
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();

	// - Nombre de fichiers
	static _TCHAR  szStatusText[MAX_STATUS];
	CString strFormat = theApp.LoadTranslatedString( "IdsStatusFiles" );
	wsprintf( szStatusText, strFormat , pMp3List->GetSize());
	// - Taille
	LONG lSize = 0;
	int nTime = 0;
	for( int i = 0 ; i < pMp3List->GetSize() ; i++ )
	{
		CAudioFile *pMp3 = pMp3List->GetAt(i);
		lSize += pMp3->GetSize();
		nTime += pMp3->GetTime();
	}
	CString strSize = CAudioFile::FormatSize( lSize );
	CString strTime = CAudioFile::FormatTime( nTime );
	// Modifie la StatusBar
	pMainFrame->SetStatusText( strItemText , "" , szStatusText, strSize, strTime );
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	CMp3ExpDoc* pDoc = GetDocument();
	CTreeCtrl& tree = GetTreeCtrl();
	
	_TREEITEM *pItem = tree.GetSelectedItem();
	if( (pItem != NULL) && !m_bResetInProgress )
	{
		CMp3FileList* pMp3List = (CMp3FileList*) tree.GetItemData( pItem );
//		pDoc->UpdateAllViews ( this , UPDATE_FILE_LIST , pMp3List );
		CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
		(pMainFrame->GetRightPane())->FillList( pMp3List );
		SetStatusInfos( tree.GetItemText( pItem ), pMp3List );

	}
}

void CLeftView::FillTree( CTreeCtrl& tree )
{
	CMp3ExpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int nImage;
	CString strRoot;

	pDoc->GetPathInfos( strRoot, nImage );

	m_hitemRoot = tree.InsertItem( strRoot , nImage , nImage );
	tree.SetItemData( m_hitemRoot, (DWORD) pDoc->GetFileList());

	switch( m_nViewType )
	{
		case VIEW_ARTIST:
		{
			POSITION posArtist = pDoc->m_mapArtist.GetStartPosition();
			while( posArtist != NULL )
			{
				CArtist* pArtist;
				CString strArtist;
				pDoc->m_mapArtist.GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
				ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
				HTREEITEM hItem = tree.InsertItem( pArtist->GetName(), ICON_ARTIST , ICON_ARTIST, m_hitemRoot, TVI_SORT );
				tree.SetItemData( hItem, (DWORD) pArtist->GetFileList());

				POSITION pos = pArtist->m_mapAlbum.GetStartPosition( );
				while( pos != NULL )
				{
					CAlbum* pAlbum;
					CString strAlbum;
					pArtist->m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
					ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
					HTREEITEM hSubItem = tree.InsertItem( strAlbum, ICON_ALBUM , ICON_ALBUM, hItem, TVI_SORT );
					tree.SetItemData( hSubItem, (DWORD) pAlbum->GetFileList());
				}

			
			}
		}
		break;

		case VIEW_GENRE:
		{
			POSITION posGenre = pDoc->m_mapGenre.GetStartPosition();
			while( posGenre != NULL )
			{
				CGenre* pGenre;
				CString strGenre;
				pDoc->m_mapGenre.GetNextAssoc( posGenre, strGenre, (CObject*&)pGenre );
				ASSERT( pGenre->IsKindOf( RUNTIME_CLASS( CGenre ) ) );
				HTREEITEM hItem = tree.InsertItem( pGenre->GetName(), ICON_GENRE , ICON_GENRE, m_hitemRoot, TVI_SORT );
				tree.SetItemData( hItem, (DWORD) pGenre->GetFileList());

				POSITION posArtist = pGenre->m_mapArtist.GetStartPosition();
				while( posArtist != NULL )
				{
					CArtist* pArtist;
					CString strArtist;
					pGenre->m_mapArtist.GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
					ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
					HTREEITEM hSubItem = tree.InsertItem( strArtist, ICON_ARTIST , ICON_ARTIST, hItem, TVI_SORT );
					tree.SetItemData( hSubItem, (DWORD) pArtist->GetFileList());

					POSITION pos = pArtist->m_mapAlbum.GetStartPosition( );
					while( pos != NULL )
					{
						CAlbum* pAlbum;
						CString strAlbum;
						pArtist->m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
						ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
						HTREEITEM hSubItem2 = tree.InsertItem( strAlbum, ICON_ALBUM , ICON_ALBUM, hSubItem, TVI_SORT );
						tree.SetItemData( hSubItem2, (DWORD) pAlbum->GetFileList());
					}


				}				
			}
		}
		break;

		case VIEW_ALBUM:
		{
			POSITION posAlbum = pDoc->m_mapAlbum.GetStartPosition();
			while( posAlbum != NULL )
			{
				CAlbum* pAlbum;
				CString strAlbum;
				pDoc->m_mapAlbum.GetNextAssoc( posAlbum, strAlbum, (CObject*&)pAlbum );
				ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
				HTREEITEM hItem = tree.InsertItem( pAlbum->GetName(), ICON_ALBUM , ICON_ALBUM, m_hitemRoot, TVI_SORT );
				tree.SetItemData( hItem, (DWORD) pAlbum->GetFileList());

				POSITION pos = pAlbum->m_mapArtist.GetStartPosition( );
				while( pos != NULL )
				{
					CArtist* pArtist;
					CString strArtist;
					pAlbum->m_mapArtist.GetNextAssoc( pos, strArtist, (CObject*&)pArtist );
					ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
					HTREEITEM hSubItem = tree.InsertItem( strArtist, ICON_ARTIST , ICON_ARTIST, hItem, TVI_SORT );
					tree.SetItemData( hSubItem, (DWORD) pArtist->GetFileList());
				}
			}
		}
		break;

	}
	tree.Expand( m_hitemRoot , TVE_EXPAND );   

}

void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();

	CTreeCtrl& tree = GetTreeCtrl();

	switch( lHint )
	{
	case UPDATE_SET_VIEW_ARTIST:
		m_nViewType = VIEW_ARTIST;
		break;

	case UPDATE_SET_VIEW_GENRE:
		m_nViewType = VIEW_GENRE;
		break;

	case UPDATE_SET_VIEW_ALBUM:
		m_nViewType = VIEW_ALBUM;
		break;

	case UPDATE_RESET_TREEVIEW:
		pMainFrame->OnResetTreeViewBegin();
		m_bResetInProgress = TRUE;
		SaveCurrentItem();
		tree.DeleteAllItems();
		break;

	case UPDATE_FILL_TREEVIEW:
		FillTree( tree );
		m_bResetInProgress = FALSE;
		RestoreCurrentItem();
		pMainFrame->OnResetTreeViewEnd();
		break;
	}
}


void CLeftView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTreeCtrl& tree = GetTreeCtrl();
	UINT nFlags;
	POINT point;
	::GetCursorPos( &point );
	CPoint pt( point );
	this->ScreenToClient( &pt );
	HTREEITEM hTreeItem = tree.HitTest( pt , &nFlags );

	if( hTreeItem != NULL )
	{
		tree.SelectItem(hTreeItem);
	}
	
	CMenu menu;
	menu.CreatePopupMenu();
	theApp.CreateDynamicMenu( &menu , _gMenuTree , MENU_SIZE , "TreeMenu" );
	
	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, theApp.m_pMainWnd, NULL ); 
	
	*pResult = 0;

  
}


void CLeftView::OnFileExportAll() 
{
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
	pMainFrame->SendMessage( WM_COMMAND, ID_EDIT_SELECT_ALL );
	pMainFrame->SendMessage( WM_COMMAND, ID_FILE_EXPORT );

}


void CLeftView::OnUpdateFileExportAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	
}


void CLeftView::DragOverUpdate( CPoint point )
{
 	CTreeCtrl& tree = GetTreeCtrl();
 	UINT nFlags;
 	HTREEITEM hTreeItem = tree.HitTest( point , &nFlags );
 	if( (hTreeItem != NULL) && (hTreeItem != m_hTreeDropHilited))
 	{
		if( m_hTreeDropHilited != NULL )
		{
			tree.SetItemState( m_hTreeDropHilited, 0, TVIS_DROPHILITED);
		}
		m_hTreeDropHilited = hTreeItem;
		HTREEITEM hTreePrevItem = tree.GetPrevSiblingItem( hTreeItem );
		if( hTreePrevItem != NULL )
		{
			tree.EnsureVisible( hTreePrevItem );
		}
		else
		{
			hTreePrevItem = tree.GetParentItem( hTreeItem );
			if( hTreePrevItem != NULL )
			{
				tree.EnsureVisible( hTreePrevItem );
			}
		}
		HTREEITEM hTreeNextItem = tree.GetNextSiblingItem( hTreeItem );
		if( hTreeNextItem != NULL )
		{
			tree.EnsureVisible( hTreeNextItem );
		}
		tree.SetItemState( hTreeItem, TVIS_DROPHILITED, TVIS_DROPHILITED);
		m_nExpandTimer = ::GetTickCount();
 	}
	else
	{
		if( (hTreeItem == m_hTreeDropHilited) && ( (::GetTickCount() - m_nExpandTimer) > 1000 ))
		{
			tree.Expand(hTreeItem, TVE_EXPAND );
		}
	}

}

void CLeftView::DragOverEnd()
{

 	CTreeCtrl& tree = GetTreeCtrl();
	tree.SetItemState( m_hTreeDropHilited, FALSE, TVIS_DROPHILITED);
	tree.SetItemState( m_hTreeBeforeDrag, m_nStateBeforeDrag, TVIS_DROPHILITED | TVIS_SELECTED);
	if( m_hTreeDropHilited != NULL )
	{
		tree.SetItemState( m_hTreeDropHilited, 0, TVIS_DROPHILITED);
	}
	m_hTreeDropHilited = NULL;

}

void CLeftView::DragEnter()
{
 	CTreeCtrl& tree = GetTreeCtrl();
	m_hTreeDropHilited = NULL;
	m_hTreeBeforeDrag = tree.GetSelectedItem();
	m_nStateBeforeDrag = tree.GetItemState( m_hTreeBeforeDrag , TVIS_DROPHILITED | TVIS_SELECTED );

}

void CLeftView::OnDropMp3()
{
 	CString strArtist;
 	CString strAlbum;
 	CString strGenre;
  	CTreeCtrl& tree = GetTreeCtrl();
 
 	if( m_hTreeDropHilited != NULL )
 	{
		CString strQuestion;

 		if( (tree.ItemHasChildren( m_hTreeDropHilited )) && (m_hTreeDropHilited != m_hitemRoot ))
 		{
 			switch( m_nViewType )
 			{
 			case VIEW_ARTIST:
 				strArtist = tree.GetItemText( m_hTreeDropHilited );
				strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeArtist" );
 				break;
 			case VIEW_GENRE:
				if( tree.GetParentItem(m_hTreeDropHilited) == m_hitemRoot )
				{
	 				strGenre = tree.GetItemText( m_hTreeDropHilited );
					strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeGenre" );
				}
				else
				{
	 				strGenre = tree.GetItemText( tree.GetParentItem(m_hTreeDropHilited));
 					strArtist = tree.GetItemText( m_hTreeDropHilited );
					strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeGenreArtist" );
				}
 				break;
 			case VIEW_ALBUM:
 				strAlbum = tree.GetItemText( m_hTreeDropHilited );
				strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeAlbum" );
 				break;
 			}
 		}
 		else
 		{
 			switch( m_nViewType )
 			{
 			case VIEW_ARTIST:
 				strArtist = tree.GetItemText( tree.GetParentItem(m_hTreeDropHilited));
 				strAlbum = tree.GetItemText( m_hTreeDropHilited );
				strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeArtistAlbum" );
 				break;
 			case VIEW_GENRE:
 				strGenre = tree.GetItemText( tree.GetParentItem(tree.GetParentItem(m_hTreeDropHilited)));
 				strArtist = tree.GetItemText( tree.GetParentItem(m_hTreeDropHilited) );
 				strAlbum = tree.GetItemText( m_hTreeDropHilited );
				strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeGenreArtistAlbum" );
				break;
 			case VIEW_ALBUM:
 				strAlbum = tree.GetItemText( tree.GetParentItem(m_hTreeDropHilited));
 				strArtist = tree.GetItemText( m_hTreeDropHilited );
				strQuestion = theApp.LoadTranslatedString( "IdsQuestionChangeArtistAlbum" );
 				break;
 			}
 		}
		if( MessageBox( strQuestion, NULL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
		{
 			CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
 			CMp3ExpView *pView = pMainFrame->GetRightPane();
 			pView->ModifyTagInfo( strArtist, strAlbum, strGenre);
			CMp3ExpDoc* pDoc = GetDocument();
			pDoc->UpdateLists();
		}
 	}

}

void CLeftView::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	
}

LRESULT CLeftView::OnFilePlayAll( WPARAM wParam, LPARAM lParam )
{
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
	CMp3ExpView *pView = pMainFrame->GetRightPane();
	CMp3FileList mp3List;
	pView->GetAllFiles( &mp3List );
	pView->DoFileAction( this, PLAY , &mp3List );

	return (LRESULT)0;

}

LRESULT CLeftView::OnFileEnqueueAll( WPARAM wParam, LPARAM lParam )
{
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
	CMp3ExpView *pView = pMainFrame->GetRightPane();
	CMp3FileList mp3List;
	pView->GetAllFiles( &mp3List );
	pView->DoFileAction( this, ENQUEUE , &mp3List );

	return (LRESULT)0;

}


void CLeftView::SaveCurrentItem()
{
	CTreeCtrl& tree = GetTreeCtrl();
	
	_TREEITEM *pItem = tree.GetSelectedItem();
	if( (pItem != NULL) )
	{
		m_strItemText = tree.GetItemText( pItem );
	}
	
}

void CLeftView::RestoreCurrentItem()
{
	CTreeCtrl& tree = GetTreeCtrl();

	tree.SelectItem( m_hitemRoot );
	if( m_strItemText != "" )
	{
		// Recursive search
		SelectOldCurrentItem( m_hitemRoot, tree );
	}
}


void CLeftView::SelectOldCurrentItem( HTREEITEM hItem, CTreeCtrl& tree )
{
	CString strItemText = tree.GetItemText(hItem);
	if( strItemText == m_strItemText )
	{ 
		tree.SelectItem( hItem );
		return;
	}
	HTREEITEM hItemSibling = tree.GetNextSiblingItem( hItem );
	if( hItemSibling != NULL )
	{
		SelectOldCurrentItem( hItemSibling, tree );
	}

	HTREEITEM hItemChild = tree.GetChildItem( hItem );
	if( hItemChild != NULL )
	{
		SelectOldCurrentItem( hItemChild, tree );
	}

}


void CLeftView::StartFind( BOOL bFirst )
{
	CTreeCtrl& tree = GetTreeCtrl();

	if( m_strFindText != "" )
	{
		CString strFindText = m_strFindText;
		strFindText.MakeUpper();
		m_bFound = FALSE;
		if( bFirst )
		{
			m_bAterLastFound = TRUE;
			m_hLastFound = NULL;
		}
		else
		{
			m_bAterLastFound = FALSE;
		}

		// Recursive search
		Find( m_hitemRoot, tree, strFindText );
		if( !m_bFound )
		{
			CString strMsg;
			if( m_hLastFound != NULL )
			{
				strMsg.Format( theApp.LoadTranslatedString( "IdsFindNoMore" ) , m_strFindText );
			}
			else
			{
				strMsg.Format( theApp.LoadTranslatedString( "IdsFindNotFound" ) , m_strFindText );
			}

			MessageBox( strMsg, NULL, MB_ICONINFORMATION );
		}
	}
}


BOOL CLeftView::Find( HTREEITEM hItem, CTreeCtrl& tree, CString strFindText )
{
	CString strItemText = tree.GetItemText(hItem);
	strItemText.MakeUpper();
	if( strItemText.Find( strFindText ) != -1)
	{
		if( hItem == m_hLastFound )
			m_bAterLastFound = TRUE;
		if( (!m_bFound) && ( hItem != m_hLastFound) && (m_bAterLastFound))
		{
			m_hLastFound = hItem;
			tree.SelectItem( hItem );
			m_bFound = TRUE;
		}
	}

	HTREEITEM hItemChild = tree.GetChildItem( hItem );
	if( hItemChild != NULL )
	{
		Find( hItemChild, tree, strFindText );
	}

	HTREEITEM hItemSibling = tree.GetNextSiblingItem( hItem );
	if( hItemSibling != NULL )
	{
		Find( hItemSibling, tree, strFindText );
	}
	return FALSE;

}


void CLeftView::OnFilePlaylistAll() 
{
	CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();
	pMainFrame->SendMessage( WM_COMMAND, ID_EDIT_SELECT_ALL );
	pMainFrame->SendMessage( WM_COMMAND, ID_FILE_ADD_TO_PLAYLIST );
	
}

void CLeftView::OnFindfirst() 
{
	CDlgFind dlgFind;
	dlgFind.m_strFindText = m_strFindText;
	if( dlgFind.DoModal() == IDOK )
	{
		m_strFindText = dlgFind.m_strFindText;
		StartFind( TRUE );
		theApp.WriteProfileString( SECT_USR, ENTR_LASTFIND, m_strFindText );
	}
	
}

void CLeftView::OnUpdateFindfirst(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	
}

void CLeftView::OnFindnext() 
{
	StartFind( FALSE );
	
}

void CLeftView::OnUpdateFindnext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	
}
