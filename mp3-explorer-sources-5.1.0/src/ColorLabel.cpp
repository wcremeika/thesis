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
 

#include "stdafx.h"
#include "mp3exp.h"
#include "ColorLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorLabel

CColorLabel::CColorLabel()
{
	m_clrBackColor                    = ::GetSysColor(COLOR_3DFACE);
	m_clrTextBackColor                = ::GetSysColor(COLOR_3DFACE);
	m_clrTextColor                    = ::GetSysColor(COLOR_WINDOWTEXT);

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_stLF), &m_stLF);
	m_cFont.CreateFontIndirect(&m_stLF);
}

CColorLabel::~CColorLabel()
{
  m_cFont.DeleteObject();

  DeleteObject(m_hBackBrush);
}


BEGIN_MESSAGE_MAP(CColorLabel, CStatic)
	//{{AFX_MSG_MAP(CColorLabel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorLabel message handlers

void CColorLabel::SetBkColor( COLORREF color )
{
	m_clrBackColor = color;
	DeleteObject(m_hBackBrush);
	m_hBackBrush = CreateSolidBrush(m_clrBackColor);

	RedrawWindow();
}

void CColorLabel::SetTextBackColor( COLORREF color )
{
	m_clrTextBackColor = color;
	RedrawWindow();
}

void CColorLabel::SetTextColor( COLORREF color )
{
	m_clrTextColor = color;
	RedrawWindow();
}

void CColorLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect    rectClient;
	GetClientRect(&rectClient);

	// Set current font
	CFont *pOldFont = dc.SelectObject(&m_cFont);

	// Set Back Mode
	int iOldMode = dc.SetBkMode(TRANSPARENT);

	// Erase background
	CBrush cBrush;
	cBrush.CreateSolidBrush(m_clrBackColor);
	dc.FillRect(rectClient, &cBrush);
	cBrush.DeleteObject();

	dc.SetBkColor( m_clrBackColor );
	dc.SetTextColor( m_clrTextColor );

	dc.DrawText( m_strText, rectClient, DT_TOP | DT_LEFT | DT_WORDBREAK );

	// Restore
	dc.SelectObject(pOldFont);
	dc.SetBkMode(iOldMode);
	
	// Do not call CStatic::OnPaint() for painting messages
}

HBRUSH CColorLabel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return NULL;
}

void CColorLabel::SetText(LPCSTR strText)
{
	m_strText = strText;
	Invalidate();
	UpdateWindow();

}
