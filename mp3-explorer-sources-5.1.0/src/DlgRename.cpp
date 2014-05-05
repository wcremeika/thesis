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
#include "mp3/Mp3File.h"
#include "Format.h"
#include "DlgRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORMAT_FREE			0
#define FORMAT_PREDEFINED	1

extern CMp3ExpApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog


CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRename)
	m_strNewFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRename)
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_strNewFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
	//{{AFX_MSG_MAP(CDlgRename)
	ON_BN_CLICKED(IDC_FORMAT_PREDEFINED, OnFormatPredefined)
	ON_BN_CLICKED(IDC_FORMAT_FREE, OnFormatFree)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_FORMATS, OnSelchangeCmbFormats)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRename message handlers

void CDlgRename::OnFormatPredefined() 
{
	CComboBox* pCBox = (CComboBox *) GetDlgItem( IDC_CMB_FORMATS );
	int nIndex = pCBox->GetCurSel();
	if( nIndex != CB_ERR )
	{
		CFormat* pFormat = (CFormat *) pCBox->GetItemData( nIndex );
		GetDlgItem( IDC_EDIT_NEW_NAME )->SetWindowText( pFormat->GetFilename( m_pMp3 ));
		
	}
	SetMode(FORMAT_PREDEFINED);
	
}

void CDlgRename::OnFormatFree() 
{
	GetDlgItem( IDC_EDIT_NEW_NAME )->EnableWindow( TRUE );
	SetMode(FORMAT_FREE);

}

void CDlgRename::OnOK() 
{
	// Si le mode est format prédéfini et que la séléction est correcte
	// la variable m_bValidFormat est à TRUE, sinon il faut utiliser le
	// nom plutôt que le format.

	m_bValidFormat = FALSE;
	if( GetMode() == FORMAT_PREDEFINED )
	{
		CComboBox* pCBox = (CComboBox *) GetDlgItem( IDC_CMB_FORMATS );
		int nIndex = pCBox->GetCurSel();
		if( nIndex != CB_ERR )
		{
			m_format.Copy( (CFormat *) pCBox->GetItemData( nIndex ));
			m_bValidFormat = TRUE;
		}
	}
	CDialog::OnOK();
}


BOOL CDlgRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox* pCBox = (CComboBox *) GetDlgItem( IDC_CMB_FORMATS );
	int nDefaultIndex;
	theApp.m_nFormatCount;

	if( theApp.m_nFormatCount != 0 )
	{
		for( int i = 0 ; i < theApp.m_nFormatCount ; i++ )
		{
			CFormat* pFormat = new CFormat();
			pFormat->Load( i + 1 );
			if( i == theApp.m_nDefaultFormat )
			{
				nDefaultIndex = i;
			}
			int nIndex = pCBox->AddString( pFormat->m_strName );
			pCBox->SetItemData( nIndex , (DWORD) pFormat );
		}
		pCBox->SetCurSel( nDefaultIndex );
		if( m_bSingleFile )
		{
			SetMode( FORMAT_FREE );
		}
		else
		{
			// Griser le mode Nom libre
			GetDlgItem( IDC_FORMAT_FREE )->EnableWindow( FALSE );
			SetMode( FORMAT_PREDEFINED );
		}

	}
	else
	{
		if( m_bSingleFile )
		{
			// Disable the combo
			GetDlgItem( IDC_CMB_FORMATS )->EnableWindow( FALSE );
			GetDlgItem( IDC_FORMAT_PREDEFINED )->EnableWindow( FALSE );
			SetMode( FORMAT_FREE );
		}
		else
		{
			// No format availbale => can't rename
			CString strMsg = theApp.LoadTranslatedString( "IdsErrorFormatNotAvailable" );
			MessageBox( strMsg , NULL, MB_ICONWARNING ); 
			EndDialog(FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRename::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// Destruction des formats
	CComboBox* pCBox = (CComboBox *) GetDlgItem( IDC_CMB_FORMATS );
	for( int i = 0 ; i < pCBox->GetCount() ; i++ )
	{
		CFormat* pFormat = (CFormat *) pCBox->GetItemData( i );
		delete pFormat;
	}

	
}

void CDlgRename::OnSelchangeCmbFormats() 
{
	OnFormatPredefined();
}

void CDlgRename::SetMode(int nMode)
{
	CButton *pRadioPredefined = (CButton *) GetDlgItem( IDC_FORMAT_PREDEFINED );
	CButton *pRadioFree = (CButton *) GetDlgItem( IDC_FORMAT_FREE );

	switch( nMode )
	{
	case FORMAT_FREE:
		pRadioFree->SetCheck(TRUE);
		pRadioPredefined->SetCheck(FALSE);
		GetDlgItem( IDC_EDIT_NEW_NAME )->EnableWindow( TRUE );
		break;

	case FORMAT_PREDEFINED:
		pRadioFree->SetCheck(FALSE);
		pRadioPredefined->SetCheck(TRUE);
		GetDlgItem( IDC_EDIT_NEW_NAME )->EnableWindow( FALSE );
		break;
	}
	m_nMode = nMode;
}

int CDlgRename::GetMode()
{
	return m_nMode;
}
