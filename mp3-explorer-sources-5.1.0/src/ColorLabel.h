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
 
#if !defined(AFX_COLORLABEL_H__3A7E1BE4_B6EB_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_COLORLABEL_H__3A7E1BE4_B6EB_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorLabel window

class CColorLabel : public CStatic
{
// Construction
public:
	CColorLabel();

// Attributes
public:
	COLORREF          m_clrBackColor;
	COLORREF          m_clrTextBackColor;
	COLORREF          m_clrTextColor;
	HBRUSH            m_hBackBrush;
	LOGFONT           m_stLF;
	CFont             m_cFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strText;
	void SetText( LPCSTR strText );
	void SetBkColor( COLORREF color );
	void SetTextBackColor( COLORREF color );
	void SetTextColor( COLORREF color );

	virtual ~CColorLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorLabel)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLABEL_H__3A7E1BE4_B6EB_11D4_BEE0_00400556855E__INCLUDED_)
