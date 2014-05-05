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
 

#if !defined(AFX_NORMALIZATIONPROFILE_H__13D9A463_5C95_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_NORMALIZATIONPROFILE_H__13D9A463_5C95_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct {
	int nFieldIndex;
	LPCSTR pstrFieldResName;
	int nNorm;
	_TCHAR *lpEntry;
} NORMALIZEDFIELD;

#define NUM_NORMALIZED_FIELDS 4

static NORMALIZEDFIELD g_normalized_fields[NUM_NORMALIZED_FIELDS]= {
	{ TITLE,	"IdsTitle",		NORMALIZE_PROPER, _T("Title") },
	{ ALBUM,	"IdsAlbum",		NORMALIZE_PROPER, _T("Album") },
	{ ARTIST,	"IdsArtist",	NORMALIZE_PROPER, _T("Artist") },
	{ COMMENT,	"IdsComment",	NORMALIZE_NONE, _T("Comment") }
};


class CNormalizationProfile : public CObject  
{

private:
	BOOL m_bReplaceUnderscores;
public:
	void SetUnderscoresReplacement( BOOL bSet ){ m_bReplaceUnderscores = bSet; };
	BOOL GetUnderscoresReplacement(){ return m_bReplaceUnderscores; };

	CNormalizationProfile( const CNormalizationProfile& profile );
	CNormalizationProfile& operator =(const CNormalizationProfile &profile);
	void Save();
	void Load();
	int m_arrayNorms[NUM_INFORMATION];
	void SetNorm( int nIndex, int nNorm );
	int GetNorm( int nIndex );
	CNormalizationProfile();
	virtual ~CNormalizationProfile();

};

#endif // !defined(AFX_NORMALIZATIONPROFILE_H__13D9A463_5C95_11D4_BEE0_00400556855E__INCLUDED_)
