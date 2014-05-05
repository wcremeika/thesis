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
 

#if !defined(AFX_MP3Exp_H__6097C6C4_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
#define AFX_MP3Exp_H__6097C6C4_4E77_11D2_87FC_893779BA9A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "afxole.h"
#include "resource.h"       // main symbols
#include "mp3/Mp3File.h"
#include "Mp3List.h"
#include "DropTarget.h"
#include "NormalizationProfile.h"


// Section et entrées du registre
#define REGISTRY_KEY "Bepo Softs"
#define SECT_MP3 "MP3"
#define ENTR_FILEPATH "FilePath"
#define ENTR_FILEPATH_DEF "c:\\"
#define ENTR_FILEEXT "FileExt"
#define ENTR_FILEEXT_DEF "*.mp3"
#define SECT_USR "User settings"
#define ENTR_FAVORITE_ADRESSES "FavoriteAddresses"
#define ENTR_SHOWLABELS "ShowLabels"
#define ENTR_VIEWTYPE "ViewType"
#define ENTR_ADDRESS "Address%d"
#define ENTR_ADDRESS_DEF ""
#define MAX_ADDRESS 15
#define MAX_BUFFER	255
#define ENTR_CX "cx"
#define ENTR_CY "cy"
#define ENTR_X "x"
#define ENTR_Y "y"
#define ENTR_CX_DEF 600
#define ENTR_CY_DEF 400
#define ENTR_X_DEF 50 
#define ENTR_Y_DEF 50
#define ENTR_DBLECLICK	"DoubleClickAction"
#define ENTR_FULLROWSEL	"FullRowSelection"
#define ENTR_ALIGNMENT  "ColumnAlignment"
#define ENTR_SORTINGCOL "SortingColumn"
#define ENTR_SECONDARYSORTINGCOL "SecondarySortingColumn"
#define ENTR_PLAYLISTSORTINGCOL "PlaylistSortingColumn"
#define ENTR_COL_SETTINGS "ColumnsSettings"	
#define ENTR_ALL_COLS	"DisplayAll"
#define ENTR_FORMAT_COUNT "FormatCount"
#define ENTR_FORMAT_DEFAULT "DefaultFormat"
#define ENTR_SHOWVIEWER "ShowMiniExplorer"
#define ENTR_CX1 "cx1"
#define ENTR_CX2 "cx2"
#define ENTR_CY1 "cy1"
#define ENTR_CY2 "cy2"
#define ENTR_CX1_DEF 300
#define ENTR_CX2_DEF 300
#define ENTR_CY1_DEF 200
#define ENTR_CY2_DEF 200
#define ENTR_LASTFIND "LastFindString"
#define ENTR_AUTO_RENAME "AutoRename"
#define ENTR_DISPLAY_PLAYLISTS "ViewPlaylists"
#define ENTR_MAXIMIZED "Maximized"

#define SECT_COLUMNS "Column%d"
#define ENTR_COL_ALIGN	"Alignment"
#define ENTR_COL_ORDER	"Order"
#define ENTR_COL_DISPLAY "Displayed"
#define ENTR_COL_WIDTH	"Width"

#define SECT_ADVANCED "AdvancedOptions"
#define ENTR_CMD_PLAY "CommandPlay"
#define ENTR_CMD_ENQUEUE "CommandEnqueue"
#define ENTR_CMD_PLAY_DEF "Play"
#define ENTR_CMD_ENQUEUE_DEF "Enqueue"
#define ENTR_MAX_ROW "MaxRow"
#define ENTR_MAX_ROW_DEF 10000
#define ENTR_CACHED_CATALOG "UseCachedCatalog"
#define ENTR_SYSTRAY "MinimizeToSystray"
#define ENTR_DISABLE_CONNECTION "DisableConnection"
#define ENTR_WRITE_ID3V1_TAGS "WriteId3v1Tags"
#define ENTR_USE_ID3V2_TAGS "UseId3v2Tags"

#define SECT_TAG "TagOptions"
#define ENTR_INTELLITAG_VALIDATE "IntelliTagValidate"
#define ENTR_NORMALIZE_TAG "NormalizeTag"


#define SECT_PLAYLIST "Playlist"
#define ENTR_PL_SCAN_M3U "ScanM3U"
#define ENTR_PL_SCAN_PLS "ScanPLS"

#define SECT_FILE "Playlist"
#define ENTR_FILE_SCAN_MP3 "ScanMP3"


#define SECT_FORMATS "Format%d"
#define ENTR_FMT_NAME "Name"
#define ENTR_FMT_FIELD1 "Field1"
#define ENTR_FMT_FIELD2 "Field2"
#define ENTR_FMT_FIELD3 "Field3"
#define ENTR_FMT_FIELD4 "Field4"
#define ENTR_FMT_SEPARATOR1 "Separator1"
#define ENTR_FMT_SEPARATOR2 "Separator2"
#define ENTR_FMT_SEPARATOR3 "Separator3"
#define ENTR_FMT_UNDERSCORES "Underscores"
#define ENTR_FMT_CASSE "ConvertCase"

#define SECT_EXPORT "Export"
#define ENTR_EXP_CHOOSEFILE			"ChooseFile"
#define ENTR_EXP_DEFAULT_FORMAT		"DefaultFormat"
#define ENTR_EXP_REINIT				"ReinitFile"
#define ENTR_EXP_DEFINITION_NAME	"DefinitionName"
#define ENTR_EXP_DEFINITION_FILE	"DefinitionFile"
#define ENTR_EXP_FILENAME			"Filename"


#define SECT_SETUP			"Setup"
#define ENTR_PATH			"InstallPath"
#define ENTR_RESDLL			"Plugin"
#define MP3E_INI			"MP3-EXPLORER.INI"
#define MP3E_EXE			"MP3-EXPLORER.EXE"
#define ENTR_PLAYER			"Player"
#define ENTR_DEBUG			"Debug"
#define ENTR_LANGUAGE		"LanguageFile"

#define SECT_SEARCH			"Search"
#define ENTR_ENGINE_DLL		"Engine" 

#define SECT_NORMALIZE				"TagNormalization"
#define ENTR_NORMALIZE_FORMAT		"Field%d"
#define ENTR_NORMALIZE_UNDERSCORES	"ReplaceUnderscores"


// Type de vues de l'arborescence
#define VIEW_ARTIST 0
#define VIEW_GENRE  1
#define VIEW_ALBUM  2

// double click action
#define PLAY		0
#define ENQUEUE		1
#define PROPERTIES	2

// columns alignement
#define ALIGN_LEFT		0
#define ALIGN_CENTER	1


#define MAX_STATUS	255	 //Taille maxi du buffer pour la ligne de status

// Icons IDS
#define	ICON_TITLE		0
#define ICON_DRIVE		1
#define ICON_FOLDER		2
#define	ICON_GENRE		3
#define ICON_ARTIST		4
#define ICON_ALBUM		5
#define ICON_FAVORITE	6
#define ICON_PLAYLIST	7

#define COL_NAME		0
#define COL_TITLE		1
#define COL_ALBUM		2
#define COL_LOCATION	3
#define COL_SIZE		4
#define COL_ARTIST		5
#define COL_YEAR		6
#define COL_GENRE		7
#define COL_COMMENT		8



#define	WM_SYSTRAY_NOTIFY			WM_USER

#define SNAPSHOT_FILE	"mp3cache.mex"
#define TMP_PLAYLIST	"~playlst.m3u"

#define HTMLVIEW_DIR	"\\HtmlView\\"
#define SKIN_DIR		"\\skin\\"
#define LANGUAGE_DIR		"\\lang\\"


#define PLAYLIST_MEMORY	0
#define PLAYLIST_EXT_M3U "*.M3U"
#define PLAYLIST_EXT_PLS "*.PLS"
#define PLAYLIST_M3U	1
#define PLAYLIST_PLS	2

#define FILE_EXT_MP3	"*.MP3"
#define FILE_TYPE_MP3	1
#define FILE_TYPE_WMA	2

//#define CACHE_FILE_HEADER  "CACHEV310"
#define CACHE_FILE_HEADER  "CACHEV391"
#define RESDLL_VERSION "5.1.0"
#define TRIALTEST_VERSION 430


#define WM_PLAY		WM_USER + 100
#define WM_ENQUEUE	WM_USER + 101
#define WM_DELETE	WM_USER + 102

#define COLOR_LINK			RGB(64,0,128)
#define COLOR_CREDITS		RGB(137,115,217)
#define COLOR_WHITE			RGB( 255, 255, 255 )
#define COLOR_BLACK			RGB( 0, 0, 0 )
#define COLOR_LISTBOX_SEL	RGB( 90, 0, 200 )


#define MP3EXPLORER_CLASSNAME _T("Mp3ExplorerWndClass")
#define MP3PLAYER_CLASSNAME _T("Mp3PlayerWndClass")

#define GENRE_ID3V1 _T("(ID3 v1) ")
#define GENRE_ID3V1_LEN 9

#define RES_NONE _T("NONE")

typedef struct {
	int  nItemFlag;
	int	 nItemID;
	char *pText;
}
DYNAMIC_MENU;


/////////////////////////////////////////////////////////////////////////////
// CMp3ExpApp:
// See mp3Exp.cpp for the implementation of this class
//

class CMp3ExpApp : public CWinApp
{
public:
	CMenu m_menuMain;
	CMenu m_menuFile;
	CMenu m_menuEdit;
	CMenu m_menuDisplay;
	CMenu m_menuView;
	CMenu m_menuHelp;

	void CreateMainMenu();
	void CreateDynamicMenu( CMenu* menu , DYNAMIC_MENU pDynMenu[] , int nMenuSize , const char* lpcstrTranslateSection);
	BOOL WriteXmlFile();
	CString GetScanPath();
	CString m_strListviewBgBitmap;
	CNormalizationProfile m_NormeProfile;

	BOOL EditId3v1Properties(CMp3FileList *mp3list);
	BOOL EditId3v2Properties(CMp3FileList *mp3list);

	BOOL m_bNormalizeTag;
	BOOL m_bIntelliTagValidate;
	BOOL m_bWriteId3v1Tags;
	BOOL m_bUseId3v2Tags;
	BOOL m_bDisableConnection;
	void DebugTrace( const char* strTrace );
	CString m_strDebugLogFile;
	CString m_strErrorLogFile;
	BOOL m_bDebug;
	BOOL IsInternetConnected();
	BOOL m_bDisplayPlaylists;
	BOOL DoFileIntelliTag(CMp3FileList *mp3list);
	BOOL DoFileNormalize(CMp3FileList *mp3list);
	BOOL DoFileExport(CMp3FileList *mp3list);
	BOOL DoFileDelete(CMp3FileList *mp3list);
	BOOL DoFilePlaylist(CMp3FileList *mp3list);
	BOOL DoFileRename( CMp3FileList *mp3list);
	BOOL DoFileProperties( CMp3FileList *mp3list);
	void DoFileAction( int nAction, CMp3FileList *mp3list);
	void SetAutoRename( BOOL bSet );
	void SetFormatCount( int nFormatCount );
	BOOL m_bAutoRename;
	int m_nFormatCount;
	CString m_strLastFindString;
	int m_nCx1;
	int m_nCx2;
	int m_nCy1;
	int m_nCy2;
	BOOL m_bMaximized;

	CString m_strEnginePath;
	CString m_strEngineDll;
	BOOL m_bPlayer;
	int GetDefaultFormat();
	BOOL m_bClassRegistered;
	BOOL RegisterAppWndClass();
	static int ConvHex( CString strHex );
	static COLORREF GetColor( int IDS , COLORREF colorDefault );
	COLORREF m_colorListboxSel;
	COLORREF m_colorLink;
	COLORREF m_colorCreditsText;
	COLORREF m_colorCreditsBackground;

	COLORREF m_colorViewText;
	COLORREF m_colorViewTextBk;
	COLORREF m_colorViewBackground;
	COLORREF m_colorViewSelectText;
	COLORREF m_colorViewSelectBackground;
	COLORREF m_colorTreeText;
	COLORREF m_colorTreeBackground;
	COLORREF m_colorHtmlMenuText;
	COLORREF m_colorHtmlMenuBackground;
	COLORREF m_colorHtmlMenuLabel;
	COLORREF m_colorHtmlMenuLabelBackground;
	COLORREF m_colorHtmlMenuLink;
	COLORREF m_colorSplashText;

	CString m_strPluginDll;
	CString m_strPluginPath;
	CString m_strLanguageFile;
	CString m_strLanguagePath;
	CString m_strInstallPath;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	HINSTANCE m_hInstDLL;
	BOOL m_bSmallViewer;
	BOOL m_bScanFileMp3;
	BOOL m_bScanPlaylistPls;
	BOOL m_bScanPlaylistM3u;
	CTime m_timeFirstUse;
	CTime m_timeCurrent;

	BOOL m_bDefaultExport;
	BOOL m_bChooseExportFile;
	BOOL m_bExportReinit;
	CString m_strExportFile;
	CString m_strExportDefFile;
	CString m_strExportDefName;


	int m_nPlaylistSortingCol;
	int m_nDefaultFormat;
	BOOL m_bSystray;
	BOOL m_bUseCachedCatalog;
	int m_nSecondSortingCol;
	BOOL m_bDisplayAll;
	BOOL m_bColumnsSettings;
	CString m_strFavoriteAddresses;
	int m_nSortingCol;
	int m_nDoubleClickAction;
	BOOL m_bFullRowSelection;
	void LoadSettings();
	void GetInitialPosition( int& cx, int& cy, int& x, int& y );
	int m_nViewType;
	CString m_strFileExt;
	CString m_strFilePath;
	int	m_nShowLabels;
	CString m_strAddress[MAX_ADDRESS];

	CString m_strCmdPlay;
	CString m_strCmdEnqueue;
	UINT m_nMaxRow;

	CString Translate( const char* strSection, const char* strKey );
	CString LoadTranslatedString( const char* strKey );

	CMp3ExpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3ExpApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMp3ExpApp)
	afx_msg void OnAppAbout();
	afx_msg void OnPlayer();
	afx_msg void OnUpdatePlayer(CCmdUI* pCmdUI);
	afx_msg void OnNetsearch();
	afx_msg void OnFilePublish();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int m_nInitCx;
	int m_nInitCy;
	int m_nInitX;
	int m_nInitY;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3Exp_H__6097C6C4_4E77_11D2_87FC_893779BA9A44__INCLUDED_)
