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
#include "MyImageList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyImageList::CMyImageList()
{

}

CMyImageList::~CMyImageList()
{

}

BOOL CMyImageList::Create(int cx, int cy, int nRes)
{

	if( !CImageList::Create( cx, cy, ILC_COLOR24 | ILC_MASK, 0, 16))
		return FALSE;

	// Load 256 colour bitmap containing the images...
	HBITMAP hBitmap(
	HBITMAP(
		::LoadImage(
			::AfxGetResourceHandle(),
			MAKEINTRESOURCE( nRes ),
			IMAGE_BITMAP,
			0, 0,
			LR_COLOR)
			)
		);
	_ASSERTE(hBitmap);

	if (hBitmap)
	{
		// Plonk bitmap into image list...
		ImageList_AddMasked( this->m_hImageList, hBitmap, RGB(255, 255, 255));       
		// Destroy bitmap
		DeleteObject(hBitmap);
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}
