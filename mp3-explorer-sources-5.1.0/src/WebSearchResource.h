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

#if !defined(AFX_WEBSEARCHRESOURCE_H__9AF7099E_64FB_41F4_A4CE_4214AE08167E__INCLUDED_)
#define AFX_WEBSEARCHRESOURCE_H__9AF7099E_64FB_41F4_A4CE_4214AE08167E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWebSearchResource  
{
private:
	CString m_strName;
	CString m_strCode;
	CString m_strDescription;
	CString m_strType;
	CString m_strUrl;

public:
	BOOL AcceptSearchTerms();
	CString m_strSearchTerms;
	int GetImageIndex();
	const char* GetDescription() { return m_strDescription; };
	const char* GetName() { return m_strName; };
	const char* GetCode() { return m_strCode; };
	const char* GetUrl() { return m_strUrl; };
	int GetNameLength() { return m_strName.GetLength(); };
	BOOL Init( CString strInitString );
	CWebSearchResource();
	virtual ~CWebSearchResource();

};

#endif // !defined(AFX_WEBSEARCHRESOURCE_H__9AF7099E_64FB_41F4_A4CE_4214AE08167E__INCLUDED_)
