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
 
#include "../stdafx.h"

#include "stdio.h"
#include "mp3file.h"
#include "mpegheader.h"


#define MAX_TAG_SIZE 128
#define MAX_ERROR 1024


#define IMAGE_MIMETYPE "image/bmp"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef interface IWMAudioDRMCallback IWMAudioDRMCallback;


CString CAudioFile::strUndefined = _T("<Undefined>");
CString CAudioFile::strNotAvailable = _T("<Not available>");
CString CAudioFile::strSizeFormat = _T("%5.1f MB");
CString CAudioFile::strProtected = _T("<Protected>");
CString CAudioFile::strBitrateFormat = _T("%d kbit/s");
CString CAudioFile::strDebugLogFile;

BOOL CAudioFile::bWriteId3v1Tag = FALSE;
BOOL CAudioFile::bReadId3v2Tag = TRUE;
BOOL CAudioFile::bWriteId3v2Tag = TRUE;
BOOL CAudioFile::bDebug = FALSE;

IMPLEMENT_DYNCREATE(CMp3File, CObject)



///////////////////////////////////////////////////////////////////////
// Fichiers MP3

CMp3File::CMp3File()
{
	m_bTag = FALSE;
	m_nFileType = FILE_TYPE_MP3;
	m_nTagSize = 0;

}

CMp3File::CMp3File( CAudioFile *pAudioFile )
{
	CMp3File();
	CAudioFile::CAudioFile( pAudioFile );

}


CMp3File::~CMp3File()
{
}


CAudioFile* CMp3File::GetCopy()
{
	CAudioFile* pAudioFile = new CMp3File();
	pAudioFile->Copy( this );
	return pAudioFile;
}


void CMp3File::Serialize(CArchive& ar , CString strPath)
{
	CAudioFile::Serialize( ar , strPath );
}


CString CMp3File::GetFileExt() 
{ 
	return CString(".mp3");
}


BOOL CMp3File::ExtractField(CString& strField, char *tag, int nOffset, int nLength)
{
	static TCHAR buffer[MAX_TAG_SIZE];

	strncpy( buffer, tag + nOffset, nLength );
	buffer[nLength] = 0;
	strField = buffer;
	strField.TrimRight();
	return TRUE;

}


BOOL CMp3File::GetTagField( ID3_Tag& tag, CString& strField, ID3_FrameID nFrameID, ID3_FieldID nFieldID, int nBufferSize )
{
	ID3_Frame *frame;

	if( frame = tag.Find( nFrameID ))
	{
		frame->Field( nFieldID ).Get( strField.GetBuffer( nBufferSize ) , nBufferSize ); 
		strField.ReleaseBuffer();
		return TRUE;
	}
	return FALSE;
}

BOOL CMp3File::GetTagNumField(ID3_Tag &tag, int& nValue, ID3_FrameID nFrameID, ID3_FieldID nFieldID)
{
	ID3_Frame *frame;

	if( frame = tag.Find( nFrameID ))
	{
		nValue = frame->Field( nFieldID ).Get(); 
		return TRUE;
	}
	return FALSE;
}


BOOL CMp3File::SetTagField( ID3_Tag& tag, const char* strField, ID3_FrameID nFrameID, ID3_FieldID nFieldID )
{
	ID3_Frame *frame;

	if( frame = tag.Find( nFrameID ))
	{
		frame->Field( nFieldID ).Set( strField ); 
	}
	else
	{
		frame = new ID3_Frame();
		frame->SetID( nFrameID );
		frame->Field( nFieldID ).Set( strField ); 
		tag.AddFrame( frame , true );
	}
	return TRUE;
}


BOOL CMp3File::SetTagNumField(ID3_Tag &tag, int nValue, ID3_FrameID nFrameID, ID3_FieldID nFieldID)
{
	ID3_Frame *frame;

	if( frame = tag.Find( nFrameID ))
	{
		frame->Field( nFieldID ).Set( nValue ); 
	}
	else
	{
		frame = new ID3_Frame();
		frame->SetID( nFrameID );
		frame->Field( nFieldID ).Set( nValue ); 
		tag.AddFrame( frame , true );
	}
	return TRUE;
}



/*
BOOL CMp3File::ReadTag(CString strAltLocation)
{
	ID3_Tag tag;
	
	flags_t fTagType = ID3TT_ID3V1;

	if( CMp3File::bReadId3v2Tag )
	{
		fTagType |= ID3TT_ID3V2;
	}


	if( tag.Link( GetFullPath() , fTagType ))
	{
		

		m_nSize = tag.GetFileSize() / 1000L;
		m_strSize = FormatSize( m_nSize );
		GetTagField( tag , m_strTitle,  ID3FID_TITLE );
		GetTagField( tag , m_strArtist, ID3FID_LEADARTIST );
		GetTagField( tag , m_strAlbum,  ID3FID_ALBUM );
		GetTagField( tag , m_strYear,   ID3FID_YEAR );
		GetTagField( tag , m_strComment,ID3FID_COMMENT );

		CString strGenreId;
		GetTagField( tag , strGenreId,  ID3FID_CONTENTTYPE );
		if( (strGenreId.GetLength() > 0 ) && (strGenreId[0] == '(') )
		{
			int nGenre;
			sscanf( strGenreId, "(%d)", &nGenre );
			m_strGenre = GetGenreFromIndex( nGenre );
		}
		else
		{
			m_strGenre = ( strGenreId != "" ) ? strGenreId : CAudioFile::strUndefined;
		}

		GetTagField( tag , m_strTrackNum,	ID3FID_TRACKNUM );
		int nRating = 0;
		GetTagNumField( tag , nRating ,	ID3FID_POPULARIMETER, ID3FN_RATING );
		m_nRating = nRating / 25;
		GetTagField( tag , m_strArtistUrl,	ID3FID_WWWARTIST, ID3FN_URL );

		m_bHasLyrics = ( tag.Find( ID3FID_UNSYNCEDLYRICS ) != NULL );
		m_bHasPicture = ( tag.Find( ID3FID_PICTURE ) != NULL );

		m_nTagSize = tag.GetPrependedBytes();

	}
	else
	{
		TRACE( "Error reading file tag !\n");
//		TRACE( e->GetErrorDesc());

		m_strTitle = CAudioFile::strNotAvailable;
		m_strArtist = CAudioFile::strNotAvailable;
		m_strAlbum = CAudioFile::strNotAvailable;
		m_strYear = CAudioFile::strNotAvailable;
		m_strComment = CAudioFile::strNotAvailable;
		m_strGenre = CAudioFile::strNotAvailable;

		m_bTag = FALSE;
		m_strLayer = CAudioFile::strNotAvailable;
		m_strMode = CAudioFile::strNotAvailable;
		m_strBitrate = CAudioFile::strNotAvailable;
		m_nBitrate = -1;
		m_strFrequency = CAudioFile::strNotAvailable;
		m_nSize = 0;
		m_strSize = FormatSize( m_nSize );

		m_strAltLocation = strAltLocation;

		return FALSE;
	}

	return TRUE;
}
*/


BOOL CMp3File::ReadTag(CString strAltLocation)
{
	if( CAudioFile::bReadId3v2Tag )
	{
		return ReadId3v2Tag( strAltLocation );
	}
	else
	{
		return ReadId3v1Tag( strAltLocation );
	}
	return FALSE;
}



BOOL CMp3File::ReadId3v1Tag(CString strAltLocation)
{
	CFile file;

	try 
	{
		if( file.Open( m_strLocation + "\\" + m_strName , CFile::modeRead | CFile::typeBinary ))
		{
			static TCHAR buffer[129];
			LONG nOffset = file.Seek( -128L , CFile::end );
			file.Read( &buffer , sizeof( buffer) );
			file.Close();

			m_nSize = nOffset / 1000L;
	
			buffer[128] = '\0';

			if( (buffer[0] == 'T') && (buffer[1] == 'A') && (buffer[2] == 'G'))
			{
				m_bTag = TRUE;
				ExtractField( m_strTitle,  buffer , 3 , 30 );
				ExtractField( m_strArtist, buffer , 33 , 30 );
				ExtractField( m_strAlbum,  buffer , 63 , 30 );
				ExtractField( m_strYear,   buffer , 93 , 4 );
				if( buffer[125] == '\0' )
				{
					ExtractField( m_strComment,buffer , 97 , 28 );
					int nTrack = (int) buffer[126];
					m_strTrackNum.Format( "%02d", nTrack );
				}
				else
				{
					ExtractField( m_strComment,buffer , 97 , 30 );

				}
				int nGenre = (int) buffer[127];
				m_strGenre = GetGenreFromIndex( nGenre );

			}
		}
		else
		{
			CString strNonAvailable;
			m_strTitle = CAudioFile::strNotAvailable;
			m_strArtist = CAudioFile::strNotAvailable;
			m_strAlbum = CAudioFile::strNotAvailable;
			m_strYear = CAudioFile::strNotAvailable;
			m_strComment = CAudioFile::strNotAvailable;
			m_strGenre = CAudioFile::strNotAvailable;


			m_bTag = FALSE;
			m_strLayer = CAudioFile::strNotAvailable;
			m_strMode = CAudioFile::strNotAvailable;
			m_strBitrate = CAudioFile::strNotAvailable;
			m_nBitrate = -1;
			m_strFrequency = CAudioFile::strNotAvailable;
			m_nSize = 0;

			m_strAltLocation = strAltLocation;
			return FALSE;	

		}

	}
	catch ( CException *e )
	{
		TRACE( "Error reading ID3v1 tag !\n");
		HandleException( e , "Error reading ID3v1 tag" );
		return FALSE;
	}
	return TRUE;
}


BOOL CMp3File::ReadId3v2Tag(CString strAltLocation)
{
	ID3_Tag tag;

	try 
	{
		if( tag.Link( GetFullPath() , LINK_READONLY ) != LINK_ERROR )
		{
			m_nSize = tag.GetFileSize() / 1000L;
			GetTagField( tag , m_strTitle,  ID3FID_TITLE );
			GetTagField( tag , m_strArtist, ID3FID_LEADARTIST );
			GetTagField( tag , m_strAlbum,  ID3FID_ALBUM );
			GetTagField( tag , m_strYear,   ID3FID_YEAR );
			GetTagField( tag , m_strComment,ID3FID_COMMENT );

			CString strGenreId;
			GetTagField( tag , strGenreId,  ID3FID_CONTENTTYPE );
			if( (strGenreId.GetLength() > 0 ) && (strGenreId[0] == '(') )
			{
				int nGenre;
				sscanf( strGenreId, "(%d)", &nGenre );
				m_strGenre = GetGenreFromIndex( nGenre );
			}
			else
			{
				m_strGenre = ( strGenreId != "" ) ? strGenreId : CAudioFile::strUndefined;
			}

			GetTagField( tag , m_strTrackNum,	ID3FID_TRACKNUM );
			int nRating = 0;
			GetTagNumField( tag , nRating ,	ID3FID_POPULARIMETER, ID3FN_RATING );
			m_nRating = nRating / 25;
			GetTagField( tag , m_strArtistUrl,	ID3FID_WWWARTIST, ID3FN_URL );

			m_bHasLyrics = ( tag.Find( ID3FID_UNSYNCEDLYRICS ) != NULL );
			m_bHasPicture = ( tag.Find( ID3FID_PICTURE ) != NULL );

			m_nTagSize = tag.m_nOldTagSize;
			
		}
		else
		{
			m_strTitle = CAudioFile::strNotAvailable;
			m_strArtist = CAudioFile::strNotAvailable;
			m_strAlbum = CAudioFile::strNotAvailable;
			m_strYear = CAudioFile::strNotAvailable;
			m_strComment = CAudioFile::strNotAvailable;
			m_strGenre = CAudioFile::strNotAvailable;

			m_bTag = FALSE;
			m_strLayer = CAudioFile::strNotAvailable;
			m_strMode = CAudioFile::strNotAvailable;
			m_strBitrate = CAudioFile::strNotAvailable;
			m_nBitrate = -1;
			m_strFrequency = CAudioFile::strNotAvailable;
			m_nSize = 0;

			m_strAltLocation = strAltLocation;
			return FALSE;

		}

	}
	
	catch ( CException *e )
	{
		TRACE( "Error reading ID3v2 tag !\n");
		HandleException( e , "Error reading ID3v2 tag" );
		return FALSE;
	}
	return TRUE;
}


BOOL CMp3File::ReadMpegHeader()
{
	try 
	{
		CBitStream stream(m_strLocation + "\\" + m_strName);
		CMpegHeader hdr;
		Crc16 *crc;

		UINT nVbrFrameCount;
		if( hdr.ReadHeader( &stream, &crc, nVbrFrameCount, m_nTagSize))
		{
			m_strLayer = hdr.GetVersion();
			m_strLayer += " Layer ";
			m_strLayer += hdr.GetLayer();
			m_strMode = hdr.GetMode();
			m_strFrequency = hdr.GetSampleFrequency();
			if( hdr.IsVBR() )
			{
				m_nTime = hdr.GetVbrDuration();
				if( m_nTime !=0 )
				{
					m_nBitrate = ( m_nSize * 1000 * 8 ) / m_nTime;
					m_strBitrate.Format( "%d  kbit/s VBR" , m_nBitrate / 1000 );
				}
				else
				{
					m_nBitrate = 99000; // dummy bitrate
					m_strBitrate.Format( "%d  kbit/s VBR" , m_nBitrate / 1000 );
				}
			}
			else
			{
				m_strBitrate = hdr.GetBitrate();
				m_nBitrate = hdr.GetBitrateValue();
				if( m_nBitrate != 0 )
				{
					m_nTime = ( m_nSize * 1000 * 8 ) / m_nBitrate;
				}
				else
				{
					m_nTime = 0; //dummy duration
				}
			}
		}
		else
		{
			TRACE( "Error reading file header " + m_strLocation + "\\" + m_strName +" !\n");
		}

	}
	catch ( CFileException *e )
	{
		TRACE( "Error reading file's MPEG header !\n");
		HandleException( e , "Error reading file's MPEG header" );
		return FALSE;
	}
	return TRUE;

}


BOOL CMp3File::ReadFileInfos( CString strLocation, CString strFileName, CString strAltLocation )
{
	// Informations concernant le fichier
	m_strName = strFileName;
	m_strLocation = strLocation;

	// Si il s'agit d'un fichier distant ne pas essayer de lire les infos
	if( strLocation.Find( "http://" ) == 0 )
		return TRUE;

	BuildFullUpperPath();


	if( !ReadTag( strAltLocation ) ) return FALSE;
	if( !ReadMpegHeader() ) return FALSE;
	return TRUE;
}

/*
BOOL CMp3File::WriteFileInfos( CTag *tagNew )
{

	CString strTitle =  ( tagNew->GetTitle() != "" ) ? tagNew->GetTitle(): m_strTitle ;
	CString strArtist = ( tagNew->GetArtist() != "" ) ? tagNew->GetArtist() : m_strArtist ;
	CString strAlbum =  ( tagNew->GetAlbum() != "" ) ? tagNew->GetAlbum() : m_strAlbum ;
	CString strYear =  ( tagNew->GetYear() != "" ) ? tagNew->GetYear() : m_strYear ;
	CString strComment =  ( tagNew->GetComment() != "" ) ? tagNew->GetComment() : m_strComment;
	CString strGenre = ( tagNew->GetGenre() != "" ) ? tagNew->GetGenre() : m_strGenre;
	CString strTrackNum =  ( tagNew->GetTrackNum() != "" ) ? tagNew->GetTrackNum() : m_strTrackNum;
	int nRating = tagNew->GetRating();
	CString strArtistUrl =  ( tagNew->GetArtistUrl() != "" ) ? tagNew->GetArtistUrl() : m_strArtistUrl;

	// strLyrics != "" que si les paroles doivent être réécrites
	CString strLyrics = tagNew->GetLyrics();
	BOOL bHasLyrics = tagNew->HasLyrics();

	// strPicture != "" que si l'image doit être réécrite
	CString strPictureFile = tagNew->GetPicture();
	BOOL bHasPicture = tagNew->HasPicture();


	flags_t fTagType = 0;

	// Write ID3 v1 tag
	if( CAudioFile::bWriteId3v1Tag )
	{
		fTagType |= ID3TT_ID3V1;
	}
	// Write ID3 v2
	if( CAudioFile::bWriteId3v2Tag )
	{
		fTagType |= ID3TT_ID3V2;
	}

	ID3_Tag tag;
	
	try 
	{
		tag.Link( GetFullPath(),  fTagType );
	
		SetTagField( tag, strTitle,  ID3FID_TITLE );		
		SetTagField( tag, strArtist, ID3FID_LEADARTIST );		
		SetTagField( tag, strAlbum,  ID3FID_ALBUM );		
		SetTagField( tag, strYear,	 ID3FID_YEAR );		
		SetTagField( tag, strComment,  ID3FID_COMMENT );
		
		SetTagField( tag , strGenre,  ID3FID_CONTENTTYPE );
		SetTagField( tag , strTrackNum,  ID3FID_TRACKNUM );
		SetTagNumField( tag , nRating * 25,  ID3FID_POPULARIMETER, ID3FN_RATING);
		SetTagField( tag , strArtistUrl,  ID3FID_WWWARTIST, ID3FN_URL );
		
		if( strLyrics != "" )
			SetTagField( tag, strLyrics,  ID3FID_UNSYNCEDLYRICS );		
		if( strPictureFile != "" )
			SetPicture( tag , strPictureFile );
		tag.Update();
	}
	catch( CException* e ) 
	{
		TRACE( "Error writing ID3 tag" );
		e->Delete();
		return FALSE;
	}

	m_strTitle = strTitle;
	m_strArtist = strArtist;
	m_strAlbum = strAlbum;
	m_strYear = strYear;
	m_strComment = strComment;
	m_strGenre = strGenre;
	m_strTrackNum = strTrackNum;
	m_nRating = nRating;
	m_strArtistUrl = strArtistUrl;
	m_bHasLyrics = bHasLyrics;
	m_bHasPicture = bHasPicture;

	return TRUE;

}
*/

BOOL CMp3File::WriteFileInfos( CTag *tagNew )
{

	CString strTitle =  ( tagNew->GetTitle() != "" ) ? tagNew->GetTitle(): m_strTitle ;
	CString strArtist = ( tagNew->GetArtist() != "" ) ? tagNew->GetArtist() : m_strArtist ;
	CString strAlbum =  ( tagNew->GetAlbum() != "" ) ? tagNew->GetAlbum() : m_strAlbum ;
	CString strYear =  ( tagNew->GetYear() != "" ) ? tagNew->GetYear() : m_strYear ;
	CString strComment =  ( tagNew->GetComment() != "" ) ? tagNew->GetComment() : m_strComment;
	CString strGenre = ( tagNew->GetGenre() != "" ) ? tagNew->GetGenre() : m_strGenre;
	CString strTrackNum =  ( tagNew->GetTrackNum() != "" ) ? tagNew->GetTrackNum() : m_strTrackNum;
	int nRating = tagNew->GetRating();
	CString strArtistUrl =  ( tagNew->GetArtistUrl() != "" ) ? tagNew->GetArtistUrl() : m_strArtistUrl;

	// strLyrics != "" que si les paroles doivent être réécrites
	CString strLyrics = tagNew->GetLyrics();
	BOOL bHasLyrics = tagNew->HasLyrics();

	// strPicture != "" que si l'image doit être réécrite
	CString strPictureFile = tagNew->GetPicture();
	BOOL bHasPicture = tagNew->HasPicture();


	// Write ID3 v1 tag

	if( CAudioFile::bWriteId3v1Tag )
	{
		static TCHAR szTag[129];
		unsigned char nGenre = CAudioFile::GetGenreIndex( m_strGenre );
		if( strTrackNum != "" )
		{
			// ID3 v1.1
			int nTrack;

			sscanf( strTrackNum, "%d", &nTrack );

			sprintf( szTag, "TAG%- 30s%- 30s%- 30s%- 4s%- 28s", 
				strTitle.Left(30), 
				strArtist.Left(30),
				strAlbum.Left(30),
				strYear.Left(4),
				strComment.Left(28)
			);
			szTag[125] = 0;
			szTag[126] = (unsigned char) nTrack;
			szTag[127] = nGenre;
			szTag[128] = 0;
		}
		else
		{
			// ID3 v1.0
			sprintf( szTag, "TAG%- 30s%- 30s%- 30s%- 4s%- 30s%1c", 
				strTitle.Left(30), 
				strArtist.Left(30),
				strAlbum.Left(30),
				strYear.Left(4),
				strComment.Left(30),
				nGenre
			);
		}

		if( !WriteID3v1Tag( szTag ))
			return FALSE;
	}
	// Write ID3 v2

	if( CAudioFile::bWriteId3v2Tag )
	{

		ID3_Tag tag;
		try 
		{
	
			if( tag.Link( GetFullPath(), LINK_WRITE ) != LINK_ERROR ) 
			{
				SetTagField( tag, strTitle,  ID3FID_TITLE );		
				SetTagField( tag, strArtist, ID3FID_LEADARTIST );		
				SetTagField( tag, strAlbum,  ID3FID_ALBUM );		
				SetTagField( tag, strYear,	 ID3FID_YEAR );		
				SetTagField( tag, strComment,  ID3FID_COMMENT );
		
	/*			// v4.0 genre storage

				CString strGenreId;
				strGenreId.Format("(%u)", nGenre );
				SetTagField( tag , strGenreId,  ID3FID_CONTENTTYPE );
	*/	
				SetTagField( tag , strGenre,  ID3FID_CONTENTTYPE );
				SetTagField( tag , strTrackNum,  ID3FID_TRACKNUM );
				SetTagNumField( tag , nRating * 25,  ID3FID_POPULARIMETER, ID3FN_RATING);
				SetTagField( tag , strArtistUrl,  ID3FID_WWWARTIST, ID3FN_URL );
		
				if( strLyrics != "" )
					SetTagField( tag, strLyrics,  ID3FID_UNSYNCEDLYRICS );		
				if( strPictureFile != "" )
					SetPicture( tag , strPictureFile );
				tag.Update();
			}
			else
			{
				return FALSE;
			}
		}
		catch( CException* e ) 
		{
			TRACE( "Error writing ID3v2 tag" );
			HandleException( e , "Error writing ID3v2 tag" );
			return FALSE;
		}

	}
	else
	{
		TRACE("bWriteId3v2Tag = FALSE \n" );
	}

	m_strTitle = strTitle;
	m_strArtist = strArtist;
	m_strAlbum = strAlbum;
	m_strYear = strYear;
	m_strComment = strComment;
	m_strGenre = strGenre;
	m_strTrackNum = strTrackNum;
	m_nRating = nRating;
	m_strArtistUrl = strArtistUrl;
	m_bHasLyrics = bHasLyrics;
	m_bHasPicture = bHasPicture;

	return TRUE;

}


BOOL CMp3File::WriteID3v1Tag(const char *strTag)
{
	CFile file;
	BOOL bTag;
	
	try 
	{
		if( file.Open( GetFullPath() , CFile::modeRead | CFile::typeBinary ))
		{
			static TCHAR buffer[129];
			file.Seek( -128L , CFile::end );
			file.Read( &buffer , sizeof( buffer) );
		
			bTag = ( (buffer[0] == 'T') && (buffer[1] == 'A') && (buffer[2] == 'G'))? TRUE : FALSE;

			// Force l'attribut ReadOnly
			CFileStatus status;
			file.GetStatus( status );
			file.Close();
			if( status.m_attribute & 0x01 /*readOnly*/ )
			{
				status.m_attribute = status.m_attribute - 0x01 /*readOnly*/ ;
				file.SetStatus( m_strLocation + "\\" + m_strName, status );
			}
			
			// Ouvre de nouveau pour écrire

			file.Open( GetFullPath() , CFile::modeWrite | CFile::typeBinary );
			if( bTag )
			{
				file.Seek( -128L , CFile::end );
			}
			else
			{
				file.Seek( 0L , CFile::end );
			}
			file.Write( strTag , 128 );
			file.Close();
		}
		else
		{
			return FALSE;
		}
	}
	catch ( CException *e )
	{
		TRACE( "Error writing ID3v1 tag !\n");
		HandleException( e , "Error writing ID3v1 tag" );
		return FALSE;
	}

	return TRUE;
}

CString CMp3File::GetLyrics()
{
	CString strLyrics;

	ID3_Tag tag;

//	if( tag.Link( GetFullPath() , ID3TT_ID3V2 ) )

	if( tag.Link( GetFullPath() , LINK_READONLY ) != LINK_ERROR )
	{
		GetTagField( tag , strLyrics,  ID3FID_UNSYNCEDLYRICS, ID3FN_TEXT, 10000 );
	}
	return strLyrics;
}

BOOL CMp3File::GetPicture( CString& strTmpFile)
{

	ID3_Tag tag;

//	if( tag.Link( GetFullPath() , ID3TT_ID3V2 ) )
	if( tag.Link( GetFullPath() , LINK_READONLY ) != LINK_ERROR )
	{
		ID3_Frame *frame;

		if( frame = tag.Find( ID3FID_PICTURE ))
		{
			// Check the picture format
			CString strFormat;
			frame->Field( ID3FN_MIMETYPE ).Get( strFormat.GetBuffer(1024) , 1024 );
			strFormat.ReleaseBuffer();
			if( strFormat !=  IMAGE_MIMETYPE )
				return FALSE;

			// Extract the picture in a temporary file
			if( strTmpFile == "" )
			{
				char tempdir[MAX_PATH];
				::GetTempPath( MAX_PATH, tempdir );
				::GetTempFileName( tempdir, "bmp", 0 , strTmpFile.GetBuffer(MAX_PATH) );
				strTmpFile.ReleaseBuffer();
			}
			frame->Field( ID3FN_DATA ).ToFile( strTmpFile ); 
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;

}

BOOL CMp3File::SetPicture( ID3_Tag& tag, const char* strFilename )
{
	ID3_Frame *frame;

	if( frame = tag.Find( ID3FID_PICTURE ))
	{
		frame->Field( ID3FN_MIMETYPE ) = IMAGE_MIMETYPE;
		frame->Field( ID3FN_DATA ).FromFile( strFilename ); 
	}
	else
	{
		frame = new ID3_Frame();
		frame->SetID( ID3FID_PICTURE );
		frame->Field( ID3FN_MIMETYPE ) = IMAGE_MIMETYPE;
		frame->Field( ID3FN_DATA ).FromFile( strFilename ); 
		tag.AddFrame( frame , true );
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////
// CAudioFile : Classe de base des fichiers Audio

CAudioFile::CAudioFile()
{

	// Valeurs par défaut des informations contenues dans l'étiquette ID3
	m_strTitle = "";
	// Artiste = Undefined par défaut
	m_strArtist = CAudioFile::strUndefined;
	m_strAlbum = CAudioFile::strUndefined;
	m_strYear = "";
	m_strComment = "";
	m_strGenre = CAudioFile::strUndefined;

	m_strLayer = "";
	m_strMode = "";
	m_strBitrate = "";
	m_nBitrate = 0;
	m_strFrequency = "";
	m_nTime = 0;
	m_nSize = 0;
	m_strSize = "";

	m_strTrackNum = "";
	m_nRating = 0;
	m_strArtistUrl = "";
	m_bHasLyrics = FALSE;
	m_bHasPicture = FALSE;
}

void CAudioFile::Copy(CAudioFile *pAudioFile)
{
	m_nBitrate = pAudioFile->m_nBitrate;
	m_nSize = pAudioFile->m_nSize;
	m_strSize = pAudioFile->m_strSize;
	m_nTime = pAudioFile->m_nTime;
	m_strAlbum = pAudioFile->m_strAlbum;
	m_strArtist = pAudioFile->m_strArtist;
	m_strGenre = pAudioFile->m_strGenre;
	m_strBitrate = pAudioFile->m_strBitrate;
	m_strComment = pAudioFile->m_strComment;
	m_strFrequency = pAudioFile->m_strFrequency;
	m_strLayer = pAudioFile->m_strLayer;
	m_strLocation = pAudioFile->m_strLocation;
	m_strMode = pAudioFile->m_strMode;
	m_strName = pAudioFile->m_strName;
	m_strTitle = pAudioFile->m_strTitle;
	m_strYear = pAudioFile->m_strYear;
	m_strFullUpperPath = pAudioFile->m_strFullUpperPath;

	m_strTrackNum = pAudioFile->m_strTrackNum;
	m_nRating = pAudioFile->m_nRating;
	m_strArtistUrl = pAudioFile->m_strArtistUrl;
	m_bHasLyrics = pAudioFile->m_bHasLyrics;
	m_bHasPicture = pAudioFile->m_bHasPicture;

}


CAudioFile::CAudioFile(CAudioFile *pMp3)
{
	m_nBitrate = pMp3->m_nBitrate;
	m_nSize = pMp3->m_nSize;
	m_strSize = pMp3->m_strSize;
	m_nTime = pMp3->m_nTime;
	m_strAlbum = pMp3->m_strAlbum;
	m_strArtist = pMp3->m_strArtist;
	m_strGenre = pMp3->m_strGenre;
	m_strComment = pMp3->m_strComment;
	m_strBitrate = pMp3->m_strBitrate;
	m_strFrequency = pMp3->m_strFrequency;
	m_strLayer = pMp3->m_strLayer;
	m_strLocation = pMp3->m_strLocation;
	m_strMode = pMp3->m_strMode;
	m_strName = pMp3->m_strName;
	m_strTitle = pMp3->m_strTitle;
	m_strYear = pMp3->m_strYear;
	m_strFullUpperPath = pMp3->m_strFullUpperPath;
	m_nRating = pMp3->m_nRating;
}

CAudioFile::~CAudioFile()
{
}

CAudioFile* CAudioFile::GetCopy()
{
	ASSERT( FALSE );
	return NULL;
}

void CAudioFile::BuildFullUpperPath()
{
	m_strUpperLocation = m_strLocation + "\\";
	m_strUpperLocation.MakeUpper();
	m_strFullUpperPath = m_strUpperLocation + m_strName;
	m_strFullUpperPath.MakeUpper();

}


CString CAudioFile::GetShortInfo()
{
	CString strInfo;
	strInfo.Format( "%s (%s)", GetTitle(), GetArtist());
	return strInfo;
}

CString CAudioFile::GetFullPath()
{
	if( m_strLocation != "" )
	{
		if( m_strLocation.Find( "http://" ) == 0 )
		{
			return m_strLocation + "/" + m_strName;
		}
		else
		{
			return m_strLocation + "\\" + m_strName;
		}

	}
	if( m_strAltLocation != "" )
	{
		return m_strAltLocation + "\\" + m_strName;
	}
	return m_strName;
}

CString CAudioFile::GetPath(CString strRoot, BOOL bRelative)
{
	CString strFilePath;
	CString strFullPath = GetFullPath();
	if( bRelative )
	{
		int nLen1 = strRoot.GetLength() + 1;
		int nLen2 = strFullPath.GetLength();
		strFilePath = strFullPath.Right( nLen2 - nLen1 );
	}
	else
	{
		strFilePath = GetFullPath();
	}
	return strFilePath;
}





CString CAudioFile::GetFileExt() 
{ 
	// Fonction virtuelle
	ASSERT( FALSE );
	return CString("");
}

CString CAudioFile::GetLyrics()
{
	// Fonction virtuelle
	ASSERT( FALSE );
	return CString("");
}



BOOL CAudioFile::ReadFileInfos( CString strLocation, CString strFileName, CString strAltLocation )
{
	// Fonction virtuelle
	ASSERT( FALSE );
	return TRUE;
}

BOOL CAudioFile::WriteFileInfos( CTag* tag )
{
	// Fonction virtuelle
	ASSERT( FALSE );
	return TRUE;
}



BOOL CAudioFile::IsUnderDir(CString strPath)
{
	strPath.MakeUpper();
	return ( m_strUpperLocation.Find( strPath ) != -1 );
}

CString CAudioFile::FormatSize( LONG lSize )
{
	CString strSize;
	double fSize = (double)lSize / 1000.0L;
	strSize.Format( CAudioFile::strSizeFormat , fSize );
	return strSize; 
}


CString CAudioFile::FormatTime( int nTime )
{
	static TCHAR szBuffer[256];
	int nMinute = nTime / 60;
	int nSecond = nTime - ( nMinute * 60 );
	int nHour = nMinute / 60;
	nMinute = nMinute - ( nHour * 60 ); 
	if( nHour != 0 )
	{
		wsprintf( szBuffer, "%d:%02d:%02d", nHour, nMinute, nSecond );
	}
	else
	{
		wsprintf( szBuffer, "%d:%02d", nMinute, nSecond );
	}
	return szBuffer; 
}

CString CAudioFile::GetInformation(int nIndex)
{
	switch( nIndex )
	{
	case FILENAME:
		return m_strName;
	case TITLE:
		return m_strTitle;
	case ALBUM: 
		return m_strAlbum;
	case LOCATION :
		return m_strLocation;
	case ARTIST:
		return m_strArtist;
	case FILESIZE:
		return GetFormatedSize();
	case DATE:
		return m_strYear;
	case COMMENT:
		return m_strComment;
	case GENRE:
		return GetGenre();
	case BITRATE:
		return m_strBitrate;
	case FREQUENCY:
		return m_strFrequency;
	case MODE:
		return m_strMode;
	case LAYER:
		return m_strLayer;
	case TIME:
		return FormatTime( m_nTime );
	case TRACKNUM:
		return m_strTrackNum;
	case RATING:
		return GetRating();
	case ARTISTURL:
		return m_strArtistUrl;
	case LYRICS:
		return GetHtmlLyrics();

	default:
		return "Not assigned";
	}
}

void CAudioFile::ModifyTagInfo(CString strArtist, CString strAlbum, CString strGenre)
{
	CTag tag;
	tag.SetArtist( strArtist );
	tag.SetAlbum( strAlbum );
	tag.SetGenre( strGenre );
	WriteFileInfos( &tag );
}


BOOL CAudioFile::Rename( const char* strNewName )
{

	CString strNewCleanFilename = CleanFilename(strNewName);
	try
	{
		CFile::Rename( m_strLocation + "\\" + m_strName , m_strLocation + "\\" + strNewCleanFilename );
	}
	catch ( CFileException *e )
	{
		TRACE( "Error renaming file !\n");
		HandleException( e , "Error renaming file " );
		return FALSE;
	}
	m_strName = strNewCleanFilename;
	BuildFullUpperPath();

	return TRUE; 
}

CString CAudioFile::CleanFilename(CString strSource)
{
	CString strClean;
	CString strBadChars = "\\/*?\"<>|";
	for( int i = 0 ; i < strSource.GetLength() ; i++ )
	{
		CString strChar = strSource.Mid( i , 1 );
		if( strBadChars.Find( strChar ) == -1 )
		{
			strClean += strChar;
		}
	}
	return strClean;
}

BOOL CAudioFile::Delete()
{
	try
	{
		CFile::Remove( m_strLocation + "\\" + m_strName );
	}
	catch ( CFileException *e )
	{
		TRACE( "Error deleting file !\n");
		HandleException( e , "Error deleting file" );
		return FALSE;
	}
	return TRUE; 
}


CString CAudioFile::FormatExport( CString strExportFormat )
{
	CString strOutput = strExportFormat;

	for( int i = 1 /* débute à 1 */ ; i < NUM_INFORMATION ; i++ )
	{
		strOutput.Replace( _gszExportBookmark[i], GetInformation( i ));
	}
	return strOutput;
}


/////////////////////////////////////////////////////////////////////////////
// CAudioFile serialization

void CAudioFile::Serialize(CArchive& ar , CString strPath)
{
	CString dummy2, dummy3, dummy4;
	unsigned char nOldGenreIndex = 0xFF;

	if (ar.IsStoring())
	{
		// Sauvegarde des adresses relatives pour copier le snapshot sur CD
		CString strLocation;
		strLocation = m_strLocation.Right( m_strLocation.GetLength() - strPath.GetLength());

		ar << m_strName;
		ar << m_strTitle;
		ar << strLocation;
		ar << m_strArtist;
		ar << m_strAlbum;
		ar << m_strComment;
		ar << nOldGenreIndex;
		ar << m_nSize;
		ar << m_nTime;
		ar << m_nBitrate;
		ar << m_strBitrate;
		ar << m_strFrequency;
		ar << m_strLayer;
		ar << m_strMode;
		ar << m_strYear;
		ar << m_nFileType;
		ar << m_strGenre;
		ar << m_strTrackNum;
		ar << m_nRating;
		ar << m_strArtistUrl;
		ar << m_bHasLyrics;
		ar << m_bHasPicture;
		ar << m_strSize;
		ar << dummy2;
		ar << dummy3;
		ar << dummy4;

	}
	else
	{
		CString strLocation;

		ar >> m_strName;
		ar >> m_strTitle;
		ar >> strLocation;
		ar >> m_strArtist;
		ar >> m_strAlbum;
		ar >> m_strComment;
		ar >> nOldGenreIndex;
		ar >> m_nSize;
		ar >> m_nTime;
		ar >> m_nBitrate;
		ar >> m_strBitrate;
		ar >> m_strFrequency;
		ar >> m_strLayer;
		ar >> m_strMode;
		ar >> m_strYear;
		ar >> m_nFileType;
		ar >> m_strGenre;
		ar >> m_strTrackNum;
		ar >> m_nRating;
		ar >> m_strArtistUrl;
		ar >> m_bHasLyrics;
		ar >> m_bHasPicture;
		ar >> m_strSize;
		ar >> dummy2;
		ar >> dummy3;
		ar >> dummy4;

		if( nOldGenreIndex != 0xFF )
		{
			m_strGenre = GetGenreFromIndex( nOldGenreIndex );
		}

		if( m_strSize == "" )
		{
			m_strSize = FormatSize( m_nSize );
		}


		m_strLocation = strPath + strLocation;
		if( m_strLocation.Right( 1 ) == "\\" )
		{
			m_strLocation = m_strLocation.Left( m_strLocation.GetLength() - 1 );
		}
		BuildFullUpperPath();
	}

}

void CAudioFile::DebugTrace(const char *strTrace)
{
	if( CAudioFile::bDebug )
	{
		CStdioFile file;
		if( file.Open( CAudioFile::strDebugLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText ))
		{
			file.SeekToEnd();
			CTime time;
			file.WriteString( time.FormatGmt( "[%d/%m/%Y %H:%M] - " ));
			file.WriteString( strTrace );
			file.WriteString( "\n" );
			file.Flush();
			file.Close();
		}
	}

}


///////////////////////////////////////////////////////////////////
// Class CCategory

CCategory::CCategory()
{
}

CCategory::~CCategory()
{
}




///////////////////////////////////////////////////////////////////
// Class CArtist

IMPLEMENT_DYNCREATE(CArtist, CObject) 

CArtist::CArtist()
{
}

CArtist::~CArtist()
{
	DeleteAlbums();
}

void CArtist::DeleteAlbums()
{
	POSITION pos = m_mapAlbum.GetStartPosition( );
	while( pos != NULL )
	{
		CAlbum* pAlbum;
		CString strAlbum;
		m_mapAlbum.GetNextAssoc( pos, strAlbum, (CObject*&)pAlbum );
		ASSERT( pAlbum->IsKindOf( RUNTIME_CLASS( CAlbum ) ) );
		delete pAlbum;
	}
	m_mapAlbum.RemoveAll();

}

CArtist::AddFile(CAudioFile *pMp3 , BOOL bGetAlbum )
{
	m_mp3List.Add( pMp3 );
	if( bGetAlbum )
	{
		CString strAlbumName = pMp3->GetAlbum();
		CAlbum* pAlbum = GetAlbum( strAlbumName );
		pAlbum->AddFile( pMp3 , FALSE );
	}


}

CAlbum* CArtist::GetAlbum(LPCSTR strName)
{

	CAlbum* pAlbum;

	if( ! m_mapAlbum.Lookup( strName , (CObject*&) pAlbum ))		
	{
		pAlbum = new CAlbum( strName );
		m_mapAlbum.SetAt( strName , (CObject*) pAlbum );
	}
	return pAlbum;

}


////////////////////////////////////////////////////////////////////
// Class CGenre
IMPLEMENT_DYNCREATE(CGenre, CObject) 

CGenre::CGenre()
{
}

CGenre::~CGenre()
{
	DeleteArtists();		
}

void CGenre::DeleteArtists()
{
	POSITION pos = m_mapArtist.GetStartPosition( );
	while( pos != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_mapArtist.GetNextAssoc( pos, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
		delete pArtist;
	}
	m_mapArtist.RemoveAll();

}

CArtist* CGenre::GetArtist(LPCSTR strName)
{

	CArtist* pArtist;

	if( ! m_mapArtist.Lookup( strName , (CObject*&) pArtist ))		
	{
		pArtist = new CArtist( strName );
		m_mapArtist.SetAt( strName , (CObject*) pArtist );
	}
	return pArtist;

}

CGenre::AddFile(CAudioFile *pMp3)
{
	m_mp3List.Add( pMp3 );

	CString strArtistName = pMp3->GetArtist();
	CArtist* pArtist = GetArtist( strArtistName );
	pArtist->AddFile( pMp3 , TRUE );
}


////////////////////////////////////////////////////////////////////
// Class CAlbum

IMPLEMENT_DYNCREATE(CAlbum, CObject) 

CAlbum::CAlbum()
{
}

CAlbum::~CAlbum()
{
	DeleteArtists();		
}

void CAlbum::DeleteArtists()
{
	POSITION pos = m_mapArtist.GetStartPosition( );
	while( pos != NULL )
	{
		CArtist* pArtist;
		CString strArtist;
		m_mapArtist.GetNextAssoc( pos, strArtist, (CObject*&)pArtist );
		ASSERT( pArtist->IsKindOf( RUNTIME_CLASS( CArtist ) ) );
		delete pArtist;
	}
	m_mapArtist.RemoveAll();

}

CArtist* CAlbum::GetArtist(LPCSTR strName)
{

	CArtist* pArtist;

	if( ! m_mapArtist.Lookup( strName , (CObject*&) pArtist ))		
	{
		pArtist = new CArtist( strName );
		m_mapArtist.SetAt( strName , (CObject*) pArtist );
	}
	return pArtist;

}


CAlbum::AddFile(CAudioFile *pMp3 , BOOL bGetArtist)
{
	m_mp3List.Add( pMp3 );

	if( bGetArtist )
	{
		CString strArtistName = pMp3->GetArtist();
		CArtist* pArtist = GetArtist( strArtistName );
		pArtist->AddFile( pMp3 , FALSE );
	}
}


int CALLBACK CompareMp3(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CAudioFile *f1 = (CMp3File *) lParam1;
	CAudioFile *f2 = (CMp3File *) lParam2;

	int nSortInfo;
	int nSortOrder;

	if( lParamSort < 0 )
	{
		nSortOrder = -1;
		nSortInfo = -lParamSort;
	}
	else
	{
		nSortOrder = 1;
		nSortInfo = lParamSort;
	}

	switch( nSortInfo )
	{
		case FILENAME : 
		case TITLE : 
		case ALBUM : 
		case LOCATION : 
		case ARTIST : 
		case GENRE : 
		case DATE : 
		case COMMENT : 
		case MODE : 
		case LAYER :
		case FREQUENCY : 
			if( f1->GetInformation( nSortInfo ) < f2->GetInformation( nSortInfo ) )	return -nSortOrder;
			if( f1->GetInformation( nSortInfo ) > f2->GetInformation( nSortInfo ) ) return nSortOrder;
			break;
		case TRACKNUM :
			if( f1->GetTrackNum() < f2->GetTrackNum() )	return -nSortOrder;
			if( f1->GetTrackNum() > f2->GetTrackNum() ) return nSortOrder;
			break;
		case RATING :
			if( f1->GetRatingNum() < f2->GetRatingNum() ) return nSortOrder;
			if( f1->GetRatingNum() > f2->GetRatingNum() ) return -nSortOrder;
			break;
		case FILESIZE : 
			if( f1->GetSize() < f2->GetSize() )	return nSortOrder;
			if( f1->GetSize() > f2->GetSize() ) return -nSortOrder;
			break;
		case TIME : 
			if( f1->GetTime() < f2->GetTime() )	return -nSortOrder;
			if( f1->GetTime() > f2->GetTime() )	return nSortOrder;
			break;
		case BITRATE : 
			if( f1->GetBitrateValue() < f2->GetBitrateValue() )	return -nSortOrder;
			if( f1->GetBitrateValue() > f2->GetBitrateValue() )	return nSortOrder;
			break;

	}
	return 0;
}






BOOL CAudioFile::GetPicture( CString& strTmpFile)
{
	ASSERT( FALSE );
	return FALSE;
}

BOOL CAudioFile::SetPicture( ID3_Tag& tag, const char* strFilename )
{
	ASSERT( FALSE );
	return FALSE;
}





CTag::CTag()
{
	m_strTitle = "";
	m_strArtist = "";
	m_strAlbum = "";
	m_strYear = "";
	m_strComment = "";
	m_strGenre = "";
	m_strTrackNum = "";
	m_nRating = 0;
	m_strArtistUrl = "";
	m_bHasLyrics = FALSE;
	m_bHasPicture = FALSE;
	m_strLyrics = "";
	m_strPictureFile = "";
}

CString CAudioFile::GetHtmlLyrics()
{
	CString strLyrics;
	if( m_bHasLyrics )
	{
		strLyrics = GetLyrics();
		strLyrics.Replace( "\n" , "<br>" );
	}
	return strLyrics;
}

int CAudioFile::GetTrackNum()
{
	int nNum = 0;

	if(	m_strTrackNum != "" )
	{
		::sscanf( m_strTrackNum , "%d", &nNum );
	}
	return nNum;
}

CString CAudioFile::GetGenreFromIndex(unsigned char nIndex)
{
	CString strGenre;

	// NUM_GENRE - 1 = BLANK_GENRE
	if( (nIndex >=0) && (nIndex < (NUM_GENRE-1)) )
	{
		strGenre = _gszGenre[nIndex];
	}
	else
	{
		strGenre = CAudioFile::strUndefined;
	}
	return strGenre;

}

unsigned char CAudioFile::GetGenreIndex(CString strGenre)
{
	for( int i = 0 ; i < (NUM_GENRE-1) ; i++ )
	{
		if( strGenre == _gszGenre[i] )
			return (unsigned char) i;
	}
	return (unsigned char) 0;
}


CString CAudioFile::Normalize(CString strSource, int nNorme, BOOL bRemoveUnderscores)
{
	if( bRemoveUnderscores )
		strSource.Replace( '_' , ' ' );

	switch( nNorme )
	{
	case NORMALIZE_LOWER:
		strSource.MakeLower();
		break;

	case NORMALIZE_UPPER:
		strSource.MakeUpper();
		break;

	case NORMALIZE_PROPER:
		strSource = MakeProper( strSource );
		break;

	case NORMALIZE_CAPFIRST:
		strSource = CapitalizeFirstChar( strSource);
		break;
	}

	return strSource;

}

CString CAudioFile::MakeProper(CString strInformation)
{
	CString strResult;

	int nLen = strInformation.GetLength();
	BOOL bMaj = TRUE;
	CString strChar;

	for( int i = 0 ; i < nLen ; i++ )
	{
		strChar = strInformation.Mid( i , 1 );
		TCHAR c = strChar[0];
		if( bMaj && ((isalpha(c) || ( c < 0))))
			strChar.MakeUpper();
		else
			strChar.MakeLower();
		strResult += strChar;
		bMaj = ((!isalpha(c)) && (c > 0) && ( c!='\''));
	}
	return strResult;
}


CString CAudioFile::CapitalizeFirstChar(CString strSource)
{
	CString strResult;
	CString strLower;

	switch( strSource.GetLength() )
	{
	case 0:
		break;

	case 1:
		strResult = strSource;
		strResult.MakeUpper();
		break;

	default:
		strResult = strSource.Left(1);
		strResult.MakeUpper();
		strLower = strSource.Mid(1);
		strLower.MakeLower();
		strResult += strLower;
		break;
	}
	return strResult;

}



BOOL CMp3File::IsV1Genre(LPCSTR strGenre)
{
	for( int i = 0 ; i < NUM_GENRE - 1 ; i++ )
	{
		if( lstrcmp( strGenre , _gszGenre[i]) == 0 )
			return TRUE;
	}
	return FALSE;

}

void CAudioFile::HandleException(CException *e, CString strContext )
{
		static TCHAR szError[MAX_ERROR];
		e->GetErrorMessage( szError, MAX_ERROR );
		CString strTrace = strContext;
		strTrace += "\n - File : ";
		strTrace += GetFullPath();
		strTrace += "\n - Exception : ";
		strTrace += szError;
		DebugTrace( strTrace );
		e->Delete();

}
