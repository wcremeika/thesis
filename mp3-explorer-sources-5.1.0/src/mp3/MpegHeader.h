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
 

#if !defined(AFX_MPEGHEADER_H__B7B78AC1_896F_11D2_87FD_B9A7BB87B54F__INCLUDED_)
#define AFX_MPEGHEADER_H__B7B78AC1_896F_11D2_87FD_B9A7BB87B54F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef __WIN32__
#include <wtypes.h>
#endif

#define DIR_MARKER '\\'
#define DIR_MARKER_STR "\\"

typedef   signed short  int16;	      // 16 Bit signed integer
typedef unsigned short uint16;	      // 16 Bit unsigned integer

enum e_channels { both, left, right, downmix };
enum e_syncmode { INITIAL_SYNC, STRICT_SYNC };
enum e_version { MPEG2_LSF, MPEG1 };
enum e_mode { stereo, joint_stereo, dual_channel, single_channel };
enum e_sample_frequency { fourtyfour_point_one, fourtyeight, thirtytwo };

// 16-Bit CRC checksum class:
class Crc16
{
	static const uint16 polynomial;
	uint16 crc;

public:
	Crc16() { crc = 0xFFFF; }
	void  add_bits (UINT bitstring, UINT length);
		 // feed a bitstring to the crc calculation (0 < length <= 32)
	uint16	checksum()
	// return the calculated checksum and erase it for next calls to add_bits()
	{
		register uint16 sum = crc;
		crc = 0xFFFF;
		return sum;
	}
};

/////////////////////////////////////////////////////////////////
// Xing VBR handling

#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

// structure to receive extracted header
// toc may be NULL
typedef struct {
    int h_id;       // from MPEG header, 0=MPEG2, 1=MPEG1
    int samprate;   // determined from MPEG header
    int flags;      // from Xing header data
    int frames;     // total bit stream frames from Xing header data
    int bytes;      // total bit stream bytes from Xing header data
    int vbr_scale;  // encoded vbr scale from Xing header data
    unsigned char toc[100];  // pointer to unsigned char toc_buffer[100]
                         // may be NULL if toc not desired
}   XHEADDATA;



class CBitStream;

class CMpegHeader  
{

private:

	static const UINT	frequencies[2][4];
	UINT	m_nLayer;
	UINT	m_nProtectionBit;
	UINT	m_nBitrateIndex;
	UINT	m_nPaddingBit;
	UINT	m_nModeExtension;

	e_version	m_nVersion;
	e_mode		m_nMode;
	e_sample_frequency	m_nSampleFrequency;
	UINT		m_nNumberOfSubbands; 
	UINT		m_nIntensityStereoBound;
	BOOL		m_bCopyright;
	BOOL		m_bOriginal;
	BOOL		m_bInitialSync;
	Crc16		*crc;
	UINT		*offset;
	uint16		checksum;
	UINT		m_nFrameSize;
	UINT		m_nSlots;

	// Added
	UINT		m_nTime;
	UINT		m_nBitrate;
	BOOL		m_bVBR;

public:

	CMpegHeader();

	~CMpegHeader();


	XHEADDATA m_headerXing;

	BOOL ReadHeader(CBitStream *stream, Crc16 **crcp, UINT& nVbrFrameCount, long nTagSize );

	e_version	Version()		const { return m_nVersion; }
	UINT		Layer()         const { return m_nLayer; }
	UINT		BitrateIndex() const { return m_nBitrateIndex; }
	e_sample_frequency	SampleFrequency() const	{ return m_nSampleFrequency;}
	UINT		Frequency() const {	return frequencies[m_nVersion][m_nSampleFrequency];}

	e_mode		Mode()         const { return m_nMode; }
	BOOL		Checksums()    const { return (BOOL) !m_nProtectionBit; }
	BOOL		Copyright()    const { return m_bCopyright; }
	BOOL		Original()     const { return m_bOriginal; }

	BOOL		checksum_ok () const { return (BOOL) (checksum == crc->checksum ()); }

	BOOL		Padding()        const { return (BOOL) m_nPaddingBit; }
	UINT		Slots()          const { return m_nSlots; }
	UINT		ModeExtension() const { return m_nModeExtension; }

	// Fonctions qui renvoient les infos sous forme de chaine:
	const char *	GetLayer() const;
	const char *	GetBitrate() const;
	const char *	GetSampleFrequency() const;
	const char *	GetMode() const;
	const char *	GetVersion() const;
	// Ajouts
	const int		GetBitrateValue() const; 
	const UINT		GetFrequency() const;
	BOOL			IsVBR() const { return m_bVBR; };
	UINT			GetVbrDuration() const { return m_nTime; };

};



const UINT bufferintsize = 433;
	// max. 1730 bytes per frame: 144 * 384kbit/s / 32000 Hz + 2 Bytes CRC

// Class to extract bitstrings from files:
class CBitStream
{
public:
	unsigned char	szXingBuffer[bufferintsize];

private:
	HANDLE fd;
	UINT	buffer[bufferintsize]; 
	UINT	m_nFrameSize;
	UINT	*wordpointer;
	UINT	bitindex;
	UINT	syncword;
	BOOL  single_ch_mode;

	int  current_frame_number;
	BOOL   copy;

public:

	CBitStream(const char *filename);
	~CBitStream();

	BOOL	GetHeader( UINT *headerstring, enum e_syncmode syncmode, UINT& nVbrFrameCount, long nTagSize );
	void   set_syncword(UINT syncword0);

};


#endif // !defined(AFX_MPEGHEADER_H__B7B78AC1_896F_11D2_87FD_B9A7BB87B54F__INCLUDED_)
