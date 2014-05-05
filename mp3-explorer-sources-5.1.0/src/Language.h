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
// Language.h: interface for the Language class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGE_H__511535CD_C9D3_4B64_8FD5_8291504816E1__INCLUDED_)
#define AFX_LANGUAGE_H__511535CD_C9D3_4B64_8FD5_8291504816E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLanguage  
{
public:
	CLanguage();
	virtual ~CLanguage();

	CString m_strName;
	CString m_strFilename;
	CString m_strVersion;
	CString m_strCredits;

	BOOL Load( CString strPath , CString strFilename ); 

};

#endif // !defined(AFX_LANGUAGE_H__511535CD_C9D3_4B64_8FD5_8291504816E1__INCLUDED_)
