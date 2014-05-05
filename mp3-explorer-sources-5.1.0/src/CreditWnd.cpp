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
#include "CreditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_ID	1
#define MAX_CREDIT  101

// Alignements
#define LEFT	1
#define CENTER	2
#define RIGHT	3

// Styles
#define NORMAL		0
#define BOLD	1	

typedef struct {
	int	 nLine;
	int  nAlign;
	int	 nStyle;
	char *pText;
}
CREDIT;

static CREDIT _gcredit[MAX_CREDIT] =
{ 
	{ 1 , CENTER,	BOLD,	"[ m p 3 - e x p l o r e r ]"},
	{ 3 , CENTER,	NORMAL,	" Copyright © 1998-2005 Pierre Levy "},
	{ 1 , CENTER,	NORMAL,	" Now licensed under GNU GPL "},
	{ 2 , CENTER,	BOLD,	"_________"},
	{ 4 , CENTER,	BOLD,	"[ k e y b o a r d ]"},
	{ 2 , LEFT,	BOLD,	"Bepo"}, 
	{ 0 , RIGHT,	NORMAL,	"Pierre Levy"},
	{ 3 , CENTER,	BOLD,	"[ a r t   d i r e c t o r ]"},
	{ 2 , LEFT,	BOLD,	"Zazou"}, 
	{ 0 , RIGHT,	NORMAL,	"Isabelle Labussiere"},
	{ 3 , CENTER,	BOLD,	"[ q u a l i t y   a s s u r a n c e ]"},
	{ 2 , LEFT,	BOLD,	"Funky Nico"}, 
	{ 0 , RIGHT,	NORMAL,	"Nicolas Franck"},
	{ 1 , LEFT,	BOLD,	"Bug Cruncher"}, 
	{ 0 , RIGHT,	NORMAL,	"David Jamroga"},
	{ 1 , LEFT,	BOLD,	"Echo"}, 
	{ 0 , RIGHT,	NORMAL,	"Patricia Lange"},
	{ 1 , LEFT,	BOLD,	"The Fluffy One"}, 
	{ 0 , RIGHT,	NORMAL,	"Gordon Mckeown"},
	{ 1 , LEFT,	BOLD,	"The Tasmanian"}, 
	{ 0 , RIGHT,	NORMAL,	"David Vasseur"},
	{ 1 , LEFT,	BOLD,	"Static"}, 
	{ 0 , RIGHT,	NORMAL,	"Chris Weaver"},

	{ 3 , CENTER,	BOLD,	"[ f o r e i g n   o f f i c e s ]"},
	{ 2 , LEFT,	BOLD,	"Dodi"}, 
	{ 0 , RIGHT,	NORMAL,	"Jozsef Tamas Herczeg"},
	{ 1 , LEFT,	BOLD,	"The fifth element"}, 
	{ 0 , RIGHT,	NORMAL,	"Stefan Kleck"},
	{ 1 , LEFT,	BOLD,	"Prof"}, 
	{ 0 , RIGHT,	NORMAL,	"Juan Pablo Barrio Lera"},
	{ 1 , LEFT,	BOLD,	"Wild Turkey"}, 
	{ 0 , RIGHT,	NORMAL,	"Mustafa Yildiz"},
	{ 3 , CENTER,  BOLD,   "[ e x e c u t i v e   p r o d u c e r s ]"},
	{ 2 , LEFT,	BOLD,	"B2K"}, 
	{ 0 , RIGHT,	NORMAL,	"Cecile Becourt-Foch"},
	{ 1 , LEFT,	BOLD,	"MyAsp.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Philippe Birot"},
	{ 1 , LEFT,	BOLD,	"Ipopipo"}, 
	{ 0 , RIGHT,	NORMAL,	"Mathieu Lion"},
	{ 3 , CENTER,	BOLD,	"[ c r a s h  t e s t  d u m m i e s ]"},
	{ 2 , LEFT,	BOLD,	"Little Popeye"}, 
	{ 0 , RIGHT,	NORMAL,	"Christophe Andre"},
	{ 1 , LEFT,	BOLD,	"Poupou"}, 
	{ 0 , RIGHT,	NORMAL,	"Jean Benard"},
	{ 1 , LEFT,	BOLD,	"The Patroler"}, 
	{ 0 , RIGHT,	NORMAL,	"Herve Boulnois"},
	{ 1 , LEFT,	BOLD,	"Texas Coyote"}, 
	{ 0 , RIGHT,	NORMAL,	"Yorick Kruithof"},
	{ 1 , LEFT,	BOLD,	"Pap's"}, 
	{ 0 , RIGHT,	NORMAL,	"Jacques Levy"},
	{ 1 , LEFT,	BOLD,	"Dr Bistouri"}, 
	{ 0 , RIGHT,	NORMAL,	"Guillaume Martin"},
	{ 1 , LEFT,	BOLD,	"Silver Fox"}, 
	{ 0 , RIGHT,	NORMAL,	"Gerard Mesure"},
	{ 1 , LEFT,	BOLD,	"Los Chriquetos"}, 
	{ 0 , RIGHT,	NORMAL,	"Thierry Loiseleur"},
	{ 1 , RIGHT,	NORMAL,	"Jean-Louis Nakache"},
	{ 1 , LEFT,	BOLD,	"Krsna"}, 
	{ 0 , RIGHT,	NORMAL,	"Nila"},
	{ 1 , LEFT,	BOLD,	"S.B."}, 
	{ 0 , RIGHT,	NORMAL,	"Bertrand Peltier"},
	{ 1 , LEFT,	BOLD,	"Takware"}, 
	{ 0 , RIGHT,	NORMAL,	"Eric Ras"},
	{ 1 , LEFT,	BOLD,	"Hong Kong Star"}, 
	{ 0 , RIGHT,	NORMAL,	"Bruno Thullier"},
	{ 3 , CENTER,	BOLD,	"[ s p e c i a l   t h a n k s ]"},
	{ 2 , LEFT,	BOLD,	"Daily work"}, 
	{ 0 , RIGHT,	NORMAL,	"The Centura Dream Team"},
	{ 1 , LEFT,	BOLD,	"Music & inspiration"}, 
	{ 0 , RIGHT,	NORMAL,	"Fela Anikulapo Kuti"},
	{ 1 , LEFT,	BOLD,	"Partagas y Montecristo dealer"}, 
	{ 0 , RIGHT,	NORMAL,	"Mathieu Lion"},
	{ 1 , LEFT,	BOLD,	"www.mp3.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Marcel, Marco & all the MP3.com team"},
	{ 1 , LEFT,	BOLD,	"www.dailymp3.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Jimmy Gunawan"},
	{ 1 , LEFT,	BOLD,	"www.soundcrawler.com"}, 
	{ 0 , RIGHT,	NORMAL,	"David Phan"},
	{ 1 , LEFT,	BOLD,	"www.mp3place.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Olw & Cyberwiz"},
	{ 1 , LEFT,	BOLD,	"www.cybertropix.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Scott Jarkoff"},
	{ 1 , LEFT,	BOLD,	"www.mptrois.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Tariq Krim"},
	{ 1 , LEFT,	BOLD,	"www.zdnet.com"}, 
	{ 0 , RIGHT,	NORMAL,	"ZDnet's Software Library reviewers"},
	{ 1 , LEFT,	BOLD,	"www.mpegx.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Oscar Poernomo"},
	{ 1 , LEFT,	BOLD,	"www.audiogalaxy.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Marcin M. Manek"},
	{ 1 , LEFT,	BOLD,	"www.dmusic.com"}, 
	{ 0 , RIGHT,	NORMAL,	"Matt Miga"},
	{ 1 , LEFT,	BOLD,	"www.id3.org"}, 
	{ 0 , RIGHT,	NORMAL,	"Scott Haugh"},
	{ 3 , CENTER,	BOLD,	"[ s p e c i a l ]"},
	{ 2 , CENTER,	BOLD,	"This software is dedicated to Todd"}, 
	{ 10 , CENTER,	BOLD,	"Music is the weapon of the future"}, 
	{  2 , RIGHT,	NORMAL,	"Fela (1938-1997)"}, 
	{ 10  , CENTER,	BOLD,	"_________"},
	{ 3  , CENTER,	BOLD,	"www.mp3-explorer.com"},
	{ 1  , CENTER,	BOLD,	"contact@mp3-explorer.com"}

};

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCreditWnd

CCreditWnd::CCreditWnd()
{
	m_bFirstPaint = TRUE;
}

CCreditWnd::~CCreditWnd()
{
}


BEGIN_MESSAGE_MAP(CCreditWnd, CStatic)
	//{{AFX_MSG_MAP(CCreditWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreditWnd message handlers

void CCreditWnd::Scroll()
{
	m_nY += 1;
	Invalidate(FALSE);

}


void CCreditWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if( m_bFirstPaint )
	{
		// Set DC parameters
		m_dcMem.CreateCompatibleDC( &dc );
		m_dcMem.SelectStockObject( ANSI_VAR_FONT );
		m_dcMem.SetBkColor( theApp.m_colorCreditsBackground );
		m_dcMem.SetTextColor( theApp.m_colorCreditsText );
		m_dcMem.SetBkMode( OPAQUE ); 

		// Set Font parameters
		CFont *pFont = (CFont *) m_dcMem.SelectStockObject( ANSI_VAR_FONT );
		LOGFONT lf;
		pFont->GetLogFont( &lf );
		lf.lfWeight = FW_SEMIBOLD;
		m_fontUnderlined.CreateFontIndirect( &lf );
		m_dcMem.SelectObject( m_fontUnderlined );
		CSize size = m_dcMem.GetTextExtent( "dummy" );
		m_nLineHeight = size.cy;


		// Set Window Parameters
		RECT rc;
		GetWindowRect( &rc );

		m_nWidth = rc.right - rc.left; 
		m_nHeight = rc.bottom - rc.top;
		m_nXC = m_nWidth / 2;
		m_nXL = m_nXC - 10;
		m_nXR = m_nXC + 10;
		m_nY = 0 ;

		// Calculate DC dimensions
		m_nDcHeight = 2 * m_nHeight;
		int nLine = 0;
		for( int i = 0 ; i < MAX_CREDIT ; i++ )
			nLine += _gcredit[i].nLine;
		m_nDcHeight += nLine * m_nLineHeight;

		m_bitmap.CreateCompatibleBitmap( &dc, m_nWidth , m_nDcHeight); 


		// Draw the the mem DC
		m_dcMem.SelectObject( m_bitmap );


		CBrush brush;
		brush.CreateSolidBrush( theApp.m_colorCreditsBackground );
		HBRUSH hBrushOld = (HBRUSH) m_dcMem.SelectObject( brush );
		m_dcMem.PatBlt( 0 , 0 , m_nWidth, m_nDcHeight , PATCOPY  );
		if( hBrushOld )
			m_dcMem.SelectObject( hBrushOld );
		brush.DeleteObject();

		int nY = m_nHeight;
		for( i = 0 ; i < MAX_CREDIT ; i++ )
		{
			nY += _gcredit[i].nLine * m_nLineHeight;
			PutText( m_dcMem, _gcredit[i].pText, nY , _gcredit[i].nAlign, _gcredit[i].nStyle );
		}
		
		// Start timer for rotation
		SetTimer( TIMER_ID , 40, NULL );

		// Initialization completed
		m_bFirstPaint = FALSE;

	}

	dc.BitBlt( 0 , 0 , m_nWidth, m_nHeight, &m_dcMem, 0 , m_nY,  SRCCOPY );

	// Make it loop
	if( (m_nY - m_nDcHeight) > 0 )
	{
		m_nY = 0;
	}

}


void CCreditWnd::PutText( CDC& dc , const char *lpstrText, int nY, int nAlign, int nStyle )
{
	int nX;
	int nTextAlign;
	switch( nAlign )
	{
	case CENTER:
		nX = m_nXC;
		nTextAlign = TA_CENTER;
		break;

	case RIGHT:
		nX = m_nXR;
		nTextAlign = TA_LEFT;
		break;
	
	case LEFT:
		nX = m_nXL;
		nTextAlign = TA_RIGHT;
		break;
	}

	dc.SetTextAlign( nTextAlign );
	if( nStyle == BOLD )
	{
		dc.SelectObject( m_fontUnderlined );
	}
	else
	{
		dc.SelectStockObject( ANSI_VAR_FONT );
	}

	dc.TextOut( nX , nY, lpstrText );
		
	
	
}


void CCreditWnd::OnTimer(UINT nIDEvent) 
{
	Scroll();
	
	CStatic::OnTimer(nIDEvent);
}

void CCreditWnd::OnDestroy() 
{
	CStatic::OnDestroy();
	KillTimer( TIMER_ID );

}



BOOL CCreditWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = ::AfxRegisterWndClass( CS_BYTEALIGNWINDOW );
	return CStatic::PreCreateWindow(cs);
}
