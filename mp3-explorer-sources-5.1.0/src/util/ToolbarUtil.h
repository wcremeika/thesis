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
 

// these constants represent the dimensions and number of buttons in
// the default MFC-generated toolbar. If you need something different,
// feel free to change them. For extra credit, you can load the
// toolbar's existing image list at runtime and copy the parameters from
// there.
static const UINT	kToolBarBitDepth (ILC_COLOR24);

// this color will be treated as transparent in the loaded bitmaps --
// in other words, any pixel of this color will be set at runtime to
// the user's button color. The Visual Studio toolbar editor defaults
// to 192, 192, 192 (light gray).
static const RGBTRIPLE	kBackgroundColor = {255, 255, 255};

void AttachToolbarImages (	CToolBar& wndToolBar, int nNumImages,
						    int nItemWidth, int nItemHeight, CSize sizeButton,
							UINT inNormalImageID, CImageList& ilToolbarImages,
                            UINT inDisabledImageID,CImageList& ilToolbarImagesDisabled,
                            UINT inHotImageID, CImageList& ilToolbarImagesHot);

