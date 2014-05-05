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
#include "mp3/Mp3File.h"
#include "Export.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_BUFFER_EDF 2000

#define SECT_EXPDEF			"Export parameter"
#define ENTR_EXPDEF_HEADER	"Header"
#define ENTR_EXPDEF_FOOTER	"Footer"
#define ENTR_EXPDEF_NAME	"Name"
#define ENTR_EXPDEF_RECORD	"Record"

extern CMp3ExpApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CExport, CObject)

CExport::CExport()
{
	m_strOutputFooter = "";
	m_strOutputHeader = "";
	m_strOutputRecord = "\"@FILENAME\",\"@ARTIST\",\"@TITLE\",\"@LOCATION\",\"@SIZE\",\"@ALBUM\",\"@GENRE\",\"@DATE\",\"@COMMENT\",\"@TIME\",\"@BITRATE\",\"@FREQUENCY\",\"@MODE\",\"@LAYER\"";

}

CExport::~CExport()
{

}

////////////////////////////////////////////////////////////////////
// Class CExport



BOOL CExport::CreateExportFile(const char *strFileName, CObArray *pMp3List, BOOL bReinit )
{
	
	CStdioFile file;
	UINT nFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText;;

	if( !bReinit )
	{
		nFlags |= CFile::modeNoTruncate;
	}

	try
	{
		if( file.Open( strFileName, nFlags ))
		{
			// Ecriture de l'entete
			if( m_strOutputHeader != "" )
			{
				ConvertSpecialChars( m_strOutputHeader );				
				file.WriteString( m_strOutputHeader );
			}
			// Ecriture des enregistrements
			for( int i = 0 ; i < pMp3List->GetSize() ; i++ )
			{
				CAudioFile* pMp3 = (CAudioFile*) pMp3List->GetAt(i);
				CString strOutput = pMp3->FormatExport( m_strOutputRecord );
				ConvertSpecialChars( strOutput );				
				strOutput += "\n";
				file.WriteString( strOutput );
			}
			// Ecriture de la fin de fichier
			if( m_strOutputFooter != "" )
			{
				ConvertSpecialChars( m_strOutputFooter );
				file.WriteString( m_strOutputFooter );
			}
			file.Close();
		}
	}
	catch ( CFileException *e )
	{
		e->Delete();
		TRACE( "Error exporting data !\n");
		return FALSE;
	}
	return TRUE;
}

void CExport::Load(const char *strExportDefinitionFile)
{
	static _TCHAR buffer[MAX_BUFFER_EDF];
	::GetPrivateProfileString( SECT_EXPDEF, ENTR_EXPDEF_NAME, "", buffer, sizeof( buffer), strExportDefinitionFile);
	m_strName = buffer;
	::GetPrivateProfileString( SECT_EXPDEF, ENTR_EXPDEF_HEADER, "", buffer, sizeof( buffer), strExportDefinitionFile);
	m_strOutputHeader = buffer;
	::GetPrivateProfileString( SECT_EXPDEF, ENTR_EXPDEF_RECORD, "", buffer, sizeof( buffer), strExportDefinitionFile);
	m_strOutputRecord = buffer;
	::GetPrivateProfileString( SECT_EXPDEF, ENTR_EXPDEF_FOOTER, "", buffer, sizeof( buffer), strExportDefinitionFile);
	m_strOutputFooter = buffer;
}

void CExport::ConvertSpecialChars(CString &strString)
{
	strString.Replace( "@CR" , "\n" );
	strString.Replace( "@TAB" , "\t" );
}



