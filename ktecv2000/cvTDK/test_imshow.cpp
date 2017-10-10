#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	VideoCapture video(1);
	if (!video.isOpened()) {
		return -1;
	}
	Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
	namedWindow("video demo", CV_WINDOW_AUTOSIZE);
	Mat videoFrame;

	//while (true) 
	{
		video >> videoFrame;
		/*
		if (videoFrame.empty()) {
			break;
		}*/
		imshow("video demo", videoFrame);
		waitKey(33);
	}
	return 0;
}