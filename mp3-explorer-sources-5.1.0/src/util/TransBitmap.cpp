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
 

#include "../stdafx.h"
#include "TransBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransBitmap::CTransBitmap()
{
    m_iWidth = 0;
    m_iHeight = 0;
    m_hbmMask = NULL;
}

CTransBitmap::~CTransBitmap()
{

}

void CTransBitmap::GetMetrics()
{
    // Get the width and height.
    BITMAP bm;
    GetObject(sizeof(bm), &bm);
    m_iWidth = bm.bmWidth;
    m_iHeight = bm.bmHeight;
}

int CTransBitmap::GetWidth()
{
    if ((m_iWidth == 0) || (m_iHeight == 0))
	{
        GetMetrics();
    }
    return m_iWidth;
}

int CTransBitmap::GetHeight()
{
    if ((m_iWidth == 0) || (m_iHeight == 0))
	{
        GetMetrics();
    }
    return m_iHeight;
}

void CTransBitmap::Draw(HDC hDC, int x, int y)
{
    ASSERT(hDC);
    // Create a memory DC.
    HDC hdcMem = ::CreateCompatibleDC(hDC);
    // Select the bitmap into the mem DC.
    HBITMAP hbmold = (HBITMAP)::SelectObject(hdcMem,(HBITMAP)(m_hObject));
    // Blt the bits.
    ::BitBlt(hDC,
             x, y,
             GetWidth(), GetHeight(),
             hdcMem,
             0, 0,
             SRCCOPY);
    ::SelectObject(hdcMem, hbmold);
    ::DeleteDC(hdcMem); 
}

void CTransBitmap::CreateMask(HDC hDC)
{    
	// Nuke any existing mask.
    if (m_hbmMask) 
	{        
		::DeleteObject(m_hbmMask);    
	}

    // Create memory DCs to work with.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HDC hdcImage = ::CreateCompatibleDC(hDC);
    // Create a monochrome bitmap for the mask.
    m_hbmMask = ::CreateBitmap(GetWidth(),
                               GetHeight(),
                               1,
                               1,
                               NULL);
    // Select the mono bitmap into its DC.
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);
    // Select the image bitmap into its DC.
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);
    // Set the transparency color to be the top-left pixel.
    ::SetBkColor(hdcImage, ::GetPixel(hdcImage, 0, 0));    
	// Make the mask.
    ::BitBlt(hdcMask,
		0, 0,
		GetWidth(),
		GetHeight(),
		hdcImage,
		0, 0,
		SRCCOPY);    // Tidy up.
    ::SelectObject(hdcMask, hbmOldMask);
    ::SelectObject(hdcImage, hbmOldImage);    
	::DeleteDC(hdcMask);
    ::DeleteDC(hdcImage);
}


void CTransBitmap::DrawTrans(HDC hDC, int x, int y)
{    
	ASSERT(hDC);
    if (!m_hbmMask) 
		CreateMask(hDC);
    ASSERT(m_hbmMask);
    int dx = GetWidth();
    int dy = GetHeight();
    // Create a memory DC to which to draw.
    HDC hdcOffScr = ::CreateCompatibleDC(hDC);
    // Create a bitmap for the off-screen DC that is really
    // color-compatible with the destination DC.
    HBITMAP hbmOffScr = ::CreateBitmap(dx, dy, 
                             (BYTE)GetDeviceCaps(hDC, PLANES),
                             (BYTE)GetDeviceCaps(hDC, BITSPIXEL),
                             NULL);
    // Select the buffer bitmap into the off-screen DC.
    HBITMAP hbmOldOffScr = (HBITMAP)::SelectObject(hdcOffScr, hbmOffScr);
    // Copy the image of the destination rectangle to the
    // off-screen buffer DC, so we can play with it.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hDC, x, y, SRCCOPY);
    // Create a memory DC for the source image.
    HDC hdcImage = ::CreateCompatibleDC(hDC); 
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);
    // Create a memory DC for the mask.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);
    // XOR the image with the destination.
    ::SetBkColor(hdcOffScr, RGB( 255, 255, 255));
    ::BitBlt(hdcOffScr, 0, 0, dx, dy ,hdcImage, 0, 0, SRCINVERT);
    // AND the destination with the mask.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcMask, 0,0, SRCAND);
    // XOR the destination with the image again.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy, hdcImage, 0, 0, SRCINVERT);
    // Copy the resultant image back to the screen DC.
    ::BitBlt(hDC, x, y, dx, dy, hdcOffScr, 0, 0, SRCCOPY);    // Tidy up.
    ::SelectObject(hdcOffScr, hbmOldOffScr);
    ::SelectObject(hdcImage, hbmOldImage);
    ::SelectObject(hdcMask, hbmOldMask);
    ::DeleteObject(hbmOffScr);
    ::DeleteDC(hdcOffScr);
    ::DeleteDC(hdcImage);
    ::DeleteDC(hdcMask);
}