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
#include "util/ListVwEx.h"
#include "Mp3View.h"
#include "Mainfrm.h"
#include "Mp3ExpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_STRING 255


extern CMp3ExpApp theApp;

CColumn::CColumn()
{
}
CColumn::~CColumn()
{
}


void CColumn::Copy(CColumn *pCol)
{
	m_nAlignemnt = pCol->m_nAlignemnt;
	m_nDisplayed = pCol->m_nDisplayed;
	m_nInfoIndex = pCol->m_nInfoIndex;
	m_nInfoOrder = pCol->m_nInfoOrder;
	m_nInfoType = pCol->m_nInfoType;
	m_pszTitleResName = pCol->m_pszTitleResName;
	m_nWidth = pCol->m_nWidth;
	m_pszColTitle = pCol->m_pszColTitle;

}

/////////////////////////////////////////////////////////////////////////////
// CMp3View

IMPLEMENT_DYNCREATE(CMp3View, CListViewEx)

BOOL CMp3View::m_bDisplayAll;

CMp3View::CMp3View()
{
	m_nSortedCol = theApp.m_nSortingCol;
	m_nPrevSortedCol = theApp.m_nSecondSortingCol;
	m_nLeftViewType = theApp.m_nViewType;
	m_bDisplayAll = theApp.m_bDisplayAll;
	m_nDisplayInfoType = INFO_MPEG | INFO_MUSIC | INFO_FILE ;
	InitColumns();
	DefineDisplayedColumns();
	m_pList = NULL;
	m_bCreated = FALSE;
	SetColors( theApp.m_colorViewText, 
		theApp.m_colorViewTextBk, 
		theApp.m_colorViewBackground, 
		theApp.m_colorViewSelectBackground,
		theApp.m_colorViewSelectText);
}

CMp3View::~CMp3View()
{
	// Suppression des colonnes
	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		delete m_columns.GetAt(i);
	}
	m_columns.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMp3View, CListViewEx)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopyCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//{{AFX_MSG_MAP(CMp3View)
	ON_UPDATE_COMMAND_UI(ID_FILE_PLAY, OnUpdateFilePlay)
	ON_UPDATE_COMMAND_UI(ID_FILE_ENQUEUE, OnUpdateFileEnqueue)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, OnUpdateFileExport)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_MESSAGE(WM_PLAY, OnFilePlay)
	ON_MESSAGE(WM_ENQUEUE, OnFileEnqueue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMp3View drawing

void CMp3View::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMp3View diagnostics

#ifdef _DEBUG
void CMp3View::AssertValid() const
{
	CListView::AssertValid();
}

void CMp3View::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMp3View message handlers

void CMp3View::OnInitialUpdate() 
{
//	CListView::OnInitialUpdate(); // Cette fonction est inutile car elle n'appelle seulement OnUpdate

	// Récupère la référence du LV control associé à la view
	CListCtrl& list = GetListCtrl();

	// Définit les ImageLists associées

	m_LargeImageList.Create( 32, 32, IDB_LARGEICONS );
	m_SmallImageList.Create( 16, 16, IDB_SMALL_ICONS );

	list.SetImageList(&m_LargeImageList, LVSIL_NORMAL);
	list.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	m_bCreated =TRUE;

	
}

void CMp3View::InitColumns()
{
	for( int i = 0 ; i < NUM_INFO ; i++ )
	{
		CColumn *pCol = new CColumn();
		lstrcpy( _gcolumns[i].szColTitle , theApp.LoadTranslatedString( _gcolumns[i].pstrTitleResName ));
		int nIdInfo = _gcolumns[i].nInfoIndex;
		pCol->m_pszColTitle = _gcolumns[i].szColTitle;
		pCol->m_nInfoType = _gcolumns[i].nInfoType;
		pCol->m_nInfoIndex = nIdInfo;
		
		// Charge les tailles sauvegardées des colonnes
		SetColumnWidthPref( pCol, nIdInfo , _gcolumns[i].nWidth );

		// Charge les autres options si l'utilisateur est enregistré
		if( theApp.m_bColumnsSettings )
		{
			SetColumnPref( pCol, nIdInfo );
		}
		else
		{
			pCol->m_nAlignemnt = _gcolumns[i].nAlignemnt;
			pCol->m_nDisplayed = _gcolumns[i].nDisplayed;
			pCol->m_nInfoOrder = _gcolumns[i].nInfoOrder;
		}
		m_columns.Add( pCol );
	}

}

void CMp3View::DefineDisplayedColumns()
{
	m_colsDisplayed.RemoveAll();

	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn *pcol = (CColumn *) m_columns.GetAt(i);
		if( (pcol->m_nDisplayed) || m_bDisplayAll )
		{
			for( int j = 0 ; j < m_colsDisplayed.GetSize() ; j++ )
			{
				CColumn *pdisplayedCol = (CColumn *) m_colsDisplayed.GetAt( j );
				if( pcol->m_nInfoOrder < pdisplayedCol->m_nInfoOrder )
					break;
			}
			m_colsDisplayed.InsertAt( j , pcol );

		}

	}

}

void CMp3View::SetDisplayedColumns()
{
	// Récupère la référence du LV control associé à la view
	CListCtrl& list = GetListCtrl();

	// Suppression des colonnes existantes
	DeleteColumns();

	// Insert les colonnes
	LV_COLUMN lvc;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	
	for( int i = 0 ; i < m_colsDisplayed.GetSize() ; i++ )
	{
		CColumn *pdisplayedCol = (CColumn *) m_colsDisplayed.GetAt( i );

		lvc.iSubItem = i;
		lvc.pszText = pdisplayedCol->m_pszColTitle;
		lvc.cx = pdisplayedCol->m_nWidth;
		lvc.fmt = pdisplayedCol->m_nAlignemnt;
		list.InsertColumn(i,&lvc);
	}

}

void CMp3View::SetColumnWidthPref(CColumn *pCol, int nId, int nDefWidth)
{
	static _TCHAR szSection[MAX_STRING];
	wsprintf( szSection , SECT_COLUMNS , nId );
	pCol->m_nWidth = theApp.GetProfileInt( szSection , ENTR_COL_WIDTH, nDefWidth );


}

void CMp3View::SetColumnPref(CColumn *pCol, int nId)
{
	static _TCHAR szSection[MAX_STRING];
	wsprintf( szSection , SECT_COLUMNS , nId );
	pCol->m_nAlignemnt = theApp.GetProfileInt( szSection , ENTR_COL_ALIGN, 0 );
	pCol->m_nDisplayed = theApp.GetProfileInt( szSection , ENTR_COL_DISPLAY, 0 );
	pCol->m_nInfoOrder = theApp.GetProfileInt( szSection , ENTR_COL_ORDER, 0 );

}

void CMp3View::DeleteColumns()
{
	// Récupère la référence du LV control associé à la view
	CListCtrl& list = GetListCtrl();

	// Suppression des colonnes existantes
	while( list.DeleteColumn( 0 ) );

}

void CMp3View::UpdateList()
{
	if( (m_pList == NULL) || (!m_bCreated) )
	{
		return;
	}

	CListCtrl& list = GetListCtrl();


	list.DeleteAllItems();
	// Remplit la ListView
	
	LV_ITEM lvi;
	int nCount = min( m_pList->GetSize() , (int) theApp.m_nMaxRow );
	for ( int i = 0 ; i < nCount ; i++ )
	{
		//Paramètres
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iImage = ICON_TITLE;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		// Récupération du pointeur sur le fichier MP3
		CAudioFile *pMp3 = (CAudioFile *) m_pList->GetAt(i); 

		static _TCHAR buffer[MAX_STRING];

		lvi.iSubItem = 0;
		lvi.pszText = buffer;
		list.InsertItem( &lvi );
		list.SetItemData( i , (DWORD) pMp3 );

		for( int j = 0 ; j < m_colsDisplayed.GetSize() ; j++)
		{
			CColumn *pCol = (CColumn *) m_colsDisplayed.GetAt( j );
			list.SetItemText( i , j , pMp3->GetInformation(pCol->m_nInfoIndex));
		}
	}

}

int CMp3View::GetSelectedFiles(CMp3FileList &mp3list)
{
	CListCtrl& list = GetListCtrl();
	int nCount = 0;
	int nIndex = -1;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CAudioFile *pMp3 = (CAudioFile*) list.GetItemData( nIndex );
		mp3list.Add( pMp3 );
		nCount++;		
	}
	return nCount;

}

LRESULT CMp3View::OnFilePlay(WPARAM wParam, LPARAM lParam) 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	DoFileAction( this , PLAY , &mp3list );
	return (LRESULT) 0;
}


LRESULT CMp3View::OnFileEnqueue(WPARAM wParam, LPARAM lParam) 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	DoFileAction( this, ENQUEUE , &mp3list );
	return (LRESULT) 0;
	
}

BOOL CMp3View::IsSelected()
{
	CListCtrl& list = GetListCtrl();
	return ( list.GetSelectedCount() >= 1 );
}

void CMp3View::OnUpdateFilePlay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());

}

void CMp3View::OnUpdateFileEnqueue(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}


void CMp3View::OnUpdateEditCopyCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsSelected());
}


void CMp3View::OnUpdateFileExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelected());
	
}

void CMp3View::OnFileExport() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileExport( &mp3list );

}

void CMp3View::GetCopyData( CString& strInformations )
{
	CMp3FileList mp3list;
	int nCount = GetSelectedFiles( mp3list );
	for( int i = 0 ; i < nCount ; i++ )
	{
		CAudioFile *pMp3 = mp3list.GetAt(i);
		for( int i = 0 ; i < m_colsDisplayed.GetSize() ; i++ )
		{
			CColumn* pCol = (CColumn*) m_colsDisplayed.GetAt(i);
			if( i != 0 )
				strInformations += "\t";
			strInformations += pMp3->GetInformation( pCol->m_nInfoIndex );
		}
		strInformations += "\r\n";		

	}
}

void CMp3View::OnEditCopy()
{
	CSharedFile	memFile;
	CString strInformations;

	GetCopyData( strInformations );
	memFile.Write( strInformations, strInformations.GetLength()); 
	HGLOBAL hData = memFile.Detach();
	if (OpenClipboard())
	{
		::SetClipboardData( CF_TEXT , hData);
		CloseClipboard();
	}
	else
		AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);

}


void CMp3View::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	m_menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, theApp.m_pMainWnd, NULL ); 
	
}

void CMp3View::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( m_bItemChangedHandling )
	{
		CString strTitle;
		CString strFileInfo;
		CString strSize;
		CString strTime;

		LONG lSize = 0;
		int nTime = 0;

		CMp3FileList mp3list;
		int nCount = GetSelectedFiles( mp3list );

		for( int i = 0 ; i < nCount ; i++ )
		{
			CAudioFile *pMp3 = mp3list.GetAt(i);
			strFileInfo = theApp.LoadTranslatedString( "IdsStatusOneFileSelected" );
			strTitle = pMp3->GetArtist() + " - " + pMp3->GetTitle();
			if( pMp3->GetAlbum() != "" )
				strTitle += " - Album : " + pMp3->GetAlbum();
			if( pMp3->GetYear() != "" )
				strTitle += " - " + pMp3->GetYear() ;
			if( pMp3->GetGenre() != "" )
				strTitle += " [ " + pMp3->GetGenre() + " ]";
			lSize += pMp3->GetSize();
			nTime += pMp3->GetTime();
			if( i == 0 )
			{
				CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
				pDoc->UpdateFileSelection( pMp3 );
			}
		}

		strSize = CAudioFile::FormatSize( lSize );
		strTime = CAudioFile::FormatTime( nTime );

		CMainFrame *pMainFrame = (CMainFrame *) GetParentFrame();

		if( nCount > 1 )
		{
			strTitle = "";
			static _TCHAR szStatusText[MAX_STATUS];
			CString strFormat = theApp.LoadTranslatedString( "IdsStatusFilesSelected" );
			wsprintf( szStatusText, strFormat , nCount );
			strFileInfo = szStatusText;
		}
		pMainFrame->SetStatusText( "" , strTitle, strFileInfo, strSize, strTime );
	}
	*pResult = 0;
}

void CMp3View::OnDisplayAll()
{
	m_bDisplayAll = !m_bDisplayAll;

}


void CMp3View::DoFileAction( CWnd*pWnd , int nAction, CMp3FileList *mp3list)
{
	CString strShellCmd;
	CString strMsgShellError;	
	CString strMsgShellErrorM3U;	


	switch( nAction )
	{
	case PLAY:
		strShellCmd = theApp.m_strCmdPlay;
		strMsgShellError =  theApp.LoadTranslatedString( "IdsErrorShellNoMP3Association");
		strMsgShellErrorM3U = theApp.LoadTranslatedString( "IdsErrorShellNoM3UAssociation");
		break;
	case ENQUEUE:
		strShellCmd = theApp.m_strCmdEnqueue;
		strMsgShellError =  theApp.LoadTranslatedString( "IdsErrorShellNoEnqueue");
		strMsgShellErrorM3U =  theApp.LoadTranslatedString( "IdsErrorShellNoEnqueue");
		break;
	default:
		return;
	}

	CString strTmpPlaylist;
	CString strTmpDir;
	GetTempPath( MAX_PATH , strTmpDir.GetBuffer( MAX_PATH ));
	strTmpDir.ReleaseBuffer();
	strTmpPlaylist = strTmpDir + TMP_PLAYLIST;

	CStdioFile filePlaylist;
	filePlaylist.Open( strTmpPlaylist, CFile::modeCreate | CFile::modeWrite);
	
	CString strFullPathName;

	int nCount = mp3list->GetSize();
	for( int i = 0 ; i < nCount ; i++ )
	{
		CAudioFile *pMp3 = mp3list->GetAt(i);
		strFullPathName = pMp3->GetFullPath();
		filePlaylist.WriteString( strFullPathName + "\n");
	}
	filePlaylist.Close();

	
	// Utilise l'association MP3 (pour qu'il y ait un minimum qui fonctionne en focntion du player).
	if( nCount == 1 )
	{
		if( (!CMp3ExpDoc::IsValidFile(strFullPathName))  && (strFullPathName.Find("http://") != 0) )
		{
			CString strMsg;
			strMsg.Format( theApp.LoadTranslatedString( "IdsErrorFileNotFound" ) , strFullPathName );
			if( pWnd->MessageBox( strMsg , NULL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
			{
//				CMp3ExpDoc* pDoc = (CMp3ExpDoc *) GetActiveDocument();
//				pDoc->RebuildSnapshot();
			}
			return;
				
		}
		int nExecute = (int)::ShellExecute( pWnd->m_hWnd , strShellCmd , strFullPathName , NULL , "", SW_SHOWNORMAL );
		if( nExecute <= 32 )
		{
			pWnd->MessageBox( strMsgShellError, NULL, MB_OK | MB_ICONEXCLAMATION );
			return;
		}
	}
	// Selection multiple. utilise l'association M3U pour une plus grande compatibilité
	else
	{
		int nExecute;
		nExecute = (int)::ShellExecute( pWnd->m_hWnd , strShellCmd , strTmpPlaylist , NULL , "", SW_SHOWNORMAL );
		if( nExecute <= 32 )
		{
			pWnd->MessageBox( strMsgShellErrorM3U, NULL, MB_OK | MB_ICONEXCLAMATION );
			return;
		}
	}

}

HGLOBAL CMp3View::GetHDropData()
{
	HGLOBAL hGlobal;
	CStringArray filelist;
	int nCount = GetDropFiles( filelist );

	// allocate space for DROPFILE structure plus the number of file and one extra byte for final NULL terminator
	hGlobal = GlobalAlloc(GHND|GMEM_SHARE,(DWORD) (sizeof(DROPFILES)+(_MAX_PATH)*nCount+1));
	if(hGlobal == NULL)
			return hGlobal;

	LPDROPFILES pDropFiles;
	pDropFiles = (LPDROPFILES)GlobalLock(hGlobal);
	// set the offset where the starting point of the file start
   	pDropFiles->pFiles = sizeof(DROPFILES);
	// file contains wide characters
   	pDropFiles->fWide=FALSE;

	int CurPosition = sizeof(DROPFILES);
	for (int i = 0 ; i < nCount ; i++)
	{
		CString strFilepath = filelist.GetAt(i);
		lstrcpy((LPSTR)((LPSTR)(pDropFiles)+CurPosition),TEXT(strFilepath));
		// move the current position beyond the file name copied
		// don't forget the Null terminator (+1)
		CurPosition += strFilepath.GetLength() + 1;
	 }
	// final null terminator as per CF_HDROP Format specs.
	((LPSTR)pDropFiles)[CurPosition]=0;
	GlobalUnlock(hGlobal);
 	return hGlobal;

}

int CMp3View::GetDropFiles( CStringArray& filelist)
{
	CListCtrl& list = GetListCtrl();
	int nCount = 0;
	int nIndex = -1;

	while( (nIndex = list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CAudioFile *pMp3 = (CAudioFile*) list.GetItemData( nIndex );
		filelist.Add( pMp3->GetLocation() + "\\" + pMp3->GetName() );
		nCount++;		
	}
	return nCount;
}



HGLOBAL CMp3View::GetHTextData()
{
	CString strCF_TEXT;
	GetCopyData( strCF_TEXT );

    HGLOBAL hgCF_TEXTData = GlobalAlloc(GPTR, strCF_TEXT.GetLength() + 1);   
    LPSTR   lpCF_TEXTData = (LPSTR)GlobalLock(hgCF_TEXTData);
                                                 
	lstrcpy(lpCF_TEXTData, strCF_TEXT);
	GlobalUnlock(hgCF_TEXTData); 
	
	return hgCF_TEXTData;

}
