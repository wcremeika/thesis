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
#include "mp3/mp3File.h"
#include "util/ListVwEx.h"
#include "mp3view.h"
#include "HtmlFileView.h"
#include "mp3ExpDoc.h"
#include "MainFrm.h"
#include <wininet.h>
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CMp3ExpDoc

IMPLEMENT_DYNCREATE(CMp3ExpDoc, CDocument)

BEGIN_MESSAGE_MAP(CMp3ExpDoc, CDocument)
	//{{AFX_MSG_MAP(CMp3ExpDoc)
	ON_COMMAND(ID_DISPLAY_ALL, OnDisplayAll)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_ALL, OnUpdateDisplayAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpDoc construction/destruction

CMp3ExpDoc::CMp3ExpDoc()
{
	m_pAudioFileSelected = NULL;

	m_playlistMemory.m_strFilename = theApp.LoadTranslatedString( "IdsPlaylistDefaultName" );
	m_playlistMemory.m_nType = PLAYLIST_MEMORY;
	m_pPlaylistSelected = &m_playlistMemory;

	m_nHtmlView = HTMLVIEW_DEFAULT;

	CString strLocalUrl = "file:";
	strLocalUrl += theApp.m_strInstallPath + HTMLVIEW_DIR;

	CString strHelpFile = theApp.Translate( "HtmlMenu" ,"HelpFile" );
	m_strHelpUrl = strLocalUrl + strHelpFile;

	CString strReadmeFile = theApp.Translate( "HtmlMenu" ,"ReadmeFile" );
	m_strReadmeUrl = strLocalUrl + strReadmeFile;

	m_strWebSiteUrl = "http://www.mp3-explorer.com//";
	m_strWebSiteUrl += theApp.Translate( "HtmlMenu" ,"OnlinePage" );

}

CMp3ExpDoc::~CMp3ExpDoc()
{
	ResetList();
}

BOOL CMp3ExpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	OnNewPath( theApp.GetScanPath());

	return TRUE;
}

void CMp3ExpDoc::SetNewPath( CString strNewPath )
{
	m_strPath = strNewPath;
	if( m_strPath == "*" )
	{	
		// Favorite Addresses
		m_strPathTitle = theApp.LoadTranslatedString( "IdsFavoriteAddresses" );
		m_nPathIcon = ICON_FAVORITE;

	}
	else if( m_strPath.GetLength() > 3 )
	{
		// Folder
		m_strPathTitle = m_strPath;
		m_nPathIcon = ICON_FOLDER;
	}
	else
	{
		// Drive
		static _TCHAR szVolumeName[MAX_BUFFER];
		static _TCHAR szFileSystem[MAX_BUFFER];
		DWORD dwSerialNumber;
		DWORD dwMaxComponentLength;
		DWORD dwFileSystemFlags;

		CString strVolumePath = m_strPath + "\\";
		::GetVolumeInformation( strVolumePath, szVolumeName, MAX_BUFFER, 
				&dwSerialNumber, &dwMaxComponentLength, &dwFileSystemFlags,
				szFileSystem, MAX_BUFFER);
		static _TCHAR szRoot[MAX_BUFFER];
		m_strPath.MakeUpper();
		wsprintf( szRoot, "%s (%s)", szVolumeName, m_strPath );
		m_strPathTitle = szRoot;
		m_nPathIcon = ICON_DRIVE;
	}
	
}	

	
BOOL CMp3ExpDoc::CreateDataFile( CString strPath, CString strFilename, CMp3FileList *pListMp3, CPlaylistList *pListPlaylist )
{
	CFile file;
	char buf[512];

	try
	{
		if( file.Open( strFilename , CFile::modeCreate | CFile::modeWrite ) ) 
		{
			CArchive ar( &file, CArchive::store, 512, buf );
			CString strHeader = CACHE_FILE_HEADER;
			ar << strHeader;
			pListMp3->Serialize( ar , strPath );
			if( pListPlaylist != NULL )
			{
				pListPlaylist->Serialize( ar , strPath, &m_mp3List);
			}
			ar.Flush();
			file.Close();
		}
	}
	catch ( CFileException *e )
	{
		e->Delete();
		TRACE( "Error reading file tag !\n");
		return FALSE;
	}
	return TRUE;

}


BOOL CMp3ExpDoc::CreateCacheFile(CString strPath)
{

	// Finir le path d'un anti-slash s'il n'existe pas déjà
	if( strPath.Right( 1 ) != "\\" )
		strPath = strPath + "\\";

	CString strFilename = strPath + SNAPSHOT_FILE; 
	return CreateDataFile( strPath, strFilename , &m_mp3List , &m_playlistList );

}

BOOL CMp3ExpDoc::ReadCacheFile(CString strPath)
{
	CFile file;
	char buf[512];

	// Finir le path d'un anti-slash s'il n'existe pas déjà
	if( strPath.Right( 1 ) != "\\" )
		strPath = strPath + "\\";

	CString strFileName = strPath + SNAPSHOT_FILE; 

	if( !file.Open( strFileName , CFile::modeRead ) ) 
	{
		return FALSE;
	}
	CArchive ar( &file, CArchive::load, 512, buf );
	CString strHeader = CACHE_FILE_HEADER;
	CString strFileHeader;
	ar >> strFileHeader;
	if( strFileHeader != strHeader )
	{
		CString strMsg = theApp.LoadTranslatedString( "IdsErrorBadCacheVersion" );
		// ::MessageBox( NULL, strMsg, ::AfxGetAppName() , MB_OK | MB_ICONWARNING );
		return FALSE;
	}
	m_mp3List.Serialize( ar , strPath );
	UpdateFoundCount();
	m_playlistList.Serialize( ar , strPath, &m_mp3List );
	return TRUE;

}


void CMp3ExpDoc::CreateSnapshot()
{

	if( m_strPath == "*" )
	{
		CString strBuffer = theApp.m_strFavoriteAddresses;
		CString strPath;
		int nPos;
		while(TRUE)
		{
			nPos = strBuffer.Find(';');
			if( nPos != -1 )
			{
				strPath = strBuffer.Left( nPos );
				CreateCacheFile( strPath );
				strBuffer = strBuffer.Right( strBuffer.GetLength() - nPos - 1 ); 
			}
			else
			{
				strPath = strBuffer;
				CreateCacheFile( strPath );
				break;
			}
		}
	}
	else
	{
		CreateCacheFile( m_strPath );
	}
}


void CMp3ExpDoc::SearchFiles(CString strNewPath , BOOL bScan, BOOL bResetPlaylistList )
{
	if( bScan || !theApp.m_bUseCachedCatalog )
	{
		FindFiles( strNewPath, bResetPlaylistList );
	}
	else
	{
		if( !ReadCacheFile(strNewPath) )
			FindFiles( strNewPath, bResetPlaylistList );
	}
}

void CMp3ExpDoc::RebuiltCategoryLists()
{
	for( int i = 0 ; i < m_mp3List.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = m_mp3List.GetAt(i);
		AddFileToArtistList( pMp3 );
		AddFileToGenreList( pMp3 );
		AddFileToAlbumList( pMp3 );
	}

	// Invalidate the sorted artist list
	m_slistArtist.Invalidate();

}

void CMp3ExpDoc::GetFiles( BOOL bScan )
{

	BOOL bResetPlaylistList = TRUE;

	if( m_strPath == "*" )
	{
		CString strBuffer = theApp.m_strFavoriteAddresses;
		CString strPath;
		int nPos;
		while(TRUE)
		{
			nPos = strBuffer.Find(';');
			if( nPos != -1 )
			{
				strPath = strBuffer.Left( nPos );
				SearchFiles( strPath, bScan, bResetPlaylistList );
				strBuffer = strBuffer.Right( strBuffer.GetLength() - nPos - 1 ); 
			}
			else
			{
				strPath = strBuffer;
				SearchFiles( strPath, bScan, bResetPlaylistList );
				break;
			}
			bResetPlaylistList = FALSE;
		}
	}
	else
	{
		SearchFiles( m_strPath, bScan, bResetPlaylistList );
	}
	RebuiltCategoryLists();
}

void CMp3ExpDoc::RebuildSnapshot()
{
	UpdateAllViews( NULL , UPDATE_RESET_TREEVIEW );
	ResetList();
	GetFiles( TRUE );
	UpdateAllViews( NULL , UPDATE_FILL_TREEVIEW );
	UpdateLowerPanesViews();
	CreateSnapshot();

}



void CMp3ExpDoc::ScanAll(BOOL bScan)
{
	UpdateAllViews( NULL , UPDATE_RESET_TREEVIEW );
	ResetList();
	GetFiles( bScan );
	SetTitle( m_strPathTitle );
	theApp.WriteProfileString( SECT_MP3 , ENTR_FILEPATH, m_strPath );
	UpdateAllViews( NULL , UPDATE_FILL_TREEVIEW );
	UpdateLowerPanesViews();
	CreateSnapshot();

}


BOOL CMp3ExpDoc::OnNewPath(CString strNewPath)
{
	// Normalisation du Path
	CString strFavorite = theApp.LoadTranslatedString( "IdsFavoriteAddresses" );
	if( strNewPath == strFavorite )
		strNewPath = "*";

	// Suppression du dernier caractère s'il s'agit d'un anti-slash
	CMp3ExpDoc::StripSlash( strNewPath );

	// Vérification de la validité
	if( (!CMp3ExpDoc::IsValidPath( strNewPath )) && (strNewPath != "*"))
		return FALSE;

	SetNewPath( strNewPath );
	ScanAll( FALSE );

	return TRUE;
}

void CMp3ExpDoc::StripSlash(CString &strPath)
{
	if( strPath.Right( 1 ) == "\\" )
	{
		strPath = strPath.Left( strPath.GetLength() - 1 );
	}
}

BOOL CMp3ExpDoc::FindFiles( CString strPath, BOOL bResetPlaylistList )
{

	// Recherche des fichiers MP3
	if( theApp.m_bScanFileMp3 )
	{
		ScanFileType( strPath, FILE_EXT_MP3, FILE_TYPE_MP3 );
	}

	// Recherche des playlist
	if( bResetPlaylistList )
	{
		ResetPlaylistList();
	}

	// Recherche des playlist M3U
	if( theApp.m_bScanPlaylistM3u )
	{
		ScanPlaylistType( strPath, PLAYLIST_EXT_M3U, PLAYLIST_M3U );
	}
	// Recherche des playlist PLS
	if( theApp.m_bScanPlaylistPls )
	{
		ScanPlaylistType( strPath, PLAYLIST_EXT_PLS, PLAYLIST_PLS );
	}

	return TRUE;
} 

BOOL CMp3ExpDoc::ScanFileType( CString strPath, CString strMask, int nType )
{
	CFileLocationList filelist;
	GetFileLocation( strPath, strMask, filelist );
	for( int i =  0 ; i < filelist.GetSize() ; i++ )
	{
		AddFile( filelist.GetLocation(i), filelist.GetFilename(i), nType );
	}
	return TRUE;
}

BOOL CMp3ExpDoc::GetFileLocation( CString strPath, CString strMask, CFileLocationList& filelist )
{
	CWaitCursor wait;    

	StripSlash( strPath );


	// Recherche des fichiers correspondant au masque dans le répertoire courant
	WIN32_FIND_DATA find_file;
	
	HANDLE hFile = FindFirstFile( strPath + "\\" + strMask , &find_file );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		CString strFileName = find_file.cFileName;
		filelist.AddFile( find_file.cFileName,  strPath );
		while( FindNextFile( hFile, &find_file ))
		{
			filelist.AddFile( find_file.cFileName, strPath );
		}
	}

	// Recherche des sous-répertoires

	int nPos = strPath.ReverseFind( '\\' );
	int nLen = strPath.GetLength();
	CString strCurrentDirectory; 
	if( nPos != -1 )
	{
		strCurrentDirectory = strPath.Right( nLen - nPos - 1 );
	}
	else
	{
		strCurrentDirectory = strPath;
	}
	hFile = FindFirstFile( strPath + "\\" + "*.*" , &find_file );
	if( hFile != INVALID_HANDLE_VALUE )
	{
		CString strDirectory = find_file.cFileName;
		if( find_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( /*(strDirectory != strCurrentDirectory ) && */ (strDirectory != "." ) && (strDirectory != ".." ))
				GetFileLocation( strPath + "\\" + find_file.cFileName, strMask, filelist );
		}
		while( FindNextFile( hFile, &find_file ))
		{
			if( find_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				strDirectory = find_file.cFileName;
				if( /*(strDirectory != strCurrentDirectory ) && */ (strDirectory != "." ) && (strDirectory != ".." ))
					GetFileLocation( strPath + "\\" + find_file.cFileName, strMask, filelist );
			}
		}
	}	

	return TRUE;
}

void CMp3ExpDoc::ResetList()
{
	// Suppression des fichiers

	for( int i = 0 ; i < m_mp3List.GetSize() ; i++ )
	{
		delete m_mp3List.GetAt(i);
	}
	m_mp3List.RemoveAll();

	// Suppression des liste
	ResetArtistList();
	ResetGenreList();
	ResetAlbumList();

	// Suppression des playlist
	ResetPlaylistList();

	m_pAudioFileSelected = NULL;
	m_pPlaylistSelected = NULL;

}

void CMp3ExpDoc::AddFile( CString strLocation, CString strFileName, int nType )
{
	CAudioFile* pMp3;
	switch( nType )
	{
	case FILE_TYPE_MP3:
		pMp3 = new CMp3File();
		break;

	default:
		return;
	}

	theApp.DebugTrace( strLocation + "\\" + strFileName + " : Processing ..." );
	if( pMp3->ReadFileInfos( strLocation , strFileName, "" ))
	{
		m_mp3List.Add( pMp3 ); 
		UpdateFoundCount();
		theApp.DebugTrace( pMp3->GetFullPath() + " : Processed !" );
	
	}
	else
	{
		theApp.DebugTrace( pMp3->GetFullPath() + " : Failed reading tag !" );
		delete pMp3;
	}
}

////////////////////////////////////////////////////////////////////
// Gestion des artistes

CArtist*  CMp3ExpDoc::GetArtist( LPCSTR strName )
{
	CArtist* pArtist;

	if( ! m_mapArtist.Lookup( strName , (CObject*&) pArtist ))		
	{
		pArtist = new CArtist( strName );
		m_mapArtist.SetAt( strName , (CObject*) pArtist );
	}
	return pArtist;

}


CGenre*  CMp3ExpDoc::GetGenre( LPCSTR strName )
{
	CGenre* pGenre;

	if( ! m_mapGenre.Lookup( strName , (CObject*&) pGenre ))		
	{
		pGenre = new CGenre( strName );
		m_mapGenre.SetAt( strName , (CObject*) pGenre );
	}
	return pGenre;

}

CAlbum*  CMp3ExpDoc::GetAlbum( LPCSTR strName )
{
	CAlbum* pAlbum;

	if( ! m_mapAlbum.Lookup( strName , (CObject*&) pAlbum ))		
	{
		pAlbum = new CAlbum( strName );
		m_mapAlbum.SetAt( strName , (CObject*) pAlbum );
	}
	return pAlbum;

}



void CMp3ExpDoc::UpdateLists()
{
	// Vide la TreeView pour que les pointeurs ne soient erronés
	// pendant la recherche des fichiers
	UpdateAllViews( NULL , UPDATE_RESET_TREEVIEW );

	// Vide les listes de pointeurs
	ResetArtistList();
	ResetGenreList();
	ResetAlbumList();

	// Reconstruit les listes
	for( int i = 0 ; i < m_mp3List.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = m_mp3List.GetAt(i);
		AddFileToArtistList( pMp3 );
		AddFileToGenreList( pMp3 );
		AddFileToAlbumList( pMp3 );
	}

	// Réaffiche la TreeView
	UpdateAllViews( NULL , UPDATE_FILL_TREEVIEW );
	CreateSnapshot();
}




void CMp3ExpDoc::ResetArtistList()
{

	POSITION pos = m_mapArtist.GetStartPosition();
	while( pos != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_mapArtist.GetNextAssoc( pos, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
		delete pArtist;
	}
	m_mapArtist.RemoveAll();

}


void CMp3ExpDoc::ResetGenreList()
{
	POSITION pos = m_mapGenre.GetStartPosition();
	while( pos != NULL )
	{
		CGenre* pGenre;
		CString strGenre;
		m_mapGenre.GetNextAssoc( pos, strGenre, (CObject*&)pGenre );
		ASSERT( pGenre->IsKindOf( RUNTIME_CLASS( CGenre ) ) );
		delete pGenre;
	}
	m_mapGenre.RemoveAll();

}


void CMp3ExpDoc::ResetAlbumList()
{
	POSITION pos = m_mapAlbum.GetStartPosition();
	while( pos != NULL )
	{
		CAlbum* pAlbum;
		CString strAlbum;
		m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
		ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
		delete pAlbum;
	}
	m_mapAlbum.RemoveAll();

}


void CMp3ExpDoc::AddFileToArtistList(CAudioFile* pMp3)
{
	CString strArtistName = pMp3->GetArtist();
	CArtist* pArtist = GetArtist( strArtistName );
	pArtist->AddFile( pMp3 , TRUE );

}


void CMp3ExpDoc::AddFileToGenreList(CAudioFile* pMp3)
{
	CString strGenreName = pMp3->GetGenre();
	CGenre* pGenre = GetGenre( strGenreName );
	pGenre->AddFile( pMp3 );

}

void CMp3ExpDoc::AddFileToAlbumList(CAudioFile* pMp3)
{
	CString strAlbum = pMp3->GetAlbum();
	CAlbum* pAlbum = GetAlbum( strAlbum );
	pAlbum->AddFile( pMp3 , TRUE );

}

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpDoc serialization

void CMp3ExpDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpDoc diagnostics

#ifdef _DEBUG
void CMp3ExpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMp3ExpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpDoc commands



CMp3FileList* CMp3ExpDoc::GetFileList()
{
	return &m_mp3List;
}


CString CMp3ExpDoc::GetPath()
{
	return m_strPath;
}


BOOL CMp3ExpDoc::IsValidPath(CString strPath)
{
	WIN32_FIND_DATA find_file;
	HANDLE hFile = FindFirstFile( strPath + "\\*.*", &find_file );
	return( hFile != INVALID_HANDLE_VALUE );
}

BOOL CMp3ExpDoc::IsValidFile( CString strFileName )
{
	CFileStatus status;
	return( CFile::GetStatus( strFileName, status ) != 0 );
}

void CMp3ExpDoc::GetPathInfos(CString &strPathTitle, int &nIcon)
{
	strPathTitle = m_strPathTitle;
	nIcon = m_nPathIcon;
}

// Gestion des playlist

void CMp3ExpDoc::ResetPlaylistList()
{
	m_playlistList.Reset();
	m_pPlaylistSelected = NULL;

}

BOOL CMp3ExpDoc::DeletePlaylist(CPlaylist *pPlaylist)
{
	for( int i = 0 ; i < m_playlistList.GetSize() ; i++ )
	{
		CPlaylist* p = (CPlaylist*) m_playlistList.GetAt( i );
		if( p == pPlaylist )
		{
			m_playlistList.RemoveAt( i );
			delete pPlaylist;
			m_pPlaylistSelected = &m_playlistMemory;
			return TRUE;
		}
	}
	m_pPlaylistSelected = &m_playlistMemory;
	return FALSE;
}


BOOL CMp3ExpDoc::ScanPlaylistType( CString strPath, CString strMask, int nType )
{
	CFileLocationList filelist;

	GetFileLocation( strPath, strMask, filelist );
	for( int i = 0 ; i < filelist.GetSize() ; i++ )
	{
		LoadPlaylist( filelist.GetFilename(i), filelist.GetLocation(i), nType );
	}
	return TRUE;

}


CPlaylistList* CMp3ExpDoc::GetPlaylistList()
{
	return &m_playlistList;
}

int CMp3ExpDoc::GetPlaylistCount()
{
	return m_playlistList.GetSize();
}

CPlaylist* CMp3ExpDoc::GetPlaylist(int nIndex)
{
	return (CPlaylist* ) m_playlistList.GetAt(nIndex);
}

void CMp3ExpDoc::AddPlaylist(CPlaylist *pPlaylist)
{
	m_playlistList.Add( pPlaylist );
}

CPlaylist* CMp3ExpDoc::LoadPlaylist(CString strFilename, CString strLocation, int nType )
{
	CPlaylist* pPlaylist= (CPlaylist*) new CPlaylist( strFilename, strLocation, nType );
	pPlaylist->Load( &m_mp3List );
	AddPlaylist( pPlaylist );
	return pPlaylist;
}

CFileLocation::CFileLocation(CString strFilename , CString strLocation)
{
	m_strFilename = strFilename;
	m_strLocation = strLocation;
}



CFileLocationList::CFileLocationList()
{

}

CFileLocationList::~CFileLocationList()
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		delete GetAt(i);
	}
	RemoveAll();
}

void CFileLocationList::AddFile( CString strFilename , CString strLocation )
{
	CFileLocation *pFile = new CFileLocation( strFilename , strLocation );
	Add( (CObject *) pFile );
}

CString CFileLocationList::GetFilename(int nIndex)
{
	CFileLocation *pFile = (CFileLocation *) GetAt( nIndex );
	return pFile->m_strFilename;
}

CString CFileLocationList::GetLocation(int nIndex)
{
	CFileLocation *pFile = (CFileLocation *) GetAt( nIndex );
	return pFile->m_strLocation;

}



void CMp3ExpDoc::UpdateFileSelection(CAudioFile *pAudioFile)
{
	if( pAudioFile != m_pAudioFileSelected )
	{
		m_pAudioFileSelected = pAudioFile;
		if( m_nHtmlView == HTMLVIEW_FILE )
		{
			UpdateAllViews( NULL , UPDATE_HTMLVIEW, NULL );
		}
	}

}

CString CMp3ExpDoc::GetUrl()
{
	switch( m_nHtmlView )
	{
	case HTMLVIEW_DEFAULT:
		if( (!theApp.m_bDisableConnection) && (theApp.IsInternetConnected()))
		{
			return m_strWebSiteUrl;
		}
		break;

	case HTMLVIEW_README:
		return m_strReadmeUrl;

	case HTMLVIEW_HELP:
		return m_strHelpUrl;

	case HTMLVIEW_WEBSITE:
		if( (!theApp.m_bDisableConnection) && (theApp.IsInternetConnected()))
		{
			return m_strWebSiteUrl;
		}
		else
		{
			CString strMsg = theApp.LoadTranslatedString( "IdsNotConnected" );
			theApp.DoMessageBox( strMsg, MB_OK, 0 );
		}
		break;

	case HTMLVIEW_FILE:
		if( m_pAudioFileSelected != NULL )
		{
			CString strUrl;
			if( CHtmlFileView::CreateHtmlPage( m_pAudioFileSelected, strUrl ))
			{
				return strUrl;
			}
		}
		break;

	case HTMLVIEW_SEARCH:
		if( (!theApp.m_bDisableConnection) && (theApp.IsInternetConnected()))
		{
			return m_strSearchUrl;
		}
		else
		{
			CString strMsg = theApp.LoadTranslatedString( "IdsNotConnected" );
			theApp.DoMessageBox( strMsg, MB_OK, 0 );
		}
		break;

	}
	return "";
	
}



void CMp3ExpDoc::UpdateLowerPanesViews()
{
	if( !m_pPlaylistSelected )
	{
		m_pPlaylistSelected = &m_playlistMemory;
	}
	if( theApp.m_bDisplayPlaylists )
	{
		UpdateAllViews( NULL , UPDATE_PLAYLIST_VIEW, NULL );
		UpdateAllViews( NULL , UPDATE_DETAIL_VIEW, NULL );
	}
	else
	{
		UpdateAllViews( NULL , UPDATE_HTMLVIEW, NULL );
	}
}

void CMp3ExpDoc::UpdatePlaylistSelection(CPlaylist *pPlaylist)
{

	m_pPlaylistSelected = pPlaylist;
	UpdateAllViews( NULL , UPDATE_DETAIL_VIEW, NULL );
	CMainFrame* pMainFrame = (CMainFrame*) theApp.m_pMainWnd;
	if( pMainFrame != NULL )
		pMainFrame->UpdateStatusBar( pPlaylist );

}

BOOL CMp3ExpDoc::SavePlaylist(BOOL bSaveAs)
{
	CString strNotSaved = theApp.LoadTranslatedString( "IdsPlaylistNotSaved" );
	BOOL bNew = ((m_pPlaylistSelected->m_strLocation == strNotSaved) || bSaveAs);
	CString strFilename;
	CString strLocation;

	if( bNew )
	{
		CString strFilter;
		strFilter = theApp.LoadTranslatedString( "IdsPlaylistFilter" );
		CFileDialog dlgFile( FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter );
	
		dlgFile.m_ofn.lpstrInitialDir = GetTitle();
		if( dlgFile.DoModal() != IDOK )
		{
			return FALSE;
		}
		strFilename = dlgFile.GetFileName();
		strLocation = dlgFile.GetPathName();
		strLocation = strLocation.Left( strLocation.GetLength() - strFilename.GetLength() - 1 );
		CPlaylist* pNewPlaylist = new CPlaylist( m_pPlaylistSelected );
		pNewPlaylist->m_strLocation = strLocation;
		CString strExtension = dlgFile.GetFileExt();
		strExtension.MakeUpper();
		switch ( dlgFile.m_ofn.nFilterIndex )
		{
		case 1:
			if( strExtension != "M3U" )
				strFilename += ".m3u";
			pNewPlaylist->m_strFilename = strFilename;
			pNewPlaylist->m_nType = PLAYLIST_M3U;
			break;
		case 2:
			if( strExtension != "PLS" )
				strFilename += ".pls";
			pNewPlaylist->m_strFilename = strFilename;
			pNewPlaylist->m_nType = PLAYLIST_PLS;
			break;
		}

		if( pNewPlaylist->Save())
		{
			// Vérifie si la playlist est sous l'adresse courante
			CString strCurrentPath = m_strPath;
			strCurrentPath.MakeUpper();
			strLocation.MakeUpper();
			if( (strLocation.Find( strCurrentPath ) != -1 ))
			{
				AddPlaylist( pNewPlaylist );
				m_pPlaylistSelected = pNewPlaylist;
			}
			else
			{
				delete pNewPlaylist;
			}
		}
		else
		{
			delete pNewPlaylist;
			return FALSE;
		}
	}
	else
	{
		if( !m_pPlaylistSelected->Save())
			return FALSE;
	}
	UpdateAllViews( NULL, UPDATE_PLAYLIST_VIEW , NULL );
	CreateSnapshot();
	return TRUE;

}

void CMp3ExpDoc::PlaylistReset()
{
	m_pPlaylistSelected->Reset();
	UpdateAllViews( NULL, UPDATE_DETAIL_VIEW , NULL);
}

void CMp3ExpDoc::PlaylistShuffle()
{
	m_pPlaylistSelected->Shuffle();
	UpdateAllViews( NULL, UPDATE_DETAIL_VIEW , NULL );
}

BOOL CMp3ExpDoc::PlaylistDelete()
{
	CString strMsg;

	// Vérifier que ce n'est pas la working playlist
	if( m_pPlaylistSelected->m_nType == PLAYLIST_MEMORY )
	{
		strMsg = theApp.LoadTranslatedString( "IdsErrorDeletingMemoryPlaylist" );
		::AfxMessageBox( strMsg , MB_OK | MB_ICONWARNING , 0 );
		return FALSE;
	}
	
	// Ask confirmation
	strMsg.Format( theApp.LoadTranslatedString( "IdsQuestionFileDelete" ) , m_pPlaylistSelected->GetFullPath() );
	if( ::AfxMessageBox( strMsg , MB_YESNO | MB_ICONQUESTION, 0 ) == IDYES )
	{
		m_pPlaylistSelected->Delete();
		DeletePlaylist( m_pPlaylistSelected );
		UpdateAllViews( NULL, UPDATE_PLAYLIST_VIEW, NULL );
		CreateSnapshot();
	}
	return TRUE;

}


void CMp3ExpDoc::SetHtmlView(int nView)
{
	m_nHtmlView = (HTMLVIEW) nView;
	UpdateAllViews( NULL , UPDATE_HTMLVIEW, NULL );

}

void CMp3ExpDoc::SetSearchUrl(CString strUrl)
{
	m_strSearchUrl = strUrl;
	m_nHtmlView = HTMLVIEW_SEARCH;

	UpdateAllViews( NULL , UPDATE_HTMLVIEW, NULL );

}

void CMp3ExpDoc::OnDisplayAll() 
{
	CMp3View::OnDisplayAll();
	UpdateAllViews( NULL , UPDATE_FILE_LIST );
	UpdateAllViews( NULL , UPDATE_DETAIL_VIEW );
	
}

void CMp3ExpDoc::OnUpdateDisplayAll(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CMp3View::IsDisplayAll() );
	
}

void CMp3ExpDoc::UpdateFoundCount()
{
	// Affichage du nombre de fichiers trouvés
	CMainFrame* pMainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CString strFileFoundCount;
	strFileFoundCount.Format( theApp.LoadTranslatedString( "IdsFileFindCount" ), m_mp3List.GetSize() );
	if( pMainFrame != NULL )
		pMainFrame->SetFileFound( strFileFoundCount );
	else
		CSplashWnd::SetFileFound( strFileFoundCount );
}

