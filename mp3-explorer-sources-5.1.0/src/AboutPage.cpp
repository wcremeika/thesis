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
#include "AboutPage.h"
#include "Mp3Exp.h"

extern CMp3ExpApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAboutPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(CAboutPage2, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CAboutPage1 property page

CAboutPage1::CAboutPage1() : CPropertyPage(CAboutPage1::IDD)
{
	//{{AFX_DATA_INIT(CAboutPage1)
	//}}AFX_DATA_INIT
	m_strHomepageUrl = "http://www.mp3-explorer.com/";
}

CAboutPage1::~CAboutPage1()
{
}

void CAboutPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutPage1)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutPage1)
	ON_BN_CLICKED(IDC_BT_HOMEPAGE, OnButtonHomepage)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutPage1::OnButtonHomepage()
 
{
	ShellExecute( m_hWnd , "open" , m_strHomepageUrl, NULL, NULL, SW_SHOWNORMAL );
	
}

void CAboutPage1::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( nIDCtl == IDC_BT_HOMEPAGE )
	{
		CDC dc;
		dc.Attach( lpDrawItemStruct->hDC );
		CFont *pFont = (CFont *) dc.SelectStockObject( ANSI_VAR_FONT );
		LOGFONT lf;
		pFont->GetLogFont( &lf );
		lf.lfUnderline = TRUE;
		CFont fontUnderlined;
		fontUnderlined.CreateFontIndirect( &lf );
		dc.SelectObject( fontUnderlined );
		dc.SetTextColor( theApp.m_colorLink );
		dc.SetTextAlign( TA_CENTER );
		int nX = (lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left)/2;
		dc.TextOut( nX , 0, m_strHomepageUrl );
		dc.Detach();
	}
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);

}


/////////////////////////////////////////////////////////////////////////////
// CAboutPage2 property page

CAboutPage2::CAboutPage2() : CPropertyPage(CAboutPage2::IDD)
{
	//{{AFX_DATA_INIT(CAboutPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAboutPage2::~CAboutPage2()
{
}

void CAboutPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutPage2)
	DDX_Control(pDX, IDC_CREDIT, m_wndCredit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutPage2)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutPage2::OnTimer(UINT nIDEvent) 
{
	CPropertyPage::OnTimer(nIDEvent);
}


void CAboutPage2::OnDestroy() 
{
	CPropertyPage::OnDestroy();
		
}



BOOL CAboutPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Translations
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "AboutPage1", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "AboutPage1", "Item2" ));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
