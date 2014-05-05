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
 

#ifndef __OPTIONPAGE_H__
#define __OPTIONPAGE_H__

#include "util/TransBitmap.h"
#include "mp3/Mp3File.h"	// Added by ClassView
#include "NormalizationProfile.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// COptionPage1 dialog

class COptionPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage1)

// Construction
public:
	COptionPage1();
	~COptionPage1();

// Dialog Data
	//{{AFX_DATA(COptionPage1)
	enum { IDD = IDD_OPTIONPAGE1 };
	BOOL m_bFullRowSelection;
	BOOL m_bJustifyLeft;
	BOOL m_bJustifyCenter;
	BOOL m_bActionPlay;
	BOOL m_bActionEnqueue;
	BOOL m_bActionProperties;
	UINT m_nMaxRow;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionPage2 dialog

class COptionPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage2)

// Construction
public:
	int m_nFontWidth;
	CFont m_font;
	CTransBitmap m_bmCheck;
	int m_nFontHeight;
	CString GetColumnText( CColumn *pCol);
	void FillListBox();
	void ResetList();
	COptionPage2();
	~COptionPage2();
	CObArray  m_columns;
	void GetColumnsInfos( CObArray& columns );
	void SetColumnsInfos( CObArray& columns );
	int InsertSorted( CColumn *pCol );
// Dialog Data
	//{{AFX_DATA(COptionPage2)
	enum { IDD = IDD_OPTIONPAGE2 };
	CListBox	m_listbox;
	BOOL	m_bDisplayed;
	BOOL	m_bLeftAlign;
	BOOL	m_bCenterAlign;
	BOOL	m_bRightAlign;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeColumnList();
	afx_msg void OnLeftAlign();
	afx_msg void OnRightAlign();
	afx_msg void OnCenterAlign();
	afx_msg void OnDisplayLimited();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDblclkColumnList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage3 dialog

class COptionPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage3)

// Construction
public:
//	CString m_strFavoriteAddresses;
	COptionPage3();
	~COptionPage3();
// Dialog Data
	//{{AFX_DATA(COptionPage3)
	enum { IDD = IDD_OPTIONPAGE3 };
	CString	m_strFavoriteAddresses;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage4 dialog

class COptionPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage4)

// Construction
public:
	COptionPage4();
	~COptionPage4();
// Dialog Data
	//{{AFX_DATA(COptionPage4)
	enum { IDD = IDD_OPTIONPAGE4 };
	CString	m_strCmdEnqueue;
	CString	m_strCmdPlay;
	BOOL	m_bUseCachedCatalog;
	BOOL	m_bDisableConnection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage5 dialog

class COptionPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage5)


// Construction
public:
	void UpdatePreview();
	CMp3File m_mp3;
	void UpdateList();
	void FreeFormats();
	void LoadFormats();
	void SaveFormats();
	CObArray m_formatList;
	COptionPage5();
	~COptionPage5();

protected:
	int m_nFontWidth;
	CFont m_font;
	CTransBitmap m_bmCheck;
	int m_nFontHeight;

	// Dialog Data
	//{{AFX_DATA(COptionPage5)
	enum { IDD = IDD_OPTIONPAGE5 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage5)
	afx_msg void OnAddFormat();
	afx_msg void OnDeleteFormat();
	afx_msg void OnModifyFormat();
	afx_msg void OnFormatDefault();
	afx_msg void OnDblclkListFormats();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListFormats();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage6 dialog

class COptionPage6 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage6)

// Construction
public:
	void UpdateFields();
	BOOL m_bDefaultExport;
	BOOL m_bChooseExportFile;
	CString m_strExportDefFile;
	COptionPage6();
	~COptionPage6();

// Dialog Data
	//{{AFX_DATA(COptionPage6)
	enum { IDD = IDD_OPTIONPAGE6 };
	BOOL	m_bExportReinit;
	CString	m_strExportDefName;
	CString	m_strExportFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage6)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage6)
	afx_msg void OnChooseExport();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioChooseFile();
	afx_msg void OnRadioDefaultFile();
	afx_msg void OnRadioFormatDefault();
	afx_msg void OnRadioFormatSpecific();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage7 dialog

class COptionPage7 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage7)

// Construction
public:
	COptionPage7();
	~COptionPage7();

// Dialog Data
	//{{AFX_DATA(COptionPage7)
	enum { IDD = IDD_OPTIONPAGE7 };
	BOOL	m_bScanM3u;
	BOOL	m_bScanPls;
	BOOL	m_bScanMp3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage7)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage7)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage8 dialog

class COptionPage8 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage8)

// Construction
public:
	CString m_strCurrentDll;
	int m_nIndexNone;
	COptionPage8();
	~COptionPage8();

// Dialog Data
	//{{AFX_DATA(COptionPage8)
	enum { IDD = IDD_OPTIONPAGE8 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage8)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage8)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboPlugins();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionPage9 dialog

class COptionPage9 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage9)

// Construction
public:
	int GetFieldIndex();
	CNormalizationProfile m_profile;
	COptionPage9();
	~COptionPage9();

// Dialog Data
	//{{AFX_DATA(COptionPage9)
	enum { IDD = IDD_OPTIONPAGE9 };
	BOOL	m_bValidate;
	BOOL	m_bNormalize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage9)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage9)
	afx_msg void OnNochange();
	afx_msg void OnLower();
	afx_msg void OnUpper();
	afx_msg void OnProper();
	afx_msg void OnFirstUpper();
	afx_msg void OnReplaceUnderscore();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFieldList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage10 dialog

class COptionPage10 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage10)

// Construction
public:
	void UpdateUIWriteId3v1Tags();
	COptionPage10();
	~COptionPage10();

// Dialog Data
	//{{AFX_DATA(COptionPage10)
	enum { IDD = IDD_OPTIONPAGE10 };
	BOOL	m_bWriteId3v1Tags;
	BOOL	m_bUseId3v2;
	BOOL	m_bUseId3v1Only;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage10)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage10)
	afx_msg void OnRadioId3v2();
	afx_msg void OnRadioId3v1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// COptionPage11 dialog

class COptionPage11 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPage11)

// Construction
public:
	CString m_strCurrentLanguage;
	int m_nIndexNone;
	COptionPage11();
	~COptionPage11();

// Dialog Data
	//{{AFX_DATA(COptionPage11)
	enum { IDD = IDD_OPTIONPAGE11 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPage11)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPage11)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLanguages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif // __OPTIONPAGE_H__
