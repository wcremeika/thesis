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
 

#if !defined(AFX_FORMAT_H__F98EBCC4_F0E3_11D3_BEE0_00400556855E__INCLUDED_)
#define AFX_FORMAT_H__F98EBCC4_F0E3_11D3_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CASSE_MIN		1
#define CASSE_MAJ		2
#define CASSE_MIXTE		3
#define CASSE_NO_MODIF	4


class CFormat : public CObject  
{
public:
	CFormat();
	virtual ~CFormat();

public:
	BOOL m_bDefault;
	void Copy( CFormat *pFormat );
	CString m_strName;
	CString SetMixedCasse( CString strInformation );
	CString ConvertCasse( CString strInformation );
	int m_nCasse;
	BOOL m_bUnderscore;
	CString m_strSeparator1;
	CString m_strSeparator2;
	CString m_strSeparator3;
	int m_nFieldIndex1;
	int m_nFieldIndex2;
	int m_nFieldIndex3;
	int m_nFieldIndex4;
	CString GetFilename( CAudioFile* pMp3 );
	BOOL Save( int nIndex );
	BOOL Load( int nIndex );

};

#endif // !defined(AFX_FORMAT_H__F98EBCC4_F0E3_11D3_BEE0_00400556855E__INCLUDED_)
