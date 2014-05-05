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
 
//
// This class defines custom modal property sheet 
// CID3v2Sheet.
 
#ifndef __ID3V2SHEET_H__
#define __ID3V2SHEET_H__

#include "ID3v2Page.h"

/////////////////////////////////////////////////////////////////////////////
// CID3v2Sheet

class CID3v2Sheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CID3v2Sheet)

// Construction
public:
	CID3v2Sheet(CWnd* pWndParent = NULL);


// Attributes
public:
	CID3v2Page1 m_Page1;
	CID3v2Page2 m_Page2;
	CID3v2Page3 m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CID3v2Sheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CID3v2Sheet();


private:
	BOOL SetPageTitle (int nPage, LPTSTR pszText);

// Generated message map functions
protected:
	//{{AFX_MSG(CID3v2Sheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __ID3V2SHEET_H__
