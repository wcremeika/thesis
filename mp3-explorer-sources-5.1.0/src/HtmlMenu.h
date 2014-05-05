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
 

#if !defined(AFX_HTMLMENU_H__6900D0E3_F664_11D3_BEE0_00400556855E__INCLUDED_)
#define AFX_HTMLMENU_H__6900D0E3_F664_11D3_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlMenu form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "util/ComboBoxAuto.h"
#include "ColorLabel.h"

class CEngine;
class CHyperLink;

class CHtmlMenu : public CFormView
{
protected:
	CHtmlMenu();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlMenu)

private:
	CBrush m_brushBackground;

// Form Data
public:
	//{{AFX_DATA(CHtmlMenu)
	enum { IDD = IDD_HTML_MENU };
	CHyperLink	m_linkFileView;
	CHyperLink	m_linkHelp;
	CHyperLink	m_linkReadme;
	CHyperLink	m_linkWebSite;
	CComboBoxAuto	m_comboWebSearchResources;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL m_bOnLine;

	CMyImageList m_imagelistComboWebSearchResources;
	CColorLabel m_labelDescription;

	CObArray m_listWebSearchResources;
	CString m_strWebSearchResource;

	void FillWebSearchResourcesCombo();
	void ResetWebSearchResources();
	int GetWebSearchResources( CObArray& listWebSearchResources, CString strList );
	CString GetWebSearchResourcesString();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlMenu)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHtmlMenu();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlMenu)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSelchangeComboWebSearchResources();
	afx_msg void OnSearch();
	afx_msg void OnDestroy();
	afx_msg void OnSetfocusWebSearchResourceDescription();
	afx_msg void OnDropdownComboexWebSearchResources();
	afx_msg LRESULT OnLink(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLMENU_H__6900D0E3_F664_11D3_BEE0_00400556855E__INCLUDED_)
