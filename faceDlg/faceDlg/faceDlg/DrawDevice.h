//------------------------------------------------------------------------------
// <copyright file="DrawDevice.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Manages the drawing of bitmap data

#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include "Img_Prepro.h"
// DrawDevice class

class DrawDevice
{
private:
	HWND                     m_hwnd;
	D2D1_POINT_2F			 m_imageCenter;
	// Format information
	UINT                     m_sourceHeight;
	UINT                     m_sourceWidth;
	LONG                     m_stride;
	int	                     m_rotation;		// display rotation (in degrees)

	RECT                     m_rectDest;		// Destination rectangle
	D2D1_RECT_U              m_d2dRectDest;		// Direct2D Destination rectangle

												// Direct2D 
	ID2D1Factory *           m_pD2DFactory;
	ID2D1HwndRenderTarget *  m_pRenderTarget;
	ID2D1Bitmap *            m_pBitmap;

	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1StrokeStyle* m_pStrokeStyle;
public:
	bool	fact_detected;
	rectangle	face_rect;
public:
	DrawDevice();
	virtual ~DrawDevice();

	HRESULT EnsureResources();
	void DiscardResources();

	// Implied bits per pixel is 32
	bool Initialize(
		HWND hwnd_img,
		ID2D1Factory * pD2DFactory,
		int sourceWidth,
		int sourceHeight,
		int Stride,
		int rotation			// display rotation (in degrees)
	);

	// Draw a 32 bit per pixel image of previously specified width and height to the associated hwnd
	bool Draw(BYTE * pBits, unsigned long cbBits);
	bool DrawLine(cv::Rect face_rst);
};

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
