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
 

#if !defined(AFX_MP3FILE_H__6097C6D6_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_MP3FILE_H__6097C6D6_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mp3File.h : header file
//

#include "id3v2/id3_tag.h"
#include <afxcoll.h>

#define FILE_EXT_MP3	"*.MP3"
#define FILE_EXT_WMA	"*.WMA"
#define FILE_TYPE_MP3	1
#define FILE_TYPE_WMA	2


#ifdef _WMA
	#include "wmaudiosdk.h"
#else
	struct IWMAudioInfo; // Classe bidon
	struct IWMAudioReader; // Classe bidon
#endif //_WMA

/////////////////////////////////////////////////////////////////////////////
// CMp3File 
#define WT_SINGLE_EDIT 0x01
#define WT_MULTI_EDIT 0x02


#define FILENAME	1
#define ARTIST		2
#define	TITLE		3
#define	LOCATION	4
#define FILESIZE	5
#define ALBUM		6
#define GENRE		7
#define	DATE		8
#define COMMENT		9
#define TIME		10
#define BITRATE		11
#define FREQUENCY	12
#define MODE		13
#define LAYER		14
#define TRACKNUM	15
#define RATING		16
#define ARTISTURL	17
#define LYRICS		18

#define NUM_INFORMATION 19

#define NUM_BITRATE		15
#define NUM_RATING		11
#define NUM_GENRE		149
#define BLANK_GENRE (NUM_GENRE-1)


// PLS playlist management
#define SECT_PLS				"playlist"
#define ENTR_PLS_NB_ENTRIES		"NumberOfEntries"
#define ENTR_PLS_FILE			"File%d"

#define NORMALIZE_NONE		0
#define NORMALIZE_UPPER		1
#define NORMALIZE_LOWER		2
#define NORMALIZE_PROPER	3
#define NORMALIZE_CAPFIRST	4




static _TCHAR * _gszRating[NUM_RATING] = {
_T(""),
_T("1"),
_T("2"),
_T("3"),
_T("4"),
_T("5"),
_T("6"),
_T("7"),
_T("8"),
_T("9"),
_T("10")
};

static unsigned char _gnRating[NUM_RATING] = {
0,
1,
2,
3,
4,
5,
6,
7,
8,
9,
10
};

static _TCHAR * _gszBitrate[NUM_BITRATE] = {
_T(""),
_T("32 kbit/s"),
_T("48 kbit/s"),
_T("56 kbit/s"),
_T("64 kbit/s"),
_T("80 kbit/s"),
_T("96 kbit/s"),
_T("112 kbit/s"),
_T("128 kbit/s"),
_T("160 kbit/s"),
_T("192 kbit/s"),
_T("224 kbit/s"),
_T("256 kbit/s"),
_T("320 kbit/s"),
_T("384 kbit/s")
};

static int _gnBitrate[NUM_BITRATE] = {
0,
32000,
48000,
56000,
64000,
80000,
96000,
112000,
128000,
160000,
192000,
224000,
256000,
320000,
384000
};

static _TCHAR * _gszGenre[NUM_GENRE] = {
_T("Blues"), 
_T("Classic Rock"), 
_T("Country"), 
_T("Dance"), 
_T("Disco"), 
_T("Funk"), 
_T("Grunge"), 
_T("Hip-Hop"), 
_T("Jazz"), 
_T("Metal"), 
_T("New Age"), 
_T("Oldies"), 
_T("Other"), 
_T("Pop"), 
_T("R&B"), 
_T("Rap"), 
_T("Reggae"), 
_T("Rock"), 
_T("Techno"), 
_T("Industrial"), 
_T("Alternative"), 
_T("Ska"), 
_T("Death Metal"), 
_T("Pranks"), 
_T("Soundtrack"), 
_T("Euro-Techno"), 
_T("Ambient"), 
_T("Trip-Hop"), 
_T("Vocal"), 
_T("Jazz+Funk"), 
_T("Fusion"), 
_T("Trance"), 
_T("Classical"), 
_T("Instrumental"), 
_T("Acid"), 
_T("House"), 
_T("Game"), 
_T("Sound Clip"), 
_T("Gospel"), 
_T("Noise"), 
_T("AlternRock"), 
_T("Bass"), 
_T("Soul"), 
_T("Punk"), 
_T("Space"), 
_T("Meditative"), 
_T("Instrumental Pop"), 
_T("Instrumental Rock"), 
_T("Ethnic"), 
_T("Gothic"), 
_T("Darkwave"), 
_T("Techno-Industrial"), 
_T("Electronic"), 
_T("Pop-Folk"), 
_T("Eurodance"), 
_T("Dream"), 
_T("Southern Rock"), 
_T("Comedy"), 
_T("Cult"), 
_T("Gangsta"), 
_T("Top 40"), 
_T("Christian Rap"), 
_T("Pop/Funk"), 
_T("Jungle"), 
_T("Native American"), 
_T("Cabaret"), 
_T("New Wave"), 
_T("Psychadelic"), 
_T("Rave"), 
_T("Showtunes"), 
_T("Trailer"), 
_T("Lo-Fi"), 
_T("Tribal"), 
_T("Acid Punk"), 
_T("Acid Jazz"), 
_T("Polka"), 
_T("Retro"), 
_T("Musical"), 
_T("Rock & Roll"), 
_T("Hard Rock"), 
_T("Folk"), 
_T("Folk/Rock"), 
_T("National Folk"), 
_T("Swing"), 
_T("Fast-Fusion"), 
_T("Bebob"), 
_T("Latin"), 
_T("Revival"), 
_T("Celtic"), 
_T("Bluegrass"), 
_T("Avantgarde"), 
_T("Gothic Rock"), 
_T("Progressive Rock"), 
_T("Psychedelic Rock"), 
_T("Symphonic Rock"), 
_T("Slow Rock"), 
_T("Big Band"), 
_T("Chorus"), 
_T("Easy Listening"), 
_T("Acoustic"), 
_T("Humour"), 
_T("Speech"), 
_T("Chanson"), 
_T("Opera"), 
_T("Chamber Music"), 
_T("Sonata"), 
_T("Symphony"), 
_T("BootyBass"), 
_T("Primus"), 
_T("Porn Groove"), 
_T("Satire"), 
_T("Slow Jam"), 
_T("Club"), 
_T("Tango"), 
_T("Samba"), 
_T("Folklore"), 
_T("Ballad"), 
_T("Power Ballad"), 
_T("Rhythmic Soul"), 
_T("Freestyle"), 
_T("Duet"), 
_T("Punk Rock"), 
_T("Drum Solo"), 
_T("A Cappella"), 
_T("Euro-House"), 
_T("Dance Hall"), 
_T("Goa"), 
_T("Drum & Bass"), 
_T("Club-House"), 
_T("Hardcore"), 
_T("Terror"), 
_T("Indie"), 
_T("Britpop"), 
_T("Negerpunk"), 
_T("Polsk Punk"), 
_T("Beat"), 
_T("Christian Gangsta"), 
_T("Heavy Metal"), 
_T("Black Metal"), 
_T("Crossover"), 
_T("Contemporary Chorus"), 
_T("Christian Rock"), 
_T("Merengue"), 
_T("Salsa"), 
_T("Thrash Metal"), 
_T("Anime"), 
_T("Jpop"), 
_T("Synthpop"),
_T("")
};

static _TCHAR *_gszExportBookmark[NUM_INFORMATION] = {	
	"",
	"@FILENAME",	
	"@ARTIST",		
	"@TITLE",
	"@LOCATION",	
	"@SIZE",		
	"@ALBUM",		
	"@GENRE",		
	"@DATE",		
	"@COMMENT",		
	"@TIME",		
	"@BITRATE",		
	"@FREQUENCY",	
	"@MODE",		
	"@LAYER",
	"@TRACKNUM",
	"@RATING",
	"@WWWARTISTURL",
	"@LYRICS"
};

// Fonction de comparaison de 2 Mp3 pour le tri
int CALLBACK CompareMp3(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK ComparePlaylist(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

class CTag : public CObject
{
	CString m_strTitle;
	CString m_strArtist;
	CString m_strAlbum;
	CString m_strYear;
	CString m_strComment;
	CString m_strGenre;
	CString m_strTrackNum;
	unsigned char m_nRating;
	CString m_strArtistUrl;
	CString m_strLyrics;
	CString m_strPictureFile;
	BOOL m_bHasPicture;
	BOOL m_bHasLyrics;

public:
	CTag();
	CString		GetTitle() { return m_strTitle;};
 	CString		GetArtist() { return m_strArtist;};
 	CString		GetAlbum() { return m_strAlbum;};
 	CString		GetYear() { return m_strYear;} ;
 	CString		GetComment() { return m_strComment;};
	CString		GetGenre() { return m_strGenre; };
	CString		GetTrackNum() { return m_strTrackNum; };
	unsigned char GetRating() { return m_nRating; };
	CString		GetArtistUrl() { return m_strArtistUrl; };
	CString		GetPicture() { return m_strPictureFile; };
	CString		GetLyrics() { return m_strLyrics; };
	BOOL		HasLyrics()  { return m_bHasLyrics; };
	BOOL		HasPicture()  { return m_bHasPicture; };

	void SetTitle( CString strTitle )     { m_strTitle = strTitle; };
	void SetArtist( CString strArtist )   { m_strArtist = strArtist; };
	void SetAlbum( CString strAlbum )     { m_strAlbum = strAlbum; };
	void SetYear( CString strYear )       { m_strYear = strYear; };
	void SetComment( CString strComment ) { m_strComment = strComment; };
	void SetGenre( CString strGenre ) { m_strGenre = strGenre; };
	void SetTrackNum( CString strTrackNum ) { m_strTrackNum = strTrackNum; };
	void SetRating( unsigned char nRating ) { m_nRating = nRating; };
	void SetArtistUrl( CString strArtistUrl ) { m_strArtistUrl = strArtistUrl; };
	void SetPicture( const char* strFilename ){ m_strPictureFile = strFilename; };
	void SetLyrics( const char* strLyrics ){ m_strLyrics = strLyrics; };
	void HasLyrics( BOOL bDoHave )  { m_bHasLyrics = bDoHave; };
	void HasPicture( BOOL bDoHave )  { m_bHasPicture = bDoHave; };


};

class CAudioFile : public CObject
{
protected:
//	DECLARE_DYNCREATE(CAudioFile)

public:
	// Construction

	CAudioFile();
	~CAudioFile();
	CAudioFile( CAudioFile* pMp3 );

public: 
	// static variables 
	static CString strUndefined;
	static CString strNotAvailable;
	static CString strProtected;
	static CString strSizeFormat;
	static CString strBitrateFormat;
	static CString strDebugLogFile;
	static BOOL bWriteId3v1Tag;
	static BOOL bReadId3v2Tag;
	static BOOL bWriteId3v2Tag;
	static BOOL bDebug;

protected:
	void BuildFullUpperPath();
	// Attibutes
	CString m_strName;
	CString m_strLocation;
	CString m_strTitle;
	CString m_strArtist;
	CString m_strAlbum;
	CString m_strYear;
	CString m_strComment;
	CString m_strLayer;
	CString	m_strBitrate;
	CString	m_strFrequency;
	CString m_strMode;
	int m_nBitrate;
	int m_nTime;
	long m_nSize;
	CString m_strUpperLocation;
	CString m_strSize;

	CString m_strGenre;
	int m_nFileType;
//	int m_nTrackNo;
	CString m_strTrackNum;
	unsigned char  m_nRating;
	CString m_strArtistUrl;

	CString m_strAltLocation;

public:
	// Interface

	const CString&		GetName() { return m_strName;};
 	const CString&		GetLocation() { return m_strLocation;};
 	const CString&		GetTitle() { return m_strTitle;};
 	const CString&		GetArtist() { return m_strArtist;};
 	const CString&		GetAlbum() { return m_strAlbum;};
	const CString&		GetGenre() { return m_strGenre;};
 	const CString&		GetYear() { return m_strYear;} ;
 	const CString&		GetComment() { return m_strComment;};
 	int					GetTime() { return m_nTime;} ;
	int					GetBitrateValue() { return m_nBitrate;} ;
	LONG				GetSize() { return m_nSize; };
	const CString&		GetFullUpperPath() { return m_strFullUpperPath; };
	const CString&		GetTrack() { return m_strTrackNum; };
	unsigned char		GetRatingNum() { return m_nRating; };
	const CString&		GetArtistUrl() { return m_strArtistUrl; };

	CString&			GetFormatedSize() { return m_strSize; };
	CString				GetRating() { return CString( _gszRating[ m_nRating ] ); };
	int					GetType() { return m_nFileType; };

	void SetTitle( const CString& strTitle )		{ m_strTitle = strTitle; };
	void SetArtist( const CString& strArtist )		{ m_strArtist = strArtist; };
	void SetAlbum( const CString& strAlbum )		{ m_strAlbum = strAlbum; };
	void SetYear( const CString& strYear )			{ m_strYear = strYear; };
	void SetGenre( const CString& strGenre )		{ m_strGenre = strGenre; };
	void SetComment( const CString& strComment )	{ m_strComment = strComment; };
	void SetTrackNum( const CString& strTrackNum )	{ m_strTrackNum = strTrackNum; };
	void SetRating( unsigned char  nRating )		{ m_nRating = nRating; };
	void SetArtistUrl( const CString& strArtistUrl ){ m_strArtistUrl = strArtistUrl; };



	// Operations
	BOOL IsUnderDir( CString strPath );
	CString GetFullPath();
	BOOL Delete();
	CString CleanFilename( CString strSource );
	BOOL Rename( const char*  strNewName );

	void ModifyTagInfo(CString strArtist, CString strAlbum, CString strGenre);
	CString GetInformation(int nIndex);

	CString FormatExport( CString strExportFormat );


	// Statics
	static CString FormatSize( LONG lSize );
	static CString FormatTime( int nTime );
	static unsigned char GetGenreIndex( CString strGenre );
	static CString MakeProper(CString strInformation);
	static CString Normalize( CString strSource, int nNorme, BOOL bRemoveUnderscores );
	static CString CapitalizeFirstChar( CString strSource );

	static void DebugTrace(const char *strTrace);

public:
	void HandleException( CException *e, CString strContext);
	CString GetGenreFromIndex( unsigned char nIndex );
	int GetTrackNum();
	CString GetHtmlLyrics();
	BOOL m_bHasPicture;
	BOOL m_bHasLyrics;
	CString m_strFullUpperPath;
	CString GetPath( CString strRoot , BOOL bRelative );
	CString GetShortInfo();
	void Copy( CAudioFile* pAudioFile );

	// Overridables
	virtual	BOOL ReadFileInfos( CString strFileName, CString strLocation, CString strAltLocation ) = 0;	
	virtual	BOOL WriteFileInfos( CTag *tag ) = 0;
	virtual CString GetFileExt() = 0;
	virtual CAudioFile* GetCopy() = 0;
	virtual BOOL SetPicture( ID3_Tag& tag, const char* strFilename ) = 0;
	virtual BOOL GetPicture( CString& strTmpFile) = 0;
	virtual CString GetLyrics() = 0;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioFile)
	public:
	virtual void Serialize(CArchive& ar, CString strPath );   // overridden for document i/o
	protected:
	//}}AFX_VIRTUAL

};


class CMp3File : public CAudioFile
{
protected:
	DECLARE_DYNCREATE(CMp3File)

public:
	CMp3File();
	CMp3File( CAudioFile *pAudioFile );
	~CMp3File();

public:
	static BOOL IsV1Genre( LPCSTR strGenre );

	long m_nTagSize;
	BOOL SetTagNumField( ID3_Tag& tag, int nValue, ID3_FrameID nFrameID, ID3_FieldID nFieldID );
	BOOL GetTagNumField( ID3_Tag& tag, int& nValue, ID3_FrameID nFrameID, ID3_FieldID nFieldID);
	virtual BOOL SetPicture( ID3_Tag& tag, const char* strFilename );
	virtual BOOL GetPicture( CString& strTmpFile);
	virtual CString GetLyrics();
	BOOL WriteID3v1Tag( const char *strTag );
	// Overridables

	virtual	BOOL ReadFileInfos( CString strFileName, CString strLocation, CString strAltLocation );	
	virtual	BOOL WriteFileInfos( CTag *tag );
	virtual CString GetFileExt();
	virtual CAudioFile* GetCopy();

private:
//	Gestion des tags
	BOOL m_bTag;
	LONG m_nTagOffset;

	int ReadTag(CString strAltLocation);
	int ReadId3v1Tag(CString strAltLocation);
	int ReadId3v2Tag(CString strAltLocation);
	BOOL ExtractField( CString& strField, char* tag, int nOffset, int nLength );

	BOOL GetTagField( ID3_Tag& tag, CString& strField, ID3_FrameID nFrameID, ID3_FieldID nFieldID = ID3FN_TEXT, int nBufferSize = 4096 );
	BOOL SetTagField( ID3_Tag& tag, const char* strField, ID3_FrameID nFrameID, ID3_FieldID nFieldID = ID3FN_TEXT );
	
//  Gestion des informations MPEG	
	BOOL m_bVbr;
	BOOL ReadMpegHeader();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioFile)
	public:
	virtual void Serialize(CArchive& ar, CString strPath );   // overridden for document i/o
	protected:
	//}}AFX_VIRTUAL

};





/////////////////////////////////////////////////////////////////////////////
// CMp3FileList document

/////////////////////////////////////////////////////////////////////////////
// CMp3FileList document

class CMp3FileList : public CObArray
{
protected:
	DECLARE_DYNCREATE(CMp3FileList)

// Attributes
public:
	CMp3FileList();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3FileList)
	public:
	virtual void Serialize(CArchive& ar, CString strPath );   // overridden for document i/o
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetIndex( CAudioFile* pAudioFile );
	void Shuffle();
	void Swap( int n1 , int n2 );
	BOOL MoveDown(CAudioFile *pMp3 );
	BOOL MoveUp( CAudioFile* pMp3 );
	int GetSize();
	void DeleteFile( CAudioFile *pMp3);
	int Add( CAudioFile* pMp3 );
	CAudioFile* GetAt( int nIndex );
	virtual ~CMp3FileList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


class CPlaylist : public CObject
{
protected:
	DECLARE_DYNCREATE(CPlaylist)

public:
	CPlaylist();
	CPlaylist( CPlaylist* pPlaylist );
	CPlaylist( CString strFilename, CString strLocation, int nType );
	~CPlaylist();

public:
	CString	m_strFilename;
	CString	m_strLocation;
	int m_nType;
	CMp3FileList m_mp3List;


public:
	void Reset();
	BOOL CanBeRelative();
	void Shuffle();
	CString GetName();
	BOOL IsUnderDir( CString strPath );
	BOOL Save();
	CAudioFile* GetCachedMp3File(CMp3FileList *pMp3List, CString strLocation , CString strFilename );
	CString GetFullPath();
	BOOL Delete();
	void Load( CMp3FileList* pMp3List );
	void LoadM3u( CMp3FileList* pMp3List );
	void LoadPls( CMp3FileList* pMp3List );
	BOOL SaveM3u();
	BOOL SavePls();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioFile)
	public:
	virtual void Serialize(CArchive& ar , CString strPath );   // overridden for document i/o
	protected:
	//}}AFX_VIRTUAL

};


class CCategory : public CObject
{
protected:

	CString m_strName;
	CMp3FileList m_mp3List;
	void ResetList();


public:
	CObArray m_listName;
	CMp3FileList* GetFileList() { return &m_mp3List; };
	CCategory( const CString& strName ) { m_strName = strName; };
	CCategory();
	~CCategory();
	const CString& GetName() { return m_strName; };

};

class CAlbum;


class CArtist : public  CCategory
{
public:
	DECLARE_DYNCREATE(CArtist)

	CMapStringToOb m_mapAlbum;

	void DeleteAlbums();
	CAlbum* GetAlbum( LPCSTR strName );

	CArtist();
	~CArtist();
	CArtist( const CString& strName ) { m_strName = strName; };

	AddFile( CAudioFile *pmp3 , BOOL bGetAlbum );

};


class CGenre : public CCategory
{
public:
	DECLARE_DYNCREATE(CGenre)

	CMapStringToOb m_mapArtist;

	void DeleteArtists();
	CArtist* GetArtist( LPCSTR strName );

	CGenre();
	~CGenre();
	CGenre( const CString& strName ) { m_strName = strName; };

	AddFile( CAudioFile *pMp3 );

};

class CAlbum : public CCategory
{
public:

	DECLARE_DYNCREATE(CAlbum)

	CMapStringToOb m_mapArtist;

	void DeleteArtists();
	CArtist* GetArtist( LPCSTR strName );


	CAlbum();
	~CAlbum();
	CAlbum( const CString& strName ) { m_strName = strName; };

	AddFile( CAudioFile *pMp3 , BOOL bGetArtist );


};


class CPlaylistList : public CObArray
{
protected:
	DECLARE_DYNCREATE(CPlaylistList)

// Attributes
public:
	CPlaylistList();           // protected constructor used by dynamic creation
	virtual ~CPlaylistList();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3FileList)
	public:
	virtual void Serialize(CArchive& ar, CString strPath, CMp3FileList *pMp3List );   // overridden for document i/o
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Reset();
	int GetSize();
	void DeleteFile( CPlaylist *pMp3);
	int Add( CPlaylist* pPlaylist );
	CPlaylist* GetAt( int nIndex );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};



#endif // !defined(AFX_MP3FILE_H__6097C6D6_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
