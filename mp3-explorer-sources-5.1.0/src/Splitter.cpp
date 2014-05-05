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
// Splitter.cpp
// Copyright (c) 1998-2000 Pierre LEVY

#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Note: afxData.cxBorder and afxData.cyBorder aren't used anymore
#define CX_BORDER   1
#define CY_BORDER   1

AFX_STATIC void AFXAPI _AfxLayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray, int nMax, int nSize, int nSizeSplitter);
AFX_STATIC void AFXAPI _AfxDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout, CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);
/*
AFX_STATIC void AFXAPI _AfxDeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= CX_BORDER || cy <= CY_BORDER);
		pWnd->ModifyStyle(bNeedBorder ? 0 : WS_BORDER,
			bNeedBorder ? WS_BORDER : 0);
	}
	CRect rect(x, y, x+cx, y+cy);

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) ||
		  pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		rect.InflateRect( CX_BORDER*2, CY_BORDER*2);

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}
*/
// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
/*
AFX_STATIC void AFXAPI _AfxLayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
	int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			ASSERT(CX_BORDER*2 == CY_BORDER*2);
			(pInfo-1)->nCurSize += nSize + CX_BORDER*2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				ASSERT(CX_BORDER*2 == CY_BORDER*2);
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - CX_BORDER*2))
					pInfo->nCurSize -= (nSizeSplitter - CY_BORDER*2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

*/
/////////////////////////////////////////////////////////////////////////////
// CSplitter
IMPLEMENT_DYNCREATE(CSplitter, CSplitterWnd)

CSplitter::CSplitter()
{ 
    
}

CSplitter::~CSplitter()
{                 
}


BOOL CSplitter::ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size)
{
  CCreateContext context;
  BOOL bSetActive;
	       
	if ((GetPane(row,col)->IsKindOf(pViewClass))==TRUE)
		return FALSE;
				    
   
	// Get pointer to CDocument object so that it can be used in the creation 
	// process of the new view
	CDocument * pDoc= ((CView *)GetPane(row,col))->GetDocument();
	CView * pActiveView=GetParentFrame()->GetActiveView();
	if (pActiveView==NULL || pActiveView==GetPane(row,col))
		bSetActive=TRUE;
	else
		bSetActive=FALSE;

	// set flag so that document will not be deleted when view is destroyed
	pDoc->m_bAutoDelete=FALSE;    
    // Delete existing view 
	((CView *) GetPane(row,col))->DestroyWindow();
    // set flag back to default 
    pDoc->m_bAutoDelete=TRUE;
 
    // Create new view                      
   
	context.m_pNewViewClass=pViewClass;
	context.m_pCurrentDoc=pDoc;
	context.m_pNewDocTemplate=NULL;
	context.m_pLastView=NULL;
	context.m_pCurrentFrame=NULL;
   
	CreateView(row,col,pViewClass,size, &context);
   
	CView * pNewView= (CView *)GetPane(row,col);
	pNewView->OnInitialUpdate();

	if (bSetActive==TRUE)
		GetParentFrame()->SetActiveView(pNewView);
   
//	RecalcLayout(); 
//	GetPane(row,col)->SendMessage(WM_PAINT);

   
   return TRUE;
}
    

BEGIN_MESSAGE_MAP(CSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitter message handlers



//DEL void CSplitter::RecalcLayout()
//DEL {
//DEL 	ASSERT_VALID(this);
//DEL 	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane
//DEL 
//DEL   TRY {
//DEL 	  
//DEL 	CRect rectClient;
//DEL 	GetClientRect(rectClient);
//DEL 	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
//DEL 
//DEL 	CRect rectInside;
//DEL 	GetInsideRect(rectInside);
//DEL 
//DEL 	// layout columns (restrict to possible sizes)
//DEL 	_AfxLayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
//DEL 	_AfxLayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);
//DEL 
//DEL 	// adjust the panes (and optionally scroll bars)
//DEL 
//DEL 	// give the hint for the maximum number of HWNDs
//DEL 	AFX_SIZEPARENTPARAMS layout;
//DEL //PL	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);
//DEL 	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 3);
//DEL 
//DEL 	// size of scrollbars
//DEL 	int cx = (rectClient.right - rectInside.right);
//DEL 	int cy = (rectClient.bottom - rectInside.bottom);
//DEL 
//DEL 	// reposition size box
//DEL 	if (m_bHasHScroll && m_bHasVScroll)
//DEL 	{
//DEL 		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
//DEL 		ASSERT(pScrollBar != NULL);
//DEL 
//DEL 		// fix style if necessary
//DEL 		BOOL bSizingParent = (GetSizingParent() != NULL);
//DEL 		// modifyStyle returns TRUE if style changes
//DEL 		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP|SBS_SIZEBOX,
//DEL 				bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
//DEL 			pScrollBar->Invalidate();
//DEL 		pScrollBar->EnableWindow(bSizingParent);
//DEL 
//DEL 		// reposition the size box
//DEL 		_AfxDeferClientPos(&layout, pScrollBar,
//DEL 			rectInside.right,
//DEL 			rectInside.bottom, cx, cy, TRUE);
//DEL 	}
//DEL 
//DEL 	// reposition scroll bars
//DEL 	if (m_bHasHScroll)
//DEL 	{
//DEL 		int cxSplitterBox = m_cxSplitter;// split box bigger
//DEL 		int x = rectClient.left;
//DEL 		int y = rectInside.bottom;
//DEL 		for (int col = 0; col < m_nCols; col++)
//DEL 		{
//DEL 			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
//DEL 			ASSERT(pScrollBar != NULL);
//DEL 			int cx = m_pColInfo[col].nCurSize;
//DEL 			if (col == 0 && m_nCols < m_nMaxCols)
//DEL 				x += cxSplitterBox, cx -= cxSplitterBox;
//DEL 			_AfxDeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
//DEL 			x += cx + m_cxSplitterGap;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	if (m_bHasVScroll)
//DEL 	{
//DEL 		int cySplitterBox = m_cySplitter;// split box bigger
//DEL 		int x = rectInside.right;
//DEL 		int y = rectClient.top;
//DEL 		for (int row = 0; row < m_nRows; row++)
//DEL 		{
//DEL 			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
//DEL 			ASSERT(pScrollBar != NULL);
//DEL 			int cy = m_pRowInfo[row].nCurSize;
//DEL 			if (row == 0 && m_nRows < m_nMaxRows)
//DEL 				y += cySplitterBox, cy -= cySplitterBox;
//DEL 			_AfxDeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
//DEL 			y += cy + m_cySplitterGap;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	//BLOCK: Reposition all the panes
//DEL 	{
//DEL 		int x = rectClient.left;
//DEL 		for (int col = 0; col < m_nCols; col++)
//DEL 		{
//DEL 			int cx = m_pColInfo[col].nCurSize;
//DEL 			int y = rectClient.top;
//DEL 			for (int row = 0; row < m_nRows; row++)
//DEL 			{
//DEL 				int cy = m_pRowInfo[row].nCurSize;
//DEL 				CWnd* pWnd = GetPane(row, col);
//DEL 				_AfxDeferClientPos(&layout, pWnd, x, y, cx, cy, FALSE);
//DEL 				y += cy + m_cySplitterGap;
//DEL 			}
//DEL 			x += cx + m_cxSplitterGap;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// move and resize all the windows at once!
//DEL 	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
//DEL 		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
//DEL 
//DEL 	// invalidate all the splitter bars (with NULL pDC)
//DEL 	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
//DEL   }
//DEL   CATCH_ALL( e )
//DEL   {
//DEL 	char szBuffer[2001];
//DEL 	e->GetErrorMessage( szBuffer , 2000 );
//DEL   }
//DEL   END_CATCH_ALL
//DEL 
//DEL }

