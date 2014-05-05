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
 

#ifndef __ABOUTPAGE_H__
#define __ABOUTPAGE_H__

#include "CreditWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutPage1 dialog

class CAboutPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CAboutPage1)

// Construction
public:
	CAboutPage1();
	~CAboutPage1();
	CString m_strHomepageUrl;

// Dialog Data
	//{{AFX_DATA(CAboutPage1)
	enum { IDD = IDD_ABOUTPAGE1 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAboutPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAboutPage1)
	afx_msg void OnButtonHomepage();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CAboutPage2 dialog

class CAboutPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CAboutPage2)

// Construction
public:
	CAboutPage2();
	~CAboutPage2();

// Dialog Data
	//{{AFX_DATA(CAboutPage2)
	enum { IDD = IDD_ABOUTPAGE2 };
	CCreditWnd	m_wndCredit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAboutPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAboutPage2)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};





#endif // __ABOUTPAGE_H__
