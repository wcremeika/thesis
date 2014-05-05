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
#include "IntelliTagParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SEP_HYPHEN '-'
#define SEP_UNDERSCORE '_'

/*
artist-title
(artist)-title
[artist]-title
artist-track-title
(artist)-track-title
[artist]-track-title
artist-album-title
(artist)-album-title
[artist]-album-title
artist-album-track-title
(artist)-album-track-title
[artist]-album-track-title
track-artist-title
artist_title
artist_track_title
artist_album_title
artist_album_track_title
track_artist_title
(artiste) titre
[artiste] titre
titre
*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntelliTagParser::CIntelliTagParser()
{

}

CIntelliTagParser::~CIntelliTagParser()
{

}

CIntelliTagParser::CIntelliTagParser(CString strFilename)
{
	m_strFilename = strFilename;

	StripExtension();

	m_nHyphenCount = GetSeparatorCount( SEP_HYPHEN );
	m_nUnderscoreCount = GetSeparatorCount( SEP_UNDERSCORE );

	if( (m_nHyphenCount > 0) && (m_nHyphenCount < 4 ) )
	{
		ExtractElements( SEP_HYPHEN );
		switch( m_nHyphenCount )
		{
		case 1:
			if( IsTrackNum(  m_strElement[0] ))			
			{
				// track - title
				m_strTrack = m_strElement[0];
			}
			else
			{
				// artist - title
				// (artist) - title
				// [artist] - title
				m_strArtist = m_strElement[0];
				StripBracesAndBrakets( m_strArtist );
			}
			m_strTitle  = m_strElement[1];
			break;
		case 2:
			if( IsTrackNum(  m_strElement[0] ))
			{
				// track - artist - title
				m_strTrack  = m_strElement[0];
				m_strArtist = m_strElement[1];
				m_strTitle  = m_strElement[2];
			}
			else if( IsTrackNum(  m_strElement[1] ))
			{
				// artist - track - title
				// (artist) - track - title
				// [artist] - track - title
				m_strArtist = m_strElement[0];
				m_strTrack  = m_strElement[1];
				m_strTitle  = m_strElement[2];
				StripBracesAndBrakets( m_strArtist );
			}
			else
			{
				// artist - album - title
				// (artist) - album - title
				// [artist] - album - title
				m_strArtist = m_strElement[0];
				m_strAlbum  = m_strElement[1];
				m_strTitle  = m_strElement[2];
				StripBracesAndBrakets( m_strArtist );
			}
			break;
		case 3:
			// artist - album - track - title
			// (artist) - album - track - title
			// [artist] - album - track - title
			if( IsTrackNum( m_strElement[2] ))
			{
				m_strArtist = m_strElement[0];
				m_strAlbum  = m_strElement[1];
				m_strTrack  = m_strElement[2];
				m_strTitle  = m_strElement[3];
				StripBracesAndBrakets( m_strArtist );
			} 
			else if( IsTrackNum( m_strElement[1] ))
			{
				m_strArtist = m_strElement[0];
				m_strTrack  = m_strElement[1];
				m_strAlbum  = m_strElement[2];
				m_strTitle  = m_strElement[3];
				StripBracesAndBrakets( m_strArtist );
			} 
			else if( IsTrackNum( m_strElement[0] ))
			{
				m_strTrack  = m_strElement[0];
				m_strArtist = m_strElement[1];
				m_strAlbum  = m_strElement[2];
				m_strTitle  = m_strElement[3];
				StripBracesAndBrakets( m_strArtist );
			} 
			else
			{
				m_strArtist = m_strElement[0];
				m_strAlbum  = m_strElement[1];
				m_strTitle  = m_strElement[2] + m_strElement[3];
				StripBracesAndBrakets( m_strArtist );
			} 
			break;

		}

	}
	else
	{
		if( (m_nUnderscoreCount > 0) && (m_nUnderscoreCount < 4 ) )
		{
			ExtractElements( SEP_UNDERSCORE );
			switch( m_nHyphenCount )
			{
			case 1:
				if( IsTrackNum(  m_strElement[0] ))			
				{
					// track_title
					m_strTrack = m_strElement[0];
				}
				else
				{
					// artist_title
					m_strArtist = m_strElement[0];
				}
				m_strTitle  = m_strElement[1];
				break;
			case 2:
				if( IsTrackNum(  m_strElement[0] ))
				{
					// track_artist_title
					m_strTrack  = m_strElement[0];
					m_strArtist = m_strElement[1];
					m_strTitle  = m_strElement[2];
				}
				else if( IsTrackNum(  m_strElement[1] ))
				{
					// artist_track_title
					m_strArtist = m_strElement[0];
					m_strTrack  = m_strElement[1];
					m_strTitle  = m_strElement[2];
				}
				else
				{
					// artist_album_title
					m_strArtist = m_strElement[0];
					m_strAlbum  = m_strElement[1];
					m_strTitle  = m_strElement[2];
				}
				break;
			case 3:
				// artist_album_track_title
				if( IsTrackNum( m_strElement[2] ))
				{
					m_strArtist = m_strElement[0];
					m_strAlbum  = m_strElement[1];
					m_strTrack  = m_strElement[2];
					m_strTitle  = m_strElement[3];
				} 
				else if( IsTrackNum( m_strElement[1] ))
				{
					m_strArtist = m_strElement[0];
					m_strTrack  = m_strElement[1];
					m_strAlbum  = m_strElement[2];
					m_strTitle  = m_strElement[3];
				} 
				else
				{
					m_strArtist = m_strElement[0];
					m_strAlbum  = m_strElement[1];
					m_strTitle  = m_strElement[2] + m_strElement[3];
				} 
				break;

			}
		}
		else
		{
			if( ExtractElements( '(' , ')' ) )
			{
				// (artist) title
				m_strArtist = m_strElement[0];
				m_strTitle  = m_strElement[1];
			} 
			else if( ExtractElements( '[' , ']' ) )
			{
				// [artist] title
				m_strArtist = m_strElement[0];
				m_strTitle  = m_strElement[1];
			} 
			else
			{
				// title
				m_strTitle = m_strFilename;
			}
		}

	}
	
}

int CIntelliTagParser::GetSeparatorCount( TCHAR ch )
{
	int nCount = 0;
	int nPos = 0;

	while( (nPos = m_strFilename.Find( ch , nPos )) != -1 )
	{
		nCount++;
		nPos++;
	}
	return nCount;

}


int CIntelliTagParser::ExtractElements( TCHAR ch )
{
	int nIndex = 0;
	int nStart = 0;
	int nEnd;

	while( (nEnd = m_strFilename.Find( ch , nStart )) != -1 )
	{
		m_strElement[nIndex] = m_strFilename.Mid( nStart , nEnd - nStart );
		m_strElement[nIndex].TrimLeft();
		m_strElement[nIndex].TrimRight();

		nIndex++;
		nStart = nEnd + 1;
	}
	m_strElement[nIndex] = m_strFilename.Mid( nStart );
	m_strElement[nIndex].TrimLeft();
	m_strElement[nIndex].TrimRight();

	return nIndex;
}

BOOL CIntelliTagParser::ExtractElements( TCHAR chStart , TCHAR chEnd )
{
	if( m_strFilename.Find( chStart ) != 0 )
		return FALSE;

	int nEnd = m_strFilename.Find( chEnd );
	if( nEnd == -1 )
		return FALSE;

	m_strElement[0] = m_strFilename.Mid( 1 , nEnd - 1 );
	m_strElement[0].TrimLeft();
	m_strElement[0].TrimRight();
	m_strElement[1] = m_strFilename.Mid( nEnd + 1 );
	m_strElement[1].TrimLeft();
	m_strElement[1].TrimRight();
	return TRUE;;
}




void CIntelliTagParser::StripExtension()
{
	int nPos = m_strFilename.ReverseFind( '.' );
	if( nPos != -1 )
	{
		m_strFilename = m_strFilename.Left( nPos );
	}
}



BOOL CIntelliTagParser::IsTrackNum(CString strElement)
{
	if( strElement.GetLength() != 2 )
		return FALSE;
	if( (strElement[0] >= '0') &&  (strElement[0] <= '9') && 
		(strElement[1] >= '0') &&  (strElement[1] <= '9') )
		return TRUE;
	return FALSE;
}

void CIntelliTagParser::StripBracesAndBrakets(CString &strSource)
{

	int nLen = strSource.GetLength();
	if( ( strSource.Find('(') == 0 ) && ( strSource.Find(')') == (nLen - 1)))
	{
		strSource = strSource.Mid( 1 , nLen - 2 );
		strSource.TrimLeft();
		strSource.TrimRight();
	}
	if( ( strSource.Find('[') == 0 ) && ( strSource.Find(']') == (nLen - 1)))
	{
		strSource = strSource.Mid( 1 , nLen - 2 );
		strSource.TrimLeft();
		strSource.TrimRight();
	}



}

