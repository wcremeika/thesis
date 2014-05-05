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
 
 
#if !defined(AFX_DLGRENAME_H__C3DF36E4_DD7A_11D2_BEDC_AF2432458573__INCLUDED_)
#define AFX_DLGRENAME_H__C3DF36E4_DD7A_11D2_BEDC_AF2432458573__INCLUDED_

#include "mp3/Mp3File.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog

class CDlgRename : public CDialog
{
// Construction
public:
	BOOL m_bValidFormat;
	int GetMode();
	void SetMode(int nMode);
	int m_nMode;
	CAudioFile* m_pMp3;
	CFormat m_format;
	CDlgRename(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bSingleFile;
// Dialog Data
	//{{AFX_DATA(CDlgRename)
	enum { IDD = IDD_RENAME };
	CString	m_strNewFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRename)
	afx_msg void OnFormatPredefined();
	afx_msg void OnFormatFree();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCmbFormats();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRENAME_H__C3DF36E4_DD7A_11D2_BEDC_AF2432458573__INCLUDED_)
