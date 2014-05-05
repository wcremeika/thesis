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
#include "Mp3Exp.h"
#include "Format.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMp3ExpApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormat::CFormat()
{
	m_strName = "";
	m_nFieldIndex1 = 0;
	m_nFieldIndex2 = 0;
	m_nFieldIndex3 = 0;
	m_nFieldIndex4 = 0;
	m_strSeparator1 = "";
	m_strSeparator2 = "";
	m_strSeparator3 = "";
	m_nCasse = CASSE_MIN;
	m_bUnderscore = FALSE;

}

CFormat::~CFormat()
{

}

////////////////////////////////////////////////////////////////////
// Class CFormat


BOOL CFormat::Load(int nIndex)
{
	static _TCHAR szSection[MAX_PATH];
	wsprintf( szSection , SECT_FORMATS , nIndex );

	m_strName = theApp.GetProfileString( szSection, ENTR_FMT_NAME, "" );
	m_nFieldIndex1 = theApp.GetProfileInt( szSection, ENTR_FMT_FIELD1, 0 );
	m_nFieldIndex2 = theApp.GetProfileInt( szSection, ENTR_FMT_FIELD2, 0 );
	m_nFieldIndex3 = theApp.GetProfileInt( szSection, ENTR_FMT_FIELD3, 0 );
	m_nFieldIndex4 = theApp.GetProfileInt( szSection, ENTR_FMT_FIELD4, 0 );
	m_strSeparator1 = theApp.GetProfileString( szSection, ENTR_FMT_SEPARATOR1, "" );
	m_strSeparator2 = theApp.GetProfileString( szSection, ENTR_FMT_SEPARATOR2, "" );
	m_strSeparator3 = theApp.GetProfileString( szSection, ENTR_FMT_SEPARATOR3, "" );
	m_bUnderscore = theApp.GetProfileInt( szSection, ENTR_FMT_UNDERSCORES, FALSE );
	m_nCasse = theApp.GetProfileInt( szSection, ENTR_FMT_CASSE, CASSE_MIN );

	return TRUE;
}

BOOL CFormat::Save(int nIndex)
{
	static _TCHAR szSection[MAX_PATH];
	wsprintf( szSection , SECT_FORMATS , nIndex );

	theApp.WriteProfileString( szSection, ENTR_FMT_NAME, m_strName );
	theApp.WriteProfileInt( szSection, ENTR_FMT_FIELD1, m_nFieldIndex1 );
	theApp.WriteProfileInt( szSection, ENTR_FMT_FIELD2, m_nFieldIndex2 );
	theApp.WriteProfileInt( szSection, ENTR_FMT_FIELD3, m_nFieldIndex3 );
	theApp.WriteProfileInt( szSection, ENTR_FMT_FIELD4, m_nFieldIndex4 );
	theApp.WriteProfileString( szSection, ENTR_FMT_SEPARATOR1, "\"" + m_strSeparator1 + "\"" );
	theApp.WriteProfileString( szSection, ENTR_FMT_SEPARATOR2, "\"" + m_strSeparator2 + "\"" );
	theApp.WriteProfileString( szSection, ENTR_FMT_SEPARATOR3, "\"" + m_strSeparator3 + "\"" );
	theApp.WriteProfileInt( szSection, ENTR_FMT_UNDERSCORES, m_bUnderscore);
	theApp.WriteProfileInt( szSection, ENTR_FMT_CASSE, m_nCasse );

	return TRUE;
}

CString CFormat::GetFilename(CAudioFile *pMp3)
{
	CString strFilename;
	if( m_nFieldIndex1 )
		strFilename = ConvertCasse( pMp3->GetInformation( m_nFieldIndex1 ));
	strFilename += m_strSeparator1;
	if( m_nFieldIndex2 )
		strFilename += ConvertCasse( pMp3->GetInformation( m_nFieldIndex2 ));
	strFilename += m_strSeparator2;
	if( m_nFieldIndex3 )
		strFilename += ConvertCasse( pMp3->GetInformation( m_nFieldIndex3 ));
	strFilename += m_strSeparator3;
	if( m_nFieldIndex4 )
		strFilename += ConvertCasse( pMp3->GetInformation( m_nFieldIndex4 ));
	if( m_bUnderscore )
		strFilename.Replace( " ", "_" );
	strFilename += pMp3->GetFileExt();
	return strFilename;
}

CString CFormat::ConvertCasse(CString strInformation)
{
	switch( m_nCasse )
	{
	case CASSE_MIN:
		strInformation.MakeLower();
		break;

	case CASSE_MAJ:
		strInformation.MakeUpper();
		break;

	case CASSE_MIXTE:
		strInformation = SetMixedCasse( strInformation );
		break;
	
	}
	return strInformation;
}

CString CFormat::SetMixedCasse(CString strInformation)
{
	CString strResult;

	int nLen = strInformation.GetLength();
	BOOL bMaj = TRUE;
	CString strChar;

	for( int i = 0 ; i < nLen ; i++ )
	{
		strChar = strInformation.Mid( i , 1 );
		if( bMaj )
			strChar.MakeUpper();
		else
			strChar.MakeLower();
		strResult += strChar;
		bMaj = ( strChar == " " );

	}
	return strResult;
}


void CFormat::Copy(CFormat *pFormat)
{
	m_strName = pFormat->m_strName;
	m_nFieldIndex1 = pFormat->m_nFieldIndex1;
	m_nFieldIndex2 = pFormat->m_nFieldIndex2;
	m_nFieldIndex3 = pFormat->m_nFieldIndex3;
	m_nFieldIndex4 = pFormat->m_nFieldIndex4;
	m_strSeparator1 = pFormat->m_strSeparator1;
	m_strSeparator2 = pFormat->m_strSeparator2;
	m_strSeparator3 = pFormat->m_strSeparator3;
	m_nCasse = pFormat->m_nCasse;
	m_bUnderscore = pFormat->m_bUnderscore;

}

