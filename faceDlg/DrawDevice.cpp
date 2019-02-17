//------------------------------------------------------------------------------
// <copyright file="DrawDevice.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Manages the drawing of bitmap data

#include "DrawDevice.h"

inline LONG Width(const RECT& r)
{
	return r.right - r.left;
}

inline LONG Height(const RECT& r)
{
	return r.bottom - r.top;
}

//-------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------
DrawDevice::DrawDevice() :
	m_hwnd(0),
	m_sourceWidth(0),
	m_sourceHeight(0),
	m_stride(0),
	m_rotation(0),
	m_pD2DFactory(NULL),
	m_pRenderTarget(NULL),
	m_pBitmap(0),
	fact_detected(false)
{
}

//-------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------
DrawDevice::~DrawDevice()
{
	DiscardResources();
	SafeRelease(m_pD2DFactory);
}

//-------------------------------------------------------------------
// EnsureResources
//
// Ensure necessary Direct2d resources are created
//-------------------------------------------------------------------
HRESULT DrawDevice::EnsureResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{

		D2D1_SIZE_U size = D2D1::SizeU(m_sourceWidth, m_sourceHeight);
		//RECT rc;
		//GetClientRect(m_hwnd, &rc);
		//D2D1_SIZE_U size = D2D1::SizeU(
		//	rc.right - rc.left,
		//	rc.bottom - rc.top
		//);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		if (FAILED(hr)) {
			return hr;
		}

		// Create a bitmap that we can copy image data into and then render to the target
		hr = m_pRenderTarget->CreateBitmap(
			D2D1::SizeU(m_sourceWidth, m_sourceHeight),
			D2D1::BitmapProperties(
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE
				)),
			&m_pBitmap
		);

		if (FAILED(hr)) {
			SafeRelease(m_pRenderTarget);
			return hr;
		}

		float dashes[] = { 1.0f, 2.0f, 2.0f, 3.0f, 2.0f, 2.0f };
		hr = m_pD2DFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&m_pStrokeStyle
		);

		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Green, 1.0f),
			&m_pBlackBrush
		);
		//Rotate the view
		m_imageCenter = D2D1::Point2F(
			m_sourceWidth / 2.0f, m_sourceHeight / 2.0f
		);
		if (90 == m_rotation || -90 == m_rotation) {
			// the rotation matrix
			D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(
				(float)m_rotation,
				m_imageCenter
			);
			// after rotation, part of the image may be out of the view. So we
			// need to scale the image so that it can be fitted into the view
			// the scaling matrix.
			// use negative Y scaling to flip the image, because the bitmap buffer
			// is upside-down
			D2D1_SIZE_F scaleXY = D2D1::SizeF(1.333f, -0.75f);
			D2D1::Matrix3x2F scaling = D2D1::Matrix3x2F::Scale(
				scaleXY, m_imageCenter
			);
			m_pRenderTarget->SetTransform(rotation * scaling);
		}
		else {
			// use negative Y scaling to flip the image, because the bitmap buffer
			// is upside-down
			D2D1_SIZE_F scaleXY = D2D1::SizeF(1.0f, -1.0f);
			D2D1::Matrix3x2F scaling = D2D1::Matrix3x2F::Scale(
				scaleXY, m_imageCenter
			);
			m_pRenderTarget->SetTransform(scaling);
		}
	}

	return hr;
}

//-------------------------------------------------------------------
// DiscardResources
//
// Dispose Direct2d resources 
//-------------------------------------------------------------------
void DrawDevice::DiscardResources()
{
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pBitmap);
}

//-------------------------------------------------------------------
// Initialize
//
// Set the window to draw to, video format, etc.
//-------------------------------------------------------------------
bool DrawDevice::Initialize(
	HWND hwnd_img,
	ID2D1Factory * pD2DFactory,
	int sourceWidth,
	int sourceHeight,
	int Stride,
	int rotation
)
{
	m_hwnd = hwnd_img;
	// One factory for the entire application so save a pointer here
	m_pD2DFactory = pD2DFactory;

	m_pD2DFactory->AddRef();

	// Get the frame size
	m_stride = Stride;
	m_sourceWidth = sourceWidth;
	m_sourceHeight = sourceHeight;

	// display rotation (in degrees)
	m_rotation = -rotation;
	/*if (m_rotation == 0)
	m_rotation = 180;*/

	return true;
}


//-------------------------------------------------------------------
// DrawFrame
//
// Draw the video frame.
//-------------------------------------------------------------------

bool DrawDevice::Draw(BYTE * pBits, unsigned long cbBits)
{
	// incorrectly sized image data passed in
	if (cbBits < ((m_sourceHeight - 1) * m_stride) + (m_sourceWidth * 4))
	{
		return false;
	}

	// create the resources for this draw device
	// they will be recreated if previously lost
	HRESULT hr = EnsureResources();

	if (FAILED(hr))
	{
		return false;
	}

	// Copy the image that was passed in into the direct2d bitmap
	hr = m_pBitmap->CopyFromMemory(NULL, pBits, m_stride);

	if (FAILED(hr))
	{
		return false;
	}

	m_pRenderTarget->BeginDraw();

	//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SpringGreen));

	// Draw the bitmap stretched to the size of the window
	D2D1_SIZE_F size = m_pBitmap->GetSize();

	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(2.f, 0.f);
	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

	m_pRenderTarget->DrawBitmap(
		m_pBitmap/*,
				 D2D1::RectF(
				 upperLeftCorner.x,
				 upperLeftCorner.y,
				 upperLeftCorner.x + rtSize.width,
				 upperLeftCorner.y + rtSize.height)*/
	);
	/*FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;*/

	if (fact_detected) {
		D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
			D2D1::RectF(face_rect.left(), 480-face_rect.top(), face_rect.right(), 480-face_rect.bottom()),
			10.f,
			10.f
		);
		m_pRenderTarget->DrawRoundedRectangle(roundedRect, m_pBlackBrush, 5.f, m_pStrokeStyle);
	}


	D2D1_SIZE_F scaleXY = D2D1::SizeF(1.0f, -1.0f);
	D2D1::Matrix3x2F scaling = D2D1::Matrix3x2F::Scale(
		scaleXY, m_imageCenter
	);
	m_pRenderTarget->SetTransform(scaling);


	hr = m_pRenderTarget->EndDraw();

	// Device lost, need to recreate the render target
	// We'll dispose it now and retry drawing
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardResources();
	}

	return SUCCEEDED(hr);
}

