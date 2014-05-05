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
#include "mp3/mp3file.h"
#include "NormalizationProfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DUMMY_VALUE 999


extern CMp3ExpApp theApp;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNormalizationProfile::CNormalizationProfile()
{

}

CNormalizationProfile::~CNormalizationProfile()
{

}

int CNormalizationProfile::GetNorm(int nIndex)
{
	int nNorm = 0;

	for( int i = 0 ; i < NUM_NORMALIZED_FIELDS ; i++ )
	{
		if( nIndex == g_normalized_fields[i].nFieldIndex )
		{
			nNorm = m_arrayNorms[ nIndex ];
		}
	}
	return nNorm;

}

void CNormalizationProfile::SetNorm(int nIndex, int nNorm)
{
	for( int i = 0 ; i < NUM_NORMALIZED_FIELDS ; i++ )
	{
		if( nIndex == g_normalized_fields[i].nFieldIndex )
		{
			m_arrayNorms[ nIndex ] = nNorm;
		}
	}
}


void CNormalizationProfile::Load()
{
	for( int i = 0 ; i < NUM_NORMALIZED_FIELDS ; i++ )
	{
		int nEntry = theApp.GetProfileInt( SECT_NORMALIZE, g_normalized_fields[i].lpEntry, DUMMY_VALUE );
		if( nEntry != DUMMY_VALUE )
		{
			m_arrayNorms[g_normalized_fields[i].nFieldIndex] = nEntry;
		}
		else
		{
			m_arrayNorms[g_normalized_fields[i].nFieldIndex] = g_normalized_fields[i].nNorm;
		}
	}
	m_bReplaceUnderscores = theApp.GetProfileInt( SECT_NORMALIZE, ENTR_NORMALIZE_UNDERSCORES, TRUE );

}

void CNormalizationProfile::Save()
{
	for( int i = 0 ; i < NUM_NORMALIZED_FIELDS ; i++ )
	{
		int nValue = m_arrayNorms[g_normalized_fields[i].nFieldIndex];
		theApp.WriteProfileInt( SECT_NORMALIZE, g_normalized_fields[i].lpEntry, nValue );
	}
	theApp.WriteProfileInt( SECT_NORMALIZE, ENTR_NORMALIZE_UNDERSCORES, m_bReplaceUnderscores );

}

CNormalizationProfile::CNormalizationProfile(const CNormalizationProfile &profile)
{
	for( int i = 0 ; i < NUM_INFORMATION ; i++ )
	{
		m_arrayNorms[i] = profile.m_arrayNorms[i];
	}
	m_bReplaceUnderscores = profile.m_bReplaceUnderscores;
}



CNormalizationProfile& CNormalizationProfile::operator =(const CNormalizationProfile &profile)
{
	for( int i = 0 ; i < NUM_INFORMATION ; i++ )
	{
		m_arrayNorms[i] = profile.m_arrayNorms[i];
	}
	m_bReplaceUnderscores = profile.m_bReplaceUnderscores;
	return *this;

}
