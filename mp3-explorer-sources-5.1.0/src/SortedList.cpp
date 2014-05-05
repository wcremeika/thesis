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
#include "SortedList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortedList::CSortedList()
{
	m_bValid = FALSE;

}

CSortedList::~CSortedList()
{
	RemoveAll();
}

void CSortedList::Build( _TCHAR* lpszItem[] , int nItemCount )
{
	RemoveAll();

	for( int i = 0 ; i < nItemCount ; i++ )
	{
		BOOL bInserted = FALSE;
		POSITION pos = GetHeadPosition();
		while( pos != NULL )
		{
			if( GetAt( pos ) > lpszItem[i]  )
			{
				InsertBefore( pos, lpszItem[i]  );
				bInserted = TRUE;
				break;
			}
			GetNext( pos );
		}
		if( !bInserted )
		{
			AddTail( lpszItem[i] );
		}
	}
}



void CSortedList::Build(CMapStringToOb *pMap)
{
	RemoveAll();


	POSITION pos = pMap->GetStartPosition();

	while( pos != NULL )
	{
		CObject* pObject;
		CString strItem;
		pMap->GetNextAssoc( pos, strItem, (CObject*&)pObject );

		BOOL bInserted = FALSE;
		POSITION pos2 = GetHeadPosition();
		while( pos2 != NULL )
		{
			if( strItem < GetAt( pos2 ) )
			{
				InsertBefore( pos2, strItem );
				bInserted = TRUE;
				break;
			}
			GetNext( pos2 );
		}
		if( !bInserted )
		{
			AddTail( strItem );
		}

	}
	m_bValid = TRUE;
}

void CSortedList::Invalidate()
{
	m_bValid = FALSE;
}


CStringList* CSortedList::GetList(CMapStringToOb *pMap)
{
	if( ! m_bValid )
	{
		Build( pMap );
	}
	return (CStringList*) this;
}


CStringList* CSortedList::GetList( _TCHAR* lpszItem[] , int nItemCount )
{
	if( ! m_bValid )
	{
		Build( lpszItem , nItemCount );
	}
	return (CStringList*) this;
}
