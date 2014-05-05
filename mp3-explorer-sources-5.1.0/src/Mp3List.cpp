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
#include "Mp3List.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMp3FileList

IMPLEMENT_DYNCREATE(CMp3FileList, CObArray)

CMp3FileList::CMp3FileList()
{
}


CMp3FileList::~CMp3FileList()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMp3FileList diagnostics

#ifdef _DEBUG
void CMp3FileList::AssertValid() const
{
	CObArray::AssertValid();
}

void CMp3FileList::Dump(CDumpContext& dc) const
{
	CObArray::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMp3FileList serialization

void CMp3FileList::Serialize(CArchive& ar , CString strPath )
{

	int nFileCount;

	if (ar.IsStoring())
	{
		// Compte les fichiers à mettre dans le cache localisé à strPath
		nFileCount = 0;
		for( int i = 0 ; i < GetSize() ; i++ )
		{
			CAudioFile *pMp3 = (CAudioFile *) GetAt(i);
			if( pMp3->IsUnderDir( strPath ))
				nFileCount++;
		}
		// Sauvegarde le nombre trouvé, et les fichiers
		ar << nFileCount;
		for( i = 0 ; i < GetSize() ; i++ )
		{
			CAudioFile *pMp3 = (CAudioFile *) GetAt(i);
			if( pMp3->IsUnderDir( strPath ))
			{
				ar << pMp3->GetType();
				pMp3->Serialize( ar , strPath );
			}
		}
	}
	else
	{
		ar >> nFileCount;
		for( int i = 0 ; i < nFileCount ; i++ )
		{
			int nType;

			ar >> nType;

			CAudioFile* pAudioFile;
			switch( nType )
			{
			case FILE_TYPE_MP3:
				pAudioFile = new CMp3File();
				break;

			}
			pAudioFile->Serialize( ar , strPath );
			Add( pAudioFile );

		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CMp3FileList commands


CAudioFile* CMp3FileList::GetAt(int nIndex)
{
	return (CAudioFile*) CObArray::GetAt( nIndex );
}

int CMp3FileList::GetSize()
{
	return CObArray::GetSize();
}

int CMp3FileList::Add(CAudioFile *pMp3)
{
	return  CObArray::Add( (CObject*) pMp3 );
}

void CMp3FileList::DeleteFile(CAudioFile *pMp3ToDelete)
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		if( pMp3ToDelete == GetAt( i ))
		{
			RemoveAt( i );
			delete pMp3ToDelete;
			break;
		}
	}
}

BOOL CMp3FileList::MoveUp(CAudioFile *pMp3)
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		if( pMp3 == GetAt( i ))
		{
			if( i == 0 )
			{
				return FALSE; //premier de la liste
			}
			else
			{
				CAudioFile *pMp3Prev = GetAt(i - 1);
				SetAt( i - 1 , pMp3 );
				SetAt( i , pMp3Prev );
				return TRUE;
			}
		}
	}
	return FALSE;

}

BOOL CMp3FileList::MoveDown(CAudioFile *pMp3)
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		if( pMp3 == GetAt( i ))
		{
			if( i == ( GetSize() - 1 ))
			{
				return FALSE; //dernier de la liste
			}
			else
			{
				CAudioFile *pMp3Next = GetAt(i + 1);
				SetAt( i + 1 , pMp3 );
				SetAt( i , pMp3Next );
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CMp3FileList::Swap(int n1, int n2)
{
	CAudioFile* p1 = GetAt( n1 );
	CAudioFile* p2 = GetAt( n2 );
	SetAt( n1 , (CObject *) p2 );
	SetAt( n2 , (CObject *) p1 );

}


/////////////////////////////////////////////////////////////////////////////
// CPlaylist

IMPLEMENT_DYNCREATE(CPlaylist, CObject)

CPlaylist::CPlaylist()
{
	m_strLocation = theApp.LoadTranslatedString( "IdsPlaylistNotSaved" );

}

CPlaylist::~CPlaylist()
{
	Reset();
}


BOOL CPlaylist::IsUnderDir(CString strPath)
{
	CString strLocation = m_strLocation + "\\";
	strPath.MakeUpper();
	strLocation.MakeUpper();
	return ( strLocation.Find( strPath ) != -1 );
}

BOOL CPlaylist::CanBeRelative()
{
	CString strPlaylistPath = m_strLocation;
	strPlaylistPath.MakeUpper();

	for( int i = 0 ; i < m_mp3List.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = m_mp3List.GetAt(i);
		CString strLocation = pMp3->GetLocation(); 
		strLocation.MakeUpper();
		if( strLocation.Find( strPlaylistPath ) == -1 )
			return FALSE;
	}
	return TRUE;
}


BOOL CPlaylist::SaveM3u()
{
	CStdioFile filePlaylist;
	if( filePlaylist.Open( GetFullPath() , CFile::modeCreate | CFile::modeWrite))
	{
		BOOL bRelative = CanBeRelative();

		for( int i = 0 ; i < m_mp3List.GetSize() ; i++ )
		{
			CAudioFile *pMp3 = m_mp3List.GetAt(i);
			CString strFilePath = pMp3->GetPath( m_strLocation , bRelative );
			filePlaylist.WriteString( strFilePath + "\n");
		}
		filePlaylist.Close();
	}
	else
	{
		CString strMsg;
		strMsg.Format( theApp.LoadTranslatedString( "IdsErrorOpenFile" ), GetFullPath());
		::MessageBox( NULL, strMsg, ::AfxGetAppName(), MB_ICONERROR );
		return FALSE;
	}
	return TRUE;

}

BOOL CPlaylist::SavePls()
{
	CString strPlaylistFile = GetFullPath();
	int nNbEntries = m_mp3List.GetSize();
	CString strNbEntries;
	strNbEntries.Format("%d", nNbEntries );
	::WritePrivateProfileString( SECT_PLS , ENTR_PLS_NB_ENTRIES, strNbEntries , strPlaylistFile );

	BOOL bRelative = CanBeRelative();

	for( int i = 0 ; i < nNbEntries ; i++ )
	{
		CAudioFile *pMp3 = m_mp3List.GetAt(i);
		CString strFilePath = pMp3->GetPath( m_strLocation , bRelative );
		CString strEntry;
		strEntry.Format( ENTR_PLS_FILE , i + 1 );
		::WritePrivateProfileString( SECT_PLS, strEntry, strFilePath, strPlaylistFile);
	}

	return TRUE;

}


CPlaylist::CPlaylist(CString strFilename, CString strLocation, int nType)
{
	m_strFilename= strFilename;
	m_strLocation = strLocation;
	m_nType = nType;
}

void CPlaylist::Load( CMp3FileList* pMp3List )
{
	switch( m_nType )
	{
	case PLAYLIST_M3U:
		LoadM3u( pMp3List );
		break;
	case PLAYLIST_PLS:
		LoadPls( pMp3List );
		break;
	}
}

void CPlaylist::LoadM3u( CMp3FileList* pMp3List )
{
	CStdioFile filePlaylist;
	if( filePlaylist.Open( m_strLocation + "\\" + m_strFilename , CFile::modeRead))
	{
		CString strFullPath;
		while( filePlaylist.ReadString( strFullPath ))
		{
			int nLen = strFullPath.GetLength();
			int nPos = strFullPath.ReverseFind('\\');
			int nPosExt = strFullPath.ReverseFind('.');
			CString strExtension = strFullPath.Right( nLen - nPosExt - 1 ); 
			strExtension.MakeUpper();
			if((strExtension != "MP3") && (strExtension != "WMA"))
				continue;
			CString strFilename = strFullPath.Right( nLen - nPos - 1 );
			CString strLocation = strFullPath.Left( nPos );
			CAudioFile* pMp3 = GetCachedMp3File( pMp3List, strLocation , strFilename );
			if( pMp3 != NULL )
			{
				m_mp3List.Add( pMp3 );
			}
		}
		filePlaylist.Close();
	}
}

void CPlaylist::LoadPls( CMp3FileList* pMp3List )
{
	CString strPlaylistFile = GetFullPath();
	int nNbEntries = ::GetPrivateProfileInt( SECT_PLS , ENTR_PLS_NB_ENTRIES, 0 , strPlaylistFile );

	for( int i = 0 ; i < nNbEntries ; i++ )
	{
		CString strEntry;
		strEntry.Format( ENTR_PLS_FILE , i + 1 );
		CString strFullPath;
		::GetPrivateProfileString( SECT_PLS, strEntry, "", strFullPath.GetBuffer(MAX_PATH), MAX_PATH, strPlaylistFile);
		strFullPath.ReleaseBuffer();
		int nLen = strFullPath.GetLength();
		int nPos = strFullPath.ReverseFind('\\');
		CString strFilename = strFullPath.Right( nLen - nPos - 1 );
		CString strLocation = strFullPath.Left( nPos );
		CAudioFile* pMp3 = GetCachedMp3File( pMp3List, strLocation , strFilename );
		if( pMp3 != NULL )
		{
			m_mp3List.Add( pMp3 );
		}
	}

}



BOOL CPlaylist::Delete()
{
	try
	{
		CFile::Remove( m_strLocation + "\\" + m_strFilename );
	}
	catch ( CFileException *e )
	{
		e->Delete();
		TRACE( "Error deleting file !\n");
		return FALSE;
	}
	return TRUE; 

}

CPlaylist::CPlaylist(CPlaylist *pPlaylist)
{
	m_strFilename = pPlaylist->m_strFilename;
	m_strLocation = pPlaylist->m_strLocation;
	m_nType = pPlaylist->m_nType;

	for( int i = 0 ; i < pPlaylist->m_mp3List.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = (CAudioFile *) pPlaylist->m_mp3List.GetAt(i);
		CAudioFile *pNewMp3 = pMp3->GetCopy();
		m_mp3List.Add( pNewMp3 );
	}
}

int CALLBACK ComparePlaylist(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CPlaylist *f1 = (CPlaylist *) lParam1;
	CPlaylist *f2 = (CPlaylist *) lParam2;

	int nSortInfo;
	int nSortOrder;

	if( lParamSort < 0 )
	{
		nSortOrder = -1;
		nSortInfo = -lParamSort;
	}
	else
	{
		nSortOrder = 1;
		nSortInfo = lParamSort;
	}
	CString str1;
	CString str2;
	switch( nSortInfo )
	{
		case FILENAME : 
			str1 = f1->m_strFilename;
			str2 = f2->m_strFilename;
			break;
		case LOCATION : 
			str1 = f1->m_strLocation;
			str2 = f2->m_strLocation;
			break;

	}
	str1.MakeUpper();
	str2.MakeUpper();
	if( str1 < str2 ) return -nSortOrder;
	if( str1 > str2 ) return nSortOrder;

	return 0;

}

CString CPlaylist::GetFullPath()
{
	return m_strLocation + "\\" + m_strFilename;
}




CString CPlaylist::GetName()
{
	
	int nPos = m_strFilename.Find( "." );
	return m_strFilename.Left( nPos );

}


void CPlaylist::Shuffle()
{
	int nCount = m_mp3List.GetSize();
	for( int i = 0 ; i < nCount ; i++ )
	{
		int j = ( ::rand() % nCount );
		m_mp3List.Swap( i , j );
	}
}


void CPlaylist::Reset()
{
	int nCount = m_mp3List.GetSize();
	for( int i = 0 ; i < nCount ; i++ )
	{
		delete m_mp3List.GetAt(i);
	}
	m_mp3List.RemoveAll();
}


CAudioFile* CPlaylist::GetCachedMp3File(CMp3FileList *pMp3List, CString strLocation , CString strFilename )
{
	CAudioFile* pMp3Return;
	BOOL bSearchInCache = TRUE;

	CString strFilePath;
	if( strLocation != "" )
	{
		if( strLocation.Find( ':' ) == -1) 
		{
			// Si l'unité n'est pas spécifié
			strLocation = m_strLocation + "\\" + strLocation;
		}
		strFilePath = strLocation + "\\" + strFilename;

	}
	else if( strFilename.Find( "http://" ) == 0 )
	{
		// Chemin de type URL
		int nLen = strFilename.GetLength();
		int nPos = strFilename.ReverseFind( '/' );
		strLocation = strFilename.Left( nPos );
		strFilename = strFilename.Right( nLen - nPos - 1 );
		// Don't serch an URL in the cache.
		bSearchInCache = FALSE;
	}
	else 
	{
		// Chemin relatif sans sous-répertoire
		strLocation = m_strLocation;
		strFilePath = strLocation + "\\" + strFilename;
	}

	if( bSearchInCache )
	{
		/*
		strFilePath.MakeUpper();
		for( int i = 0 ; i < pMp3List->GetSize() ; i++ )
		{
			CAudioFile* pMp3 = pMp3List->GetAt(i);
			CString strCachedFile = pMp3->GetFullUpperPath();
			if( strFilePath == strCachedFile )
			{
				pMp3Return = pMp3->GetCopy();
				// If the file is found in the cache return
				return pMp3Return;
			}
		}
		*/
		for( int i = 0 ; i < pMp3List->GetSize() ; i++ )
		{
			CAudioFile* pMp3 = pMp3List->GetAt(i);
			if( strFilePath.CompareNoCase( pMp3->GetFullUpperPath() ) == 0 )
			{
				pMp3Return = pMp3->GetCopy();
				// If the file is found in the cache return
				return pMp3Return;
			}
		}
	}
	// File has not found in the cache
	// Chemin de type URL
	int nLen = strFilename.GetLength();
	int nPos = strFilename.ReverseFind( '.' );
	CString strExtension = strFilename.Right( nLen - nPos - 1 );
	strExtension.MakeUpper();
	if( strExtension == "MP3" )
	{
		pMp3Return = new CMp3File();
	}
	else
	{
		return NULL;
	}
	if( !pMp3Return->ReadFileInfos( strLocation, strFilename, m_strLocation ))
	{
		delete pMp3Return;
		return NULL;
	}
	return pMp3Return;
}


BOOL CPlaylist::Save()
{
	switch( m_nType )
	{
	case PLAYLIST_M3U:
		return SaveM3u();
	case PLAYLIST_PLS:
		return SavePls();
	}
	return FALSE;
}

void CPlaylist::Serialize(CArchive& ar , CString strPath )
{
	if (ar.IsStoring())
	{
		// Sauvegarde des adresses relatives pour copier le snapshot sur CD
		CString strLocation;
		strLocation = m_strLocation.Right( m_strLocation.GetLength() - strPath.GetLength());

		ar << m_nType;
		ar << m_strFilename;
		ar << strLocation;
	}
	else
	{
		CString strLocation;
		ar >> m_nType;
		ar >> m_strFilename;
		ar >> strLocation;
		m_strLocation = strPath + strLocation;
		if( m_strLocation.Right( 1 ) == "\\" )
		{
			m_strLocation = m_strLocation.Left( m_strLocation.GetLength() - 1 );
		}

	}

}


/////////////////////////////////////////////////////////////////////////////
// CPlaylistList

IMPLEMENT_DYNCREATE(CPlaylistList, CObArray)

CPlaylistList::CPlaylistList()
{
}


CPlaylistList::~CPlaylistList()
{
	Reset();
}

void CPlaylistList::Reset()
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		delete GetAt( i );
	}
	RemoveAll();

}

/////////////////////////////////////////////////////////////////////////////
// CPlaylistList diagnostics

#ifdef _DEBUG
void CPlaylistList::AssertValid() const
{
	CObArray::AssertValid();
}

void CPlaylistList::Dump(CDumpContext& dc) const
{
	CObArray::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlaylistList serialization

void CPlaylistList::Serialize(CArchive& ar , CString strPath, CMp3FileList *pMp3List )
{

	int nFileCount;

	if (ar.IsStoring())
	{
		nFileCount = 0;
		for( int i = 0 ; i < GetSize() ; i++ )
		{
			CPlaylist *pPlaylist = (CPlaylist *) GetAt(i);
			if( pPlaylist->IsUnderDir( strPath ))
				nFileCount++;
		}
		ar << nFileCount;
		for( i = 0 ; i < GetSize() ; i++ )
		{
			CPlaylist *pPlaylist = (CPlaylist *) GetAt(i);
			if( pPlaylist->IsUnderDir( strPath ))
				pPlaylist->Serialize( ar, strPath );
		}
	}
	else
	{
		ar >> nFileCount;
		for( int i = 0 ; i < nFileCount ; i++ )
		{
			CPlaylist* pPlaylist = new CPlaylist();
			pPlaylist->Serialize( ar , strPath );
			pPlaylist->Load( pMp3List );
			Add( pPlaylist );
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CPlaylistList commands


CPlaylist* CPlaylistList::GetAt(int nIndex)
{
	return (CPlaylist*) CObArray::GetAt( nIndex );
}

int CPlaylistList::GetSize()
{
	return CObArray::GetSize();
}

int CPlaylistList::Add(CPlaylist *pPlaylist)
{
	return  CObArray::Add( (CObject*) pPlaylist );
}

void CPlaylistList::DeleteFile(CPlaylist *pPlaylist)
{
	for( int i = 0 ; i < GetSize() ; i++ )
	{
		if( pPlaylist == GetAt( i ))
		{
			RemoveAt( i );
			delete pPlaylist;
			break;
		}
	}
}

