#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

public ref class GIFImg
{
private: 
	Bitmap^ _gifImage;
	FrameDimension^ _dimension;
	int _frameCount;
	int _currentFrame;
	bool _reverse;
	int _step;

public:
	GIFImg(String^ path);
	Image^ GetNextFrame();
	Image^ GetFrame(int index);
};
