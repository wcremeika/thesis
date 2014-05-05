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
#include "Mp3ExpDoc.h"
#include "util/ListVwEx.h"
#include "Mp3ExpView.h"
#include "mp3/Mp3File.h"

#include "OptionSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MAX_CAPTION 255

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

COptionSheet::COptionSheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);
	AddPage(&m_Page8);
	AddPage(&m_Page9);
	AddPage(&m_Page10);
	AddPage(&m_Page11);
}

COptionSheet::~COptionSheet()
{
}


BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers



BOOL COptionSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	static _TCHAR szCaptionPage1[MAX_CAPTION];
	static _TCHAR szCaptionPage2[MAX_CAPTION];
	static _TCHAR szCaptionPage3[MAX_CAPTION];
	static _TCHAR szCaptionPage4[MAX_CAPTION];
	static _TCHAR szCaptionPage5[MAX_CAPTION];
	static _TCHAR szCaptionPage6[MAX_CAPTION];
	static _TCHAR szCaptionPage7[MAX_CAPTION];
	static _TCHAR szCaptionPage8[MAX_CAPTION];
	static _TCHAR szCaptionPage9[MAX_CAPTION];
	static _TCHAR szCaptionPage10[MAX_CAPTION];
	static _TCHAR szCaptionPage11[MAX_CAPTION];

	SetWindowText( theApp.Translate( "OptionSheet" , "Caption" ) );
	lstrcpy( szCaptionPage1 , theApp.Translate( "OptionSheet" , "Page1" ));
	lstrcpy( szCaptionPage2 , theApp.Translate( "OptionSheet" , "Page2" ));
	lstrcpy( szCaptionPage3 , theApp.Translate( "OptionSheet" , "Page3" ));
	lstrcpy( szCaptionPage4 , theApp.Translate( "OptionSheet" , "Page4" ));
	lstrcpy( szCaptionPage5 , theApp.Translate( "OptionSheet" , "Page5" ));
	lstrcpy( szCaptionPage6 , theApp.Translate( "OptionSheet" , "Page6" ));
	lstrcpy( szCaptionPage7 , theApp.Translate( "OptionSheet" , "Page7" ));
	lstrcpy( szCaptionPage8 , theApp.Translate( "OptionSheet" , "Page8" ));
	lstrcpy( szCaptionPage9 , theApp.Translate( "OptionSheet" , "Page9" ));
	lstrcpy( szCaptionPage10, theApp.Translate( "OptionSheet" , "Page10" ));
	lstrcpy( szCaptionPage11, theApp.Translate( "OptionSheet" , "Page11" ));

	SetPageTitle( 0 , szCaptionPage1 );
	SetPageTitle( 1 , szCaptionPage2 );
	SetPageTitle( 2 , szCaptionPage3 );
	SetPageTitle( 3 , szCaptionPage4 );
	SetPageTitle( 4 , szCaptionPage5 );
	SetPageTitle( 5 , szCaptionPage6 );
	SetPageTitle( 6 , szCaptionPage7 );
	SetPageTitle( 7 , szCaptionPage8 );
	SetPageTitle( 8 , szCaptionPage9 );
	SetPageTitle( 9 , szCaptionPage10 );
	SetPageTitle( 10 , szCaptionPage11 );
	
	return bResult;
}

BOOL COptionSheet::SetPageTitle (int nPage, LPTSTR pszText)
{
    CTabCtrl* pTab = GetTabControl();
    ASSERT (pTab);

    TC_ITEM ti;
    ti.mask = TCIF_TEXT;
    ti.pszText = pszText;
    VERIFY (pTab->SetItem (nPage, &ti));

    return TRUE;
}


