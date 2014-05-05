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
#include "resource.h"
#include "Mp3Exp.h"
#include "mp3/Mp3File.h"
#include "ID3v2Page.h"
#include "util/DibUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;

IMPLEMENT_DYNCREATE(CID3v2Page1, CPropertyPage)
IMPLEMENT_DYNCREATE(CID3v2Page2, CPropertyPage)
IMPLEMENT_DYNCREATE(CID3v2Page3, CPropertyPage)


#define NB_TRACKNUMS 30
TCHAR g_tracknums[NB_TRACKNUMS][3] = { 
	_T("01"),_T("02"),_T("03"),_T("04"),_T("05"),_T("06"),_T("07"),_T("08"),_T("09"),_T("10"), 
	_T("11"),_T("12"),_T("13"),_T("14"),_T("15"),_T("16"),_T("17"),_T("18"),_T("19"),_T("20"), 
	_T("21"),_T("22"),_T("23"),_T("24"),_T("25"),_T("26"),_T("27"),_T("28"),_T("29"),_T("30"), 
};


#define ICON_ID3V2	0
#define ICON_ID3V1	1

/////////////////////////////////////////////////////////////////////////////
// CID3v1Dlg Dialog

CID3v1Dlg::CID3v1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CID3v1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CID3v1Dlg)
	m_strAlbum = _T("");
	m_strComment = _T("");
	m_strTitle = _T("");
	m_strYear = _T("");
	m_strArtist = _T("");
	m_bRename = FALSE;
	m_strTrackNum = _T("");
	m_strGenre = _T("");
	//}}AFX_DATA_INIT
}

CID3v1Dlg::~CID3v1Dlg()
{
}

void CID3v1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3v1Dlg)
	DDX_CBString(pDX, IDC_LIST_ALBUM, m_strAlbum);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 30);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDV_MaxChars(pDX, m_strTitle, 30);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_strYear);
	DDV_MaxChars(pDX, m_strYear, 4);
	DDX_CBString(pDX, IDC_LIST_ARTIST, m_strArtist);
	DDX_Check(pDX, IDC_CHECK_RENAME, m_bRename);
	DDX_CBString(pDX, IDC_LIST_TRACKNUM, m_strTrackNum);
	DDX_CBString(pDX, IDC_CMB_GENRE, m_strGenre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3v1Dlg, CDialog)
	//{{AFX_MSG_MAP(CID3v1Dlg)
	ON_CBN_DROPDOWN(IDC_LIST_ALBUM, OnDropdownListAlbum)
	ON_BN_CLICKED(IDC_BT_NORMALIZE, OnNormalize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CID3v1Dlg::OnDropdownListAlbum() 
{
	CString strSelectedArtist = m_comboArtist.GetText();

	POSITION posArtist = m_pMapArtist->GetStartPosition( );
	while( posArtist != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_pMapArtist->GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
	
		if( strArtist == strSelectedArtist )
		{
			CString strText = m_comboAlbum.GetText();
			CSortedList listAlbum;

			m_comboAlbum.FillWithList( listAlbum.GetList( &pArtist->m_mapAlbum ), ICON_ALBUM, ICON_ALBUM );
			m_comboAlbum.SetText( strText );
			return;
		}
	}
}


BOOL CID3v1Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	// Translations
	SetWindowText( theApp.Translate( "ID3v1Page1", "Caption" ));
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "ID3v1Page1", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "ID3v1Page1", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "ID3v1Page1", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "ID3v1Page1", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "ID3v1Page1", "Item5" ));
	SetDlgItemText( IDC_ITEM6 , theApp.Translate( "ID3v1Page1", "Item6" ));
	SetDlgItemText( IDC_ITEM7 , theApp.Translate( "ID3v1Page1", "Item7" ));
	SetDlgItemText( IDC_BT_NORMALIZE , theApp.Translate( "ID3v1Page1", "Item8" ));
	SetDlgItemText( IDC_CHECK_RENAME , theApp.Translate( "ID3v1Page1", "Item8" ));


	theApp.BeginWaitCursor();

	// Artists
	m_comboArtist.SubclassDlgItem( IDC_LIST_ARTIST , this );
	m_imagelistComboArtist.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelistComboArtist.SetOverlayImage( 1, 1 );
	m_comboArtist.SetImageList( &m_imagelistComboArtist );

	m_comboArtist.FillWithList( m_pListArtist->GetList( m_pMapArtist ), ICON_ARTIST , ICON_ARTIST );
	m_comboArtist.SelectItem( m_strArtist );

	// Album
	m_comboAlbum.SubclassDlgItem( IDC_LIST_ALBUM , this );
	m_imagelistComboAlbum.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelistComboAlbum.SetOverlayImage( 1, 1 );
	m_comboAlbum.SetImageList( &m_imagelistComboAlbum );
	OnDropdownListAlbum();
	m_comboAlbum.SelectItem( m_strAlbum );
	
	// Tracknums
	CComboBox* pCBox = (CComboBox*) GetDlgItem(IDC_LIST_TRACKNUM);
	for( int i = 0 ; i < NB_TRACKNUMS ; i++ )
	{
		pCBox->AddString( g_tracknums[i] );
	}


	// Genres

	m_comboGenre.SubclassDlgItem( IDC_CMB_GENRE , this );
	m_imagelistComboGenre.Create( 16, 16, IDB_ID3V1 );
	m_imagelistComboGenre.SetOverlayImage( 1, 1 );

	m_comboGenre.SetImageList( &m_imagelistComboGenre );
	m_comboGenre.InitStorage( 200 , 2000 ); 

	
	m_comboGenre.FillWithList( m_pListGenre->GetList( _gszGenre , NUM_GENRE ), ICON_ID3V1 , ICON_ID3V1 );
	m_comboGenre.SelectItem( m_strGenre );


	// Automatic renaming
	CButton* pCheck = (CButton*) GetDlgItem(IDC_CHECK_RENAME);
	if( m_bRenameAvailable )
	{
		pCheck->EnableWindow( TRUE );
		CheckDlgButton( IDC_CHECK_RENAME , theApp.m_bAutoRename );
	}

	theApp.EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CID3v1Dlg::OnNormalize() 
{
	BOOL bReplaceUnderscores = theApp.m_NormeProfile.GetUnderscoresReplacement();
	CEdit* pEditTitle = (CEdit*) GetDlgItem( IDC_EDIT_TITLE );
	CEdit* pEditComment = (CEdit*) GetDlgItem( IDC_EDIT_COMMENT );


	CString strArtist = m_comboArtist.GetText();
	CString strAlbum = m_comboAlbum.GetText();
	CString strTitle;
	CString strComment;
	pEditTitle->GetWindowText( strTitle );
	pEditComment->GetWindowText( strComment );

	strArtist = CAudioFile::Normalize( strArtist, theApp.m_NormeProfile.GetNorm( ARTIST ), bReplaceUnderscores );
	strAlbum = CAudioFile::Normalize( strAlbum, theApp.m_NormeProfile.GetNorm( ALBUM ), bReplaceUnderscores );
	strTitle = CAudioFile::Normalize( strTitle, theApp.m_NormeProfile.GetNorm( TITLE ), bReplaceUnderscores );
	strComment = CAudioFile::Normalize( strComment, theApp.m_NormeProfile.GetNorm( COMMENT ), bReplaceUnderscores );

	m_comboArtist.SetText( strArtist );
	m_comboAlbum.SetText( strAlbum );
	pEditTitle->SetWindowText( strTitle );
	pEditComment->SetWindowText( strComment );


}


/////////////////////////////////////////////////////////////////////////////
// CID3v2Page1 property page

CID3v2Page1::CID3v2Page1() : CPropertyPage(CID3v2Page1::IDD)
{
	//{{AFX_DATA_INIT(CID3v2Page1)
	m_strAlbum = _T("");
	m_strComment = _T("");
	m_strTitle = _T("");
	m_strYear = _T("");
	m_strArtist = _T("");
	m_bRename = FALSE;
	m_strArtistUrl = _T("");
	m_strTrackNum = _T("");
	m_strGenre = _T("");
	//}}AFX_DATA_INIT
	m_nRating = 0;
}

CID3v2Page1::~CID3v2Page1()
{
}

void CID3v2Page1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3v2Page1)
	DDX_CBString(pDX, IDC_LIST_ALBUM, m_strAlbum);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_strYear);
	DDV_MaxChars(pDX, m_strYear, 4);
	DDX_CBString(pDX, IDC_LIST_ARTIST, m_strArtist);
	DDX_Check(pDX, IDC_CHECK_RENAME, m_bRename);
	DDX_Text(pDX, IDC_EDIT_ARTIST_URL, m_strArtistUrl);
	DDX_CBString(pDX, IDC_LIST_TRACKNUM, m_strTrackNum);
	DDX_CBString(pDX, IDC_CMB_GENRE, m_strGenre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3v2Page1, CPropertyPage)
	//{{AFX_MSG_MAP(CID3v2Page1)
	ON_CBN_DROPDOWN(IDC_LIST_ALBUM, OnDropdownListAlbum)
	ON_CBN_SELCHANGE(IDC_LIST_RATING, OnSelchangeCmbRating)
	ON_BN_CLICKED(IDC_BT_NORMALIZE, OnNormalize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CID3v2Page1::OnSelchangeCmbRating() 
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem(IDC_LIST_RATING);
	int nIndex = pCBox->GetCurSel();
	m_nRating = pCBox->GetItemData( nIndex );
	
}

void CID3v2Page1::OnDropdownListAlbum() 
{
	CString strSelectedArtist = m_comboArtist.GetText();

	POSITION posArtist = m_pMapArtist->GetStartPosition( );
	while( posArtist != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_pMapArtist->GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
	
		if( strArtist == strSelectedArtist )
		{
			CString strText = m_comboAlbum.GetText();
			CSortedList listAlbum;
			m_comboAlbum.FillWithList( listAlbum.GetList( &pArtist->m_mapAlbum ), ICON_ALBUM, ICON_ALBUM );
			m_comboAlbum.SetText( strText );
			return;
		}
	}
	
}


BOOL CID3v2Page1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Translations
	SetWindowText( theApp.Translate( "ID3v2Page1", "Caption" ));
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "ID3v2Page1", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "ID3v2Page1", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "ID3v2Page1", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "ID3v2Page1", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "ID3v2Page1", "Item5" ));
	SetDlgItemText( IDC_ITEM6 , theApp.Translate( "ID3v2Page1", "Item6" ));
	SetDlgItemText( IDC_ITEM7 , theApp.Translate( "ID3v2Page1", "Item7" ));
	SetDlgItemText( IDC_ITEM8 , theApp.Translate( "ID3v2Page1", "Item8" ));
	SetDlgItemText( IDC_ITEM9 , theApp.Translate( "ID3v2Page1", "Item9" ));
	SetDlgItemText( IDC_BT_NORMALIZE , theApp.Translate( "ID3v2Page1", "Item10" ));
	SetDlgItemText( IDC_CHECK_RENAME , theApp.Translate( "ID3v2Page1", "Item11" ));


	theApp.BeginWaitCursor();


	// Artists
	m_comboArtist.SubclassDlgItem( IDC_LIST_ARTIST , this );
	m_imagelistComboArtist.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelistComboArtist.SetOverlayImage( 1, 1 );
	m_comboArtist.SetImageList( &m_imagelistComboArtist );


	m_comboArtist.FillWithList( m_pListArtist->GetList( m_pMapArtist) , ICON_ARTIST , ICON_ARTIST );
	m_comboArtist.SelectItem( m_strArtist );

	// Album
	m_comboAlbum.SubclassDlgItem( IDC_LIST_ALBUM , this );
	m_imagelistComboAlbum.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelistComboAlbum.SetOverlayImage( 1, 1 );
	m_comboAlbum.SetImageList( &m_imagelistComboAlbum );
	OnDropdownListAlbum();
	m_comboAlbum.SelectItem( m_strAlbum );
	
	// Tracknums
	CComboBox* pCBox = (CComboBox*) GetDlgItem(IDC_LIST_TRACKNUM);
	for( int i = 0 ; i < NB_TRACKNUMS ; i++ )
	{
		pCBox->AddString( g_tracknums[i] );
	}

	// Ratings
	pCBox = (CComboBox*) GetDlgItem(IDC_LIST_RATING);
	for(  i = 0 ; i < NUM_RATING ; i++ )
	{
		int nIndex = pCBox->AddString( _gszRating[i] );
		pCBox->SetItemData ( nIndex , i );
	}

	if( ( m_nRating < NUM_RATING ) && ( m_nRating >= 0 ))
	{
		pCBox->SelectString( -1 , _gszRating[ m_nRating ] );
	}

	// Genres

	m_comboGenre.SubclassDlgItem( IDC_CMB_GENRE , this );
	m_imagelistComboGenre.Create( 16, 16, IDB_ID3V1 );
	m_imagelistComboGenre.SetOverlayImage( 1, 1 );

	m_comboGenre.SetImageList( &m_imagelistComboGenre );
	m_comboGenre.InitStorage( 200 , 2000 ); 

	if( theApp.m_bWriteId3v1Tags )
	{
		// ID3v1 genres
		m_comboGenre.FillWithList( m_pListGenre->GetList( _gszGenre , NUM_GENRE ), ICON_ID3V1 , ICON_ID3V1 );
	}
	// Add custom genres
	POSITION pos = m_pMapGenre->GetStartPosition();
	while( pos != NULL )
	{
		CGenre* pGenre;
		CString strGenre;
		m_pMapGenre->GetNextAssoc( pos, strGenre, (CObject*&)pGenre );
		ASSERT( pGenre->IsKindOf( RUNTIME_CLASS( CGenre ) ) );
		if( !( theApp.m_bWriteId3v1Tags  && CMp3File::IsV1Genre( strGenre ))) 
		{
			m_comboGenre.AddSortedItem( strGenre , 0 , 0 );
		}
	}
	m_comboGenre.SelectItem( m_strGenre );


	// Automatic renaming
	CButton* pCheck = (CButton*) GetDlgItem(IDC_CHECK_RENAME);
	if( m_bRenameAvailable )
	{
		pCheck->EnableWindow( TRUE );
		CheckDlgButton( IDC_CHECK_RENAME , theApp.m_bAutoRename );
	}

	theApp.EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CID3v2Page1::OnNormalize() 
{
	BOOL bReplaceUnderscores = theApp.m_NormeProfile.GetUnderscoresReplacement();
	CEdit* pEditTitle = (CEdit*) GetDlgItem( IDC_EDIT_TITLE );
	CEdit* pEditComment = (CEdit*) GetDlgItem( IDC_EDIT_COMMENT );


	CString strArtist = m_comboArtist.GetText();
	CString strAlbum = m_comboAlbum.GetText();
	CString strTitle;
	CString strComment;
	pEditTitle->GetWindowText( strTitle );
	pEditComment->GetWindowText( strComment );

	strArtist = CAudioFile::Normalize( strArtist, theApp.m_NormeProfile.GetNorm( ARTIST ), bReplaceUnderscores );
	strAlbum = CAudioFile::Normalize( strAlbum, theApp.m_NormeProfile.GetNorm( ALBUM ), bReplaceUnderscores );
	strTitle = CAudioFile::Normalize( strTitle, theApp.m_NormeProfile.GetNorm( TITLE ), bReplaceUnderscores );
	strComment = CAudioFile::Normalize( strComment, theApp.m_NormeProfile.GetNorm( COMMENT ), bReplaceUnderscores );

	m_comboArtist.SetText( strArtist );
	m_comboAlbum.SetText( strAlbum );
	pEditTitle->SetWindowText( strTitle );
	pEditComment->SetWindowText( strComment );


}

/////////////////////////////////////////////////////////////////////////////
// CID3v2Page2 property page

CID3v2Page2::CID3v2Page2() : CPropertyPage(CID3v2Page2::IDD)
{
	//{{AFX_DATA_INIT(CID3v2Page2)
	m_strLyrics = _T("");
	//}}AFX_DATA_INIT
}

CID3v2Page2::~CID3v2Page2()
{
}

void CID3v2Page2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3v2Page2)
	DDX_Text(pDX, IDC_EDIT_LYRICS, m_strLyrics);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3v2Page2, CPropertyPage)
	//{{AFX_MSG_MAP(CID3v2Page2)
	ON_EN_SETFOCUS(IDC_EDIT_LYRICS, OnSetfocusEditLyrics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CID3v2Page2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Translations
	SetWindowText( theApp.Translate( "ID3v1Page2", "Caption" ));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CID3v2Page2::OnSetfocusEditLyrics() 
{
	CEdit* pBox = (CEdit*) GetDlgItem( IDC_EDIT_LYRICS );
	pBox->SetSel( 0 , 0 );
	
}



/////////////////////////////////////////////////////////////////////////////
// CID3v2Page3 property page

CID3v2Page3::CID3v2Page3() : CPropertyPage(CID3v2Page3::IDD)
{
	//{{AFX_DATA_INIT(CID3v2Page3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hBitmap = NULL;
	m_hPalette = NULL;
}

CID3v2Page3::~CID3v2Page3()
{
	if( m_hBitmap )
		::DeleteObject( m_hBitmap );

	if( m_hPalette )
		::DeleteObject( m_hPalette );
}

void CID3v2Page3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3v2Page3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3v2Page3, CPropertyPage)
	//{{AFX_MSG_MAP(CID3v2Page3)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CID3v2Page3::OnButtonSelect() 
{
	CFileDialog dlgFile( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap (*.bmp)|*.bmp" );
	
	CString strTitle = theApp.LoadTranslatedString( "IdsChoosePicture" );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = ".\\";
	if( dlgFile.DoModal() == IDOK )
	{
		LoadPicture( dlgFile.GetPathName() );
	}
	
}





void CID3v2Page3::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( nIDCtl == IDC_PICTURE )
	{
		HBITMAP       hOldBitmap;
		HPALETTE      hOldPalette;
		HDC           hDC, hMemDC;
		BITMAP        bm;

		hDC = lpDrawItemStruct->hDC;

		if( m_hBitmap )
		{
			int x = 0;
			int y = 0;
			int cx = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
			int cy = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

			GetObject( m_hBitmap, sizeof(BITMAP), &bm );
			hMemDC = CreateCompatibleDC( hDC );
			hOldBitmap = (HBITMAP) SelectObject( hMemDC, m_hBitmap );
			hOldPalette = SelectPalette( hDC, m_hPalette, FALSE );
			RealizePalette( hDC );

			if((bm.bmWidth < cx ) && ( bm.bmHeight < cy ))
			{
				x = ( cx - bm.bmWidth ) / 2;
				y = ( cy - bm.bmHeight ) / 2;
				BitBlt( hDC, x, y, bm.bmWidth, bm.bmHeight,
		             hMemDC, 0, 0, SRCCOPY );
			}
			else
			{
				StretchBlt( hDC, x, y, cx, cy,
		             hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );
			}


			SelectObject( hMemDC, hOldBitmap );
			SelectPalette( hDC, hOldPalette, FALSE );
		}
	}
   	
	CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CID3v2Page3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Translations
	SetWindowText( theApp.Translate( "ID3v2Page3", "Caption" ) );
	SetDlgItemText( IDC_BUTTON_SELECT , theApp.Translate( "ID3v2Page3", "Item1" ) );
	SetDlgItemText( IDC_BUTTON_PASTE , theApp.Translate( "ID3v2Page3", "Item2" ) );

	if( m_strPictureFile != "" )
	{
		LoadBitmapFromBMPFile( m_strPictureFile, &m_hBitmap, &m_hPalette );
	}
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CID3v2Page3::OnPaste() 
{
	if( OpenClipboard() )
	{
		HANDLE hDib = GetClipboardData( CF_DIB );
		if( hDib )
		{
			// Save the DIB in a tempory file
			CString strTempFile;
			char tempdir[MAX_PATH];
			::GetTempPath( MAX_PATH, tempdir );
			::GetTempFileName( tempdir, "bmp", 0 , strTempFile.GetBuffer(MAX_PATH) );
			strTempFile.ReleaseBuffer();

			if( SaveDIB( hDib, strTempFile))
			{
				// If saved delete the actual tempory file if exists
				if( m_strPictureFile == "" )
				{
					DeleteFile( m_strPictureFile );
				}	
				LoadPicture( strTempFile );
			} 
		}
		CloseClipboard();
	}
}


BOOL CID3v2Page3::LoadPicture(const char *strFilename)
{
	HBITMAP hBitmap;
	HPALETTE hPalette;

	if( LoadBitmapFromBMPFile( strFilename, &hBitmap, &hPalette ) )
	{
		if( m_hBitmap )
			::DeleteObject( m_hBitmap );
		m_hBitmap = hBitmap;

		if( m_hPalette )
			::DeleteObject( m_hPalette );
		m_hPalette = hPalette;

		Invalidate();
		UpdateWindow();
		m_strPictureFile = strFilename;
		return TRUE;
	}
	return FALSE;
}


