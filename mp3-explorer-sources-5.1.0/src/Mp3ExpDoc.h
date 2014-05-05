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
 

#if !defined(AFX_MP3EXPDOC_H__6097C6CA_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_MP3EXPDOC_H__6097C6CA_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#include "mp3/Mp3File.h"	// Added by ClassView

#include "SortedList.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UPDATE_RESET_TREEVIEW	1L
#define UPDATE_FILL_TREEVIEW	2L
#define UPDATE_SET_VIEW_ARTIST	3L
#define UPDATE_SET_VIEW_GENRE	4L
#define UPDATE_SET_VIEW_ALBUM	5L
//#define UPDATE_VIEW_OPTIONS		6L
#define UPDATE_FILE_LIST		7L
#define UPDATE_HTMLVIEW			8L
#define UPDATE_PLAYLIST_VIEW	9L
#define UPDATE_DETAIL_VIEW		10L


enum HTMLVIEW {	HTMLVIEW_DEFAULT, 
		HTMLVIEW_FILE, 
		HTMLVIEW_README, 
		HTMLVIEW_HELP,
		HTMLVIEW_SEARCH,
		HTMLVIEW_WEBSITE
} ;


class CFileLocation : public CObject
{
public:
	CString	m_strFilename;
	CString	m_strLocation;

	CFileLocation( CString strFilename , CString strLocation );
};

class CFileLocationList : public CObArray
{
public:
	void AddFile( CString strFilename , CString strLocation );

	CFileLocationList();
	~CFileLocationList();
	CString GetLocation( int nIndex );
	CString GetFilename( int nIndex );
};

class CMp3ExpDoc : public CDocument
{
protected: // create from serialization only
	CMp3ExpDoc();
	DECLARE_DYNCREATE(CMp3ExpDoc)

// Attributes
private:
	CPlaylistList m_playlistList;
	int m_nPathIcon;
	CString m_strPathTitle;

// Operations
public:
	HTMLVIEW m_nHtmlView;

	CString m_strPath;
	CMp3FileList m_mp3List;

	CMapStringToOb m_mapArtist;
	CMapStringToOb m_mapGenre;
	CMapStringToOb m_mapAlbum;

	CSortedList m_slistArtist;
	CSortedList m_slistGenre;

	CArtist*  GetArtist( LPCSTR strName );
	CGenre*  GetGenre( LPCSTR strName );
	CAlbum*  GetAlbum( LPCSTR strName );


	void AddFile( CString strLocation, CString strFileName, int nType );


	BOOL GetFileLocation( CString strPath, CString strMask, CFileLocationList& filelist );
	BOOL FindFiles( CString strPath, BOOL bResetPlaylistList );
	BOOL ScanFileType( CString strPath, CString strMask, int nType );
	BOOL ScanPlaylistType( CString strPath, CString strMask, int nType );
	void ResetList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3ExpDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateDataFile(  CString strPath, CString strFilename, CMp3FileList *pListMp3, CPlaylistList *pListPlaylist);
	void UpdateFoundCount();
	CString m_strSearchUrl;
	void SetSearchUrl(CString strUrl );
	void SetHtmlView(int nView);
	CString m_strWebSiteUrl;
	CString m_strReadmeUrl;
	CString m_strHelpUrl;
	BOOL PlaylistDelete();
	void PlaylistShuffle();
	void PlaylistReset();
	BOOL SavePlaylist( BOOL bSaveAs );
	CPlaylist* m_pPlaylistSelected;
	void UpdatePlaylistSelection( CPlaylist* pPlaylist);
	void UpdateLowerPanesViews();
	CString GetUrl();
	CAudioFile* m_pAudioFileSelected;
	void UpdateFileSelection( CAudioFile* pAudioFile );
	CPlaylistList* GetPlaylistList();
	CPlaylist* GetPlaylist(int nIndex );
	int GetPlaylistCount();
	void AddPlaylist( CPlaylist* pPlaylist );
	CPlaylist m_playlistMemory;
	void ScanAll( BOOL bScan );
	BOOL ReadCacheFile( CString strPath );
	BOOL CreateCacheFile( CString strPath );
	void ResetPlaylistList();
	CPlaylist* LoadPlaylist( CString strFilename, CString strLocation, int nType );
	BOOL DeletePlaylist( CPlaylist* pPlaylist );
	void GetPathInfos( CString& strPathTitle, int& nIcon);
	static BOOL IsValidFile( CString strFileName );
	static BOOL IsValidPath( CString strPath );
	static void StripSlash( CString& strPath );

	void RebuildSnapshot();
	CString GetPath();
	void SetNewPath( CString strNewPath );
	void CreateSnapshot();
	void SearchFiles( CString strNewPath , BOOL bScan, BOOL bResetPlaylistList );
	void RebuiltCategoryLists();
	void GetFiles( BOOL bScan );
	BOOL OnNewPath( CString strPath );
	CMp3FileList* GetFileList();
	void AddFileToArtistList( CAudioFile* pMp3 );
	void AddFileToGenreList( CAudioFile* pMp3 );
	void AddFileToAlbumList( CAudioFile* pMp3 );
	void ResetArtistList();
	void ResetGenreList();
	void ResetAlbumList();
	void UpdateLists();
	virtual ~CMp3ExpDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMp3ExpDoc)
	afx_msg void OnDisplayAll();
	afx_msg void OnUpdateDisplayAll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3EXPDOC_H__6097C6CA_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
