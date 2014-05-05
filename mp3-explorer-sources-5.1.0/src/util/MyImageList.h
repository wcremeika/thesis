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
 

#if !defined(AFX_MYIMAGELIST_H__8EE12162_9135_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_MYIMAGELIST_H__8EE12162_9135_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcmn.h>

class CMyImageList : public CImageList  
{
public:
	BOOL Create( int cx, int cy, int nRes);
	CMyImageList();
	virtual ~CMyImageList();

};

#endif // !defined(AFX_MYIMAGELIST_H__8EE12162_9135_11D4_BEE0_00400556855E__INCLUDED_)
