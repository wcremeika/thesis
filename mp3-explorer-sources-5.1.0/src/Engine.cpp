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
#include "Engine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMp3ExpApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngine::CEngine()
{
	m_hLib = NULL;
	CString strNone = theApp.LoadTranslatedString( "IdsNone" );

	m_strName     = strNone;
	m_strFilename = strNone;
	m_strVersion  = strNone;
	m_hBitmap = NULL;
}

CEngine::~CEngine()
{

	if( m_hLib != NULL )
		::FreeLibrary( m_hLib );
	if( m_hBitmap != NULL )
		::DeleteObject( m_hBitmap );

}

BOOL CEngine::Load( CString strFilename )
{
	m_hLib = ::LoadLibrary( strFilename );
	if( m_hLib == NULL )
		return FALSE;

	int nPos = strFilename.ReverseFind( '\\' );
	m_strFilename = strFilename.Right( strFilename.GetLength() - nPos - 1 );

	void (WINAPI* lpfnGetEngineName)(LPSTR) ;
	lpfnGetEngineName = (void (WINAPI* )(LPSTR)) ::GetProcAddress (m_hLib, "GetEngineName");
	if( lpfnGetEngineName )
	{
		 (*lpfnGetEngineName) (m_strName.GetBuffer(MAX_BUFFER));
		 m_strName.ReleaseBuffer();
	}

	void (WINAPI* lpfnGetEngineVersion)(LPSTR) ;
	lpfnGetEngineVersion = (void (WINAPI* )(LPSTR)) ::GetProcAddress (m_hLib, "GetEngineVersion");
	if( lpfnGetEngineVersion )
	{
		 (*lpfnGetEngineVersion) (m_strVersion.GetBuffer(MAX_BUFFER));
		 m_strVersion.ReleaseBuffer();
	}

	m_hBitmap = ::LoadBitmap( m_hLib , "ENGINE_BMP" );

	/*	if( m_strVersion < RESDLL_VERSION )
		return FALSE;
*/	
	return TRUE;
}
