#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

class Task
{
public:
	Task(int camera);
	virtual void execute() = 0;
	struct Object
	{
		cv::Point center;
		cv::Rect bound;
	};
protected:
	int text_recong(cv::Mat src);
	int getCamera();
	void setCamera(int camera);
	int getTarget();
	void setTarget(char target);
	Object getObject();
	void setObject(Object object);

	//Target target;
private:
	char _target;
	int _camera = 0;
	Object _object;
};

class Task1 : public Task
{
public:
	Task1(int camera);
	void execute();
private:
	
};


//int text_train();
//int text_recong(Target &target);



