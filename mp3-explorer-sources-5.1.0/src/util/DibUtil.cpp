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
 

#include "../stdafx.h"
#include "DibUtil.h"

/* DIB macros */ 
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 
// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits" 
// parameter is the bit count for the scanline (biWidth * biBitCount), 
// and this macro returns the number of DWORD-aligned bytes needed  
// to hold those bits.  
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)  
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')   


WORD DIBNumColors(LPSTR lpDIB) 
{
	WORD wBitCount;
	// DIB bit count
	// If this is a Windows-style DIB, the number of colors in the
	// color table can be less than the number of bits per pixel
	// allows for (i.e. lpbi->biClrUsed can be set to some value).
	// If this is the case, return the appropriate value.
	if (IS_WIN30_DIB(lpDIB))
	{
		DWORD dwClrUsed;
		dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
		if (dwClrUsed)
			return (WORD)dwClrUsed;
	}      
	// Calculate the number of colors in the color table based on
	// the number of bits per pixel for the DIB.
	if (IS_WIN30_DIB(lpDIB))
		wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	// return number of colors based on bits per pixel
	switch (wBitCount)
	{ 
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
     }
}
  
WORD PaletteSize(LPSTR lpDIB) 
{
	// calculate the size required by the palette
	if (IS_WIN30_DIB (lpDIB))
		return (DIBNumColors(lpDIB) * sizeof(RGBQUAD));
	else
		return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
};

   
BOOL SaveDIB(HANDLE hDib, LPCSTR lpFileName) 
{ 
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file 
	LPBITMAPINFOHEADER lpBI; // Pointer to DIB info structure 
	HANDLE fh; // file handle for opened file 
	DWORD dwDIBSize; 
	DWORD dwWritten; 

	if (!hDib) 
		return FALSE; 

	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE; 

	// Get a pointer to the DIB memory, the first of which contains 
	// a BITMAPINFO structure 

	lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	if (!lpBI) 
	{ 
		CloseHandle(fh); 
		return FALSE; 
	} 

	// Check to see if we're dealing with an OS/2 DIB. If so, don't 
	// save it because our functions aren't written to deal with these 
	// DIBs. 

	if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
	{ 
		GlobalUnlock(hDib); 
		CloseHandle(fh); 
		return FALSE; 
	} 

	// Fill in the fields of the file header 

	// Fill in file type (first 2 bytes must be "BM" for a bitmap) 

	bmfHdr.bfType = DIB_HEADER_MARKER; // "BM" 

	// Calculating the size of the DIB is a bit tricky (if we want to 
	// do it right). The easiest way to do this is to call GlobalSize() 
	// on our global handle, but since the size of our global memory may have 
	// been padded a few bytes, we may end up writing out a few too 
	// many bytes to the file (which may cause problems with some apps, 
	// like HC 3.0). 
	// 
	// So, instead let's calculate the size manually. 
	// 
	// To do this, find size of header plus size of color table. Since the 
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains 
	// the size of the structure, let's use this. 

	// Partial Calculation 

	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI); 

	// Now calculate the size of the image 

	// It's an RLE bitmap, we can't calculate size, so trust the biSizeImage 
	// field 

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
		dwDIBSize += lpBI->biSizeImage; 
	else 
	{ 
		DWORD dwBmBitsSize; // Size of Bitmap Bits only 

		// It's not RLE, so size is Width (DWORD aligned) * Height 

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
		lpBI->biHeight; 

		dwDIBSize += dwBmBitsSize; 

		// Now, since we have calculated the correct size, why don't we 
		// fill in the biSizeImage field (this will fix any .BMP files which 
		// have this field incorrect). 

		lpBI->biSizeImage = dwBmBitsSize; 
	} 


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER) 

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 

	// Now, calculate the offset the actual bitmap bits will be in 
	// the file -- It's the Bitmap file header plus the DIB header, 
	// plus the size of the color table. 

	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
	PaletteSize((LPSTR)lpBI); 

	// Write the file header 

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

	// Write the DIB header and the bits -- use local version of 
	// MyWrite, so we can write more than 32767 bytes of data 

	WriteFile(fh, (LPSTR)lpBI, dwDIBSize, &dwWritten, NULL); 

	GlobalUnlock(hDib); 
	CloseHandle(fh); 

	if (dwWritten == 0) 
		return FALSE; // oops, something happened in the write 
	else 
		return TRUE; // Success code 
} 

BOOL LoadBitmapFromBMPFile( LPCTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette )
{
	BITMAP  bm;

	*phBitmap = NULL;
	*phPalette = NULL;

	// Use LoadImage() to get the image loaded into a DIBSection
	*phBitmap = (HBITMAP) LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0,
                 LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if( *phBitmap == NULL )
		return FALSE;

	// Get the color depth of the DIBSection
	GetObject(*phBitmap, sizeof(BITMAP), &bm );
	// If the DIBSection is 256 color or less, it has a color table
	if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
	{
		HDC           hMemDC;
		HBITMAP       hOldBitmap;
		RGBQUAD       rgb[256];
		LPLOGPALETTE  pLogPal;
		WORD          i;

		// Create a memory DC and select the DIBSection into it
		hMemDC = CreateCompatibleDC( NULL );
		hOldBitmap = (HBITMAP) SelectObject( hMemDC, *phBitmap );
		// Get the DIBSection's color table
		GetDIBColorTable( hMemDC, 0, 256, rgb );
		// Create a palette from the color table
		pLogPal = (LPLOGPALETTE) malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;
		for( i = 0 ; i < 256 ; i++)
		{
			pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		*phPalette = CreatePalette( pLogPal );
		// Clean up
		free( pLogPal );
		SelectObject( hMemDC, hOldBitmap );
		DeleteDC( hMemDC );
	}
	else   // It has no color table, so use a halftone palette
	{
		HDC    hRefDC;

		hRefDC = GetDC( NULL );
		*phPalette = CreateHalftonePalette( hRefDC );
		ReleaseDC( NULL, hRefDC );
	}
	return TRUE;
}

/************************************************************************* 
* 
* BitmapToDIB() 
* 
* Parameters: 
* 
* HBITMAP hBitmap - specifies the bitmap to convert 
* 
* HPALETTE hPal - specifies the palette to use with the bitmap 
* 
* Return Value: 
* 
* HDIB - identifies the device-dependent bitmap 
* 
* Description: 
* 
* This function creates a DIB from a bitmap using the specified palette. 
* 
************************************************************************/ 

HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal) 
{ 
	BITMAP bm; // bitmap structure 
	BITMAPINFOHEADER bi; // bitmap header 
	LPBITMAPINFOHEADER lpbi; // pointer to BITMAPINFOHEADER 
	DWORD dwLen; // size of memory block 
	HANDLE hDIB, h; // handle to DIB, temp handle 
	HDC hDC; // handle to DC 
	WORD biBits; // bits per pixel 

	// check if bitmap handle is valid 

	if (!hBitmap) 
		return NULL; 

	// fill in BITMAP structure, return NULL if it didn't work 

	if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm)) 
		return NULL; 

	// if no palette is specified, use default palette 

	if (hPal == NULL) 
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE); 

	// calculate bits per pixel 

	biBits = bm.bmPlanes * bm.bmBitsPixel; 

	// make sure bits per pixel is valid 

	if (biBits <= 1) 
		biBits = 1; 
	else if (biBits <= 4) 
		biBits = 4; 
	else if (biBits <= 8) 
		biBits = 8; 
	else // if greater than 8-bit, force to 24-bit 
		biBits = 24; 

	// initialize BITMAPINFOHEADER 

	bi.biSize = sizeof(BITMAPINFOHEADER); 
	bi.biWidth = bm.bmWidth; 
	bi.biHeight = bm.bmHeight; 
	bi.biPlanes = 1; 
	bi.biBitCount = biBits; 
	bi.biCompression = BI_RGB; 
	bi.biSizeImage = 0; 
	bi.biXPelsPerMeter = 0; 
	bi.biYPelsPerMeter = 0; 
	bi.biClrUsed = 0; 
	bi.biClrImportant = 0; 

	// calculate size of memory block required to store BITMAPINFO 

	dwLen = bi.biSize + PaletteSize((LPSTR)&bi); 

	// get a DC 

	hDC = GetDC(NULL); 

	// select and realize our palette 

	hPal = SelectPalette(hDC, hPal, FALSE); 
	RealizePalette(hDC); 

	// alloc memory block to store our bitmap 

	hDIB = GlobalAlloc(GHND, dwLen); 

	// if we couldn't get memory block 

	if (!hDIB) 
	{ 
	// clean up and return NULL 

		SelectPalette(hDC, hPal, TRUE); 
		RealizePalette(hDC); 
		ReleaseDC(NULL, hDC); 
		return NULL; 
	} 

	// lock memory and get pointer to it 

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 

	/// use our bitmap info. to fill BITMAPINFOHEADER 

	*lpbi = bi; 

	// call GetDIBits with a NULL lpBits param, so it will calculate the 
	// biSizeImage field for us 

	GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, (LPBITMAPINFO)lpbi, 
	DIB_RGB_COLORS); 

	// get the info. returned by GetDIBits and unlock memory block 

	bi = *lpbi; 
	GlobalUnlock(hDIB); 

	// if the driver did not fill in the biSizeImage field, make one up 
	if (bi.biSizeImage == 0) 
	bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight; 

	// realloc the buffer big enough to hold all the bits 

	dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage; 

	if (h = GlobalReAlloc(hDIB, dwLen, 0)) 
		hDIB = h; 
	else 
	{ 
		// clean up and return NULL 

		GlobalFree(hDIB); 
		hDIB = NULL; 
		SelectPalette(hDC, hPal, TRUE); 
		RealizePalette(hDC); 
		ReleaseDC(NULL, hDC); 
		return NULL; 
	} 

	// lock memory block and get pointer to it */ 

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 

	// call GetDIBits with a NON-NULL lpBits param, and actualy get the 
	// bits this time 

	if (GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, (LPSTR)lpbi + 
	(WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi), (LPBITMAPINFO)lpbi, 
	DIB_RGB_COLORS) == 0) 
	{ 
	// clean up and return NULL 

		GlobalUnlock(hDIB); 
		hDIB = NULL; 
		SelectPalette(hDC, hPal, TRUE); 
		RealizePalette(hDC); 
		ReleaseDC(NULL, hDC); 
		return NULL; 
	}	 

	bi = *lpbi; 

	// clean up 
	GlobalUnlock(hDIB); 
	SelectPalette(hDC, hPal, TRUE); 
	RealizePalette(hDC); 
	ReleaseDC(NULL, hDC); 

	// return handle to the DIB 
	return hDIB; 
} 





