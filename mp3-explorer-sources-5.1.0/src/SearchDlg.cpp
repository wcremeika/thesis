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
#include "mp3/Mp3File.h"
#include "util/ListVwEx.h"
#include "Mp3View.h"

#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENU_SIZE 14
#define MAX_STRING 255

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


extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog


CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchDlg)
	m_bHasLyrics = FALSE;
	m_bHasPicture = FALSE;
	m_nRatingMin = -1;
	m_nRatingMax = -1;
	m_nBitrateMin = -1;
	m_nBitrateMax = -1;
	m_strAlbum = _T("");
	m_strArtist = _T("");
	m_strTitle = _T("");
	m_strYearMax = _T("");
	m_strYearMin = _T("");
	m_strGenre = _T("");
	//}}AFX_DATA_INIT

}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
	DDX_Control(pDX, IDC_LIST_RESULT, m_list);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Check(pDX, IDC_CHECK_LYRICS, m_bHasLyrics);
	DDX_Check(pDX, IDC_CHECK_PICTURE, m_bHasPicture);
	DDX_CBIndex(pDX, IDC_CMB_RATING_MIN, m_nRatingMin);
	DDX_CBIndex(pDX, IDC_CMB_RATING_MAX, m_nRatingMax);
	DDX_CBIndex(pDX, IDC_CMB_BITRATE_MIN, m_nBitrateMin);
	DDX_CBIndex(pDX, IDC_CMB_BITRATE_MAX, m_nBitrateMax);
	DDX_Text(pDX, IDC_EDIT_ALBUM, m_strAlbum);
	DDX_Text(pDX, IDC_EDIT_ARTIST, m_strArtist);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_YEAR_MAX, m_strYearMax);
	DDX_Text(pDX, IDC_EDIT_YEAR_MIN, m_strYearMin);
	DDX_CBString(pDX, IDC_CMB_GENRE, m_strGenre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
	ON_BN_CLICKED(IDC_BT_SEARCH, OnSearch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULT, OnRclickListResult)
	ON_COMMAND(ID_FILE_PLAY, OnFilePlay)
	ON_COMMAND(ID_FILE_ENQUEUE, OnFileEnqueue)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_ADD_TO_PLAYLIST, OnFileAddToPlaylist)
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)
	ON_COMMAND(ID_FILE_EDIT_TAG, OnFileEditTag)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg message handlers

BOOL CSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	CString strTab1 = theApp.LoadTranslatedString( "IdsSearchTab1" );
	TabCtrlItem.pszText = (LPSTR)(LPCSTR) strTab1;
	m_tab.InsertItem( 0, &TabCtrlItem );
	CString strTab2 = theApp.LoadTranslatedString( "IdsSearchTab2" );
	TabCtrlItem.pszText = (LPSTR)(LPCSTR) strTab2;
	m_tab.InsertItem( 1, &TabCtrlItem );

	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "SearchDlg", "Item1" ));

	SetDlgItemText( IDC_LB_TITLE , theApp.Translate( "SearchDlg", "Item1" ));
	SetDlgItemText( IDC_LB_ARTIST , theApp.Translate( "SearchDlg", "Item2" ));
	SetDlgItemText( IDC_LB_ALBUM , theApp.Translate( "SearchDlg", "Item3" ));
	SetDlgItemText( IDC_LB_GENRE , theApp.Translate( "SearchDlg", "Item4" ));
	SetDlgItemText( IDC_LB_YEAR , theApp.Translate( "SearchDlg", "Item5" ));
	SetDlgItemText( IDC_LB_YEAR_MIN , theApp.Translate( "SearchDlg", "Item6" ));;
	SetDlgItemText( IDC_LB_YEAR_MAX , theApp.Translate( "SearchDlg", "Item7" ));

	SetDlgItemText( IDC_GROUP_BITRATE , theApp.Translate( "SearchDlg", "Item8" ));
	SetDlgItemText( IDC_LB_BITRATE_MIN , theApp.Translate( "SearchDlg", "Item9" ));
	SetDlgItemText( IDC_LB_BITRATE_MAX , theApp.Translate( "SearchDlg", "Item10" ));
	SetDlgItemText( IDC_GROUP_RATING , theApp.Translate( "SearchDlg", "Item11" ));
	SetDlgItemText( IDC_LB_RATING_MIN , theApp.Translate( "SearchDlg", "Item12" ));
	SetDlgItemText( IDC_LB_RATING_MAX , theApp.Translate( "SearchDlg", "Item13" ));
	SetDlgItemText( IDC_GROUP_EXTEND , theApp.Translate( "SearchDlg", "Item14" ));
	SetDlgItemText( IDC_CHECK_LYRICS , theApp.Translate( "SearchDlg", "Item15" ));
	SetDlgItemText( IDC_CHECK_PICTURE , theApp.Translate( "SearchDlg", "Item16" ));

	SetDlgItemText( IDC_BT_SEARCH , theApp.Translate( "SearchDlg", "Item17" ));
	SetDlgItemText( IDCANCEL , theApp.Translate( "SearchDlg", "Item18" ));


	CComboBox* pCBox;
	// Ratings
	pCBox = (CComboBox*) GetDlgItem(IDC_CMB_RATING_MIN);
	for(  int i = 0 ; i < NUM_RATING ; i++ )
	{
		int nIndex = pCBox->AddString( _gszRating[i] );
		pCBox->SetItemData ( nIndex , i );
	}
	pCBox->SetCurSel(0);

	pCBox = (CComboBox*) GetDlgItem(IDC_CMB_RATING_MAX);
	for(  i = 0 ; i < NUM_RATING ; i++ )
	{
		int nIndex = pCBox->AddString( _gszRating[i] );
		pCBox->SetItemData ( nIndex , i );
	}
	pCBox->SetCurSel(0);

	// Bitrates
	pCBox = (CComboBox*) GetDlgItem(IDC_CMB_BITRATE_MIN);
	for(  i = 0 ; i < NUM_BITRATE ; i++ )
	{
		int nIndex = pCBox->AddString( _gszBitrate[i] );
		pCBox->SetItemData ( nIndex , i );
	}
	pCBox->SetCurSel(0);

	pCBox = (CComboBox*) GetDlgItem(IDC_CMB_BITRATE_MAX);
	for(  i = 0 ; i < NUM_BITRATE ; i++ )
	{
		int nIndex = pCBox->AddString( _gszBitrate[i] );
		pCBox->SetItemData ( nIndex , i );
	}
	pCBox->SetCurSel(0);

	// Genres
	pCBox = (CComboBox*) GetDlgItem(IDC_CMB_GENRE);
	pCBox->InitStorage( 200 , 2000 ); 

	POSITION pos = m_pMapGenre->GetStartPosition();
	while( pos != NULL )
	{
		CGenre* pGenre;
		CString strGenre;
		m_pMapGenre->GetNextAssoc( pos, strGenre, (CObject*&)pGenre );
		ASSERT( pGenre->IsKindOf( RUNTIME_CLASS( CGenre ) ) );
		pCBox->AddString( pGenre->GetName() );
	}

	UpdateTab();


		// Insert les colonnes
	InitColumns();

	m_SmallImageList.Create( 16, 16, IDB_SMALL_ICONS );
	m_list.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	LV_COLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;

	
	for( i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn *pdisplayedCol = (CColumn *) m_columns.GetAt( i );

		lvc.iSubItem = i;
		lvc.pszText = pdisplayedCol->m_pszColTitle;
		lvc.cx = pdisplayedCol->m_nWidth;
		lvc.fmt = pdisplayedCol->m_nAlignemnt;
		m_list.InsertColumn(i,&lvc);
	}
	UpdateList();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CSearchDlg::UpdateTab()
{
	int nTab = m_tab.GetCurSel();

	int nShowTab0 = ( nTab == 0 ) ? SW_SHOW : SW_HIDE;
	int nShowTab1 = ( nTab == 1 ) ? SW_SHOW : SW_HIDE;

	(GetDlgItem(IDC_LB_TITLE))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_EDIT_TITLE))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_ARTIST))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_EDIT_ARTIST))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_ALBUM))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_EDIT_ALBUM))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_YEAR))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_YEAR_MIN))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_EDIT_YEAR_MIN))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_YEAR_MAX))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_EDIT_YEAR_MAX))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_CMB_GENRE))->ShowWindow( nShowTab0 );
	(GetDlgItem(IDC_LB_GENRE))->ShowWindow( nShowTab0 );

	(GetDlgItem(IDC_GROUP_RATING))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CMB_RATING_MIN))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CMB_RATING_MAX))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_LB_RATING_MIN))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_LB_RATING_MAX))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_GROUP_BITRATE))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CMB_BITRATE_MIN))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CMB_BITRATE_MAX))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_LB_BITRATE_MIN))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_LB_BITRATE_MAX))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_GROUP_EXTEND))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CHECK_LYRICS))->ShowWindow( nShowTab1 );
	(GetDlgItem(IDC_CHECK_PICTURE))->ShowWindow( nShowTab1 );

}

void CSearchDlg::OnSearch() 
{
	UpdateData();
	ResetResult();
	for( int i = 0 ; i < m_pList->GetSize() ; i++ )
	{
		CAudioFile* pAudioFile = m_pList->GetAt( i );
		if( Match( pAudioFile ))
		{
			CAudioFile* pNewFile;
			pNewFile = pAudioFile->GetCopy();
			m_listResult.Add( pNewFile );
		}
	}
	UpdateList();
}

BOOL CSearchDlg::Match(CAudioFile *pAudioFile)
{
	if( m_strTitle != "" )
		if( !FindString( pAudioFile->GetTitle(), m_strTitle ))
			return FALSE;

	if( m_strArtist != "" )
		if( !FindString( pAudioFile->GetArtist(), m_strArtist ))
			return FALSE;

	if( m_strAlbum != "" )
		if( !FindString( pAudioFile->GetAlbum(), m_strAlbum ))
			return FALSE;

	if( m_strYearMin != "" )
		if( pAudioFile->GetYear() < m_strYearMin )
			return FALSE;

	if( m_strYearMax != "" )
		if( pAudioFile->GetYear() > m_strYearMax )
			return FALSE;

	if( m_strGenre != "" )
		if( pAudioFile->GetGenre() != m_strGenre )
			return FALSE;

	if( m_nBitrateMin != 0 )
		if( pAudioFile->GetBitrateValue() < _gnBitrate[m_nBitrateMin] )
			return FALSE;

	if( m_nBitrateMax != 0 )
		if( pAudioFile->GetBitrateValue() > _gnBitrate[m_nBitrateMax] )
			return FALSE;

	if( m_nRatingMin != 0 )
		if( pAudioFile->GetRatingNum() < _gnRating[m_nRatingMin] )
			return FALSE;

	if( m_nRatingMax != 0 )
		if( pAudioFile->GetRatingNum() > _gnRating[m_nRatingMax] )
			return FALSE;

	if( m_bHasPicture )
		if( !pAudioFile->m_bHasPicture )
			return FALSE;

	if( m_bHasLyrics )
		if( !pAudioFile->m_bHasLyrics )
			return FALSE;


	return TRUE;
}

BOOL CSearchDlg::FindString(CString strSource, CString strFind)
{
	strSource.MakeUpper();
	strFind.MakeUpper();
	if( strSource.Find( strFind ) != -1 )
		return TRUE;
	return FALSE;

}

void CSearchDlg::InitColumns()
{
	for( int i = 0 ; i < NUM_INFO ; i++ )
	{
		CColumn *pCol = new CColumn();
		lstrcpy( _gcolumns[i].szColTitle , theApp.LoadTranslatedString( _gcolumns[i].pstrTitleResName ));
		int nIdInfo = _gcolumns[i].nInfoIndex;
		pCol->m_pszColTitle = _gcolumns[i].szColTitle;
		pCol->m_nInfoType = _gcolumns[i].nInfoType;
		pCol->m_nInfoIndex = nIdInfo;
		
		
		pCol->m_nWidth = _gcolumns[i].nWidth;
		{
			pCol->m_nAlignemnt = _gcolumns[i].nAlignemnt;
			pCol->m_nDisplayed = _gcolumns[i].nDisplayed;
			pCol->m_nInfoOrder = _gcolumns[i].nInfoOrder;
		}
		m_columns.Add( pCol );
	}

}

void CSearchDlg::UpdateList()
{
	m_list.DeleteAllItems();
	// Fill the ListView
	
	LV_ITEM lvi;

	for ( int i = 0 ; i < m_listResult.GetSize() ; i++ )
	{
		//Parameters
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iImage = ICON_TITLE;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);

		CAudioFile *pMp3 = (CAudioFile *) m_listResult.GetAt(i); 

		static _TCHAR buffer[MAX_STRING];

		lvi.iSubItem = 0;
		lvi.pszText = buffer;
		m_list.InsertItem( &lvi );
		m_list.SetItemData( i , (DWORD) pMp3 );

		for( int j = 0 ; j < m_columns.GetSize() ; j++)
		{
			CColumn *pCol = (CColumn *) m_columns.GetAt( j );
			m_list.SetItemText( i , j , pMp3->GetInformation(pCol->m_nInfoIndex));
		}
	}

}

void CSearchDlg::ResetResult()
{
	for ( int i = 0 ; i < m_listResult.GetSize() ; i++ )
	{
		CAudioFile *pMp3 = (CAudioFile *) m_listResult.GetAt(i); 
		delete pMp3;
	}
	m_listResult.RemoveAll();
}


CSearchDlg::~CSearchDlg()
{
	ResetResult();

	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		delete m_columns.GetAt(i);
	}
	m_columns.RemoveAll();
}

void CSearchDlg::OnRclickListResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POINT point;

	GetCursorPos( &point );

	CMenu menu;
	menu.CreatePopupMenu();
	theApp.CreateDynamicMenu( &menu , _gMenuFile , MENU_SIZE , "FileMenu" );

	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL ); 
	
	*pResult = 0;
}

int CSearchDlg::GetSelectedFiles(CMp3FileList &mp3list)
{
	int nCount = 0;
	int nIndex = -1;

	while( (nIndex = m_list.GetNextItem( nIndex , LVNI_SELECTED )) != -1 )
	{
		CAudioFile *pMp3 = (CAudioFile*) m_list.GetItemData( nIndex );
		mp3list.Add( pMp3 );
		nCount++;		
	}
	return nCount;

}


void CSearchDlg::OnFilePlay() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileAction( PLAY , &mp3list );
}


void CSearchDlg::OnFileEnqueue() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileAction( ENQUEUE , &mp3list );
	
}


void CSearchDlg::OnFileAddToPlaylist() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFilePlaylist( &mp3list );
	
}


void CSearchDlg::OnFileExport() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileExport( &mp3list );
	
}


void CSearchDlg::OnFileRename() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileRename( &mp3list );
	if( bUpdateList )
		UpdateList();
	
}


void CSearchDlg::OnFileEditTag() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	BOOL bUpdateList = theApp.DoFileProperties( &mp3list );
	if( bUpdateList )
		UpdateList();
	
}

void CSearchDlg::OnFileDelete() 
{
	CMp3FileList mp3list;
	GetSelectedFiles( mp3list );
	theApp.DoFileDelete( &mp3list );
	
}

void CSearchDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateTab();
	
	*pResult = 0;
}
