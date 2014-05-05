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
// COptionSheet.
 
#ifndef __OPTIONSHEET_H__
#define __OPTIONSHEET_H__

#include "OptionPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

class COptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
	COptionSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	COptionPage1 m_Page1;
	COptionPage2 m_Page2;
	COptionPage3 m_Page3;
	COptionPage4 m_Page4;
	COptionPage5 m_Page5;
	COptionPage6 m_Page6;
	COptionPage7 m_Page7;
	COptionPage8 m_Page8;
	COptionPage9 m_Page9;
	COptionPage10 m_Page10;
	COptionPage11 m_Page11;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionSheet();

private:
	BOOL SetPageTitle (int nPage, LPTSTR pszText);


// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OPTIONSHEET_H__
