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
#include "WebSearchResource.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebSearchResource::CWebSearchResource()
{

}

CWebSearchResource::~CWebSearchResource()
{

}


BOOL CWebSearchResource::Init(CString strInitString)
{
	int nPos;
	// Supprimer le caractère \r
	if( (nPos = strInitString.Find( '\r' )) != -1 )
		strInitString = strInitString.Left( nPos );

	// Lecture terme 1 : Type
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strType = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );

	// Lecture terme 2 : Search terms
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strSearchTerms = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );

	// Lecture terme 3 : Code
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strCode = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );

	// Lecture terme 4 : Nom
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strName = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );

	// Lecture terme 5 : Description
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strDescription = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );

	// Lecture terme 6 : URL
	nPos = strInitString.Find( ';' );
	if( nPos == -1 )
		return FALSE;
	m_strUrl = strInitString.Left( nPos );
	strInitString = strInitString.Right( strInitString.GetLength() - nPos - 1 );
	return TRUE;

}

int CWebSearchResource::GetImageIndex()
{
	if( m_strType == "1" )
		return 0;

	if( m_strType == "2" )
		return 1;

	if( m_strType == "3" )
		return 2;

	return 0;
}

BOOL CWebSearchResource::AcceptSearchTerms()
{
	if( m_strSearchTerms == "1" )
		return TRUE;
	return FALSE;

}

