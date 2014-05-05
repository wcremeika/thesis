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
 

#if !defined(AFX_TRANSBITMAP_H__4E12F582_966E_11D2_87FD_ABF5CBA4FC4B__INCLUDED_)
#define AFX_TRANSBITMAP_H__4E12F582_966E_11D2_87FD_ABF5CBA4FC4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTransBitmap : public CBitmap  
{
public:
	CTransBitmap();
	virtual ~CTransBitmap();
    void Draw(HDC hDC, int x, int y);
    void Draw(CDC* pDC, int x, int y);
    void DrawTrans(HDC hDC, int x, int y);
    void DrawTrans(CDC* pDC, int x, int y);
    int GetWidth();
    int GetHeight();
private:
    int m_iWidth;
    int m_iHeight;
    HBITMAP m_hbmMask;    // Handle to mask bitmap
    void GetMetrics();
    void CreateMask(HDC hDC);
};

#endif // !defined(AFX_TRANSBITMAP_H__4E12F582_966E_11D2_87FD_ABF5CBA4FC4B__INCLUDED_)
