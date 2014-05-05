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
 
#if !defined(AFX_CREDITWND_H__19A58CC2_8FB8_11D2_87FD_BBCFC79D544C__INCLUDED_)
#define AFX_CREDITWND_H__19A58CC2_8FB8_11D2_87FD_BBCFC79D544C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreditWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreditWnd window
// CreditWnd.h
// Copyright (c) 1998-2000 Pierre LEVY
//

class CCreditWnd : public CStatic
{
// Construction
public:
	CCreditWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreditWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CBitmap m_bitmap;
	int m_nDcHeight;
	CDC m_dcMem;
	CFont m_fontUnderlined;
	int m_nLineHeight;
	int m_nWidth;
	int m_nHeight;
	BOOL m_bFirstPaint;
	int m_nXR;
	int m_nXL;
	int m_nXC;
	int m_nY;
	void Scroll();
	void PutText( CDC& dc , const char *lpstrText, int nLine, int nAlign, int nStyle );

	virtual ~CCreditWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCreditWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREDITWND_H__19A58CC2_8FB8_11D2_87FD_BBCFC79D544C__INCLUDED_)
