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
#include "mp3exp.h"
#include "ID3v2Sheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MAX_CAPTION 255

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CID3v2Sheet

IMPLEMENT_DYNAMIC(CID3v2Sheet, CPropertySheet)

CID3v2Sheet::CID3v2Sheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page3);


}

CID3v2Sheet::~CID3v2Sheet()
{
}


BEGIN_MESSAGE_MAP(CID3v2Sheet, CPropertySheet)
	//{{AFX_MSG_MAP(CID3v2Sheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CID3v2Sheet message handlers



BOOL CID3v2Sheet::OnInitDialog() 
{
	static _TCHAR szCaptionPage1[MAX_CAPTION];
	static _TCHAR szCaptionPage2[MAX_CAPTION];
	static _TCHAR szCaptionPage3[MAX_CAPTION];

	BOOL bResult = CPropertySheet::OnInitDialog();
	
	SetWindowText( theApp.Translate( "ID3v2Sheet" , "Caption" ) );
	lstrcpy( szCaptionPage1 , theApp.Translate( "ID3v2Sheet" , "Page1" ));
	lstrcpy( szCaptionPage2 , theApp.Translate( "ID3v2Sheet" , "Page2" ));
	lstrcpy( szCaptionPage3 , theApp.Translate( "ID3v2Sheet" , "Page3" ));
	SetPageTitle( 0 , szCaptionPage1 );
	SetPageTitle( 1 , szCaptionPage2 );
	SetPageTitle( 2 , szCaptionPage3 );
	
	return bResult;
}


BOOL CID3v2Sheet::SetPageTitle (int nPage, LPTSTR pszText)
{
    CTabCtrl* pTab = GetTabControl();
    ASSERT (pTab);

    TC_ITEM ti;
    ti.mask = TCIF_TEXT;
    ti.pszText = pszText;
    VERIFY (pTab->SetItem (nPage, &ti));

    return TRUE;
}

