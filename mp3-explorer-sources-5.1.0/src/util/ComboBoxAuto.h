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
 
#if !defined(AFX_COMBOBOXAUTO_H__E8077683_5F07_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_COMBOBOXAUTO_H__E8077683_5F07_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboBoxAuto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBoxAuto window

class CComboBoxAuto : public CComboBoxEx
{
// Construction
public:
	CComboBoxAuto();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxAuto)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void FillWithList( CStringList *pList , int nImage, int nImageSelected );
	CString GetText();
	void SetText( CString strText );
	int SelectItem( CString strText );
	int AddItem( LPCSTR strItem, int nImage, int nImageSelected, int nIndex );
	int AddSortedItem( CString strItem, int nImage, int nImageSelected );
	CString m_strPrevText;
	BOOL m_bAutoComplete;
	virtual ~CComboBoxAuto();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboBoxAuto)
	afx_msg void OnEditChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBOXAUTO_H__E8077683_5F07_11D4_BEE0_00400556855E__INCLUDED_)
