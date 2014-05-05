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
 
#if !defined(AFX_HTMLFILEVIEW_H__1672F784_F2D7_11D3_BEE0_00400556855E__INCLUDED_)
#define AFX_HTMLFILEVIEW_H__1672F784_F2D7_11D3_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlFileView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlFileView html view
// Copyright (c) 1998-2000 Pierre LEVY
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>


class CHtmlFileView : public CHtmlView
{
protected:
	CHtmlFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlFileView)

// html Data
public:
	//{{AFX_DATA(CHtmlFileView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	static BOOL CreateHtmlPage(CAudioFile *pAudioFile, CString& strUrl);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlFileView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHtmlFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlFileView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLFILEVIEW_H__1672F784_F2D7_11D3_BEE0_00400556855E__INCLUDED_)
