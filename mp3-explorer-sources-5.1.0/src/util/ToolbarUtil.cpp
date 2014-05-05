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
#include "ToolbarUtil.h"


// find every pixel of the default background color in the specified
// bitmap and set each one to the user's button color.
static void	ReplaceBackgroundColor (CBitmap& ioBM)
{
	// figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;

	VERIFY (ioBM.GetBitmap (&bmInfo));

	// add support for additional bit depths if you choose
	VERIFY (bmInfo.bmBitsPixel == 24);
//	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));

	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);

	// get a pointer to the pixels
	DIBSECTION  ds;

	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));

	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	VERIFY (pixels != NULL);

	// get the user's preferred button color from the system
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = {
	GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

	// search through the pixels, substituting the user's button
	// color for any pixel that has the magic background color
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels [i].rgbtBlue == kBackgroundColor.rgbtBlue 
		&& pixels [i].rgbtGreen == kBackgroundColor.rgbtGreen 
		&& pixels [i].rgbtRed == kBackgroundColor.rgbtRed)
		{
			pixels [i] = userBackgroundColor;
		}
	}
}


// create an image list for the specified BMP resource
static void	MakeToolbarImageList (UINT inBitmapID,
	  						    int nItemWidth, int nItemHeight,
                                CImageList&	outImageList, int nNumImages)
{
	CBitmap		bm;

	// if we use CBitmap::LoadBitmap() to load the bitmap, the colors
	// will be reduced to the bit depth of the main screen and we won't
	// be able to access the pixels directly. To avoid those problems,
	// we'll load the bitmap as a DIBSection instead and attach the
	// DIBSection to the CBitmap.
	VERIFY (bm.Attach (::LoadImage (::AfxFindResourceHandle(
	MAKEINTRESOURCE (inBitmapID), RT_BITMAP),
	MAKEINTRESOURCE (inBitmapID), IMAGE_BITMAP, 0, 0,
	(LR_DEFAULTSIZE | LR_CREATEDIBSECTION))));

	// replace the specified color in the bitmap with the user's
	// button color
	::ReplaceBackgroundColor (bm);

	// create a 24 bit image list with the same dimensions and number
	// of buttons as the toolbar
	VERIFY (outImageList.Create (
	nItemWidth, nItemHeight, kToolBarBitDepth, nNumImages, 0));

	// attach the bitmap to the image list
	VERIFY (outImageList.Add (&bm, RGB (0, 0, 0)) != -1);
}


// load the high color toolbar images and attach them to m_wndToolBar
void AttachToolbarImages (	CToolBar& wndToolBar, int nNumImages,
							    int nItemWidth, int nItemHeight, CSize sizeButton,
								UINT inNormalImageID, CImageList& ilToolbarImages,
                                UINT inDisabledImageID,CImageList& ilToolbarImagesDisabled,
                                UINT inHotImageID, CImageList& ilToolbarImagesHot) 
{
	// make high-color image lists for each of the bitmaps
	::MakeToolbarImageList (inNormalImageID, nItemWidth, nItemHeight, ilToolbarImages, nNumImages);
	::MakeToolbarImageList (inDisabledImageID, nItemWidth, nItemHeight, ilToolbarImagesDisabled, nNumImages);
	::MakeToolbarImageList (inHotImageID, nItemWidth, nItemHeight, ilToolbarImagesHot, nNumImages);

	// get the toolbar control associated with the CToolbar object
	CToolBarCtrl&	barCtrl = wndToolBar.GetToolBarCtrl();

	// attach the image lists to the toolbar control
	barCtrl.SetImageList (&ilToolbarImages);
	barCtrl.SetDisabledImageList (&ilToolbarImagesDisabled);
	barCtrl.SetHotImageList (&ilToolbarImagesHot);
	barCtrl.SetButtonSize( sizeButton );
}


