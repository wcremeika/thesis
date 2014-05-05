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
#include "LeftView.h"
#include <locale.h>
#include "AboutSheet.h"
#include "util/ListVwEx.h"
#include "PlaylistView.h"
#include "PlaylistDetailView.h"
#include "Splash.h"
#include "ID3v2Sheet.h"
#include "Format.h"
#include "Export.h"
#include "DlgRename.h"
#include "IntelliTagParser.h"
#include "IntelliTagDlgValidate.h"
#include "NormalizationProfile.h"
#include "XmlFile.h"

#include <wininet.h>
#include "AfxSock.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MULTI_FILE_CHANGE "NOPICTURE"

#define TAG_PLAYLIST			_T("playlist")
#define TAG_GENRE				_T("genre")
#define TAG_GENRE_NAME			_T("genre-name")
#define TAG_ARTIST				_T("artist")
#define TAG_ARTIST_NAME			_T("artist-name")
#define TAG_ALBUM				_T("album")
#define TAG_ALBUM_NAME			_T("album-name")
#define TAG_TITLE				_T("title")


#define SIZE_MENU_FILE 17
#define SIZE_MENU_EDIT 8
#define SIZE_MENU_DISPLAY 5
#define SIZE_MENU_VIEW 14
#define SIZE_MENU_HELP 1


static DYNAMIC_MENU _gMenuFile[] =
{ 
	{ MF_STRING , ID_FILE_BROWSE_ADDRESS , "Address" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_PLAY , "Play" },
	{ MF_STRING , ID_FILE_ENQUEUE , "Enqueue" },
	{ MF_STRING , ID_FILE_ADD_TO_PLAYLIST , "Playlist" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_EDIT_TAG , "Properties" },
	{ MF_STRING , ID_FILE_DELETE , "Delete" },
	{ MF_STRING , ID_FILE_RENAME , "Rename" },
	{ MF_STRING , ID_FILE_EXPORT , "Export" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_PLAYLIST_SAVE , "Save" },
	{ MF_STRING , ID_PLAYLIST_SAVEAS , "SaveAs" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_VIEW_OPTIONS , "Options" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_APP_EXIT , "Exit" }

};

static DYNAMIC_MENU _gMenuEdit[] =
{ 
	{ MF_STRING , ID_EDIT_COPY , "Copy" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_EDIT_SELECT_ALL , "SelectAll" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FINDFIRST , "Find" },
	{ MF_STRING , ID_FINDNEXT , "FindNext" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_SEARCH , "AdvancedSearch" }
};


static DYNAMIC_MENU _gMenuDisplay[] =
{ 
	{ MF_STRING , ID_DISPLAY_ARTIST , "Artist" },
	{ MF_STRING , ID_DISPLAY_GENRE , "Genre" },
	{ MF_STRING , ID_DISPLAY_ALBUM , "Album" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_DISPLAY_ALL , "AllInfos" },
};

static DYNAMIC_MENU _gMenuView[] =
{ 
	{ MF_STRING , ID_VIEW_TOOLBAR , "Toolbar" },
	{ MF_STRING , ID_VIEW_ADRESSBAR , "Addressbar" },
	{ MF_STRING , ID_VIEW_STATUS_BAR , "Statusbar" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_VIEW_SMALL_VIEWER , "LAB" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_VIEW_PLAYLIST , "Playlist" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_VIEW_LARGEICON , "LargeIcons" },
	{ MF_STRING , ID_VIEW_SMALLICON , "SmallIcons" },
	{ MF_STRING , ID_VIEW_LIST , "List" },
	{ MF_STRING , ID_VIEW_DETAILS , "Details" },
	{ MF_SEPARATOR , 0 , NULL },
	{ MF_STRING , ID_FILE_UPDATE_SNAPSHOT , "Refresh" }

};

static DYNAMIC_MENU _gMenuHelp[] =
{ 
	{ MF_STRING , ID_APP_ABOUT , "About" }
};

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpApp

BEGIN_MESSAGE_MAP(CMp3ExpApp, CWinApp)
	//{{AFX_MSG_MAP(CMp3ExpApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOLS_PLAYER, OnPlayer)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PLAYER, OnUpdatePlayer)
	ON_COMMAND(ID_TOOLS_NETSEARCH, OnNetsearch)
	ON_COMMAND(ID_FILE_PUBLISH, OnFilePublish)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpApp construction

CMp3ExpApp::CMp3ExpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hInstDLL = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMp3ExpApp object

CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMp3ExpApp initialization

BOOL CMp3ExpApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	// CG end

	// Register the AppClassname;
	if( !RegisterAppWndClass())
		return FALSE;

	m_strPluginDll = GetProfileString( SECT_SETUP , ENTR_RESDLL , "" );
	m_strInstallPath = GetProfileString( SECT_SETUP, ENTR_PATH, "" );
	m_strPluginPath = m_strInstallPath + SKIN_DIR;
	m_strLanguagePath = m_strInstallPath + LANGUAGE_DIR;
	m_bPlayer = GetProfileInt( SECT_SETUP , ENTR_PLAYER, FALSE );
	m_strLanguageFile = GetProfileString( SECT_SETUP, ENTR_LANGUAGE, "en.lng" );


	m_strEngineDll = GetProfileString( SECT_SEARCH , ENTR_ENGINE_DLL , "" );
	m_strEnginePath = m_strInstallPath + "\\Engines\\";
	m_strDebugLogFile = m_strInstallPath + "\\Debug.txt";
	m_strErrorLogFile = m_strInstallPath + "\\error.log";

	if( (m_strPluginDll != "") )
	{
		if( (m_hInstDLL = ::LoadLibrary( m_strPluginPath + m_strPluginDll )) != NULL)
		{
			CString strVersion;
			::LoadString( m_hInstDLL, IDS_PLUGIN_VERSION , strVersion.GetBuffer( MAX_BUFFER ), MAX_BUFFER );
			strVersion.ReleaseBuffer();
			if( strVersion == RESDLL_VERSION )
				::AfxSetResourceHandle( m_hInstDLL );
		}
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDS_OLE_INIT_FAILED);
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox("Socket Failed");
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	LoadSettings();



	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMp3ExpDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CMainFrame* pFrame = (CMainFrame*) m_pMainWnd;
	CreateMainMenu();
	pFrame->SetMenu( &m_menuMain );

	// The one and only window has been initialized, so show and update it.
	if( pFrame->IsIconic() && m_bSystray )
	{
		pFrame->ShowWindow(SW_HIDE);
		pFrame->m_bHidden = TRUE;
	}
	else if( m_bMaximized )
	{
		pFrame->ShowWindow(SW_MAXIMIZE);
	}

	pFrame->ShowExplorerWindows();

	return TRUE;
}

void CMp3ExpApp::LoadSettings()
{
	// Initialisation des variables static de la classe CAudioFile
	CAudioFile::strUndefined = theApp.LoadTranslatedString( "IdsUndefined" );
	CAudioFile::strNotAvailable = theApp.LoadTranslatedString( "IdsNotAvailable" );
	CAudioFile::strProtected = theApp.LoadTranslatedString( "IdsProtected" );
	CAudioFile::strSizeFormat = theApp.LoadTranslatedString( "IdsFormatSize" );
	CAudioFile::strBitrateFormat = theApp.LoadTranslatedString( "IdsFormatBitrate" );


//	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	m_strFilePath = GetProfileString( SECT_MP3 , ENTR_FILEPATH , ENTR_FILEPATH_DEF );
	m_nShowLabels = GetProfileInt(SECT_USR, ENTR_SHOWLABELS, TRUE );
	m_nViewType = GetProfileInt(SECT_USR, ENTR_VIEWTYPE, VIEW_ARTIST );
	m_bDisplayAll = GetProfileInt(SECT_USR, ENTR_ALL_COLS, FALSE );

	static _TCHAR szEntry[MAX_BUFFER];
	for( int i = 0 ; i < MAX_ADDRESS ; i++ )
	{
		wsprintf( szEntry, ENTR_ADDRESS, i );
		m_strAddress[i] = GetProfileString( SECT_USR, szEntry, ENTR_ADDRESS_DEF );
	}
	// Position de la fenêtre
	m_nInitCx = GetProfileInt(SECT_USR, ENTR_CX, ENTR_CX_DEF );
	m_nInitCy = GetProfileInt(SECT_USR, ENTR_CY, ENTR_CY_DEF );
	m_nInitX = GetProfileInt(SECT_USR, ENTR_X, ENTR_X_DEF );
	m_nInitY = GetProfileInt(SECT_USR, ENTR_Y, ENTR_Y_DEF );
	m_bMaximized = GetProfileInt(SECT_USR, ENTR_MAXIMIZED, FALSE );

	// Taille des vues
	m_nCx1 = GetProfileInt(SECT_USR, ENTR_CX1, ENTR_CX1_DEF );
	m_nCx2 = GetProfileInt(SECT_USR, ENTR_CX2, ENTR_CX2_DEF );
	m_nCy1 = GetProfileInt(SECT_USR, ENTR_CY1, ENTR_CY1_DEF );
	m_nCy2 = GetProfileInt(SECT_USR, ENTR_CY2, ENTR_CY2_DEF );

	// Options
	m_bFullRowSelection = GetProfileInt(SECT_USR, ENTR_FULLROWSEL, TRUE );
	m_nDoubleClickAction = GetProfileInt(SECT_USR, ENTR_DBLECLICK, PLAY );
	m_nSortingCol = GetProfileInt(SECT_USR, ENTR_SORTINGCOL, COL_NAME );
	m_nSecondSortingCol = GetProfileInt(SECT_USR, ENTR_SECONDARYSORTINGCOL, COL_NAME );
	m_bColumnsSettings = GetProfileInt(SECT_USR, ENTR_COL_SETTINGS, FALSE );
	m_nPlaylistSortingCol = GetProfileInt(SECT_USR, ENTR_PLAYLISTSORTINGCOL, FILENAME );

	m_bDisplayPlaylists = GetProfileInt(SECT_USR, ENTR_DISPLAY_PLAYLISTS, FALSE );
	m_strFavoriteAddresses = GetProfileString( SECT_USR, ENTR_FAVORITE_ADRESSES,  "" );
	if( m_strFavoriteAddresses == "" )
		m_strFavoriteAddresses = m_strFilePath;

	m_bIntelliTagValidate = GetProfileInt(SECT_TAG, ENTR_INTELLITAG_VALIDATE, TRUE );
	m_bNormalizeTag = GetProfileInt(SECT_TAG, ENTR_NORMALIZE_TAG, TRUE );

	m_strCmdPlay = GetProfileString( SECT_ADVANCED, ENTR_CMD_PLAY,  ENTR_CMD_PLAY_DEF );
	m_strCmdEnqueue = GetProfileString( SECT_ADVANCED, ENTR_CMD_ENQUEUE,  ENTR_CMD_ENQUEUE_DEF );
	m_nMaxRow = GetProfileInt(SECT_ADVANCED, ENTR_MAX_ROW, ENTR_MAX_ROW_DEF );
	m_bUseCachedCatalog = GetProfileInt(SECT_ADVANCED, ENTR_CACHED_CATALOG, TRUE );

	m_bDisableConnection	= GetProfileInt( SECT_ADVANCED, ENTR_DISABLE_CONNECTION, FALSE );
	m_bWriteId3v1Tags = GetProfileInt( SECT_ADVANCED, ENTR_WRITE_ID3V1_TAGS, TRUE );
	CAudioFile::bWriteId3v1Tag = theApp.m_bWriteId3v1Tags;
	m_bUseId3v2Tags = GetProfileInt( SECT_ADVANCED, ENTR_USE_ID3V2_TAGS, TRUE );
	CAudioFile::bReadId3v2Tag = m_bUseId3v2Tags;
	CAudioFile::bWriteId3v2Tag = m_bUseId3v2Tags;
	m_bSystray = TRUE;


	m_nFormatCount = GetProfileInt( SECT_USR , ENTR_FORMAT_COUNT, 0 );
	m_bAutoRename = GetProfileInt(SECT_USR, ENTR_AUTO_RENAME, FALSE );

	m_strExportDefName = GetProfileString( SECT_EXPORT, ENTR_EXP_DEFINITION_NAME,  "" );
	m_strExportDefFile = GetProfileString( SECT_EXPORT, ENTR_EXP_DEFINITION_FILE,  "" );
	m_strExportFile = GetProfileString( SECT_EXPORT, ENTR_EXP_FILENAME,  "" );
	m_bChooseExportFile = GetProfileInt(SECT_EXPORT, ENTR_EXP_CHOOSEFILE, TRUE );
	m_bDefaultExport = GetProfileInt(SECT_EXPORT, ENTR_EXP_DEFAULT_FORMAT, TRUE );
	m_bExportReinit = GetProfileInt(SECT_EXPORT, ENTR_EXP_REINIT, TRUE );

	m_bScanPlaylistM3u = GetProfileInt(SECT_PLAYLIST, ENTR_PL_SCAN_M3U, TRUE );
	m_bScanPlaylistPls = GetProfileInt(SECT_PLAYLIST, ENTR_PL_SCAN_PLS, TRUE );
	m_bScanPlaylistM3u = m_bScanPlaylistM3u;
	m_bScanPlaylistPls = m_bScanPlaylistPls;

	m_bScanFileMp3 = GetProfileInt(SECT_FILE, ENTR_FILE_SCAN_MP3, TRUE );

	m_bSmallViewer = GetProfileInt(SECT_USR, ENTR_SHOWVIEWER, FALSE );

	m_colorListboxSel = GetColor( IDS_COLOR_LISTBOX_SEL , COLOR_LISTBOX_SEL );
	m_colorLink = GetColor( IDS_COLOR_LINK , COLOR_LINK );
	m_colorCreditsText = GetColor( IDS_COLOR_CREDITSTEXT , COLOR_WHITE );
	m_colorCreditsBackground = GetColor( IDS_COLOR_CREDITSBACKGROUND , COLOR_BLACK );

	m_colorViewText = GetColor( IDS_COLOR_VIEWTEXT , COLOR_BLACK );
	m_colorViewTextBk = GetColor( IDS_COLOR_VIEWTEXTBG , COLOR_WHITE );
	m_colorViewBackground = GetColor( IDS_COLOR_VIEWBACKGROUND , COLOR_WHITE );
	m_colorViewSelectText= GetColor( IDS_COLOR_VIEWSELECTTEXT , COLOR_WHITE );
	m_colorViewSelectBackground = GetColor( IDS_COLOR_VIEWSELECTBACKGROUND , COLOR_LISTBOX_SEL );
	m_colorTreeText = GetColor( IDS_COLOR_TREETEXT , COLOR_BLACK );
	m_colorTreeBackground = GetColor( IDS_COLOR_TREEBACKGROUND , COLOR_WHITE );
	m_colorHtmlMenuText =  GetColor( IDS_COLOR_HTMLMENUTEXT , COLOR_BLACK );
	m_colorHtmlMenuBackground =  GetColor( IDS_COLOR_HTMLMENUBACKGROUND , COLOR_WHITE );
	m_colorHtmlMenuLabel =  GetColor( IDS_COLOR_HTMLMENULABEL , COLOR_LINK );
	m_colorHtmlMenuLabelBackground =  GetColor( IDS_COLOR_HTMLMENULABELBACKGROUND , COLOR_WHITE );
	m_colorHtmlMenuLink =  GetColor( IDS_COLOR_HTMLMENULINK , COLOR_LINK );
	m_colorSplashText = GetColor( IDS_COLOR_SPLASHTEXT , COLOR_BLACK );

	m_strLastFindString = GetProfileString(SECT_USR, ENTR_LASTFIND, "");


	// Debug
	m_bDebug = GetProfileInt(SECT_SETUP, ENTR_DEBUG, FALSE );
	CAudioFile::bDebug = TRUE;
	CAudioFile::strDebugLogFile = m_strErrorLogFile;

	if( ! m_strListviewBgBitmap.LoadString( IDS_LISTVIEW_BG_BITMAP ))
	{
		m_strListviewBgBitmap = RES_NONE;
	}

	m_NormeProfile.Load();

}

void CMp3ExpApp::GetInitialPosition( int& cx, int& cy, int& x, int& y )
{
	cx = m_nInitCx;
	cy = m_nInitCy;
	x = m_nInitX;
	y = m_nInitY;
}


void CMp3ExpApp::OnAppAbout()
{
	CAboutSheet aboutSheet;

	aboutSheet.DoModal();

}






int CMp3ExpApp::ExitInstance() 
{
   if( m_bClassRegistered)
       ::UnregisterClass( MP3EXPLORER_CLASSNAME ,AfxGetInstanceHandle());

   if( m_hInstDLL != NULL )
		::FreeLibrary( m_hInstDLL );
	
	return CWinApp::ExitInstance();
}

BOOL CMp3ExpApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

COLORREF CMp3ExpApp::GetColor(int nIDS, COLORREF colorDefault)
{
	CString strColor;
	strColor.LoadString( nIDS );
	if( strColor.GetLength() != 6 )
		return colorDefault;
	CString strRed = strColor.Left( 2 );
	CString strGreen = strColor.Mid( 2 , 2 );
	CString strBlue = strColor.Right( 2 );
	int nRed = ConvHex( strRed );
	int nGreen = ConvHex( strGreen );
	int nBlue = ConvHex( strBlue );
	return RGB( nRed, nGreen, nBlue );
}


int CMp3ExpApp::ConvHex(CString strHex)
{
	int n = 0;
	strHex.MakeUpper();
	int nLen = strHex.GetLength();
	for( int i = 0 ; i < nLen ; i ++ )
	{
		TCHAR c = strHex.GetAt( nLen - i - 1 );
		int nBase = (int) ::pow( 16 , i );

		if( c == '1' ) n += 1 * nBase;  
		else if ( c == '2' ) n += 2 * nBase;
		else if ( c == '3' ) n += 3 * nBase;
		else if ( c == '4' ) n += 4 * nBase;
		else if ( c == '5' ) n += 5 * nBase;
		else if ( c == '6' ) n += 6 * nBase;
		else if ( c == '7' ) n += 7 * nBase;
		else if ( c == '8' ) n += 8 * nBase;
		else if ( c == '9' ) n += 9 * nBase;
		else if ( c == 'A' ) n += 10 * nBase;
		else if ( c == 'B' ) n += 11 * nBase;
		else if ( c == 'C' ) n += 12 * nBase;
		else if ( c == 'D' ) n += 13 * nBase;
		else if ( c == 'E' ) n += 14 * nBase;
		else if ( c == 'F' ) n += 15 * nBase;
	}
	return n;
}

BOOL CMp3ExpApp::RegisterAppWndClass()
{
	WNDCLASS wndcls;

     // Start with NULL defaults.
     memset(&wndcls, 0, sizeof(WNDCLASS));

     wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
     wndcls.lpfnWndProc = ::DefWindowProc;
     wndcls.hInstance = AfxGetInstanceHandle();
     wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different
                                                  // icon
     wndcls.hCursor = LoadCursor( IDC_ARROW );
     wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
     wndcls.lpszMenuName = NULL;

     // Specify our own class name for using FindWindow later
     wndcls.lpszClassName = MP3EXPLORER_CLASSNAME;

     // Register new class and return result from ::RegisterClass().
	 m_bClassRegistered =  ::AfxRegisterClass(&wndcls);

     return m_bClassRegistered;

}

void CMp3ExpApp::SetFormatCount(int nFormatCount)
{
	m_nFormatCount = nFormatCount;
	WriteProfileInt( SECT_USR , ENTR_FORMAT_COUNT, nFormatCount );

}

void CMp3ExpApp::SetAutoRename(BOOL bSet)
{
	m_bAutoRename = bSet;
	WriteProfileInt( SECT_USR , ENTR_AUTO_RENAME, m_bAutoRename );

}

int CMp3ExpApp::GetDefaultFormat()
{
	// Return the default format index [1..n] or 0 if not found

	if( m_nFormatCount != 0 )
	{
		for( int i = 0 ; i < m_nFormatCount ; i++ )
		{
			CFormat format;
			format.Load( i + 1 );
			if( i == m_nDefaultFormat )
			{
				return( i + 1 );
			}
		}
	}
	return 0;

}


void CMp3ExpApp::OnPlayer() 
{
     CWnd *PlayerWnd, *ChildCWnd;
     if (PlayerWnd = CWnd::FindWindow( MP3PLAYER_CLASSNAME ,NULL))
     {
       // if so, does it have any popups?
       ChildCWnd=PlayerWnd->GetLastActivePopup();

       // Bring the main window to the top.
       PlayerWnd->BringWindowToTop();

       // If iconic, restore the main window.
       if (PlayerWnd->IsIconic())
          PlayerWnd->ShowWindow(SW_RESTORE);

       // If there was an active popup, bring it along too
       if (PlayerWnd != ChildCWnd)
          ChildCWnd->BringWindowToTop();
	 }
	 else
	 {
		 CString strPlayer = m_strInstallPath + "\\" + "Mp3 player.exe";
		 ::ShellExecute( m_pMainWnd->m_hWnd, NULL, strPlayer, NULL, m_strInstallPath, SW_SHOW );

	 }
	
	
}

void CMp3ExpApp::OnUpdatePlayer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	
}

void CMp3ExpApp::OnNetsearch() 
{

	if( m_strEngineDll != "" )
	{
		HINSTANCE hLib = ::LoadLibrary( m_strEnginePath + m_strEngineDll );
		if( hLib )
		{
			void (WINAPI* lpfnDlgNetSearch)();
			lpfnDlgNetSearch = (void (WINAPI*)()) ::GetProcAddress (hLib, "DlgNetSearch");
			if( lpfnDlgNetSearch )
				(*lpfnDlgNetSearch) ();
			FreeLibrary (hLib);
		}
	} 
	else
	{
		CString strMessage = theApp.LoadTranslatedString( "IdsChooseEngine");
		if( DoMessageBox( strMessage, MB_YESNO, 0 ) == IDYES )
		{
			m_pMainWnd->PostMessage( WM_COMMAND, ID_VIEW_OPTIONS, 0 );
		}

	}
	
}

void CMp3ExpApp::DoFileAction( int nAction, CMp3FileList *mp3list)
{
	CString strShellCmd;
	CString strMsgShellError;	
	CString strMsgShellErrorM3U;	


	switch( nAction )
	{
	case PLAY:
		strShellCmd = m_strCmdPlay;
		strMsgShellError = theApp.LoadTranslatedString( "IdsErrorShellNoMP3Association" );
		strMsgShellErrorM3U = theApp.LoadTranslatedString( "IdsErrorShellNoM3UAssociation" );
		break;
	case ENQUEUE:
		strShellCmd = m_strCmdEnqueue;
		strMsgShellError = theApp.LoadTranslatedString( "IdsErrorShellNoEnqueue" );
		strMsgShellErrorM3U = theApp.LoadTranslatedString( "IdsErrorShellNoEnqueue" );
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
		if( (!CMp3ExpDoc::IsValidFile(strFullPathName)) && (strFullPathName.Find("http://") != 0))
		{
			CString strMsg;
			strMsg.Format( theApp.LoadTranslatedString( "IdsErrorFileNotFound" ) , strFullPathName );
			if( DoMessageBox( strMsg , MB_YESNO | MB_ICONQUESTION, 0 ) == IDYES )
			{
//				CMp3ExpDoc* pDoc = (CMp3ExpDoc *) GetActiveDocument();
//				pDoc->RebuildSnapshot();
			}
			return;
				
		}
		int nExecute = (int)::ShellExecute( ::AfxGetMainWnd()->m_hWnd , strShellCmd , strFullPathName , NULL , "", SW_SHOWNORMAL );
		if( nExecute <= 32 )
		{
			DoMessageBox( strMsgShellError, MB_OK | MB_ICONEXCLAMATION, 0 );
			return;
		}
	}
	// Selection multiple. utilise l'association M3U pour une plus grande compatibilité
	else
	{
		int nExecute;
		nExecute = (int)::ShellExecute( ::AfxGetMainWnd()->m_hWnd , strShellCmd , strTmpPlaylist , NULL , "", SW_SHOWNORMAL );
		if( nExecute <= 32 )
		{
			DoMessageBox( strMsgShellErrorM3U, MB_OK | MB_ICONEXCLAMATION, 0 );
			return;
		}
	}

}

BOOL CMp3ExpApp::DoFileProperties(CMp3FileList *mp3list)
{
	BOOL bUpdateList = FALSE;
	if( m_bUseId3v2Tags )
	{
		bUpdateList = EditId3v2Properties(mp3list);
	}
	else
	{
		bUpdateList = EditId3v1Properties(mp3list);
	}
	return bUpdateList;
}

BOOL CMp3ExpApp::EditId3v1Properties(CMp3FileList *mp3list)
{
	CID3v1Dlg dlg;
	int nFlag = WT_MULTI_EDIT;
	BOOL bSingleMP3 = FALSE;
	BOOL bUpdateList = FALSE;

	BeginWaitCursor();

	// S'il n'y a qu'un seul fichier de sélectionné, alors
	// initialiser la boîte de dialogue avec les informations de ce fichier
	if( mp3list->GetSize() == 1 )
	{
		CAudioFile* pMp3 = mp3list->GetAt( 0 );

		// Affiche par défaut dans le titre le nom du fichier moins l'extension ".mp3" si 
		// le titre  n'est pas renseigné.
		CString strTitle = pMp3->GetTitle();
		CString strArtist = pMp3->GetArtist();

		if( strTitle == "" )
		{
			CString strDefTitle = pMp3->GetName();
			// Suppression de l'extension
			strDefTitle = strDefTitle.Left( strDefTitle.GetLength() - 4 );
			int nPos = strDefTitle.Find("-");
			CString strUndefined = theApp.LoadTranslatedString( "IdsUndefined" );
			if( nPos != -1 ) 
			{
				strTitle = strDefTitle.Right( strDefTitle.GetLength() - nPos - 1 );
				strTitle.TrimLeft();
				if((strArtist == strUndefined ) || (strArtist == "" ))
				{
					strArtist = strDefTitle.Left( nPos - 1 );
					strArtist.TrimRight();
				}
			}
			else
			{
				strTitle = strDefTitle;
			}
		}
		dlg.m_strTitle = strTitle;
		dlg.m_strArtist = strArtist;
		dlg.m_strAlbum = pMp3->GetAlbum();
		dlg.m_strYear = pMp3->GetYear();
		dlg.m_strComment = pMp3->GetComment();
		dlg.m_strGenre = pMp3->GetGenre();
		dlg.m_strTrackNum = pMp3->GetTrack();

		nFlag = WT_SINGLE_EDIT;
	}

	// Enregistre les valeurs initiales de l'artiste et du genre 
	CString strArtist = dlg.m_strArtist;
	CString strGenre = dlg.m_strGenre;
	CString strAlbum = dlg.m_strAlbum;

	// Liste d'artistes
	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();
	dlg.m_pMapArtist = &(pDoc->m_mapArtist);
	dlg.m_pMapGenre = &(pDoc->m_mapGenre);
	dlg.m_pListArtist = &(pDoc->m_slistArtist );
	dlg.m_pListGenre = &(pDoc->m_slistGenre );

	// Il faut un format par défaut pour le renommage automatique
	int nFormat = GetDefaultFormat();
	dlg.m_bRenameAvailable = nFormat;

	EndWaitCursor();

	if( dlg.DoModal() == IDOK )
	{
		BeginWaitCursor();
		BOOL bRename = FALSE;
		CFormat format;
		SetAutoRename( dlg.m_bRename && nFormat);

		// Set rename flag if the option is checked and format available
		if( dlg.m_bRename && nFormat )
		{
			format.Load( nFormat );
			bRename = TRUE;
		}
		for( int i = 0 ; i < mp3list->GetSize() ; i++ )
		{
			CAudioFile* pMp3 = (CAudioFile*) mp3list->GetAt( i );
			CTag tag;
			tag.SetTitle( dlg.m_strTitle.Left(30) );
			tag.SetArtist( dlg.m_strArtist.Left(30) );
			tag.SetAlbum( dlg.m_strAlbum.Left(30) );
			tag.SetComment( dlg.m_strComment.Left(30) );
			tag.SetYear( dlg.m_strYear.Left(4) );
			tag.SetGenre( dlg.m_strGenre );
			tag.SetTrackNum( dlg.m_strTrackNum );

			if( !pMp3->WriteFileInfos( &tag ))
			{
				CString strError = theApp.LoadTranslatedString( "IdsErrorWritingTag" );
				DoMessageBox( strError, MB_OK | MB_ICONWARNING , 0 );
			}
			else
			{
				// Si l'opération s'est bien passée, renommer éventuellement les fichiers
				if( bRename )
				{
					pMp3->Rename( format.GetFilename( pMp3 ));
				}
			}

		}

		// Gestion des mises à jour

		// Ne réafficher les listes que si l'artiste ou le genre ont été modifiés
		if( (strArtist != dlg.m_strArtist.Left(30))
			 || (strGenre != dlg.m_strGenre)
			 || (strAlbum != dlg.m_strAlbum))
		{
			pDoc->UpdateLists();
		}
		// Sinon ne réafficher que la ListView
		else
		{
			bUpdateList = TRUE;
		}
		// remise à jour du fichier cache
		pDoc->CreateSnapshot();
		EndWaitCursor();
	}
	return bUpdateList;
}


BOOL CMp3ExpApp::EditId3v2Properties(CMp3FileList *mp3list)
{
	CID3v2Sheet dlg;
	int nFlag = WT_MULTI_EDIT;
	BOOL bSingleMP3 = FALSE;
	CString strLyrics;
	CString strTmpPictureFile;
	BOOL bUpdateList = FALSE;

	BeginWaitCursor();

	// S'il n'y a qu'un seul fichier de sélectionné, alors
	// initialiser la boîte de dialogue avec les informations de ce fichier
	if( mp3list->GetSize() == 1 )
	{
		CAudioFile* pMp3 = mp3list->GetAt( 0 );

		// Affiche par défaut dans le titre le nom du fichier moins l'extension ".mp3" si 
		// le titre  n'est pas renseigné.
		CString strTitle = pMp3->GetTitle();
		CString strArtist = pMp3->GetArtist();

		if( strTitle == "" )
		{
			CString strDefTitle = pMp3->GetName();
			// Suppression de l'extension
			strDefTitle = strDefTitle.Left( strDefTitle.GetLength() - 4 );
			int nPos = strDefTitle.Find("-");
			CString strUndefined = theApp.LoadTranslatedString( "IdsUndefined" );
			if( nPos != -1 ) 
			{
				strTitle = strDefTitle.Right( strDefTitle.GetLength() - nPos - 1 );
				strTitle.TrimLeft();
				if((strArtist == strUndefined ) || (strArtist == "" ))
				{
					strArtist = strDefTitle.Left( nPos - 1 );
					strArtist.TrimRight();
				}
			}
			else
			{
				strTitle = strDefTitle;
			}
		}
		dlg.m_Page1.m_strTitle = strTitle;
		dlg.m_Page1.m_strArtist = strArtist;
		dlg.m_Page1.m_strAlbum = pMp3->GetAlbum();
		dlg.m_Page1.m_strYear = pMp3->GetYear();
		dlg.m_Page1.m_strComment = pMp3->GetComment();
		dlg.m_Page1.m_strGenre = pMp3->GetGenre();
		dlg.m_Page1.m_strTrackNum = pMp3->GetTrack();
		dlg.m_Page1.m_nRating = pMp3->GetRatingNum();
		dlg.m_Page1.m_strArtistUrl = pMp3->GetArtistUrl();


		// If only one MP3 then handle Lyrics and picture
		if( pMp3->GetType() == FILE_TYPE_MP3 )
		{
			// Add lyrics
			dlg.AddPage( &dlg.m_Page2 );
			strLyrics = pMp3->GetLyrics();
			dlg.m_Page2.m_strLyrics = strLyrics;

			// Add Picture
			pMp3->GetPicture( strTmpPictureFile );
			if( strTmpPictureFile != "" )
				dlg.m_Page3.m_strPictureFile = strTmpPictureFile;
		}
		else
		{
			dlg.RemovePage( &dlg.m_Page3 );
		}
		nFlag = WT_SINGLE_EDIT;
	}
	else
	{
		dlg.m_Page1.m_nRating = -1;
		dlg.m_Page3.m_strPictureFile = MULTI_FILE_CHANGE;
	}

	// Cache le bouton Apply de la fenêtre Properties
	dlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	// Enregistre les valeurs initiales de l'artiste et du genre 
	CString strArtist = dlg.m_Page1.m_strArtist;
	CString strGenre = dlg.m_Page1.m_strGenre;
	CString strAlbum = dlg.m_Page1.m_strAlbum;
	int nRating = dlg.m_Page1.m_nRating;

	// Liste d'artistes
	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();
	dlg.m_Page1.m_pMapArtist = &(pDoc->m_mapArtist);
	dlg.m_Page1.m_pMapGenre = &(pDoc->m_mapGenre);
	dlg.m_Page1.m_pListArtist = &(pDoc->m_slistArtist );
	dlg.m_Page1.m_pListGenre = &(pDoc->m_slistGenre );

	// Il faut un format par défaut pour le renommage automatique
	int nFormat = GetDefaultFormat();
	dlg.m_Page1.m_bRenameAvailable = nFormat;

	EndWaitCursor();

	if( dlg.DoModal() == IDOK )
	{
		BeginWaitCursor();
		BOOL bRename = FALSE;
		CFormat format;
		SetAutoRename( dlg.m_Page1.m_bRename && nFormat);

		// Set rename flag if the option is checked and format available
		if( dlg.m_Page1.m_bRename && nFormat )
		{
			format.Load( nFormat );
			bRename = TRUE;
		}
		for( int i = 0 ; i < mp3list->GetSize() ; i++ )
		{
			CAudioFile* pMp3 = (CAudioFile*) mp3list->GetAt( i );
			CTag tag;
			tag.SetTitle( dlg.m_Page1.m_strTitle );
			tag.SetArtist( dlg.m_Page1.m_strArtist );
			tag.SetAlbum( dlg.m_Page1.m_strAlbum );
			tag.SetComment( dlg.m_Page1.m_strComment );
			tag.SetYear( dlg.m_Page1.m_strYear );
			tag.SetGenre( dlg.m_Page1.m_strGenre );
			tag.SetTrackNum( dlg.m_Page1.m_strTrackNum );
			if( dlg.m_Page1.m_nRating != nRating )
			{
				tag.SetRating( dlg.m_Page1.m_nRating );
			}
			else
			{
				tag.SetRating( pMp3->GetRatingNum() );
			}

			tag.SetArtistUrl( dlg.m_Page1.m_strArtistUrl );
			if( pMp3->GetType() == FILE_TYPE_MP3 )
			{
				// If lyrics modified
				if( dlg.m_Page2.m_strLyrics != strLyrics )
				{
					tag.SetLyrics( dlg.m_Page2.m_strLyrics );
					tag.HasLyrics( ( dlg.m_Page2.m_strLyrics != "" ) ? TRUE : FALSE);
				}
				else
				{
					// Keep the original value
					tag.HasLyrics( pMp3->m_bHasLyrics );
				}
				

				// If picture
				if( dlg.m_Page3.m_strPictureFile != "" )
				{
					if( dlg.m_Page3.m_strPictureFile != MULTI_FILE_CHANGE )
					{
						tag.SetPicture( dlg.m_Page3.m_strPictureFile );
						tag.HasPicture( TRUE );
					}
					else
					{
						// Keep the original value
						tag.HasPicture( pMp3->m_bHasPicture );
					}
				}
				// Delete tempory file if used
				if( strTmpPictureFile != "" )
				{
					::DeleteFile( strTmpPictureFile );
				}
			}


			if( !pMp3->WriteFileInfos( &tag ))
			{
				CString strError = theApp.LoadTranslatedString( "IdsErrorWritingTag" );
				DoMessageBox( strError, MB_OK | MB_ICONWARNING , 0 );
			}
			else
			{
				// Si l'opération s'est bien passée, renommer éventuellement les fichiers
				if( bRename )
				{
					pMp3->Rename( format.GetFilename( pMp3 ));
				}
			}

		}

		// Gestion des mises à jour

		// Ne réafficher les listes que si l'artiste ou le genre ont été modifiés
		if( (strArtist != dlg.m_Page1.m_strArtist.Left(30))
			 || (strGenre != dlg.m_Page1.m_strGenre)
			 || (strAlbum != dlg.m_Page1.m_strAlbum))
		{
			pDoc->UpdateLists();
		}
		// Sinon ne réafficher que la ListView
		else
		{
			bUpdateList = TRUE;
		}
		// remise à jour du fichier cache
		pDoc->CreateSnapshot();
		EndWaitCursor();
	}
	return bUpdateList;
}

BOOL CMp3ExpApp::DoFileRename(CMp3FileList *mp3list)
{
	BOOL bUpdateList = FALSE;

	if( mp3list->GetSize() < 1 )
		return bUpdateList;
	
	CAudioFile *pMp3First = mp3list->GetAt( 0 );

	CDlgRename dlgRename;
	dlgRename.m_pMp3 = pMp3First;
	if( mp3list->GetSize() == 1 )
	{
		dlgRename.m_bSingleFile = TRUE;
		dlgRename.m_strNewFileName = pMp3First->GetName();
	}
	else
	{
		dlgRename.m_bSingleFile = FALSE;
	}

	if( dlgRename.DoModal() == IDOK )
	{
		if( dlgRename.m_bValidFormat )
		{
			for( int i = 0 ; i < mp3list->GetSize() ; i++ )
			{
				CAudioFile *pMp3 = mp3list->GetAt( i );
				pMp3->Rename( dlgRename.m_format.GetFilename( pMp3 ));
			}
		}
		else
		{
			pMp3First->Rename( dlgRename.m_strNewFileName );
		}

		CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();
		bUpdateList = TRUE;
		pDoc->CreateSnapshot();
	}
	return bUpdateList;

}

BOOL CMp3ExpApp::DoFilePlaylist(CMp3FileList *mp3list)
{
	CMainFrame *pMainFrame = (CMainFrame *) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();
	
	for( int i = 0 ; i < mp3list->GetSize() ; i++ )
	{
		CAudioFile *pMp3 = mp3list->GetAt( i );
		CAudioFile *pNewMp3 = pMp3->GetCopy();
		pDoc->m_pPlaylistSelected->m_mp3List.Add( pNewMp3 );
	}
	pDoc->UpdateAllViews( NULL, UPDATE_DETAIL_VIEW ) ;
	return TRUE;
}

BOOL CMp3ExpApp::DoFileDelete(CMp3FileList *mp3list)
{
	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();

	if( mp3list->GetSize() == 1 )
	{
		CAudioFile *pMp3First = mp3list->GetAt(0);
		CString strMsg;
		strMsg.Format( theApp.LoadTranslatedString( "IdsQuestionFileDelete" ) , pMp3First->GetLocation() + "\\" + pMp3First->GetName());
		if( DoMessageBox( strMsg , MB_YESNO | MB_ICONQUESTION, 0 ) == IDYES )
		{
			pMp3First->Delete();
			pDoc->m_mp3List.DeleteFile( pMp3First );
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		CString strMsg = theApp.LoadTranslatedString( "IdsQuestionFileDeleteAll" );
		if( DoMessageBox( strMsg , MB_YESNO | MB_ICONQUESTION, 0 ) == IDYES )
		{
			
			for( int i = 0 ; i < mp3list->GetSize() ; i++ )
			{
				CAudioFile *pMp3 = mp3list->GetAt( i );
				pMp3->Delete();
				pDoc->m_mp3List.DeleteFile( pMp3 );

			}
		}
		else
		{
			return FALSE;
		}
	}
	pDoc->UpdateLists();
	return FALSE;

}

BOOL CMp3ExpApp::DoFileExport(CMp3FileList *mp3list)
{
	CString strExportFilename;

	if( m_bChooseExportFile )
	{
		CString strFilter = theApp.LoadTranslatedString( "IdsExportFilter" );
		CFileDialog dlgFile( FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter );
	
		dlgFile.m_ofn.lpstrInitialDir = ".\\";
		if( dlgFile.DoModal() != IDOK )
		{
			return FALSE;
		}
		switch( dlgFile.m_ofn.nFilterIndex )
		{
		case 1:
			strExportFilename = dlgFile.GetPathName() + ".txt";
			break;
		case 2:
			strExportFilename = dlgFile.GetPathName() + ".htm";
			break;
		default:
			strExportFilename = dlgFile.GetPathName();
			break;
		}
	}
	else
	{
		strExportFilename = m_strExportFile;
	}

	CExport export;
	if( !m_bDefaultExport )
	{
		export.Load( m_strExportDefFile );
	}
	if( !export.CreateExportFile( strExportFilename, mp3list, m_bExportReinit ))
	{
		return FALSE;
	}
	CString strMsg = theApp.LoadTranslatedString( "IdsExportSuccessful" );
	DoMessageBox( strMsg, MB_OK | MB_ICONINFORMATION , 0 );
	return TRUE;	

}

BOOL CMp3ExpApp::IsInternetConnected()
{
	BOOL bResult = TRUE;
	DWORD dwFlags = 1;
 
/*	if (InternetGetConnectedState( &dwFlags , 0 ) == FALSE)
	{
		// Don't attempt connection or it will bring up the dialog
		bResult = FALSE;
	}
	else
*/	{
		if( dwFlags & INTERNET_CONNECTION_PROXY )
		{
			return TRUE;
		}

		AfxSocketInit();
		CSocket socket;
		if ( !socket.Create() )
		{                                                       
		    TRACE( "Unable to create the socket." );
			bResult = FALSE;
		}   
		// You can use www.microsoft.com in order to check whether DNS is available
		// or numeric IP otherwise 
		else if ( !socket.Connect( "www.microsoft.com", 80 ) ) 
		{     //  207.46.130.150
			TRACE( "Unable to connect to server" );        
			socket.Close();
			bResult = FALSE;
		}
	}
	return bResult;
		
}

void CMp3ExpApp::DebugTrace(const char *strTrace)
{
	if( m_bDebug )
	{
		CStdioFile file;
		if( file.Open( m_strDebugLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText ))
		{
			file.SeekToEnd();
			file.WriteString( strTrace );
			file.WriteString( "\n" );
			file.Flush();
			file.Close();
		}
	}

}

BOOL CMp3ExpApp::DoFileIntelliTag(CMp3FileList *mp3list)
{
	BOOL bUpdateList = FALSE;
	BOOL bUpdateTree = FALSE;

	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();

	BOOL bReplaceUnderscores = m_NormeProfile.GetUnderscoresReplacement();
	int nCount = mp3list->GetSize();
	BOOL bYesToAll = FALSE;

	for( int i = 0 ; i < nCount ; i++ )
	{
		BOOL bCanceled = FALSE;
		CAudioFile* pAudioFile = (CAudioFile*) mp3list->GetAt( i );
		CString strFilename = pAudioFile->GetName();

		// Old values
		CString strArtist = pAudioFile->GetArtist();
		CString strAlbum = pAudioFile->GetAlbum();

		CIntelliTagParser tag( strFilename );

		CTag tagNew;

		tagNew.HasLyrics( pAudioFile->m_bHasLyrics );
		tagNew.HasPicture( pAudioFile->m_bHasPicture );

		if( m_bIntelliTagValidate && !bYesToAll )
		{
			CIntelliTagDlgValidate dlgValidate;
			dlgValidate.m_bMultipleFiles = ( nCount > 1 );
			dlgValidate.m_strFilename = strFilename;
			dlgValidate.m_strArtist = CAudioFile::Normalize( tag.GetArtist(), m_NormeProfile.GetNorm( ARTIST ), bReplaceUnderscores );
			dlgValidate.m_strTitle = CAudioFile::Normalize( tag.GetTitle(), m_NormeProfile.GetNorm( TITLE ), bReplaceUnderscores );
			dlgValidate.m_strAlbum = CAudioFile::Normalize( tag.GetAlbum(), m_NormeProfile.GetNorm( ALBUM ), bReplaceUnderscores );
			dlgValidate.m_strTrack = tag.GetTrack();
			dlgValidate.m_pMapArtist = &(pDoc->m_mapArtist);
			dlgValidate.m_pListArtist = &(pDoc->m_slistArtist);
			int nRet = dlgValidate.DoModal();
			if( nRet == IDOK )
			{
				bYesToAll = dlgValidate.m_bYesToAll;
				tagNew.SetArtist( dlgValidate.m_strArtist );
				tagNew.SetTitle( dlgValidate.m_strTitle );
				tagNew.SetAlbum( dlgValidate.m_strAlbum );
				tagNew.SetTrackNum( dlgValidate.m_strTrack );
			}
			else
			{
				bCanceled = TRUE;
				if( dlgValidate.m_bCancelAll )
				{
					break;
				}
				else
				{
					continue;
				}
			}
		}
		else
		{
			tagNew.SetArtist( CAudioFile::Normalize( tag.GetArtist(), m_NormeProfile.GetNorm( ARTIST ), bReplaceUnderscores));
			tagNew.SetTitle( CAudioFile::Normalize( tag.GetTitle(), m_NormeProfile.GetNorm( TITLE ), bReplaceUnderscores ));
			tagNew.SetAlbum( CAudioFile::Normalize( tag.GetAlbum(), m_NormeProfile.GetNorm( ALBUM ), bReplaceUnderscores ));
			tagNew.SetTrackNum( tag.GetTrack() );
		}

		if( !bCanceled )
		{
			pAudioFile->WriteFileInfos( &tagNew );

			// Ne réafficher les listes que si l'artiste ou l'album ont été modifiés
			if( (strArtist != tagNew.GetArtist())
				 || (strAlbum != tagNew.GetAlbum()))
			{
				bUpdateTree = TRUE;
			}
			bUpdateList = TRUE;
		}

	}
	if( bUpdateList )
	{
		BeginWaitCursor();
		if( bUpdateTree )
		{
			pDoc->UpdateLists();
		}
		// remise à jour du fichier cache
		pDoc->CreateSnapshot();
		EndWaitCursor();
	}

	return bUpdateList;
}

BOOL CMp3ExpApp::DoFileNormalize(CMp3FileList *mp3list)
{
	BOOL bUpdateList = FALSE;
	BOOL bUpdateTree = FALSE;

	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();

	BOOL bReplace = m_NormeProfile.GetUnderscoresReplacement();
	for( int i = 0 ; i < mp3list->GetSize() ; i++ )
	{
		CAudioFile* pAudioFile = (CAudioFile*) mp3list->GetAt( i );


		CString strArtist = CAudioFile::Normalize( pAudioFile->GetArtist(), m_NormeProfile.GetNorm( ARTIST ), bReplace );
		CString strTitle = CAudioFile::Normalize( pAudioFile->GetTitle(), m_NormeProfile.GetNorm( TITLE ), bReplace );
		CString strAlbum = CAudioFile::Normalize( pAudioFile->GetAlbum(), m_NormeProfile.GetNorm( ALBUM ), bReplace );
		CString strComment = CAudioFile::Normalize( pAudioFile->GetAlbum(), m_NormeProfile.GetNorm( ALBUM ), bReplace );

		// If any change then rewrite the tag
		if( ( strArtist  != pAudioFile->GetArtist())  ||
			( strAlbum   != pAudioFile->GetAlbum())   ||
			( strComment != pAudioFile->GetComment()) ||
			( strTitle   != pAudioFile->GetTitle()) )
		{
			CTag tagNew;

			tagNew.SetArtist( strArtist );
			tagNew.SetTitle( strTitle );
			tagNew.SetAlbum( strAlbum );
			tagNew.SetComment( strComment );
			tagNew.HasLyrics( pAudioFile->m_bHasLyrics );
			tagNew.HasPicture( pAudioFile->m_bHasPicture );

			CString strOldArtist = pAudioFile->GetArtist();
			CString strOldAlbum = pAudioFile->GetAlbum();

			pAudioFile->WriteFileInfos( &tagNew );

			// if artist or album has changed the redraw the tree
			if( (strArtist != strOldArtist) || (strAlbum != strOldAlbum))
			{
				bUpdateTree = TRUE;
			}
			bUpdateList = TRUE;
		}

	}
	if( bUpdateList )
	{
		BeginWaitCursor();
		if( bUpdateTree )
		{
			pDoc->UpdateLists();
		}
		// remise à jour du fichier cache
		pDoc->CreateSnapshot();
		EndWaitCursor();
	}

	return bUpdateList;
}

CString CMp3ExpApp::GetScanPath()
{
	return m_strFilePath;
}

void CMp3ExpApp::OnFilePublish() 
{
	WriteXmlFile();
}

BOOL CMp3ExpApp::WriteXmlFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) m_pMainWnd;
	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) pMainFrame->GetActiveDocument();

	CXmlFile file;
	CString strXmlFile = m_strInstallPath + "\\playlist.xml";
	file.Open( strXmlFile , CFile::modeCreate | CFile::modeWrite );
	file.WriteHeader();
	file.WriteTagBegin( TAG_PLAYLIST , 0 , CXmlFile::CR );


	POSITION posGenre = pDoc->m_mapGenre.GetStartPosition();
	while( posGenre != NULL )
	{
		CGenre* pGenre;
		CString strGenre;
		pDoc->m_mapGenre.GetNextAssoc( posGenre, strGenre, (CObject*&)pGenre );
		ASSERT( pGenre->IsKindOf( RUNTIME_CLASS( CGenre ) ) );
		file.WriteTagBegin( TAG_GENRE , 1 , CXmlFile::CR );
		file.WriteTagBegin( TAG_GENRE_NAME , 2 , CXmlFile::NOCR );
		file.WriteData( pGenre->GetName() , 0 , CXmlFile::NOCR );
		file.WriteTagEnd( TAG_GENRE_NAME , 0 , CXmlFile::CR );

		POSITION posArtist = pGenre->m_mapArtist.GetStartPosition();
		while( posArtist != NULL )
		{
			CArtist* pArtist;
			CString strArtist;
			pGenre->m_mapArtist.GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
			ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
			file.WriteTagBegin( TAG_ARTIST , 2 , CXmlFile::CR );
			file.WriteTagBegin( TAG_ARTIST_NAME , 3 , CXmlFile::NOCR );
			file.WriteData( pArtist->GetName() , 0 , CXmlFile::NOCR );
			file.WriteTagEnd( TAG_ARTIST_NAME , 0 , CXmlFile::CR );
			
			POSITION pos = pArtist->m_mapAlbum.GetStartPosition( );
			while( pos != NULL )
			{
				CAlbum* pAlbum;
				CString strAlbum;
				pArtist->m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
				ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
				file.WriteTagBegin( TAG_ALBUM , 3 , CXmlFile::CR );
				file.WriteTagBegin( TAG_ALBUM_NAME , 4 , CXmlFile::NOCR );
				file.WriteData( pAlbum->GetName() , 0 , CXmlFile::NOCR );
				file.WriteTagEnd( TAG_ALBUM_NAME , 0 , CXmlFile::CR );

				for( int i = 0 ; i < pAlbum->GetFileList()->GetSize() ; i++ )
				{
					CAudioFile* pAudioFile = pAlbum->GetFileList()->GetAt( i );
					file.WriteTagBegin( TAG_TITLE , 4 , CXmlFile::NOCR );
					file.WriteData( pAudioFile->GetTitle() , 0 , CXmlFile::NOCR );
					file.WriteTagEnd( TAG_TITLE , 0 , CXmlFile::CR );
				}
				file.WriteTagEnd( TAG_ALBUM , 3 , CXmlFile::CR );
			}
			file.WriteTagEnd( TAG_ARTIST , 2 , CXmlFile::CR );
		}
		file.WriteTagEnd( TAG_GENRE , 1 , CXmlFile::CR );
	}
	file.WriteTagEnd( TAG_PLAYLIST , 0 , CXmlFile::CR );

	return TRUE;



}

	
CString CMp3ExpApp::Translate( const char* strSection, const char* strKey ) 
{
	static _TCHAR buffer[MAX_BUFFER];
	CString strFile = m_strLanguagePath + m_strLanguageFile; 
	::GetPrivateProfileString( strSection , strKey, "", buffer, sizeof( buffer ), strFile ); 
	CString strTranslated = buffer;
	return strTranslated;
}

CString CMp3ExpApp::LoadTranslatedString( const char* strKey ) 
{
	static _TCHAR buffer[MAX_BUFFER];
	CString strFile = m_strLanguagePath + m_strLanguageFile; 
	::GetPrivateProfileString( "StringTable" , strKey, "", buffer, sizeof( buffer ), strFile ); 
	CString strTranslated = buffer;
#ifdef _DEBUG
	if( strTranslated == "" )
	{
		CString strTrace = "The key ";
		strTrace += strKey;
		strTrace += " not found in the language file !\n";
		TRACE( strTrace );
	}

#endif
	
	return strTranslated;

}

void CMp3ExpApp::CreateMainMenu()
{
	m_menuMain.CreateMenu();
	m_menuFile.CreatePopupMenu();
	m_menuEdit.CreatePopupMenu();
	m_menuDisplay.CreatePopupMenu();
	m_menuView.CreatePopupMenu();
	m_menuHelp.CreatePopupMenu();


	CreateDynamicMenu( &m_menuFile, _gMenuFile , SIZE_MENU_FILE, "MainMenu" );
	CreateDynamicMenu( &m_menuEdit, _gMenuEdit , SIZE_MENU_EDIT, "MainMenu" );
	CreateDynamicMenu( &m_menuDisplay, _gMenuDisplay , SIZE_MENU_DISPLAY, "MainMenu" );
	CreateDynamicMenu( &m_menuView, _gMenuView , SIZE_MENU_VIEW, "MainMenu" );
	CreateDynamicMenu( &m_menuHelp, _gMenuHelp , SIZE_MENU_HELP, "MainMenu" );

	m_menuMain.AppendMenu( MF_POPUP , (UINT) m_menuFile.m_hMenu, Translate( "MainMenu" , "File" ));
	m_menuMain.AppendMenu( MF_POPUP , (UINT) m_menuEdit.m_hMenu , Translate( "MainMenu" , "Edit" ));
	m_menuMain.AppendMenu( MF_POPUP , (UINT) m_menuDisplay.m_hMenu , Translate( "MainMenu" , "Display" ));
	m_menuMain.AppendMenu( MF_POPUP , (UINT) m_menuView.m_hMenu , Translate( "MainMenu" , "View" ));
	m_menuMain.AppendMenu( MF_POPUP , (UINT) m_menuHelp.m_hMenu , Translate( "MainMenu" , "Help" ));
	
}

void CMp3ExpApp::CreateDynamicMenu( CMenu* menu , DYNAMIC_MENU pDynMenu[] , int nMenuSize , const char* lpcstrTranslateSection)
{
	for( int i = 0 ; i < nMenuSize ; i++ )
	{
		if( pDynMenu[i].nItemID != 0 )
		{
			CString strItemText = Translate( lpcstrTranslateSection , pDynMenu[i].pText );
			menu->AppendMenu( pDynMenu[i].nItemFlag, pDynMenu[i].nItemID , strItemText );
		}
		else
		{
			menu->AppendMenu( MF_SEPARATOR, 0 , (LPCSTR) NULL );
		}
		
	}

}