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
 

#if !defined(AFX_SEARCHDLG_H__4AA4F621_EE22_11D3_BEE0_00400556855E__INCLUDED_)
#define AFX_SEARCHDLG_H__4AA4F621_EE22_11D3_BEE0_00400556855E__INCLUDED_

#include "mp3/Mp3File.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog

class CSearchDlg : public CDialog
{
// Construction
public:
	CMapStringToOb* m_pMapGenre;


	int GetSelectedFiles(CMp3FileList &mp3list);
	CMyImageList m_SmallImageList;
	~CSearchDlg();
	void ResetResult();
	void UpdateList();
	CObArray m_columns;
	void InitColumns();
	BOOL FindString( CString strSource, CString strFind );
	BOOL Match( CAudioFile* pAudioFile );
	CMp3FileList m_listResult;
	CMp3FileList* m_pList;
	void UpdateTab();
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchDlg)
	enum { IDD = IDD_SEARCH };
	CListCtrl	m_list;
	CTabCtrl	m_tab;
	BOOL	m_bHasLyrics;
	BOOL	m_bHasPicture;
	int		m_nRatingMin;
	int		m_nRatingMax;
	int		m_nBitrateMin;
	int		m_nBitrateMax;
	CString	m_strAlbum;
	CString	m_strArtist;
	CString	m_strTitle;
	CString	m_strYearMax;
	CString	m_strYearMin;
	CString	m_strGenre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnRclickListResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFilePlay();
	afx_msg void OnFileEnqueue();
	afx_msg void OnFileExport();
	afx_msg void OnFileAddToPlaylist();
	afx_msg void OnFileRename();
	afx_msg void OnFileEditTag();
	afx_msg void OnFileDelete();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHDLG_H__4AA4F621_EE22_11D3_BEE0_00400556855E__INCLUDED_)
