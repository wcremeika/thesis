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
#include "Mp3Exp.h"
#include "Mp3ExpDoc.h"
#include "MainFrm.h"
#include "HtmlFileView.h"
#include "util/DibUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PICTURE_FILE		"Picture.bmp"

extern CMp3ExpApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CHtmlFileView

IMPLEMENT_DYNCREATE(CHtmlFileView, CHtmlView)

CHtmlFileView::CHtmlFileView()
{
	//{{AFX_DATA_INIT(CHtmlFileView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

CHtmlFileView::~CHtmlFileView()
{
}

void CHtmlFileView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlFileView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlFileView, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlFileView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlFileView diagnostics

#ifdef _DEBUG
void CHtmlFileView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlFileView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CHtmlFileView message handlers

void CHtmlFileView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	if( lHint == UPDATE_HTMLVIEW )
	{
		CMp3ExpDoc* pDoc = (CMp3ExpDoc*) GetDocument();
		CString strUrl = pDoc->GetUrl();
		if( strUrl != "" )
		{
			Navigate2( strUrl );
		}
		else
		{
			LoadFromResource( IDR_HTML_DEFAULT );
		}
	}
	
}



BOOL CHtmlFileView::CreateHtmlPage(CAudioFile *pAudioFile, CString& strUrl)
{
	CString strFilePage = theApp.LoadTranslatedString( "IdsFilePage" );
	CString strFilePageTemplate;
	strFilePageTemplate.LoadString( IDS_FILE_PAGE_TEMPLATE );
	CString strHtmlDir = theApp.m_strInstallPath + HTMLVIEW_DIR;
	strFilePageTemplate = strHtmlDir + strFilePageTemplate;
	strFilePage = strHtmlDir + strFilePage;
	static bNoPicture;

	CStdioFile fileIn;
	CStdioFile fileOut;
	try {

		if( fileIn.Open( strFilePageTemplate , CFile::modeRead ))
		{
			if( fileOut.Open( strFilePage, CFile::modeWrite | CFile::modeCreate ))
			{
				CString strPage;
				CString strLine;
				while( fileIn.ReadString( strLine ))
				{
					strPage += strLine;
				}
				strPage = pAudioFile->FormatExport( strPage );

				// Génération de l'image
				CString strImageFile = strHtmlDir + PICTURE_FILE;
				if( pAudioFile->m_bHasPicture )
				{
					pAudioFile->GetPicture( strImageFile );
					bNoPicture = FALSE;
				}
				else
				{
					// Ne pas réécire l'image si c'est la dernière créee
					if( !bNoPicture )
					{
						
//						HBITMAP hBitmap = ::LoadBitmap( ::AfxGetInstanceHandle(), "NOPICTURE" );
						HBITMAP hBitmap = ::LoadBitmap( ::AfxGetResourceHandle(), "NOPICTURE" );
						HDIB hDib = ::BitmapToDIB( hBitmap , NULL ); 
						if( hDib != NULL )
						{
							::SaveDIB( hDib , strImageFile );
							::DeleteObject( hBitmap );
							::GlobalFree( hDib );
						}
						bNoPicture = TRUE;
					}
				}

				fileOut.WriteString( strPage );
				fileOut.Close();
				fileIn.Close();
				strUrl = strFilePage;
				return TRUE;
			}
		}
	}
	catch( CFileException *e)
	{
		TRACE( "Error Creating HTML file");
		e->Delete();
	}
	return FALSE;

}
