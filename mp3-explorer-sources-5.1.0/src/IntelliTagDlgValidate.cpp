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
#include "IntelliTagDlgValidate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIntelliTagDlgValidate dialog


CIntelliTagDlgValidate::CIntelliTagDlgValidate(CWnd* pParent /*=NULL*/)
	: CDialog(CIntelliTagDlgValidate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntelliTagDlgValidate)
	m_strFilename = _T("");
	m_strAlbum = _T("");
	m_strArtist = _T("");
	m_strTitle = _T("");
	m_strTrack = _T("");
	//}}AFX_DATA_INIT
	m_bCancelAll = FALSE;
	m_bYesToAll = FALSE;
	m_bMultipleFiles = FALSE;
}


void CIntelliTagDlgValidate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntelliTagDlgValidate)
	DDX_Text(pDX, IDC_FILENAME, m_strFilename);
	DDX_Text(pDX, IDC_CMB_ALBUM, m_strAlbum);
	DDX_Text(pDX, IDC_CMB_ARTIST, m_strArtist);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_TRACK, m_strTrack);
	DDV_MaxChars(pDX, m_strTrack, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntelliTagDlgValidate, CDialog)
	//{{AFX_MSG_MAP(CIntelliTagDlgValidate)
	ON_CBN_DROPDOWN(IDC_CMB_ARTIST, OnDropdownCmbArtist)
	ON_BN_CLICKED(IDC_BT_YESTOALL, OnYesToAll)
	ON_BN_CLICKED(IDC_BT_CANCELALL, OnBtCancelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntelliTagDlgValidate message handlers

BOOL CIntelliTagDlgValidate::OnInitDialog() 
{
	CDialog::OnInitDialog();


	SetWindowText( theApp.Translate( "IntellitagDlg", "Caption" ));
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "IntellitagDlg", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "IntellitagDlg", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "IntellitagDlg", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "IntellitagDlg", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "IntellitagDlg", "Item5" ));
	SetDlgItemText( IDC_ITEM6 , theApp.Translate( "IntellitagDlg", "Item6" ));
	SetDlgItemText( IDC_ITEM7 , theApp.Translate( "IntellitagDlg", "Item7" ));
	SetDlgItemText( IDC_BT_YESTOALL , theApp.Translate( "IntellitagDlg", "ButtonYesToAll" )); 
	SetDlgItemText( IDOK , theApp.Translate( "IntellitagDlg", "ButtonYes" )); 
	SetDlgItemText( IDCANCEL , theApp.Translate( "IntellitagDlg", "ButtonNo" )); 
	SetDlgItemText( IDC_BT_CANCELALL , theApp.Translate( "IntellitagDlg", "ButtonNoToAll" )); 
	
	m_imagelist.Create( 16, 16, IDB_SMALL_ICONS );
	m_imagelist.SetOverlayImage( 1, 1 );

	// Artists
	m_comboArtist.SubclassDlgItem( IDC_CMB_ARTIST , this );
	m_comboArtist.SetImageList( &m_imagelist );

	m_comboArtist.FillWithList( m_pListArtist->GetList( m_pMapArtist)  , ICON_ARTIST , ICON_ARTIST );
	m_comboArtist.SelectItem( m_strArtist );

	// Album
	m_comboAlbum.SubclassDlgItem( IDC_CMB_ALBUM , this );
	m_comboAlbum.SetImageList( &m_imagelist );
	OnDropdownCmbArtist();
	m_comboAlbum.SelectItem( m_strAlbum );

	CString strConfirm;
	
	if( !m_bMultipleFiles )
	{
		CButton* pButton = (CButton*) GetDlgItem( IDC_BT_YESTOALL );
		pButton->ShowWindow( SW_HIDE );

		pButton = (CButton*) GetDlgItem( IDC_BT_CANCELALL );
		pButton->ShowWindow( SW_HIDE );
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIntelliTagDlgValidate::OnDropdownCmbArtist() 
{
	CString strSelectedArtist = m_comboArtist.GetText();

	POSITION posArtist = m_pMapArtist->GetStartPosition();
	while( posArtist != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_pMapArtist->GetNextAssoc( posArtist, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );

		if( strSelectedArtist == strArtist )
		{
			CString strText = m_comboAlbum.GetText();
			m_comboAlbum.ResetContent();

			POSITION pos = pArtist->m_mapAlbum.GetStartPosition( );
			while( pos != NULL )
			{
				CAlbum* pAlbum;
				CString strAlbum;
				pArtist->m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
				ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
				m_comboAlbum.AddSortedItem( strAlbum, ICON_ALBUM, ICON_ALBUM );
			}

			m_comboAlbum.SetText( strText );
			return;
		}
	}
	
}

void CIntelliTagDlgValidate::OnYesToAll() 
{
	m_bYesToAll = TRUE;
	OnOK();
	
}

void CIntelliTagDlgValidate::OnBtCancelAll() 
{
	m_bCancelAll = TRUE;
	OnCancel();
	
}
