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
 

#include "../stdafx.h"
#include "ComboBoxAuto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboBoxAuto

CComboBoxAuto::CComboBoxAuto()
{
	m_bAutoComplete = TRUE;

}

CComboBoxAuto::~CComboBoxAuto()
{
}


BEGIN_MESSAGE_MAP(CComboBoxAuto, CComboBoxEx)
	//{{AFX_MSG_MAP(CComboBoxAuto)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxAuto message handlers





void CComboBoxAuto::OnEditChange() 
{

	// if we are not to auto update the text, get outta here
	if (!m_bAutoComplete) 
		return;

	CString strText, strItem;
	DWORD dwSel;
	WORD wHi, wLo;
	int iNotEqual;

	GetWindowText(strText);
	dwSel = GetEditSel();
	wHi = HIWORD(dwSel);
	wLo = LOWORD(dwSel);
//	iNotEqual = m_bCaseSensitive ?
//		m_strPrevText.Compare(strText) : m_strPrevText.CompareNoCase(strText);

	iNotEqual = m_strPrevText.CompareNoCase(strText);
	if (!strText.IsEmpty() &&
		(wHi == wLo) && ((int)wHi == strText.GetLength()) &&
		iNotEqual &&
		(m_strPrevText.GetLength() <= strText.GetLength()))
	{
		int nIndex = 0;
		BOOL bFound = FALSE;
		while( nIndex < GetCount() )
		{
			GetLBText( nIndex, strItem );
			if( strText.CompareNoCase( strItem.Left( strText.GetLength() )) == 0 )
			{
				CEdit* pEdit = GetEditCtrl();
				pEdit->SetWindowText(strItem);
				pEdit->SetSel(strText.GetLength(), -1);
				CComboBox* pCBox = GetComboBoxCtrl();
				pCBox->SetCurSel( nIndex );
				bFound = TRUE;
				break;
			}
			nIndex++;
		}
		if( !bFound )
		{
			// Clear the field to suppress images
			SetCurSel( -1 );
			CEdit* pEdit = GetEditCtrl();
			pEdit->SetWindowText(strText);
			pEdit->SetSel(strText.GetLength(), strText.GetLength());
		}
	}
	m_strPrevText = strText;
		
	m_bAutoComplete = TRUE;
  
}



int CComboBoxAuto::AddSortedItem(CString strItem, int nImage, int nImageSelected)
{
	CString strEntry;
	int nIndex = 0;


	while( nIndex < GetCount() ) 
	{
		GetLBText( nIndex , strEntry );
		if( strEntry > strItem )
			break;
		nIndex++;
	}
	return AddItem( strItem, nImage, nImageSelected, nIndex );
}

int CComboBoxAuto::AddItem(LPCSTR strItem, int nImage, int nImageSelected, int nIndex)
{
	COMBOBOXEXITEM cbeItem;
	cbeItem.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	cbeItem.iItem = nIndex;
	cbeItem.pszText = (LPSTR) strItem;
	cbeItem.cchTextMax = MAX_PATH - 1;
	cbeItem.iImage = nImage;
	cbeItem.iSelectedImage = nImageSelected;
	return InsertItem( &cbeItem );
	
}

int CComboBoxAuto::SelectItem( CString strText )
{
	int nIndex = FindStringExact( -1 , strText );
	if( nIndex != -1 )
	{
		SetCurSel( nIndex );
	}
	return nIndex;
}

void CComboBoxAuto::SetText(CString strText)
{
	CEdit* pEdit = GetEditCtrl();
	pEdit->SetWindowText( strText );

}


CString CComboBoxAuto::GetText()
{
	CString strText;
	CEdit* pEdit = GetEditCtrl();
	pEdit->GetWindowText( strText );
	return strText;

}

void CComboBoxAuto::FillWithList(CStringList *pList , int nImage, int nImageSelected )
{
	ResetContent();
	int nIndex = 0;
	POSITION pos;
	for( pos = pList->GetHeadPosition(); pos != NULL ; nIndex++)
	{
		AddItem( pList->GetNext( pos ), nImage, nImageSelected, nIndex );
	}

}
