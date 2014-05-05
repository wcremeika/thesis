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
 

#if !defined(AFX_XmlFile_H__61A90D4E_AF3A_4613_A4EC_47A77E714C25__INCLUDED_)
#define AFX_XmlFile_H__61A90D4E_AF3A_4613_A4EC_47A77E714C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <afxtempl.h>

class CXmlFile : public CStdioFile  
{
private:
	static CMap<int,int,CString,CString> mapEncode;

public:
	void InitEncodeMap();
	CString Encode( CString strSource );
	static BOOL CR;
	static BOOL NOCR;

	void WriteData( LPCSTR strData , int nIndent , BOOL bCR );
	void WriteTagEnd( LPCSTR strTag,  int nIndent , BOOL bCR );
	void WriteTagBegin( LPCSTR strTag,  int nIndent , BOOL bCR );
	void WriteHeader();
	CXmlFile();
	virtual ~CXmlFile();

};

#endif // !defined(AFX_XmlFile_H__61A90D4E_AF3A_4613_A4EC_47A77E714C25__INCLUDED_)
