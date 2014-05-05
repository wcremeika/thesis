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
#include "Mp3ExpDoc.h"
#include "Engine.h"
#include "util/HyperLink.h"
#include "HtmlMenu.h"
#include "WebSearchResource.h"
#include <afxinet.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHtmlMenu

IMPLEMENT_DYNCREATE(CHtmlMenu, CFormView)

CHtmlMenu::CHtmlMenu()
	: CFormView(CHtmlMenu::IDD)
{
	//{{AFX_DATA_INIT(CHtmlMenu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bOnLine = FALSE;
	m_brushBackground.CreateSolidBrush( theApp.m_colorHtmlMenuBackground );
}

CHtmlMenu::~CHtmlMenu()
{
}

void CHtmlMenu::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlMenu)
	DDX_Control(pDX, IDC_LINK_FILEVIEW, m_linkFileView);
	DDX_Control(pDX, IDC_LINK_HELP, m_linkHelp);
	DDX_Control(pDX, IDC_LINK_README, m_linkReadme);
	DDX_Control(pDX, IDC_LINK_WEBSITE, m_linkWebSite);
	DDX_Control(pDX, IDC_COMBOEX_WEB_SEARCH_RESOURCES, m_comboWebSearchResources);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlMenu, CFormView)
	//{{AFX_MSG_MAP(CHtmlMenu)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_COMBOEX_WEB_SEARCH_RESOURCES, OnSelchangeComboWebSearchResources)
	ON_BN_CLICKED(IDC_BT_SEARCH, OnSearch)
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_WEB_SEARCH_RESOURCE_DESCRIPTION, OnSetfocusWebSearchResourceDescription)
	ON_CBN_DROPDOWN(IDC_COMBOEX_WEB_SEARCH_RESOURCES, OnDropdownComboexWebSearchResources)
	ON_MESSAGE( WM_LINK_ACTION, OnLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlMenu diagnostics

#ifdef _DEBUG
void CHtmlMenu::AssertValid() const
{
	CFormView::AssertValid();
}

void CHtmlMenu::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlMenu message handlers


HBRUSH CHtmlMenu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor != CTLCOLOR_EDIT )
	{
		hbr = (HBRUSH) m_brushBackground;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CHtmlMenu::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	BOOL bDrawLabel = FALSE;
	CString strButtonText;

	if( nIDCtl == IDC_LB_LOCAL )
	{
		strButtonText = theApp.Translate( "HtmlMenu" , "Local" );
		bDrawLabel = TRUE;
	}

	if( nIDCtl == IDC_LB_INTERNET )
	{
		strButtonText = theApp.Translate( "HtmlMenu" , "Internet" );
		bDrawLabel = TRUE;
	}

	if( nIDCtl == IDC_LB_WEB_SEARCH_RESOURCES )
	{
		strButtonText = theApp.Translate( "HtmlMenu" , "Search" );;
		bDrawLabel = TRUE;
	}

	if( bDrawLabel )
	{
		CDC dc;
		dc.Attach( lpDrawItemStruct->hDC );
		CFont *pFont = (CFont *) dc.SelectStockObject( ANSI_VAR_FONT );
		LOGFONT lf;
		pFont->GetLogFont( &lf );
		if( bDrawLabel )
		{
			// Labels
			lf.lfWeight = FW_BOLD;
		}
		else
		{
			// Boutons
			lf.lfUnderline = ( lpDrawItemStruct->itemState & ODS_SELECTED ) ? FALSE : TRUE;
		}
		CFont fontUnderlined;
		fontUnderlined.CreateFontIndirect( &lf );
		dc.SelectObject( fontUnderlined );
		dc.SetTextColor( theApp.m_colorHtmlMenuLabel );
		dc.SetBkColor( theApp.m_colorHtmlMenuLabelBackground );
		dc.SetTextAlign( TA_LEFT );
		dc.TextOut( 0 , 0, strButtonText );
		dc.Detach();
	}

	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}



void CHtmlMenu::OnSelchangeComboWebSearchResources() 
{
	CComboBoxEx* pCombo = (CComboBoxEx *) GetDlgItem( IDC_COMBOEX_WEB_SEARCH_RESOURCES );
	int nIndex = pCombo->GetCurSel();
	if( nIndex != - 1 )
	{
		CWebSearchResource *pWebSearchResource = (CWebSearchResource *) pCombo->GetItemData( nIndex );
		if( pWebSearchResource != NULL )
		{
			m_strWebSearchResource = pWebSearchResource->GetCode();
			m_labelDescription.SetText( pWebSearchResource->GetDescription() );

			CEdit* pEdit = (CEdit*) GetDlgItem( IDC_EDIT_SEARCH );
			if( pWebSearchResource->AcceptSearchTerms() )
			{
				pEdit->ShowWindow( SW_SHOW );
			}
			else
			{
				pEdit->ShowWindow( SW_HIDE );
			}
		}
	}
	
}


void CHtmlMenu::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
/*	if( !m_bInit )
		Init();
*/	
}

void CHtmlMenu::OnSearch() 
{
	if( m_strWebSearchResource != "" )
	{
		CString strSearch;
		CEdit* pEdit = (CEdit*) GetDlgItem( IDC_EDIT_SEARCH );
		pEdit->GetWindowText( strSearch );

		CString strUrl;
		for( int i = 0 ; i < m_listWebSearchResources.GetSize() ; i++ )
		{
			CWebSearchResource* pWebSearchResource = (CWebSearchResource*) m_listWebSearchResources.GetAt( i );
			if( pWebSearchResource->GetCode() == m_strWebSearchResource )
			{
				strUrl = pWebSearchResource->GetUrl() + strSearch;
				break;
			}
						
		}
		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
		pDoc->SetSearchUrl( strUrl );
	} 
		
}



CString CHtmlMenu::GetWebSearchResourcesString()
{

	CString strList;

	CString strWebSearchResource;
	CString strWebSearchResourceFile = theApp.Translate( "HtmlMenu" , "WebSearchResourceFile" );
	strWebSearchResource = theApp.m_strInstallPath + "\\" + strWebSearchResourceFile;
	CStdioFile file;
	if( file.Open( strWebSearchResource , CFile::modeRead ) )
	{
		CString strLine;
		while( file.ReadString( strLine ))
		{
			strList += strLine + "\n";
		}
	}

	return strList;
}

int CHtmlMenu::GetWebSearchResources(CObArray &listWebSearchResources, CString strList )
{
	// Read all file lines
	CString strBuffer = strList;
	int nPos;
	while( (nPos = strBuffer.Find('\n')) != -1)
	{
		CWebSearchResource* WebSearchResource = (CWebSearchResource*) new CWebSearchResource();
		if( WebSearchResource->Init( strBuffer.Left( nPos )))
		{
			listWebSearchResources.Add( (CObject *) WebSearchResource );
		}
		else
		{
			delete WebSearchResource;
		}
		strBuffer = strBuffer.Right( strBuffer.GetLength() - nPos - 1 ); 
	}
	return listWebSearchResources.GetSize();

}

void CHtmlMenu::ResetWebSearchResources()
{
	for( int i = 0 ; i < m_listWebSearchResources.GetSize() ; i++ )
	{
		CWebSearchResource *pWebSearchResource = (CWebSearchResource *) m_listWebSearchResources.GetAt( i );
		delete pWebSearchResource;
	}
	m_listWebSearchResources.RemoveAll();
	if( ::IsWindow( m_comboWebSearchResources.m_hWnd ))
		m_comboWebSearchResources.ResetContent();
}

void CHtmlMenu::OnDestroy() 
{
	CFormView::OnDestroy();
	
	ResetWebSearchResources();
	
}

void CHtmlMenu::OnSetfocusWebSearchResourceDescription() 
{
	// Renvoie le focus sur un autre bouton pour ne pas faire de modifs
	CWnd* pWnd = (CWnd*) GetDlgItem( IDC_LB_WEB_SEARCH_RESOURCES );
	pWnd->SetFocus();
	
}

void CHtmlMenu::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// Combo WebSearchResources
	m_imagelistComboWebSearchResources.Create( 16, 16, IDB_WEB_SEARCH_RESOURCES );
	m_imagelistComboWebSearchResources.SetOverlayImage( 1, 1 );

	m_comboWebSearchResources.SetImageList( &m_imagelistComboWebSearchResources );
	FillWebSearchResourcesCombo();

	// Links
	m_linkFileView.Init( theApp.Translate( "HtmlMenu" , "LinkFileView" ), HTMLVIEW_FILE );
	m_linkFileView.SetColours( theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink );

	m_linkHelp.Init( theApp.Translate( "HtmlMenu" , "LinkViewHelp" ), HTMLVIEW_HELP );
	m_linkHelp.SetColours( theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink );

	m_linkReadme.Init( theApp.Translate( "HtmlMenu" , "LinkViewReadme" ), HTMLVIEW_README );
	m_linkReadme.SetColours( theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink );

	m_linkWebSite.Init( theApp.Translate( "HtmlMenu" , "LinkWebSite" ), HTMLVIEW_WEBSITE );
	m_linkWebSite.SetColours( theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink, theApp.m_colorHtmlMenuLink );


	// Bouton rechercher
	CString strSearch = theApp.Translate( "HtmlMenu" , "SearchButton" );
	CButton* pButton = (CButton*) GetDlgItem( IDC_BT_SEARCH );
	pButton->SetWindowText( strSearch );

	// Description
	m_labelDescription.SubclassDlgItem( IDC_WEB_SEARCH_RESOURCE_DESCRIPTION, this );
	m_labelDescription.SetBkColor( theApp.m_colorHtmlMenuBackground );
	m_labelDescription.SetTextBackColor( theApp.m_colorHtmlMenuBackground );
	m_labelDescription.SetTextColor( theApp.m_colorHtmlMenuText );
	
}

void CHtmlMenu::OnDropdownComboexWebSearchResources() 
{
	FillWebSearchResourcesCombo();
	
}

void CHtmlMenu::FillWebSearchResourcesCombo()
{

	// If the Application is offline then return
	if( (theApp.m_bDisableConnection) || (!theApp.IsInternetConnected()) )
	{
		ResetWebSearchResources();
		return;
	}
	// If the combo is already filled up then return
	if( m_comboWebSearchResources.GetCount() > 0 )
	{
		return;
	}

	CString strList = GetWebSearchResourcesString();
	GetWebSearchResources( m_listWebSearchResources , strList );

	// Ajouter les partenaires trouvés à partir de liste de fichiers
	for( int i = 0 ; i < m_listWebSearchResources.GetSize() ; i++ )
	{
		CWebSearchResource* pWebSearchResource = (CWebSearchResource*) m_listWebSearchResources.GetAt( i );
		int nIndex = m_comboWebSearchResources.AddSortedItem( pWebSearchResource->GetName(), pWebSearchResource->GetImageIndex(), pWebSearchResource->GetImageIndex());
		m_comboWebSearchResources.SetItemData( nIndex, (DWORD) pWebSearchResource );
	}

}

LRESULT CHtmlMenu::OnLink( WPARAM wParam, LPARAM lParam )
{

	CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
	pDoc->SetHtmlView( wParam );

	return (LRESULT)0;

}
