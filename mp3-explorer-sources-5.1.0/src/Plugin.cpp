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
#include "Plugin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMp3ExpApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlugin::CPlugin()
{
	CString strNone = theApp.LoadTranslatedString( "IdsNone" );

	m_strName     = strNone;
	m_strFilename = strNone;
	m_strVersion  = strNone;
	m_strCredits  = strNone;

}

CPlugin::~CPlugin()
{

}

BOOL CPlugin::Load( CString strFilename )
{
	HINSTANCE hInstDLL = ::LoadLibrary( strFilename );
	if( hInstDLL == NULL )
		return FALSE;

	int nPos = strFilename.ReverseFind( '\\' );
	m_strFilename = strFilename.Right( strFilename.GetLength() - nPos - 1 );
	::LoadString( hInstDLL, IDS_PLUGIN_NAME , m_strName.GetBuffer( MAX_BUFFER ), MAX_BUFFER );
	m_strName.ReleaseBuffer();
	::LoadString( hInstDLL, IDS_PLUGIN_VERSION , m_strVersion.GetBuffer( MAX_BUFFER ), MAX_BUFFER );
	m_strVersion.ReleaseBuffer();
	::LoadString( hInstDLL, IDS_PLUGIN_CREDITS , m_strCredits.GetBuffer( MAX_BUFFER ), MAX_BUFFER );
	m_strCredits.ReleaseBuffer();
	if( m_strVersion < RESDLL_VERSION )
		return FALSE;

	return TRUE;
}
