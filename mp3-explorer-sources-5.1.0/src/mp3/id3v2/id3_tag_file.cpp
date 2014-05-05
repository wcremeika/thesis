//  The authors have released ID3Lib as Public Domain (PD) and claim no copyright,
//  patent or other intellectual property protection in this work.  This means that
//  it may be modified, redistributed and used in commercial and non-commercial
//  software and hardware without restrictions.  ID3Lib is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
//  
//  The ID3Lib authors encourage improvements and optimisations to be sent to the
//  ID3Lib coordinator, currently Dirk Mahoney (dirk@id3.org).  Approved
//  submissions may be altered, and will be included and released under these terms.
//  
//  Mon Nov 23 18:34:01 1998

// PL
#include "../../stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
// Fin PL

#include <string.h>
#include "id3_tag.h"


bool exists( char *name )
{
	bool	doesExist = false;
	FILE	*in;

	if	( name )
	{
		if	( in = fopen ( name, "rb" ) )
		{
			doesExist = true;
			fclose ( in );
		}
	}
	else
		ID3_THROW ( ID3E_NoData );

	return doesExist;
}


bool ID3_Tag::OpenLinkedFile( int nMode )
{
	char *mode;
	
	if( nMode == LINK_READONLY )
	{
		mode = "rb";
	}
	else
	{
		mode = "rb+";
		if( ! exists ( m_strFileName ) )
			mode = "wb+";
	}

	if( fileHandle = fopen ( m_strFileName, mode ) )
	{
		fseek ( fileHandle, 0, SEEK_END );
		m_nFileSize = ftell ( fileHandle );
		fseek ( fileHandle, 0, SEEK_SET );
		return true;
	}

	return false;
}


luint ID3_Tag::Link( const char *fileInfo , int nMode )
{
	luint	posn	= 0;

	if( fileInfo )
	{
		strncpy ( m_strFileName, fileInfo, 256 );

		// if we were attached to some other file
		// file then abort
		if	( fileHandle )
			ID3_THROW ( ID3E_TagAlreadyAttached );

		GenerateTempName();
		if( !OpenLinkedFile( nMode ))
			return LINK_ERROR;

		m_nOldTagSize = ParseFromHandle();

		if	( m_nOldTagSize )
			m_nOldTagSize += ID3_TAGHEADERSIZE;

		m_nFileSize -= m_nOldTagSize;
		posn = m_nOldTagSize;
	}
	else
		ID3_THROW ( ID3E_NoData );

	return posn;
}


void ID3_Tag::Update( void )
{
	if	( HasChanged() )
		RenderToHandle();

	return;
}


void ID3_Tag::Strip( bool v1Also )
{
	FILE	*tempOut;

	if	( strlen ( m_strTempName ) > 0 )
	{
		if	( tempOut = fopen ( m_strTempName, "wb" ) )
		{
			uchar	*buffer2;

			fseek ( fileHandle, m_nOldTagSize, SEEK_SET );

			if	( buffer2 = new uchar[ BUFF_SIZE ] )
			{
				bool	done		= false;
				luint	bytesCopied	= 0;
				luint	bytesToCopy	= m_nFileSize;
				int		i;

				if	( m_bHasV1Tag && v1Also )
					bytesToCopy -= m_nExtraBytes;

				while	( ! feof ( fileHandle ) && ! done )
				{
					luint	size	= BUFF_SIZE;

					if	( ( bytesToCopy - bytesCopied ) < BUFF_SIZE )
					{
						size = bytesToCopy - bytesCopied;
						done = true;
					}

					if	( i = fread ( buffer2, 1, size, fileHandle ) )
						fwrite ( buffer2, 1, i, tempOut );

					bytesCopied += i;
				}

				delete[] buffer2;
			}

			fclose ( tempOut );
			fclose ( fileHandle );
			remove ( m_strFileName );
			rename ( m_strTempName, m_strFileName );
			OpenLinkedFile( LINK_READONLY );

			m_nOldTagSize = 0;
			m_nExtraBytes = 0;

			if	( v1Also )
				m_bHasV1Tag = false;

			m_bHasChanged = true;
		}
	}

	return;
}


