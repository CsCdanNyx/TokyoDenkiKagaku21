#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

typedef struct Target
{
	int digit;
	cv::Mat pic;
	cv::Point center;
	cv::Rect bound;
};

int text_train();
int text_recong(Target &target);



