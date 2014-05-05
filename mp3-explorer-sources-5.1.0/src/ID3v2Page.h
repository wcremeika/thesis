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
 

#ifndef __ID3V2PAGE_H__
#define __ID3V2PAGE_H__

#include "util/ComboBoxAuto.h"
#include "SortedList.h"

/////////////////////////////////////////////////////////////////////////////
// CID3v1Dlg dialog

class CID3v1Dlg : public CDialog
{

// Construction
public:
	CMyImageList m_imagelistComboGenre;
	CMyImageList m_imagelistComboArtist;
	CMyImageList m_imagelistComboAlbum;
	CComboBoxAuto m_comboGenre;
	CComboBoxAuto m_comboArtist;
	CComboBoxAuto m_comboAlbum;

	BOOL m_bRenameAvailable;
	CMapStringToOb* m_pMapArtist;
	CMapStringToOb* m_pMapGenre;
	CSortedList*	m_pListArtist;
	CSortedList*	m_pListGenre;

	CID3v1Dlg(CWnd* pParent = NULL);
	~CID3v1Dlg();

// Dialog Data
	//{{AFX_DATA(CID3v1Dlg)
	enum { IDD = IDD_ID3V1PAGE1 };
	CString	m_strAlbum;
	CString	m_strComment;
	CString	m_strTitle;
	CString	m_strYear;
	CString	m_strArtist;
	BOOL	m_bRename;
	CString	m_strTrackNum;
	CString	m_strGenre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CID3v1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CID3v1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownListAlbum();
	afx_msg void OnNormalize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CID3v2Page1 dialog

class CID3v2Page1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CID3v2Page1)

// Construction
public:
	CMyImageList m_imagelistComboGenre;
	CMyImageList m_imagelistComboArtist;
	CMyImageList m_imagelistComboAlbum;
	CComboBoxAuto m_comboGenre;
	CComboBoxAuto m_comboArtist;
	CComboBoxAuto m_comboAlbum;

	BOOL m_bRenameAvailable;

	CSortedList*	m_pListArtist;
	CSortedList*	m_pListGenre;
	CMapStringToOb* m_pMapArtist;
	CMapStringToOb* m_pMapGenre;

//	CMapStringToOb* m_pMapGenre;

	int		m_nRating;
	CID3v2Page1();
	~CID3v2Page1();

// Dialog Data
	//{{AFX_DATA(CID3v2Page1)
	enum { IDD = IDD_ID3V2PAGE1 };
	CString	m_strAlbum;
	CString	m_strComment;
	CString	m_strTitle;
	CString	m_strYear;
	CString	m_strArtist;
	BOOL	m_bRename;
	CString	m_strArtistUrl;
	CString	m_strTrackNum;
	CString	m_strGenre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CID3v2Page1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CID3v2Page1)
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownListAlbum();
	afx_msg void OnSelchangeCmbRating();
	afx_msg void OnNormalize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CID3v2Page2 dialog

class CID3v2Page2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CID3v2Page2)

// Construction
public:
	CID3v2Page2();
	~CID3v2Page2();

// Dialog Data
	//{{AFX_DATA(CID3v2Page2)
	enum { IDD = IDD_ID3V2PAGE2 };
	CString	m_strLyrics;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CID3v2Page2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CID3v2Page2)
	afx_msg void OnSetfocusEditLyrics();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CID3v2Page3 dialog

class CID3v2Page3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CID3v2Page3)

// Construction
public:
	BOOL LoadPicture( const char* strFilename );
	CString m_strPictureFile;
	CID3v2Page3();
	~CID3v2Page3();
	HBITMAP       m_hBitmap;
	HPALETTE      m_hPalette;
// Dialog Data
	//{{AFX_DATA(CID3v2Page3)
	enum { IDD = IDD_ID3V2PAGE3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CID3v2Page3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CID3v2Page3)
	afx_msg void OnButtonSelect();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __ID3V2PAGE_H__
