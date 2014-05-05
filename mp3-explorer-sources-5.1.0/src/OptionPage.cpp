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
#include "OptionPage.h"
#include "Format.h"
#include "DlgFormat.h"
#include "Plugin.h"
#include "Language.h"
#include "Engine.h"
#include "Export.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(COptionPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage2, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage3, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage4, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage5, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage6, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage7, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage8, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage9, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionPage11, CPropertyPage)


extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COptionPage1 property page

COptionPage1::COptionPage1() : CPropertyPage(COptionPage1::IDD)
{
	//{{AFX_DATA_INIT(COptionPage1)
	m_nMaxRow = 0;
	//}}AFX_DATA_INIT
}

COptionPage1::~COptionPage1()
{
}

void COptionPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Check(pDX, IDC_CHECK_FULLROW, m_bFullRowSelection);
	DDX_Check(pDX, IDC_RADIO_PLAY, m_bActionPlay);
	DDX_Check(pDX, IDC_RADIO_ENQUEUE, m_bActionEnqueue);
	DDX_Check(pDX, IDC_RADIO_PROPERTIES, m_bActionProperties);
	DDX_Text(pDX, IDC_EDIT_MAX_ROW, m_nMaxRow);
	DDV_MinMaxUInt(pDX, m_nMaxRow, 1, 20000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage1, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CPropertySheet *pWndSheet = (CPropertySheet *) GetParent();
	CPropertyPage *pWndPage = pWndSheet->GetPage( 0 );
	pWndPage->SetWindowText( theApp.Translate( "OptionPage1", "Caption" ));
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage1", "Item1" ));
	SetDlgItemText( IDC_RADIO_PLAY , theApp.Translate( "OptionPage1", "Item2" ));
	SetDlgItemText( IDC_RADIO_ENQUEUE , theApp.Translate( "OptionPage1", "Item3" ));
	SetDlgItemText( IDC_RADIO_PROPERTIES , theApp.Translate( "OptionPage1", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "OptionPage1", "Item5" ));
	SetDlgItemText( IDC_CHECK_FULLROW , theApp.Translate( "OptionPage1", "Item6" ));
	SetDlgItemText( IDC_ITEM7 , theApp.Translate( "OptionPage1", "Item7" ));
	SetDlgItemText( IDC_ITEM8 , theApp.Translate( "OptionPage1", "Item8" ));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// COptionPage2 property page

COptionPage2::COptionPage2() : CPropertyPage(COptionPage2::IDD)
{
	//{{AFX_DATA_INIT(COptionPage2)
	m_bDisplayed = FALSE;
	//}}AFX_DATA_INIT

    // Load the font we want to use.
    m_font.CreateStockObject(ANSI_VAR_FONT);
    // Get the metrics of the font.    
	CDC dc;    
	dc.CreateCompatibleDC(NULL);
    CFont* pfntOld = (CFont*) dc.SelectObject(&m_font);    
	TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);    
	dc.SelectObject(pfntOld);
    m_nFontHeight = tm.tmHeight;    
	m_nFontWidth = tm.tmMaxCharWidth;
    // Load the bitmap we want.    
	m_bmCheck.LoadBitmap(IDB_CHECK);
}

COptionPage2::~COptionPage2()
{
	ResetList();

}

void COptionPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage2)
	DDX_Control(pDX, IDC_COLUMN_LIST, m_listbox);
	DDX_Check(pDX, IDC_DISPLAY_LIMITED, m_bDisplayed);
	DDX_Check(pDX, IDC_LEFT_ALIGN, m_bLeftAlign);
	DDX_Check(pDX, IDC_CENTER_ALIGN, m_bCenterAlign);
	DDX_Check(pDX, IDC_RIGHT_ALIGN, m_bRightAlign);
	//}}AFX_DATA_MAP
}

void COptionPage2::GetColumnsInfos( CObArray& columns )
{
	ResetList();
	for( int i = 0 ; i < columns.GetSize() ; i++ )
	{
		CColumn *pColSource = (CColumn *) columns.GetAt(i);
		CColumn *pCol = new CColumn();
		pCol->Copy( pColSource );
		m_columns.Add( pCol );
	}
}

void COptionPage2::ResetList()
{
	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn *pCol = (CColumn *) m_columns.GetAt(i);
		delete pCol;
	}
	m_columns.RemoveAll();

}

void COptionPage2::SetColumnsInfos( CObArray& columns )
{
	ASSERT(  m_columns.GetSize() ==  columns.GetSize() );
	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn *pColSource = (CColumn *) m_columns.GetAt(i);
		CColumn *pCol = (CColumn *) columns.GetAt( i );;
		pCol->Copy( pColSource );
	}
}


BEGIN_MESSAGE_MAP(COptionPage2, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage2)
	ON_LBN_SELCHANGE(IDC_COLUMN_LIST, OnSelchangeColumnList)
	ON_BN_CLICKED(IDC_LEFT_ALIGN, OnLeftAlign)
	ON_BN_CLICKED(IDC_RIGHT_ALIGN, OnRightAlign)
	ON_BN_CLICKED(IDC_CENTER_ALIGN, OnCenterAlign)
	ON_BN_CLICKED(IDC_DISPLAY_LIMITED, OnDisplayLimited)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnMoveDown)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_LBN_DBLCLK(IDC_COLUMN_LIST, OnDblclkColumnList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COptionPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	FillListBox();

	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage2", "Item1" ));
	SetDlgItemText( IDC_BUTTON_UP , theApp.Translate( "OptionPage2", "Item2" ));
	SetDlgItemText( IDC_BUTTON_DOWN , theApp.Translate( "OptionPage2", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage2", "Item4" ));
	SetDlgItemText( IDC_LEFT_ALIGN , theApp.Translate( "OptionPage2", "Item5" ));
	SetDlgItemText( IDC_CENTER_ALIGN , theApp.Translate( "OptionPage2", "Item6" ));
	SetDlgItemText( IDC_RIGHT_ALIGN , theApp.Translate( "OptionPage2", "Item7" ));
	SetDlgItemText( IDC_DISPLAY_LIMITED , theApp.Translate( "OptionPage2", "Item8" ));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPage2::FillListBox()
{
	m_listbox.ResetContent();
	for( int i = 0 ; i < m_columns.GetSize() ; i++ )
	{
		CColumn *pCol = (CColumn *) m_columns.GetAt(i);
		InsertSorted( pCol );
	}
	m_listbox.SetCurSel(0);
	OnSelchangeColumnList();
}
CString COptionPage2::GetColumnText(CColumn *pCol)
{
	CString strText = pCol->m_pszColTitle;
	if( ! pCol->m_nDisplayed )
	{
		strText = "(" + strText + ")";
	}
	return strText;
}

int COptionPage2::InsertSorted( CColumn *pColToInsert )
{
	int nOrder = pColToInsert->m_nInfoOrder;
	for( int i = 0 ; i < m_listbox.GetCount() ; i++ )
	{
		CColumn *pCol = (CColumn *) m_listbox.GetItemData(i);
		if( nOrder < pCol->m_nInfoOrder )
			break;
	}
	int nIndex = m_listbox.InsertString( i , GetColumnText( pColToInsert ));
	m_listbox.SetItemData( nIndex, (DWORD) pColToInsert );
	return nIndex;

}

void COptionPage2::OnSelchangeColumnList() 
{
	int nIndex = m_listbox.GetCurSel();
	CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
	m_bDisplayed = (BOOL) pCol->m_nDisplayed;
	m_bLeftAlign = ( pCol->m_nAlignemnt == LVCFMT_LEFT );
	m_bCenterAlign = ( pCol->m_nAlignemnt == LVCFMT_CENTER );
	m_bRightAlign = ( pCol->m_nAlignemnt == LVCFMT_RIGHT );
	UpdateData(FALSE);
	
}

void COptionPage2::OnLeftAlign() 
{
	int nIndex = m_listbox.GetCurSel();
	CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
	pCol->m_nAlignemnt = LVCFMT_LEFT;
}

void COptionPage2::OnRightAlign() 
{
	int nIndex = m_listbox.GetCurSel();
	CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
	pCol->m_nAlignemnt = LVCFMT_RIGHT;
	
}

void COptionPage2::OnCenterAlign() 
{
	int nIndex = m_listbox.GetCurSel();
	CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
	pCol->m_nAlignemnt = LVCFMT_CENTER;
	
}

void COptionPage2::OnDisplayLimited() 
{
	int nIndex = m_listbox.GetCurSel();
	CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
	UpdateData( TRUE );
	pCol->m_nDisplayed = m_bDisplayed;
	// Supprimer l'élément à l'index nIndex et le réinserer
	m_listbox.DeleteString( nIndex );
	m_listbox.SetCurSel( InsertSorted( pCol ));
	
}

void COptionPage2::OnDblclkColumnList() 
{
	m_bDisplayed = !m_bDisplayed;
	// Remets à jour la checkbox
	UpdateData( FALSE );
	// Appel la fonction de changement de l'affichage
	OnDisplayLimited();
}


void COptionPage2::OnMoveUp() 
{
	int nIndex = m_listbox.GetCurSel();
	if( nIndex != 0 )
	{
		// Récupérer le pointeur sur la colonne
		CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
		pCol->m_nInfoOrder -= 1;
		// Modifier la colonne précédente
		CColumn *pColPrev = (CColumn *) m_listbox.GetItemData( nIndex - 1 );
		pColPrev->m_nInfoOrder += 1;
		// Supprimer l'élément à l'index nIndex et le réinserer
		m_listbox.DeleteString( nIndex );
		m_listbox.SetCurSel( InsertSorted( pCol ));
	}
	
}

void COptionPage2::OnMoveDown() 
{
	int nIndex = m_listbox.GetCurSel();
	if( nIndex != m_listbox.GetCount() - 1 )
	{
		// Récupérer le pointeur sur la colonne
		CColumn *pCol = (CColumn *) m_listbox.GetItemData( nIndex );
		pCol->m_nInfoOrder += 1;
		// Modifier la colonne précédente
		CColumn *pColNext = (CColumn *) m_listbox.GetItemData( nIndex + 1 );
		pColNext->m_nInfoOrder -= 1;
		// Supprimer l'élément à l'index nIndex et le réinserer
		m_listbox.DeleteString( nIndex );
		m_listbox.SetCurSel( InsertSorted( pCol ));
	}
}

void COptionPage2::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if( nIDCtl == IDC_COLUMN_LIST )
	{

		lpMeasureItemStruct->itemHeight = max(m_nFontHeight,
                                          m_bmCheck.GetHeight());	
	}
	CPropertyPage::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void COptionPage2::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( nIDCtl == IDC_COLUMN_LIST )
	{

		CColumn* pCol;
	    HFONT hfntOld;
	    CRect rcRow;
	    CRect rcText;

		rcRow = lpDrawItemStruct->rcItem; 
		rcText = lpDrawItemStruct->rcItem; 
		rcText.left = lpDrawItemStruct->rcItem.left + m_bmCheck.GetWidth() + 2;

		// Get a pointer to the object.        
		pCol = (CColumn*) lpDrawItemStruct->itemData;
	    ASSERT(pCol);        

		switch (lpDrawItemStruct->itemAction) 
		{
		case ODA_DRAWENTIRE:        // Draw the whole line of information.
			// Check if we need to show selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
	        // Set up the font we want to use.
		    hfntOld = (HFONT) ::SelectObject(lpDrawItemStruct->hDC, m_font.m_hObject);
			// Erase the entire area.
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			// Draw the bitmap in place.
			if( pCol->m_nDisplayed )
			{
				m_bmCheck.DrawTrans(lpDrawItemStruct->hDC, rcRow.left, rcRow.top);
			}
	        // Check if we need to show focus state.
		    if (lpDrawItemStruct->itemState & ODS_FOCUS) 
			{
				::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			}
			// Move the text over to just beyond the bitmap.
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, pCol->m_pszColTitle, -1,  &rcText, DT_LEFT | DT_VCENTER);
			::SelectObject(lpDrawItemStruct->hDC, hfntOld);        
			break;    

		case ODA_FOCUS:
		    // Toggle the focus state.
			::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			break;

		case ODA_SELECT:        // Toggle the selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, pCol->m_pszColTitle, -1,  &rcText, DT_LEFT | DT_VCENTER);
			break;    
	
		default:
		    break;    
		}
	}
	else
	{
		CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COptionPage3 property page

COptionPage3::COptionPage3() : CPropertyPage(COptionPage3::IDD)
{
	//{{AFX_DATA_INIT(COptionPage3)
	m_strFavoriteAddresses = _T("");
	//}}AFX_DATA_INIT
}

COptionPage3::~COptionPage3()
{
}

void COptionPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage3)
	DDX_Text(pDX, IDC_EDIT_FAVORITE_ADDRESSES, m_strFavoriteAddresses);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPage3, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage3", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage3", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "OptionPage3", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage3", "Item4" ));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// COptionPage4 property page

COptionPage4::COptionPage4() : CPropertyPage(COptionPage4::IDD)
{
	//{{AFX_DATA_INIT(COptionPage4)
	m_strCmdEnqueue = _T("");
	m_strCmdPlay = _T("");
	m_bUseCachedCatalog = FALSE;
	//}}AFX_DATA_INIT
}

COptionPage4::~COptionPage4()
{
}

void COptionPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage4)
	DDX_Text(pDX, IDC_EDIT_ENQUEUE, m_strCmdEnqueue);
	DDX_Text(pDX, IDC_EDIT_PLAY, m_strCmdPlay);
	DDX_Check(pDX, IDC_CHECK_CACHED_CATALOG, m_bUseCachedCatalog);
	DDX_Check(pDX, IDC_CHECK_NO_CONNECTION, m_bDisableConnection);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPage4, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage4", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage4", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "OptionPage4", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage4", "Item4" ));
	SetDlgItemText( IDC_CHECK_CACHED_CATALOG , theApp.Translate( "OptionPage4", "Item5" ));
	SetDlgItemText( IDC_CHECK_NO_CONNECTION , theApp.Translate( "OptionPage4", "Item6" ));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// COptionPage5 property page

COptionPage5::COptionPage5() : CPropertyPage(COptionPage5::IDD)
{
	//{{AFX_DATA_INIT(COptionPage5)
	//}}AFX_DATA_INIT
    // Load the font we want to use.
    m_font.CreateStockObject(ANSI_VAR_FONT);
    // Get the metrics of the font.    
	CDC dc;    
	dc.CreateCompatibleDC(NULL);
    CFont* pfntOld = (CFont*) dc.SelectObject(&m_font);    
	TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);    
	dc.SelectObject(pfntOld);
    m_nFontHeight = tm.tmHeight;    
	m_nFontWidth = tm.tmMaxCharWidth;
    // Load the bitmap we want.    
	m_bmCheck.LoadBitmap(IDB_CHECK);
}

COptionPage5::~COptionPage5()
{
}

void COptionPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage4)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionPage5, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage5)
	ON_BN_CLICKED(IDC_BT_ADD_FORMAT, OnAddFormat)
	ON_BN_CLICKED(IDC_BT_DELETE_FORMAT, OnDeleteFormat)
	ON_BN_CLICKED(IDC_BT_MODIFY_FORMAT, OnModifyFormat)
	ON_BN_CLICKED(IDC_CHK_FORMAT_DEF, OnFormatDefault)
	ON_LBN_DBLCLK(IDC_LIST_FORMATS, OnDblclkListFormats)
	ON_LBN_SELCHANGE(IDC_LIST_FORMATS, OnSelchangeListFormats)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COptionPage5::OnAddFormat() 
{

	CDlgFormat dlgFormat;
	CFormat *pFormat = new CFormat();
	dlgFormat.m_pFormat = pFormat;
	dlgFormat.m_pMp3 = &m_mp3;
	if( dlgFormat.DoModal() != IDOK )
	{
		delete pFormat;
	}
	else
	{
		m_formatList.Add( (CObject *) pFormat);
		UpdateList();
	}
}

void COptionPage5::OnModifyFormat() 
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	int nIndex = pListBox->GetCurSel();
	CFormat* pFormat = (CFormat *) m_formatList.GetAt( nIndex );
	CFormat formatTemp;
	formatTemp.Copy( pFormat );
	CDlgFormat dlgFormat;
	dlgFormat.m_pFormat = &formatTemp;
	dlgFormat.m_pMp3 = &m_mp3;
	if( dlgFormat.DoModal() == IDOK )
	{
		pFormat->Copy( &formatTemp );
		UpdateList();
	}
	pListBox->SetCurSel( nIndex );
	
}


void COptionPage5::OnDeleteFormat() 
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	int nIndex = pListBox->GetCurSel();
	if( nIndex != LB_ERR )
	{
		CFormat* pFormat = (CFormat *) m_formatList.GetAt( nIndex );
		delete pFormat;
		m_formatList.RemoveAt( nIndex );
		UpdateList();
	}
	
}


void COptionPage5::OnFormatDefault() 
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	int nIndex = pListBox->GetCurSel();
	if( nIndex != LB_ERR )
	{
		CFormat* pFormat = (CFormat*) m_formatList.GetAt(nIndex);
		CButton* pButton = (CButton*) GetDlgItem( IDC_CHK_FORMAT_DEF );
		if( pButton->GetCheck() )
		{
			pFormat->m_bDefault = TRUE;
			theApp.m_nDefaultFormat = nIndex;
		}
		else
		{
			pFormat->m_bDefault = FALSE;
			if( theApp.m_nDefaultFormat == nIndex)
				theApp.m_nDefaultFormat = -1;
		}
		UpdateList();
		pListBox->SetCurSel(nIndex);

	}
	
}

void COptionPage5::OnDblclkListFormats() 
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	int nIndex = pListBox->GetCurSel();
	if( nIndex != LB_ERR )
	{
		CFormat* pFormat = (CFormat*) m_formatList.GetAt(nIndex);
		CButton* pButton = (CButton*) GetDlgItem( IDC_CHK_FORMAT_DEF );
		pButton->SetCheck( TRUE );
		pFormat->m_bDefault = TRUE;
		theApp.m_nDefaultFormat = nIndex;
		UpdateList();
		pListBox->SetCurSel(nIndex);
	}
	
}

void COptionPage5::SaveFormats()
{
	for( int i = 0 ; i < m_formatList.GetSize() ; i++ )
	{
		CFormat* pFormat = (CFormat*) m_formatList.GetAt(i);
		pFormat->Save( i + 1 );
	}
	theApp.SetFormatCount( i );
}

void COptionPage5::LoadFormats()
{
	for( int i = 0 ; i < theApp.m_nFormatCount ; i++ )
	{
		CFormat* pFormat = new CFormat();
		pFormat->Load( i + 1 );
		m_formatList.Add( (CObject*) pFormat );
	}
}

void COptionPage5::FreeFormats()
{
	for( int i = 0 ; i < m_formatList.GetSize() ; i++ )
	{
		CFormat* pFormat = (CFormat*) m_formatList.GetAt(i);
		delete pFormat;
	}
	m_formatList.RemoveAll();
}


void COptionPage5::UpdateList()
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	pListBox->ResetContent();
	for( int i = 0 ; i < m_formatList.GetSize() ; i++ )
	{
		CFormat* pFormat = (CFormat*) m_formatList.GetAt(i);
		pFormat->m_bDefault = ( i == theApp.m_nDefaultFormat) ? TRUE : FALSE;
		int nIndex = pListBox->AddString( pFormat->m_strName );
		pListBox->SetItemData( nIndex , (DWORD) pFormat );
		pListBox->SetCurSel( nIndex );
	}
	UpdatePreview();
		
}

BOOL COptionPage5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage5", "Item1" ));
	SetDlgItemText( IDC_BT_ADD_FORMAT , theApp.Translate( "OptionPage5", "Item2" ));
	SetDlgItemText( IDC_BT_MODIFY_FORMAT , theApp.Translate( "OptionPage5", "Item3" ));
	SetDlgItemText( IDC_BT_DELETE_FORMAT , theApp.Translate( "OptionPage5", "Item4" ));
	SetDlgItemText( IDC_CHK_FORMAT_DEF , theApp.Translate( "OptionPage5", "Item5" ));
	SetDlgItemText( IDC_ITEM6 , theApp.Translate( "OptionPage5", "Item6" ));

	m_mp3.SetArtist("James Brown");
	m_mp3.SetTitle("Rapp payback");
	m_mp3.SetAlbum("Soul syndrome");
	m_mp3.SetYear("1980");
	m_mp3.SetTrackNum("05");

	UpdateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPage5::OnSelchangeListFormats() 
{
	UpdatePreview();
}

void COptionPage5::UpdatePreview()
{
	CListBox* pListBox = (CListBox *) GetDlgItem( IDC_LIST_FORMATS );
	int nIndex = pListBox->GetCurSel();
	CString strFilename;
	if( (nIndex >= 0) && (nIndex < m_formatList.GetSize()))
	{
		CFormat* pFormat = (CFormat *) m_formatList.GetAt( nIndex );
		strFilename = pFormat->GetFilename( &m_mp3 );
		CButton* pButton = (CButton*) GetDlgItem( IDC_CHK_FORMAT_DEF );
		pButton->SetCheck( pFormat->m_bDefault );
	}
	SetDlgItemText( IDC_RENAMING_PREVIEW , strFilename );

}
void COptionPage5::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if( nIDCtl == IDC_LIST_FORMATS )
	{

		lpMeasureItemStruct->itemHeight = max(m_nFontHeight,
                                          m_bmCheck.GetHeight());	
	}
	CPropertyPage::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void COptionPage5::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( nIDCtl == IDC_LIST_FORMATS )
	{
		CFormat* pFormat;
	    HFONT hfntOld;
	    CRect rcRow;
	    CRect rcText;
		CString strText;

		rcRow = lpDrawItemStruct->rcItem; 
		rcText = lpDrawItemStruct->rcItem; 
		rcText.left = lpDrawItemStruct->rcItem.left + m_bmCheck.GetWidth() + 2;

		// Get a pointer to the object.        
		pFormat = (CFormat *) lpDrawItemStruct->itemData;
		if( pFormat != NULL )
		{
			strText = pFormat->m_strName;
		}

		switch (lpDrawItemStruct->itemAction) 
		{
		case ODA_DRAWENTIRE:        // Draw the whole line of information.
	        // Set up the font we want to use.
		    hfntOld = (HFONT) ::SelectObject(lpDrawItemStruct->hDC, m_font.m_hObject);

			// Check if we need to show selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
			// Erase the entire area.
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			// Draw the bitmap in place.
			if( pFormat->m_bDefault )
			{
				m_bmCheck.DrawTrans(lpDrawItemStruct->hDC, rcRow.left, rcRow.top);
			}

			// Check if we need to show focus state.
		    if (lpDrawItemStruct->itemState & ODS_FOCUS) 
			{
				::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			}
			// Move the text over to just beyond the bitmap.
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, strText, -1,  &rcText, DT_LEFT | DT_VCENTER);
			::SelectObject(lpDrawItemStruct->hDC, hfntOld);        

			break;    

		case ODA_FOCUS:
		    // Toggle the focus state.
			::DrawFocusRect(lpDrawItemStruct->hDC, &(rcText));        
			break;

		case ODA_SELECT:        // Toggle the selection state.
			if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_WHITE );
				::SetBkColor(lpDrawItemStruct->hDC, theApp.m_colorListboxSel );
			}
			else
			{
				::SetTextColor(lpDrawItemStruct->hDC, COLOR_BLACK );
				::SetBkColor(lpDrawItemStruct->hDC, COLOR_WHITE );
			}
		    ::ExtTextOut(lpDrawItemStruct->hDC, rcRow.left, rcRow.top,	ETO_OPAQUE,	&rcText, "", 0, NULL); 
			rcText.left += 5;
			::DrawText(lpDrawItemStruct->hDC, strText, -1,  &rcText, DT_LEFT | DT_VCENTER);
			break;    
	
		default:
		    break;    
		}
	}
	else
	{
		CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COptionPage6 property page

COptionPage6::COptionPage6() : CPropertyPage(COptionPage6::IDD)
{
	//{{AFX_DATA_INIT(COptionPage6)
	m_bExportReinit = FALSE;
	m_strExportDefName = _T("");
	m_strExportFile = _T("");
	//}}AFX_DATA_INIT
}

COptionPage6::~COptionPage6()
{
}

void COptionPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage6)
	DDX_Check(pDX, IDC_CHECK_EXPORT_REINIT, m_bExportReinit);
	DDX_Text(pDX, IDC_EDIT_EXPORT_FORMAT, m_strExportDefName);
	DDX_Text(pDX, IDC_EDIT_EXPORT_FILE, m_strExportFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage6, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage6)
	ON_BN_CLICKED(IDC_BT_CHOOSE_EXPORT, OnChooseExport)
	ON_BN_CLICKED(IDC_RADIO_CHOOSE_FILE, OnRadioChooseFile)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT_FILE, OnRadioDefaultFile)
	ON_BN_CLICKED(IDC_RADIO_FORMAT_DEFAULT, OnRadioFormatDefault)
	ON_BN_CLICKED(IDC_RADIO_FORMAT_SPECIFIC, OnRadioFormatSpecific)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COptionPage6::OnChooseExport() 
{
	CFileDialog dlgFile( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Export Definition File (*.edf)|*.edf" );
	
	CString strTitle = theApp.LoadTranslatedString( "IdsChooseExport" );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	dlgFile.m_ofn.lpstrInitialDir = ".\\";
	if( dlgFile.DoModal() == IDOK )
	{
		m_strExportDefFile = dlgFile.GetPathName();
		CExport export;
		export.Load( m_strExportDefFile );
		m_strExportDefName = export.m_strName;
		CEdit* pEdit = (CEdit*) GetDlgItem( IDC_EDIT_EXPORT_FORMAT );
		pEdit->SetWindowText( m_strExportDefName );
	}
}

BOOL COptionPage6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage6", "Item1" ));
	SetDlgItemText( IDC_RADIO_CHOOSE_FILE , theApp.Translate( "OptionPage6", "Item2" ));
	SetDlgItemText( IDC_RADIO_DEFAULT_FILE , theApp.Translate( "OptionPage6", "Item3" ));
	SetDlgItemText( IDC_CHECK_EXPORT_REINIT , theApp.Translate( "OptionPage6", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "OptionPage6", "Item5" ));
	SetDlgItemText( IDC_RADIO_FORMAT_DEFAULT , theApp.Translate( "OptionPage6", "Item6" ));
	SetDlgItemText( IDC_RADIO_FORMAT_SPECIFIC , theApp.Translate( "OptionPage6", "Item7" ));
	SetDlgItemText( IDC_BT_CHOOSE_EXPORT , theApp.Translate( "OptionPage6", "Item8" ));

	CButton* pButton;
	pButton = (CButton*) GetDlgItem( IDC_RADIO_CHOOSE_FILE );
	pButton->SetCheck( m_bChooseExportFile );
	pButton = (CButton*) GetDlgItem( IDC_RADIO_DEFAULT_FILE );
	pButton->SetCheck( !m_bChooseExportFile );

	pButton = (CButton*) GetDlgItem( IDC_RADIO_FORMAT_DEFAULT );
	pButton->SetCheck( m_bDefaultExport );
	pButton = (CButton*) GetDlgItem( IDC_RADIO_FORMAT_SPECIFIC );
	pButton->SetCheck( !m_bDefaultExport );
	UpdateFields();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPage6::UpdateFields()
{
	CButton* pButton;
	CEdit* pEdit;

	
	pButton = (CButton*) GetDlgItem( IDC_RADIO_DEFAULT_FILE );
	pEdit = (CEdit*) GetDlgItem( IDC_EDIT_EXPORT_FILE );
	pEdit->EnableWindow( pButton->GetCheck());
	
	pButton = (CButton*) GetDlgItem( IDC_RADIO_FORMAT_SPECIFIC );
	pEdit = (CEdit*) GetDlgItem( IDC_EDIT_EXPORT_FORMAT );
	pEdit->EnableWindow( pButton->GetCheck());
	CButton* pButtonChoose = (CButton*) GetDlgItem( IDC_BT_CHOOSE_EXPORT );
	pButtonChoose->EnableWindow( pButton->GetCheck());
}


void COptionPage6::OnRadioChooseFile() 
{
	UpdateFields();
	
}

void COptionPage6::OnRadioDefaultFile() 
{
	UpdateFields();
	
}

void COptionPage6::OnRadioFormatDefault() 
{
	UpdateFields();
	
}

void COptionPage6::OnRadioFormatSpecific() 
{
	UpdateFields();
	
}

void COptionPage6::OnOK() 
{
	CButton* pButton;
	pButton = (CButton*) GetDlgItem( IDC_RADIO_CHOOSE_FILE );
	m_bChooseExportFile = pButton->GetCheck();
	
	pButton = (CButton*) GetDlgItem( IDC_RADIO_FORMAT_DEFAULT );
	m_bDefaultExport = pButton->GetCheck();

	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// COptionPage7 property page

COptionPage7::COptionPage7() : CPropertyPage(COptionPage7::IDD)
{
	//{{AFX_DATA_INIT(COptionPage7)
	m_bScanM3u = FALSE;
	m_bScanPls = FALSE;
	m_bScanMp3 = FALSE;
	//}}AFX_DATA_INIT
}

COptionPage7::~COptionPage7()
{
}

void COptionPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage7)
	DDX_Check(pDX, IDC_CHECK_SCAN_M3U, m_bScanM3u);
	DDX_Check(pDX, IDC_CHECK_SCAN_PLS, m_bScanPls);
	DDX_Check(pDX, IDC_CHECK_SCAN_MP3, m_bScanMp3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage7, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COptionPage7::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage7", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage7", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "OptionPage7", "Item3" ));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// COptionPage8 property page

COptionPage8::COptionPage8() : CPropertyPage(COptionPage8::IDD)
{
	//{{AFX_DATA_INIT(COptionPage8)
	//}}AFX_DATA_INIT
}

COptionPage8::~COptionPage8()
{
}

void COptionPage8::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage8)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage8, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage8)
	ON_CBN_SELCHANGE(IDC_COMBO_PLUGINS, OnSelchangeComboPlugins)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COptionPage8::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage8", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage8", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "OptionPage8", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage8", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "OptionPage8", "Item5" ));

	CString strFilter = theApp.m_strPluginPath + "*.dll";	
	CFileFind ff;
	CStringArray filelist;
	if( ff.FindFile( strFilter ) )
	{
		BOOL bMore = TRUE;
		while( bMore )
		{
			bMore = ff.FindNextFile();
			filelist.Add( ff.GetFilePath());
		}
	}
	
	CComboBox* pCombo = (CComboBox *) GetDlgItem( IDC_COMBO_PLUGINS );

	// Ajouter le plugin None
	CPlugin* pPlugin = new CPlugin();
	int nIndexNone = pCombo->AddString( pPlugin->m_strName );
	pCombo->SetItemData( nIndexNone , (DWORD) pPlugin );
	CString strSelect = pPlugin->m_strName;
	
	CString strCurrentDll = m_strCurrentDll;
	strCurrentDll.MakeUpper();

	// Ajouter les plugins trouvés à partir de liste de fichiers
	for( int i = 0 ; i < filelist.GetSize() ; i++ )
	{
		CPlugin* pPlugin = new CPlugin();
		if( pPlugin->Load( filelist.GetAt(i) ) )
		{
			int nIndex = pCombo->AddString( pPlugin->m_strName );
			pCombo->SetItemData( nIndex , (DWORD) pPlugin );
			CString strFilename = pPlugin->m_strFilename;
			strFilename.MakeUpper();
			if( strFilename == strCurrentDll )
				strSelect = pPlugin->m_strName;
		}
		else
		{
			delete pPlugin;
		}

	}

	pCombo->SelectString( 0 , strSelect );
	OnSelchangeComboPlugins();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void COptionPage8::OnSelchangeComboPlugins() 
{
	CComboBox* pCombo = (CComboBox *) GetDlgItem( IDC_COMBO_PLUGINS );
	int nIndex = pCombo->GetCurSel();
	if( nIndex != - 1 )
	{
		CPlugin *pPlugin = (CPlugin *) pCombo->GetItemData( nIndex );
		(GetDlgItem( IDC_FILENAME ))->SetWindowText( pPlugin->m_strFilename );
		(GetDlgItem( IDC_VERSION ))->SetWindowText( pPlugin->m_strVersion );
		(GetDlgItem( IDC_CREDITS ))->SetWindowText( pPlugin->m_strCredits );

	}
	
}

void COptionPage8::OnOK() 
{
	(GetDlgItem( IDC_FILENAME ))->GetWindowText( m_strCurrentDll );
	
	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// COptionPage9 property page

COptionPage9::COptionPage9() : CPropertyPage(COptionPage9::IDD)
{
	//{{AFX_DATA_INIT(COptionPage9)
	m_bValidate = FALSE;
	m_bNormalize = FALSE;
	//}}AFX_DATA_INIT
}

COptionPage9::~COptionPage9()
{
}

void COptionPage9::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage9)
	DDX_Check(pDX, IDC_CHECK_VALIDATE, m_bValidate);
	DDX_Check(pDX, IDC_CHECK_NORMALIZE, m_bNormalize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage9, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage9)
	ON_BN_CLICKED(IDC_NOCHANGE, OnNochange)
	ON_BN_CLICKED(IDC_LOWER, OnLower)
	ON_BN_CLICKED(IDC_UPPER, OnUpper)
	ON_BN_CLICKED(IDC_PROPER, OnProper)
	ON_BN_CLICKED(IDC_CAPFIRST, OnFirstUpper)
	ON_BN_CLICKED(IDC_CHECK_UNDERSCORE, OnReplaceUnderscore)
	ON_LBN_SELCHANGE(IDC_FIELD_LIST, OnSelchangeFieldList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





void COptionPage9::OnNochange() 
{
	int nIndex = GetFieldIndex();
	if( nIndex != -1 )
	{
		m_profile.SetNorm( nIndex, NORMALIZE_NONE );
	}
	
}

void COptionPage9::OnLower() 
{
	int nIndex = GetFieldIndex();
	if( nIndex != -1 )
	{
		m_profile.SetNorm( nIndex, NORMALIZE_LOWER );
	}
	
}

void COptionPage9::OnUpper() 
{
	int nIndex = GetFieldIndex();
	if( nIndex != -1 )
	{
		m_profile.SetNorm( nIndex, NORMALIZE_UPPER );
	}
	
}

void COptionPage9::OnProper() 
{
	int nIndex = GetFieldIndex();
	if( nIndex != -1 )
	{
		m_profile.SetNorm( nIndex, NORMALIZE_PROPER );
	}
	
}

void COptionPage9::OnFirstUpper() 
{
	int nIndex = GetFieldIndex();
	if( nIndex != -1 )
	{
		m_profile.SetNorm( nIndex, NORMALIZE_CAPFIRST );
	}
	
}

void COptionPage9::OnReplaceUnderscore() 
{
	CButton* pButton = (CButton*) GetDlgItem( IDC_CHECK_UNDERSCORE );
	m_profile.SetUnderscoresReplacement( pButton->GetCheck() );
}

BOOL COptionPage9::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage9", "Item1" ));
	SetDlgItemText( IDC_CHECK_VALIDATE , theApp.Translate( "OptionPage9", "Item2" ));
	SetDlgItemText( IDC_CHECK_NORMALIZE , theApp.Translate( "OptionPage9", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage9", "Item4" ));
	SetDlgItemText( IDC_NOCHANGE , theApp.Translate( "OptionPage9", "Item5" ));
	SetDlgItemText( IDC_LOWER , theApp.Translate( "OptionPage9", "Item6" ));
	SetDlgItemText( IDC_UPPER , theApp.Translate( "OptionPage9", "Item7" ));
	SetDlgItemText( IDC_PROPER , theApp.Translate( "OptionPage9", "Item8" ));
	SetDlgItemText( IDC_CAPFIRST , theApp.Translate( "OptionPage9", "Item9" ));
	SetDlgItemText( IDC_CHECK_UNDERSCORE , theApp.Translate( "OptionPage9", "Item10" ));

	CListBox* pBox = (CListBox*) GetDlgItem( IDC_FIELD_LIST );
	for( int i = 0 ; i < NUM_NORMALIZED_FIELDS ; i++ )
	{
		CString strField;
		strField = theApp.LoadTranslatedString( g_normalized_fields[i].pstrFieldResName );
		int nIndex = pBox->AddString( strField );
		pBox->SetItemData( nIndex , g_normalized_fields[i].nFieldIndex );
	}
	pBox->SetCurSel( 0 );
	OnSelchangeFieldList();

	CButton* pButton = (CButton*) GetDlgItem( IDC_CHECK_UNDERSCORE );
	pButton->SetCheck( m_profile.GetUnderscoresReplacement() );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int COptionPage9::GetFieldIndex()
{
	int nIndex = -1;
	CListBox* pBox = (CListBox*) GetDlgItem( IDC_FIELD_LIST );
	int nCurSel = pBox->GetCurSel();
	if( nCurSel != LB_ERR )
		nIndex = pBox->GetItemData( nCurSel );
	return nIndex;
}

void COptionPage9::OnSelchangeFieldList() 
{
	int nIndex = GetFieldIndex();
	int nNorm = m_profile.GetNorm( nIndex );
	
	CButton* pButtonNone = (CButton*) GetDlgItem( IDC_NOCHANGE );
	CButton* pButtonLower = (CButton*) GetDlgItem( IDC_LOWER );
	CButton* pButtonUpper = (CButton*) GetDlgItem( IDC_UPPER );
	CButton* pButtonProper = (CButton*) GetDlgItem( IDC_PROPER );
	CButton* pButtonCapFirst = (CButton*) GetDlgItem( IDC_CAPFIRST );

	pButtonNone->SetCheck( FALSE );
	pButtonLower->SetCheck( FALSE );
	pButtonUpper->SetCheck( FALSE );
	pButtonProper->SetCheck( FALSE );
	pButtonCapFirst->SetCheck( FALSE );

	switch( nNorm )
	{
	case NORMALIZE_NONE:
		pButtonNone->SetCheck( TRUE );
		break;

	case NORMALIZE_LOWER:
		pButtonLower->SetCheck( TRUE );
		break;

	case NORMALIZE_UPPER:
		pButtonUpper->SetCheck( TRUE );
		break;

	case NORMALIZE_PROPER:
		pButtonProper->SetCheck( TRUE );
		break;

	case NORMALIZE_CAPFIRST:
		pButtonCapFirst->SetCheck( TRUE );
		break;
	}
	
}
/////////////////////////////////////////////////////////////////////////////
// COptionPage10 property page

IMPLEMENT_DYNCREATE(COptionPage10, CPropertyPage)

COptionPage10::COptionPage10() : CPropertyPage(COptionPage10::IDD)
{
	//{{AFX_DATA_INIT(COptionPage10)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionPage10::~COptionPage10()
{
}

void COptionPage10::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage10)
	DDX_Check(pDX, IDC_CHECK_WRITE_ID3V1_TAG, m_bWriteId3v1Tags);
	DDX_Check(pDX, IDC_RADIO_ID3V2, m_bUseId3v2);
	DDX_Check(pDX, IDC_RADIO_ID3V1, m_bUseId3v1Only);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage10, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage10)
	ON_BN_CLICKED(IDC_RADIO_ID3V2, OnRadioId3v2)
	ON_BN_CLICKED(IDC_RADIO_ID3V1, OnRadioId3v1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPage10 message handlers

void COptionPage10::OnRadioId3v2() 
{
	CButton* pButton = (CButton*) GetDlgItem( IDC_RADIO_ID3V1 );
	pButton->SetCheck( FALSE );

	m_bUseId3v2 = TRUE;
	UpdateUIWriteId3v1Tags();
}

void COptionPage10::OnRadioId3v1() 
{
	CButton* pButton = (CButton*) GetDlgItem( IDC_RADIO_ID3V2 );
	pButton->SetCheck( FALSE );

	m_bUseId3v2 = FALSE;
	UpdateUIWriteId3v1Tags();

	
}

BOOL COptionPage10::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	UpdateUIWriteId3v1Tags();

	SetDlgItemText( IDC_RADIO_ID3V2 , theApp.Translate( "OptionPage10", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage10", "Item2" ));
	SetDlgItemText( IDC_CHECK_WRITE_ID3V1_TAG , theApp.Translate( "OptionPage10", "Item3" ));
	SetDlgItemText( IDC_LABEL_WRITE_ID3V1_TAGS , theApp.Translate( "OptionPage10", "Item4" ));
	SetDlgItemText( IDC_RADIO_ID3V1 , theApp.Translate( "OptionPage10", "Item5" ));

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPage10::UpdateUIWriteId3v1Tags()
{
	CButton* pCheck = (CButton*) GetDlgItem( IDC_CHECK_WRITE_ID3V1_TAG );
	pCheck->EnableWindow( m_bUseId3v2 );;

	CWnd* pWndLabel = GetDlgItem( IDC_LABEL_WRITE_ID3V1_TAGS );
	pWndLabel->EnableWindow( m_bUseId3v2 );

}


/////////////////////////////////////////////////////////////////////////////
// COptionPage11 property page

COptionPage11::COptionPage11() : CPropertyPage(COptionPage11::IDD)
{
	//{{AFX_DATA_INIT(COptionPage11)
	//}}AFX_DATA_INIT
}

COptionPage11::~COptionPage11()
{
}

void COptionPage11::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPage11)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPage11, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPage11)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGES, OnSelchangeComboLanguages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COptionPage11::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemText( IDC_ITEM1 , theApp.Translate( "OptionPage11", "Item1" ));
	SetDlgItemText( IDC_ITEM2 , theApp.Translate( "OptionPage11", "Item2" ));
	SetDlgItemText( IDC_ITEM3 , theApp.Translate( "OptionPage11", "Item3" ));
	SetDlgItemText( IDC_ITEM4 , theApp.Translate( "OptionPage11", "Item4" ));
	SetDlgItemText( IDC_ITEM5 , theApp.Translate( "OptionPage11", "Item5" ));

	CString strFilter = theApp.m_strLanguagePath + "*.lng";	
	CFileFind ff;
	CStringArray filelist;
	if( ff.FindFile( strFilter ) )
	{
		BOOL bMore = TRUE;
		while( bMore )
		{
			bMore = ff.FindNextFile();
			filelist.Add( ff.GetFileName());
		}
	}
	
	CComboBox* pCombo = (CComboBox *) GetDlgItem( IDC_COMBO_LANGUAGES );

	
	CString strCurrentLanguage = m_strCurrentLanguage;
	strCurrentLanguage.MakeUpper();
	CString strSelect;

	// Add all languages into the combo
	for( int i = 0 ; i < filelist.GetSize() ; i++ )
	{
		CLanguage* pLanguage = new CLanguage();
		if( pLanguage->Load( theApp.m_strLanguagePath , filelist.GetAt(i) ) )
		{
			int nIndex = pCombo->AddString( pLanguage->m_strName );
			pCombo->SetItemData( nIndex , (DWORD) pLanguage );
			CString strFilename = pLanguage->m_strFilename;
			strFilename.MakeUpper();
			if( strFilename == strCurrentLanguage )
				strSelect = pLanguage->m_strName;
		}
		else
		{
			delete pLanguage;
		}

	}

	pCombo->SelectString( 0 , strSelect );
	OnSelchangeComboLanguages();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void COptionPage11::OnSelchangeComboLanguages() 
{
	CComboBox* pCombo = (CComboBox *) GetDlgItem( IDC_COMBO_LANGUAGES );
	int nIndex = pCombo->GetCurSel();
	if( nIndex != - 1 )
	{
		CLanguage *pLanguage = (CLanguage *) pCombo->GetItemData( nIndex );
		(GetDlgItem( IDC_FILENAME ))->SetWindowText( pLanguage->m_strFilename );
		(GetDlgItem( IDC_VERSION ))->SetWindowText( pLanguage->m_strVersion );
		(GetDlgItem( IDC_CREDITS ))->SetWindowText( pLanguage->m_strCredits );

	}
	
}

void COptionPage11::OnOK() 
{
	(GetDlgItem( IDC_FILENAME ))->GetWindowText( m_strCurrentLanguage );
	
	CPropertyPage::OnOK();
}
