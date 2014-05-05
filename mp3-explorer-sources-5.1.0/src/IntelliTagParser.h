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
 

#if !defined(AFX_INTELLITAGPARSER_H__75CD23AF_56B3_11D4_BEE0_00400556855E__INCLUDED_)
#define AFX_INTELLITAGPARSER_H__75CD23AF_56B3_11D4_BEE0_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIntelliTagParser  
{
private:
	int GetSeparatorCount( TCHAR ch );
	int ExtractElements(TCHAR ch);
	int ExtractElements(TCHAR chStart, TCHAR chEnd);
	void StripExtension();
	CString m_strTitle;
	CString m_strArtist;
	CString m_strAlbum;
	CString m_strTrack;
	int m_nUnderscoreCount;
	int m_nHyphenCount;
	CString m_strFilename;
	CString m_strElement[4];

public:
	void StripBracesAndBrakets( CString& strSource );
	BOOL IsTrackNum( CString strElement );
	const CString& GetTitle()  { return m_strTitle; };
	const CString& GetAlbum()  { return m_strAlbum; };
	const CString& GetTrack()  { return m_strTrack; };
	const CString& GetArtist() { return m_strArtist; };

	CIntelliTagParser( CString strFilename );
	CIntelliTagParser();
	virtual ~CIntelliTagParser();

};

#endif // !defined(AFX_INTELLITAGPARSER_H__75CD23AF_56B3_11D4_BEE0_00400556855E__INCLUDED_)
