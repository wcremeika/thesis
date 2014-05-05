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
 

#if !defined(AFX_DLGFORMAT_H__5A0985E4_D96A_11D2_BEDC_C65B082FB073__INCLUDED_)
#define AFX_DLGFORMAT_H__5A0985E4_D96A_11D2_BEDC_C65B082FB073__INCLUDED_

#include "mp3/Mp3File.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFormat dialog

class CDlgFormat : public CDialog
{
// Construction
public:
	void InitCombo( int nIdCombo, int nIndex );
	void ShowPreview();
	CFormat* m_pFormat;
	CMp3File* m_pMp3;
	CDlgFormat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormat)
	enum { IDD = IDD_DEFINE_FORMAT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormat)
	afx_msg void OnCheckUnderscores();
	afx_msg void OnSelchangeCmbField1();
	afx_msg void OnSelchangeCmbField2();
	afx_msg void OnSelchangeCmbField3();
	afx_msg void OnChangeEditSep1();
	afx_msg void OnChangeEditSep2();
	afx_msg void OnRadioMaj();
	afx_msg void OnRadioMin();
	afx_msg void OnRadioMixte();
	afx_msg void OnRadioNocasse();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFormatName();
	afx_msg void OnSelchangeCmbField4();
	afx_msg void OnChangeEditSep3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFORMAT_H__5A0985E4_D96A_11D2_BEDC_C65B082FB073__INCLUDED_)
