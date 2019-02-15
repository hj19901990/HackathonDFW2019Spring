#include "GIFImg.h"



GIFImg::GIFImg(String^ path)
{
	_reverse = false;
	_currentFrame = -1;
	 _step = 1;
	 _gifImage = gcnew Bitmap(path);
	
	//initialize
	_dimension = gcnew FrameDimension(_gifImage->FrameDimensionsList[0]);
	//gets the GUID
	//total frames in the animation
	_frameCount = _gifImage->GetFrameCount(_dimension);
}
Image^ GIFImg::GetNextFrame(){
	_currentFrame += _step;

	//if the animation reaches a boundary...
	if (_currentFrame >= _frameCount || _currentFrame < 1) {
		if (_reverse) {
			_step *= -1;
			//...reverse the count
			//apply it
			_currentFrame += _step;
		}
		else {
			_currentFrame = 0;
			//...or start over
		}
	}
	return GetFrame(_currentFrame);
}

Image^ GIFImg::GetFrame(int index)
{
	_gifImage->SelectActiveFrame(_dimension, index);
	//find the frame
	return (Image^)_gifImage->Clone();
	//return a copy of it
}

