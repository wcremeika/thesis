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
 

#if !defined(AFX_SPLITTER_H__9CE4B544_F47D_11D3_BEE0_00400556855E__INCLUDED_)
#define AFX_SPLITTER_H__9CE4B544_F47D_11D3_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitter frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSplitter : public CSplitterWnd
{
	DECLARE_DYNCREATE(CSplitter)

// Attributes
public:

public:
   	CSplitter();
   	
// Operations
public:
    BOOL ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size);
// Implementation
public:
	virtual ~CSplitter();

	// Generated message map functions
	//{{AFX_MSG(CSplitter)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTER_H__9CE4B544_F47D_11D3_BEE0_00400556855E__INCLUDED_)
