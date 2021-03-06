// TrainAndTest.cpp

#include<iostream>
#include<sstream>
#include<stdlib.h>

#include "text.h"
// global ///////////////////////////////////////////////////////////////////////////////

const int MIN_CONTOUR_AREA = 500;
const int MAX_CONTOUR_AREA = 10000;
const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

const std::string TRAIN_XML_PATH = "E:\\class\\TDK\\cvTDK\\x64\\Debug\\";
///////////////////////////////////////////////////////////////////////////////////////////////////
class ContourWithData {
public:
	// member variables ///////////////////////////////////////////////////////////////////////////
	std::vector<cv::Point> ptContour;           // contour
	cv::Rect boundingRect;                      // bounding rect for contour
	float fltArea;                              // area of contour
	float ratio;
	int shelf_x, shelf_y;									
	bool checkIfContourIsValid() {                            
		ratio = (float)std::max(boundingRect.width,boundingRect.height) / (float)std::min(boundingRect.width, boundingRect.height);
		
		if (fltArea < MIN_CONTOUR_AREA || ratio > 3.0)
		{
			return false;          
		}
		else
		{
			return true;                                        
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
	}

};


///////////////////////////////////////////////////////////////////////////////////////////////////
Task::Task(int camera)
{
	setCamera(camera);
}
int Task::text_recong(cv::Mat src) {
	std::vector<ContourWithData> allContoursWithData;           // declare empty vectors,
	std::vector<ContourWithData> validContoursWithData;         // we will fill these shortly

																// read in training classifications ///////////////////////////////////////////////////

	cv::Mat matClassificationInts;      // we will read the classification numbers into this variable as though it is a vector

	cv::FileStorage fsClassifications(TRAIN_XML_PATH + "classifications.xml", cv::FileStorage::READ);        // open the classifications file

	if (fsClassifications.isOpened() == false) {                                                    // if the file was not opened successfully
		std::cout << "error, unable to open training classifications file, exiting program\n\n";    // show error message
		return(-1);                                                                                  // and exit program
	}

	fsClassifications["classifications"] >> matClassificationInts;      // read classifications section into Mat classifications variable
	fsClassifications.release();                                        // close the classifications file

																		// read in training images ////////////////////////////////////////////////////////////

	cv::Mat matTrainingImagesAsFlattenedFloats;         // we will read multiple images into this single image variable as though it is a vector

	cv::FileStorage fsTrainingImages(TRAIN_XML_PATH + "images.xml", cv::FileStorage::READ);          // open the training images file

	if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
		std::cout << "error, unable to open training images file, exiting program\n\n";         // show error message
		return(-1);                                                                              // and exit program
	}

	fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
	fsTrainingImages.release();                                                 // close the traning images file

																				// train //////////////////////////////////////////////////////////////////////////////

	cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());            // instantiate the KNN object

																				// finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
																				// even though in reality they are multiple images / numbers
	kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

	// test ///////////////////////////////////////////////////////////////////////////////
	
	cv::Mat pic = src.clone();
	const char target = getTarget();
	if (target)
		std::cout << "Target = " << target << "\n";
	else
		std::cout << "Detecting what digit...\n";
	//cv::Mat pic = cv::imread(TRAIN_XML_PATH + "train_22.jpg");            // read in the test numbers image
	
	if (pic.empty()) {                                // if unable to open image
		return(-1);                                                  // and exit program
	}

	cv::Mat matGrayscale;           //
	cv::Mat matBlurred;             // declare more image variables
	cv::Mat matThresh;              //
	cv::Mat matThreshCopy;          //

	cv::cvtColor(pic, matGrayscale, CV_BGR2GRAY);         // convert to grayscale

	cv::Mat out;
	cv::namedWindow("Gray: ");
	cv::resize(matGrayscale, out, cv::Size(500,500));
	cv::imshow("Gray: ", out);
																		// blur
	cv::GaussianBlur(matGrayscale,              // input image
		matBlurred,                // output image
		cv::Size(5, 5),            // smoothing window width and height in pixels
		0);                        // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

	//cv::namedWindow("Blur: ", CV_WINDOW_NORMAL);
	//cv::imshow("Blur: ", matBlurred);

								   // filter image from grayscale to black and white
	cv::adaptiveThreshold(matBlurred,                           // input image
		matThresh,                            // output image
		255,                                  // make pixels that pass the threshold full white
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
		cv::THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
		83,                                   // size of a pixel neighborhood used to calculate threshold value
		53);                                   // constant subtracted from the mean or weighted mean


	// 8 -> 6
	// 
	cv::namedWindow("Threshold: ", CV_WINDOW_NORMAL);
	cv::resize(matThresh, out, cv::Size(800, 800));
	cv::imshow("Threshold: ", out);

	matThreshCopy = matThresh.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

	std::vector<std::vector<cv::Point> > ptContours;        // declare a vector for the contours
	std::vector<cv::Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

	cv::findContours(matThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
		ptContours,                             // output contours
		v4iHierarchy,                           // output hierarchy
		cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
		cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points


	for (int i = 0; i < ptContours.size(); i++) {               // for each contour
		ContourWithData contourWithData;                                                    // instantiate a contour with data object
		contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
		contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);         // get the bounding rect
		contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);               // calculate the contour area
		allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
	}

	for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
		if (allContoursWithData[i].checkIfContourIsValid()) {                   // check if valid
			validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
		}
	}
	// sort contours from left to right
	std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);


	std::string strFinalString;         // declare final string, this will have the final number sequence by the end of the program

	int candidate[2] = { 0, -1 };		// char , position
	float candidate_accuracy = 0;		

	for (int i = 0; i < validContoursWithData.size(); i++) {            // for each contour
		std::cout << "[points] " << validContoursWithData[i].ptContour.size() << "\n";
		std::cout << "[ratio] " << validContoursWithData[i].ratio << "\n";
		
		/// draw a green rect around the current char
		
		cv::rectangle(pic,                            // draw rectangle on original image
			validContoursWithData[i].boundingRect,        // rect to draw
			cv::Scalar(0, 255, 0),                        // green
			2);                                           // thickness
		
		cv::Mat matROI = matThresh(validContoursWithData[i].boundingRect);          // get ROI image of bounding rect

		cv::Mat matROIResized;
		cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

		cv::Mat matROIFloat;
		matROIResized.convertTo(matROIFloat, CV_32FC1);             // convert Mat to float, necessary for call to find_nearest

		cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

		cv::Mat matCurrentChar(0, 0, CV_32F);
		cv::Mat neighbor_res(0, 0, CV_32F);
		int k = 32;
		//float res = kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!
		
		float fltCurrentChar = kNearest->findNearest(matROIFlattenedFloat, k, matCurrentChar, neighbor_res, cv::noArray());
		std::cout << neighbor_res << "\n";
		int accuracy = 0;
		for (int i = 0; i < k; i++)
		{
			if (fltCurrentChar == (float)neighbor_res.at<float>(0, i))
				accuracy++;
		}

		std::cout << "Match char : " << char(int(fltCurrentChar)) << "\n";
		std::cout << " - Accuracy : " << (float)accuracy / k << "\n";
		std::cout << cv::Point(validContoursWithData[i].boundingRect.x + validContoursWithData[i].boundingRect.width / 2,
			validContoursWithData[i].boundingRect.y + validContoursWithData[i].boundingRect.height / 2) << "\n";
		cv::namedWindow("Detect", CV_WINDOW_NORMAL);
		cv::resize(pic, out, cv::Size(600, 600));
		cv::imshow("Detect", out);
		cv::waitKey(0);
		strFinalString = strFinalString + char(int(fltCurrentChar));        // append current char to full string
		
		if ( (!target || target == char(int(fltCurrentChar))) && accuracy >= 7)
		{

			if (candidate_accuracy < accuracy)
			{
				candidate[0] = char(int(fltCurrentChar));
				candidate[1] = i;
			}
			cv::rectangle(pic,                            // draw rectangle on original image
				validContoursWithData[i].boundingRect,        // rect to draw
				cv::Scalar(0, 255, 0),                        // green
				2);                                           // thickness
		}

	}
	if (!target)
	{
		if (candidate[1] != -1)
		{
			std::cout << "Find digit" << candidate[0] << " !\n";
			setTarget(candidate[0]);

		}
		else
		{
			std::cout << "No digit found !\n";
		}
	}
	else
	{
		Object object;
		int index = candidate[1];
		if (index == -1)
		{
			std::cout << "Target not found...\n";
			return -1;
		}
		object.center = cv::Point(validContoursWithData[index].boundingRect.x + validContoursWithData[index].boundingRect.width / 2,
			validContoursWithData[index].boundingRect.y + validContoursWithData[index].boundingRect.height / 2);
		object.bound = validContoursWithData[index].boundingRect;
		setObject(object);

		int src_x_unit = src.cols / 3, src_y_unit = src.rows / 3;
		object.pos = abs(object.center.x) / src_x_unit + abs(object.center.y) / src_y_unit * 3;
		std::cout << "found target at position " << object.pos << "\n";
	}
	//std::cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";       // show the full string

	cv::namedWindow("Detect", CV_WINDOW_NORMAL);
	cv::resize(pic, out, cv::Size(800, 800));
	cv::imshow("Detect", out);     

	cv::waitKey(0);                                      
	return(0);
}

int Task::getCamera()
{
	return _camera;
}
void Task::setCamera(int camera)
{
	_camera = camera;
}
int Task::getTarget()
{
	return _target;
}
void Task::setTarget(char target)
{
	_target = target;
}
Task::Object& Task::getObject()
{
	return _object;
}
void Task::setObject(Object object)
{
	_object = object;
}