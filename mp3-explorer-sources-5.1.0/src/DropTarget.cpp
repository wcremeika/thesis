/////////////////////////////////////////////////////////////////////////////
// LstDtarg.cpp : implementation of the CListDropTarget class
//
// Written by Robert Duke
// of Microsoft Product Support Services, Languages Developer Support
// Copyright (c) 1996 Microsoft Corporation. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mp3Exp.h"

#include "MainFrm.h"
#include "Mp3ExpDoc.h"
#include "util/ListVwEx.h"
#include "Mp3ExpView.h"
#include "LeftView.h"
#include "PlaylistDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMp3ExpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CListDropTarget

CListDropTarget::CListDropTarget()
{
	m_ImageFormat = ::RegisterClipboardFormat( CF_MP3_EXPLORER );
}

CListDropTarget::~CListDropTarget()
{
}

DROPEFFECT CListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
                           pDataObject, DWORD dwKeyState, CPoint point )
{   
	// Can we use this object?
	if(!pDataObject->IsDataAvailable(m_ImageFormat))
	{
		m_dropEffectCurrent = DROPEFFECT_NONE;
		return m_dropEffectCurrent;
	}

	if(pWnd->IsKindOf(RUNTIME_CLASS(CLeftView)))
	{
		((CLeftView*)pWnd)->DragEnter();
	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CPlaylistDetailView)))
	{
		((CPlaylistDetailView*)pWnd)->DragEnter();
	}


    // Check if the control key was pressed          
    if((dwKeyState & MK_CONTROL) == MK_CONTROL)
	{
        m_dropEffectCurrent = DROPEFFECT_COPY;
		return m_dropEffectCurrent;
	}
    else
	{
		// We don't do move yet
        m_dropEffectCurrent = DROPEFFECT_MOVE; 
		return m_dropEffectCurrent;
	}
} 

void CListDropTarget::OnDragLeave(CWnd* pWnd)
{
    // Call base class implementation
    COleDropTarget::OnDragLeave(pWnd);

	if(pWnd->IsKindOf(RUNTIME_CLASS(CLeftView)))
	{
		((CLeftView*)pWnd)->DragOverEnd();
	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CPlaylistDetailView)))
	{
		((CPlaylistDetailView*)pWnd)->DragOverEnd();
	}
	
}

DROPEFFECT CListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{
	if(pWnd->IsKindOf(RUNTIME_CLASS(CLeftView)))
	{
		((CLeftView*)pWnd)->DragOverUpdate( point );

	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CPlaylistDetailView)))
	{
		((CPlaylistDetailView*)pWnd)->DragOverUpdate( point );
	}

	return m_dropEffectCurrent;
}

BOOL CListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{ 

	if(pWnd->IsKindOf(RUNTIME_CLASS(CPlaylistDetailView)))
	{
		::SendMessage( theApp.m_pMainWnd->m_hWnd, WM_COMMAND, ID_FILE_ADD_TO_PLAYLIST, 0 );

	}
	if(pWnd->IsKindOf(RUNTIME_CLASS(CLeftView)))
	{
		((CLeftView*)pWnd)->OnDropMp3();

	}
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

