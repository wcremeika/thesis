// MpegHeader.cpp
// Adaptation and extension  : Pierre LEVY

/*
 *  @(#) ibitstream.cc 1.8, last edit: 6/15/94 16:51:45
 *  @(#) Copyright (C) 1993, 1994 Tobias Bading (bading@cs.tu-berlin.de)
 *  @(#) Berlin University of Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include "../stdafx.h"
#include "MpegHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT CMpegHeader::frequencies[2][4] =
{
	{22050, 24000, 16000, 1},
	{44100, 48000, 32000, 1}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMpegHeader::CMpegHeader()
{
	m_nFrameSize = 0;
	m_nSlots = 0;
	crc = NULL;
	offset = NULL;
	m_bInitialSync = false;
}


CMpegHeader::~CMpegHeader()
{
	if (offset != NULL) delete [] offset;
}



BOOL CMpegHeader::ReadHeader(CBitStream *stream, Crc16 **crcp, UINT& nVbrFrameCount, long nTagSize )
{
	UINT headerstring;

	if (!m_bInitialSync) 
	{
		if (!stream->GetHeader(&headerstring, INITIAL_SYNC, nVbrFrameCount, nTagSize))
	 		return false;

  		m_nVersion = (e_version) ((headerstring >> 19) & 1);

		if ((m_nSampleFrequency = (e_sample_frequency) ((headerstring >> 10) & 3)) == 3)
		{
				TRACE( "Unknown sample frequency in header\n" );
//				return false;
		}
	   stream->set_syncword(headerstring & 0xFFF80CC0);

	   m_bInitialSync = true;

	} 
	else 
	{
	  	if (!stream->GetHeader(&headerstring, STRICT_SYNC, nVbrFrameCount, nTagSize ))
		 	return false;
	} // m_bInitialSync

	m_nLayer         = 4 - (headerstring >> 17) & 3;
	m_nProtectionBit = (headerstring >> 16) & 1;
	m_nBitrateIndex  = (headerstring >> 12) & 0xF;
	m_nPaddingBit    = (headerstring >> 9) & 1;
	m_nMode          = (e_mode)((headerstring >> 6) & 3);

	// Support des VBR Xing
	m_bVBR = FALSE;
	if( nVbrFrameCount )
	{
		m_bVBR = TRUE;
		UINT nFrequency = GetFrequency();
		m_nTime = (nVbrFrameCount * 1152 )/ nFrequency;
	}
	return true;
}


const char *CMpegHeader::GetLayer() const
{
	switch (m_nLayer)
	{
	case 1:
		return "I";
	case 2:
		return "II";
	case 3:
		return "III";
  }
  return "?";	
}


const char *CMpegHeader::GetBitrate() const
{

  static const char *bitrate_str[2][3][16] = {
  {{"VBR", "32 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s",
    "80 kbit/s", "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s",
    "160 kbit/s", "176 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s",
    "?"},
   {"VBR", "8 kbit/s", "16 kbit/s", "24 kbit/s", "32 kbit/s",
    "40 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s", "80 kbit/s",
    "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s", "160 kbit/s",
    "?"},
   {"VBR", "8 kbit/s", "16 kbit/s", "24 kbit/s", "32 kbit/s",
    "40 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s", "80 kbit/s",
    "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s", "160 kbit/s",
    "?"}},
  {{"VBR", "32 kbit/s", "64 kbit/s", "96 kbit/s", "128 kbit/s",
    "160 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s", "288 kbit/s",
    "320 kbit/s", "352 kbit/s", "384 kbit/s", "416 kbit/s", "448 kbit/s",
    "?"},
	{"VBR", "32 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s",
    "80 kbit/s", "96 kbit/s", "112 kbit/s", "128 kbit/s", "160 kbit/s",
    "192 kbit/s", "224 kbit/s", "256 kbit/s", "320 kbit/s", "384 kbit/s",
    "?"},
	{"VBR", "32 kbit/s", "40 kbit/s", "48 kbit/s", "56 kbit/s",
    "64 kbit/s", "80 kbit/s" , "96 kbit/s", "112 kbit/s", "128 kbit/s",
    "160 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s", "320 kbit/s",
    "?"}}
  };
  if( (m_nLayer < 1) || (m_nLayer > 3))
	  return "incorrect Layer";

  return bitrate_str[m_nVersion][m_nLayer - 1][m_nBitrateIndex];
}

const int CMpegHeader::GetBitrateValue() const
{

  static int bitrate[2][3][16] = {
	{	
		{ 0 , 32000, 48000, 56000, 64000,
		80000, 96000, 112000, 128000, 144000,
		160000, 176000, 192000, 224000, 256000,
		0},
		{0, 8000, 16000, 24000, 32000,
		40000, 48000, 56000, 64000, 80000,
		96000, 112000, 128000, 144000, 160000,
		0},
		{0, 8000, 16000, 24000, 32000,
		40000, 48000, 56000, 64000, 80000,
		96000, 112000, 128000, 144000, 160000,
		0}
	},
	{	
		{0, 32000, 64000, 96000, 128000,
		160000, 192000, 224000, 256000, 288000,
		320000, 352000, 384000, 416000, 448000,
		0},
		{0, 32000, 48000, 56000, 64000,
		80000, 96000, 112000, 128000, 160000,
		192000, 224000, 256000, 320000, 384000,
		0},
		{0, 32000, 40000, 48000, 56000,
		64000, 80000 , 96000, 112000, 128000,
		160000, 192000, 224000, 256000, 320000,
		0}
	}
  };

  return bitrate[m_nVersion][m_nLayer - 1][m_nBitrateIndex];
}



const char *CMpegHeader::GetSampleFrequency() const
{
  switch (m_nSampleFrequency)
  {
	 case thirtytwo:
    	if (m_nVersion == MPEG1)
			return "32 kHz";
      else
      	return "16 kHz";
	 case fourtyfour_point_one:
    	if (m_nVersion == MPEG1)
			return "44.1 kHz";
      else
      	return "22.05 kHz";
	 case fourtyeight:
    	if (m_nVersion == MPEG1)
			return "48 kHz";
      else
      	return "24 kHz";
		
  }
  return "?";	
}

const UINT CMpegHeader::GetFrequency() const
{
  switch (m_nSampleFrequency)
  {
	 case thirtytwo:
    	if (m_nVersion == MPEG1)
			return 32000;
      else
      	return 16000;
	 case fourtyfour_point_one:
    	if (m_nVersion == MPEG1)
			return 44100;
      else
      	return 22050;
	 case fourtyeight:
    	if (m_nVersion == MPEG1)
			return 48000;
      else
      	return 24000;
		
  }
  return 44100;	
}
const char *CMpegHeader::GetMode() const
{
  switch (m_nMode)
  {
	 case stereo:
		return "Stereo";
	 case joint_stereo:
		return "Joint stereo";
	 case dual_channel:
		return "Dual channel";
	 case single_channel:
		return "Mono";
  }
  return "?";
}

const char *CMpegHeader::GetVersion() const
{
 	switch (m_nVersion)
   {
   	  case MPEG1:
        return "MPEG-1";
      case MPEG2_LSF:
		  return "MPEG-2";
   }
   return(NULL);
}



//////////////////////////////////////////////////////////////////////////
// Gestion du STREAM

#define swap_int32(int_32) (((int_32) << 24) | (((int_32) << 8) & 0x00ff0000) | \
				               (((int_32) >> 8) & 0x0000ff00) | ((int_32) >> 24))


CBitStream::CBitStream(const char *filename)
{
	SECURITY_ATTRIBUTES security;
	security.nLength              = sizeof(SECURITY_ATTRIBUTES);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle       = FALSE;

	fd = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, &security, OPEN_EXISTING, NULL, NULL);

	if (fd == INVALID_HANDLE_VALUE) 
	{
		char bad_file_msg[256];
		lstrcpy(bad_file_msg, "Error opening file: ");

		::MessageBox(NULL, lstrcat(bad_file_msg, filename) , "Invalid handle", MB_OK);
		return;
	}

	wordpointer       = buffer;
	bitindex          = 0;

	// Seeking variables
	current_frame_number = -1;
	copy = false;
}


CBitStream::~CBitStream()
{
  // close the file
  if (fd && !copy) 
		CloseHandle((void*)fd);
}


BOOL CBitStream::GetHeader( UINT *headerstring, enum e_syncmode syncmode, UINT& nVbrFrameCount, long nTagSize )
{
	BOOL result, sync;

	int numread;
	int nTry = 0;	

	// Déplace le pointeur de la taille de la tag
	SetFilePointer(fd, nTagSize, NULL, FILE_CURRENT);

	do {

		// Lecture de 4 caractères
		result = (BOOL) ReadFile( fd, (char *) headerstring, 4, (DWORD *) &numread, NULL);

		if (!(result && (numread == 4)))
		{
			return false;
		}

		if (syncmode == INITIAL_SYNC) 
		{
			sync =  ((*headerstring & 0x0000F0FF) == 0x0000F0FF);
		} 
		else 
		{
   			sync =  ((*headerstring & 0x000CF8FF) == syncword) &&
             (((*headerstring & 0xC0000000) == 0xC0000000) == single_ch_mode);
		}

		if (!sync)
		{
			// Si la synchro n'est pas trouvée revenir de 3 caractères et relire de nouveau 4 caractères
			result = (BOOL) SetFilePointer(fd, -3, NULL, FILE_CURRENT);

			// Si la synchro n'est pas atteinte après 3000 essai alors arrêter
			nTry++;
			if( nTry > 3000 )
				return FALSE;
			
		}

	} while (!sync && result);

	if (!result)
	{
		return false;
	}

	// ------------------- VBR --------------------- begin
	// Lit l'entete du fichier pour détecter les VBR de Xing. 

	UINT vbrtag;
	nVbrFrameCount = 0;

	// Recherche la chaîne "Xing" 17 octects après l'entete dans le fichier (LAME)
	::SetFilePointer(fd, 17, NULL, FILE_CURRENT);
	::ReadFile( fd, (char *) &vbrtag, 4, (DWORD *) &numread, NULL);
	if( vbrtag == 0x676e6958)
	{
		// Read the second integer
		::ReadFile( fd, (char *) &nVbrFrameCount, 4, (DWORD *) &numread, NULL);
		::ReadFile( fd, (char *) &nVbrFrameCount, 4, (DWORD *) &numread, NULL);
		nVbrFrameCount = swap_int32( nVbrFrameCount );

	}

	// Recherche la chaîne "Xing" 32 octects après l'entete dans le fichier (XING)
	::SetFilePointer(fd, 11, NULL, FILE_CURRENT);
	::ReadFile( fd, (char *) &vbrtag, 4, (DWORD *) &numread, NULL);
	if( vbrtag == 0x676e6958)
	{
		// Read the second integer
		::ReadFile( fd, (char *) &nVbrFrameCount, 4, (DWORD *) &numread, NULL);
		::ReadFile( fd, (char *) &nVbrFrameCount, 4, (DWORD *) &numread, NULL);
		nVbrFrameCount = swap_int32( nVbrFrameCount );
	}



	// ------------------- VBR --------------------- end

	UINT header = *headerstring;
	*headerstring = swap_int32(header);

	current_frame_number++;
	return true;

}


void CBitStream::set_syncword(UINT syncword0)
{
   syncword = swap_int32(syncword0 & 0xFFFFFF3F);
   single_ch_mode = ((syncword0 & 0x000000C0) == 0x000000C0);
}



//////////////////////////////////////////////////////////////////////////
// Gestion du CRC

void Crc16::add_bits (UINT bitstring, UINT length)
{
  UINT bitmask = 1 << (length - 1);
  do
	 if (!(crc & 0x8000) ^ !(bitstring & bitmask))
	 {
		crc <<= 1;
		crc ^= 0x8005; //polynomial	
	 }
	 else
		crc <<= 1;
  while (bitmask >>= 1);
}

//////////////////////////////////////////////////////////////////////////
// 

