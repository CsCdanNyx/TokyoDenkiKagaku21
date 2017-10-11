#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

vector<Point> contoursConvexHull(vector<vector<Point> > contours)
{
	vector<Point> result;
	vector<Point> pts;
	for (size_t i = 0; i< contours.size(); i++)
		for (size_t j = 0; j< contours[i].size(); j++)
			pts.push_back(contours[i][j]);
	convexHull(pts, result);
	return result;
}

int main(int, char** argv)
{
	Mat src, srcGray, srcBlur, srcCanny;

	VideoCapture video(1);
	if (!video.isOpened()) {
		return -1;
	}

	int t_min = 100, t_max = 200;
	namedWindow("bar", 0);
	createTrackbar("min", "bar", &t_min, 300);
	createTrackbar("max", "bar", &t_max, 300);
	namedWindow("Contours");
	namedWindow("contoursConvexHull");
	while (1)
	{
		video >> src;
		cvtColor(src, srcGray, CV_BGR2GRAY);
		blur(srcGray, srcBlur, Size(3, 3));

		Canny(srcBlur, srcCanny, t_min, t_max, 3, true);

		vector<vector<Point> > contours;

		findContours(srcCanny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		Mat drawing = Mat::zeros(srcCanny.size(), CV_8UC3);

		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(0, 255, 0);
			drawContours(drawing, contours, i, color, 2);
		}

		vector<Point> ConvexHullPoints = contoursConvexHull(contours);

		polylines(drawing, ConvexHullPoints, true, Scalar(0, 0, 255), 2);
		imshow("Contours", drawing);
		polylines(src, ConvexHullPoints, true, Scalar(0, 0, 255), 2);
		imshow("contoursConvexHull", src);
		waitKey(50);
	
	}
	return 0;
}