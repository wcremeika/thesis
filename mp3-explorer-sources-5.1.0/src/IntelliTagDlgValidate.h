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
 
#if !defined(AFX_INTELLITAGDLGVALIDATE_H__609D9801_5A3E_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_INTELLITAGDLGVALIDATE_H__609D9801_5A3E_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntelliTagDlgValidate.h : header file
//
#include "util/ComboBoxAuto.h"
#include "SortedList.h"

/////////////////////////////////////////////////////////////////////////////
// CIntelliTagDlgValidate dialog

class CIntelliTagDlgValidate : public CDialog
{
// Construction
public:
	BOOL m_bMultipleFiles;
	BOOL m_bCancelAll;
	BOOL m_bYesToAll;
	CIntelliTagDlgValidate(CWnd* pParent = NULL);   // standard constructor
	CMyImageList m_imagelist;
	CComboBoxAuto m_comboArtist;
	CComboBoxAuto m_comboAlbum;
	CMapStringToOb*  m_pMapArtist;
	CSortedList*	m_pListArtist;

// Dialog Data
	//{{AFX_DATA(CIntelliTagDlgValidate)
	enum { IDD = IDD_INTELLITAG_VALIDATE };
	CString	m_strFilename;
	CString	m_strAlbum;
	CString	m_strArtist;
	CString	m_strTitle;
	CString	m_strTrack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntelliTagDlgValidate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntelliTagDlgValidate)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownCmbArtist();
	afx_msg void OnYesToAll();
	afx_msg void OnBtCancelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTELLITAGDLGVALIDATE_H__609D9801_5A3E_11D4_BEE0_00400556855E__INCLUDED_)
