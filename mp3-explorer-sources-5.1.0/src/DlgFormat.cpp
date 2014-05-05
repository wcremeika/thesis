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
#include "Format.h"
#include "DlgFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_CMB_ENTRIES 7
#define MAX_STRING 255

typedef struct index_string {
	int m_nIndex;
	_TCHAR m_szStringResName[MAX_STRING];
} INDEX_STRING;

static INDEX_STRING g_index_string[MAX_CMB_ENTRIES] = { 
				{ 0 , ""}, // Blank
				{ ARTIST ,	"IdsArtist" },
				{ TITLE ,	"IdsTitle" }, 
				{ ALBUM ,	"IdsAlbum" },
				{ GENRE ,	"IdsGenre" },
				{ DATE ,	"IdsDate" },
				{ TRACKNUM , "IdsTrackNum" } 
};

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog


CDlgFormat::CDlgFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFormat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CDlgFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormat)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormat, CDialog)
	//{{AFX_MSG_MAP(CDlgFormat)
	ON_BN_CLICKED(IDC_CHECK_UNDERSCORES, OnCheckUnderscores)
	ON_CBN_SELCHANGE(IDC_CMB_FIELD1, OnSelchangeCmbField1)
	ON_CBN_SELCHANGE(IDC_CMB_FIELD2, OnSelchangeCmbField2)
	ON_CBN_SELCHANGE(IDC_CMB_FIELD3, OnSelchangeCmbField3)
	ON_EN_CHANGE(IDC_EDIT_SEP1, OnChangeEditSep1)
	ON_EN_CHANGE(IDC_EDIT_SEP2, OnChangeEditSep2)
	ON_BN_CLICKED(IDC_RADIO_LOWER, OnRadioMaj)
	ON_BN_CLICKED(IDC_RADIO_UPPER, OnRadioMin)
	ON_BN_CLICKED(IDC_RADIO_PROPER, OnRadioMixte)
	ON_BN_CLICKED(IDC_RADIO_UNCHANGED, OnRadioNocasse)
	ON_EN_CHANGE(IDC_EDIT_FORMAT_NAME, OnChangeEditFormatName)
	ON_CBN_SELCHANGE(IDC_CMB_FIELD4, OnSelchangeCmbField4)
	ON_EN_CHANGE(IDC_EDIT_SEP3, OnChangeEditSep3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat message handlers

void CDlgFormat::OnCheckUnderscores() 
{
	CButton *pChkBox = (CButton *) GetDlgItem( IDC_CHECK_UNDERSCORES );
	m_pFormat->m_bUnderscore = pChkBox->GetCheck();	
	ShowPreview();
	
}

void CDlgFormat::OnSelchangeCmbField1() 
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem( IDC_CMB_FIELD1 );
	int nIndex = pCBox->GetCurSel();
	m_pFormat->m_nFieldIndex1 = g_index_string[nIndex].m_nIndex;	
	ShowPreview();
}

void CDlgFormat::OnSelchangeCmbField2() 
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem( IDC_CMB_FIELD2 );
	int nIndex = pCBox->GetCurSel();
	m_pFormat->m_nFieldIndex2 = g_index_string[nIndex].m_nIndex;
	ShowPreview();
	
}

void CDlgFormat::OnSelchangeCmbField3() 
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem( IDC_CMB_FIELD3 );
	int nIndex = pCBox->GetCurSel();
	m_pFormat->m_nFieldIndex3 = g_index_string[nIndex].m_nIndex;
	ShowPreview();
	
}

void CDlgFormat::OnSelchangeCmbField4() 
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem( IDC_CMB_FIELD4 );
	int nIndex = pCBox->GetCurSel();
	m_pFormat->m_nFieldIndex4 = g_index_string[nIndex].m_nIndex;
	ShowPreview();
	
}

void CDlgFormat::OnChangeEditSep1() 
{
	GetDlgItemText( IDC_EDIT_SEP1 , m_pFormat->m_strSeparator1 );
	ShowPreview();
	
}

void CDlgFormat::OnChangeEditSep2() 
{
	GetDlgItemText( IDC_EDIT_SEP2 , m_pFormat->m_strSeparator2 );
	ShowPreview();
	
}

void CDlgFormat::OnChangeEditSep3() 
{
	GetDlgItemText( IDC_EDIT_SEP3 , m_pFormat->m_strSeparator3 );
	ShowPreview();
	
}

void CDlgFormat::OnRadioMaj() 
{
	m_pFormat->m_nCasse = CASSE_MAJ;
	ShowPreview();
	
}

void CDlgFormat::OnRadioMin() 
{
	m_pFormat->m_nCasse = CASSE_MIN;
	ShowPreview();
	
}

void CDlgFormat::OnRadioMixte() 
{
	m_pFormat->m_nCasse = CASSE_MIXTE;
	ShowPreview();
	
}

void CDlgFormat::OnRadioNocasse() 
{
	m_pFormat->m_nCasse = CASSE_NO_MODIF;
	ShowPreview();
	
}

void CDlgFormat::ShowPreview()
{
	CString strFilename = m_pFormat->GetFilename( m_pMp3 );
	SetDlgItemText( IDC_RENAMING_PREVIEW , strFilename );
}


BOOL CDlgFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Translations

	SetWindowText( theApp.Translate( "RenamingFormat", "Caption" ) );
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "RenamingFormat", "Item1" ) );
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "RenamingFormat", "Item2" ) );
	SetDlgItemText( IDC_CHECK_UNDERSCORES , theApp.Translate( "RenamingFormat", "Item3" ) );
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "RenamingFormat", "Item4" ) );
	SetDlgItemText( IDC_RADIO_LOWER , theApp.Translate( "RenamingFormat", "Item5" ) );
	SetDlgItemText( IDC_RADIO_UPPER , theApp.Translate( "RenamingFormat", "Item6" ) );
	SetDlgItemText( IDC_RADIO_PROPER , theApp.Translate( "RenamingFormat", "Item7" ) );
	SetDlgItemText( IDC_RADIO_UNCHANGED , theApp.Translate( "RenamingFormat", "Item8" ) );
	SetDlgItemText( IDC_ITEM9 , theApp.Translate( "RenamingFormat", "Item9" ) );
	
	
	SetDlgItemText( IDC_EDIT_FORMAT_NAME , m_pFormat->m_strName );

	// Initialisation des combos
	InitCombo( IDC_CMB_FIELD1 , m_pFormat->m_nFieldIndex1 );
	InitCombo( IDC_CMB_FIELD2 , m_pFormat->m_nFieldIndex2 );
	InitCombo( IDC_CMB_FIELD3 , m_pFormat->m_nFieldIndex3 );
	InitCombo( IDC_CMB_FIELD4 , m_pFormat->m_nFieldIndex4 );

	// Initialisation des champs de séparation
	SetDlgItemText( IDC_EDIT_SEP1 , m_pFormat->m_strSeparator1 );
	SetDlgItemText( IDC_EDIT_SEP2 , m_pFormat->m_strSeparator2 );
	SetDlgItemText( IDC_EDIT_SEP3 , m_pFormat->m_strSeparator3 );

	// Case à cocher pour les underscores
	CButton *pButton = (CButton *) GetDlgItem( IDC_CHECK_UNDERSCORES );
	pButton->SetCheck(m_pFormat->m_bUnderscore);	

	// Radio buttons pour la casse
	pButton = (CButton *) GetDlgItem( IDC_RADIO_LOWER );
	pButton->SetCheck(m_pFormat->m_nCasse == CASSE_MIN );	

	pButton = (CButton *) GetDlgItem( IDC_RADIO_UPPER );
	pButton->SetCheck(m_pFormat->m_nCasse == CASSE_MAJ );	

	pButton = (CButton *) GetDlgItem( IDC_RADIO_PROPER );
	pButton->SetCheck(m_pFormat->m_nCasse == CASSE_MIXTE );	

	pButton = (CButton *) GetDlgItem( IDC_RADIO_UNCHANGED );
	pButton->SetCheck(m_pFormat->m_nCasse == CASSE_NO_MODIF );	
	
	// Initialisation du champ de prévisualisation
	SetDlgItemText( IDC_RENAMING_PREVIEW , "" );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgFormat::InitCombo(int nIdCombo, int nIndex)
{
	CComboBox *pCBox = (CComboBox*) GetDlgItem( nIdCombo );
	CString strEntry;
	int nEntryToSelect = 0;
	for( int i = 0 ; i < MAX_CMB_ENTRIES ; i++ )
	{
		if( i )
		{
			strEntry = theApp.LoadTranslatedString( g_index_string[i].m_szStringResName );
		}
		pCBox->AddString( strEntry );
		if( g_index_string[i].m_nIndex == nIndex )
		{
			nEntryToSelect = i;
		}

	}
	pCBox->SetCurSel( nEntryToSelect );

}

void CDlgFormat::OnChangeEditFormatName() 
{
	GetDlgItemText( IDC_EDIT_FORMAT_NAME , m_pFormat->m_strName );
	
}

