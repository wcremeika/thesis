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
 

#if !defined(AFX_MP3VIEW_H__B8036FE1_06FA_11D3_BEDD_00400556855E__INCLUDED_)
#define AFX_MP3VIEW_H__B8036FE1_06FA_11D3_BEDD_00400556855E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mp3View.h : header file
//

#define NUM_ITEMS	1

#define MAX_COLTITLE_SIZE 100

#define NUM_INFO	16
#define INFO_MUSIC	0x01
#define INFO_MPEG	0x02
#define INFO_FILE	0x04

typedef struct {
	_TCHAR szColTitle[MAX_COLTITLE_SIZE];
	LPCSTR pstrTitleResName;
	int nInfoOrder;
	int	nInfoType;
	int nInfoIndex;
	int nAlignemnt;
	int nWidth;
	int nDisplayed;
} COLUMNINFO;

static COLUMNINFO _gcolumns[NUM_INFO]= {
	{ "", "IdsFile",			1,	INFO_FILE,	FILENAME,	LVCFMT_LEFT,	150 , 1},
	{ "", "IdsTitle",		2,	INFO_MUSIC,	TITLE,		LVCFMT_LEFT,	150 , 1},
	{ "", "IdsAlbum",		3,	INFO_MUSIC, ALBUM,		LVCFMT_LEFT,	150 , 1},
	{ "", "IdsArtist",		4,	INFO_MUSIC, ARTIST,		LVCFMT_LEFT,	100 , 1},
	{ "", "IdsLocation",		5,	INFO_FILE,  LOCATION,	LVCFMT_LEFT,	150 , 0},
	{ "", "IdsSize",			6,	INFO_FILE,  FILESIZE,	LVCFMT_RIGHT,	65  , 0},
	{ "", "IdsDate",			7,	INFO_MUSIC, DATE,		LVCFMT_CENTER,	50  , 1},
	{ "", "IdsTrackNum",		8,	INFO_MUSIC, TRACKNUM,	LVCFMT_CENTER,	50  , 1},
	{ "", "IdsRating",		9,	INFO_MUSIC, RATING,		LVCFMT_CENTER,	50  , 1},
	{ "", "IdsGenre",		10,	INFO_MUSIC, GENRE,		LVCFMT_LEFT,	100 , 1},
	{ "", "IdsComment",		11,	INFO_MUSIC, COMMENT,	LVCFMT_LEFT,	150 , 0},
	{ "", "IdsTime",			12,	INFO_MPEG,	TIME,		LVCFMT_RIGHT,	50  , 1},
	{ "", "IdsLayer",		13, INFO_MPEG,	LAYER,		LVCFMT_LEFT,	150 , 0},
	{ "", "IdsBitrate",		14,	INFO_MPEG,	BITRATE,	LVCFMT_RIGHT,	70  , 0},
	{ "", "IdsFrequency",	15,	INFO_MPEG,	FREQUENCY,	LVCFMT_RIGHT,	80  , 0},
	{ "", "IdsMode",			16, INFO_MPEG,	MODE,		LVCFMT_CENTER,	100 , 0},
};


class CColumn : public CObject
{
public:
	void Copy( CColumn* pCol);

	LPSTR m_pszColTitle;
	LPSTR m_pszTitleResName;
	int m_nInfoOrder;
	int	m_nInfoType;
	int m_nInfoIndex;
	int m_nDisplayed;
	int m_nAlignemnt;
	int m_nWidth;

	CColumn();
	~CColumn();
};

class CListViewEx;
class CMyImageList;
/////////////////////////////////////////////////////////////////////////////
// CMp3View view

class CMp3View : public CListViewEx
{
protected:
	CMp3View();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMp3View)


// Attributes
	static BOOL m_bDisplayAll;
	CBitmap	m_bmpBackGround;

public:
	static void OnDisplayAll();
	static BOOL IsDisplayAll(){ return m_bDisplayAll; };

protected:
	// Drag and drop
	COleDropSource m_OleDropSource;
	CListDropTarget m_OleDropTarget;
public:
	UINT m_ImageFormat;
	int m_nSortedCol;
	int m_nPrevSortedCol;
	int m_nLeftViewType;
	BOOL m_bItemChangedHandling;
protected:
	CMyImageList m_LargeImageList;
	CMyImageList m_SmallImageList;
	CObArray  m_columns;
	CObArray  m_colsDisplayed;
	int m_nDisplayInfoType;
	CMp3FileList* m_pList;
	BOOL m_bCreated;
	CMenu m_menu;

public:

// Operations
public:
	HGLOBAL GetHTextData();
	static void DoFileAction( CWnd*pWnd , int nAction, CMp3FileList* mp3list);
	void GetCopyData( CString& strInformation );
	int GetSelectedFiles( CMp3FileList& mp3list );
	void DeleteColumns();
	void SetColumnPref(CColumn *pCol, int nId);
	void SetColumnWidthPref(CColumn *pCol, int nId, int nDefWidth);
	void SetDisplayedColumns();
	void DefineDisplayedColumns();
	void InitColumns();
	BOOL IsSelected();
	void UpdateList();

	int GetDropFiles( CStringArray& filelist );
	HGLOBAL GetHDropData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMp3View)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMp3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnUpdateEditCopyCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	//{{AFX_MSG(CMp3View)
	afx_msg void OnUpdateFilePlay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileEnqueue(CCmdUI* pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnUpdateFileExport(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnFilePlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileEnqueue(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MP3VIEW_H__B8036FE1_06FA_11D3_BEDD_00400556855E__INCLUDED_)
