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
	int text_recong(cv::Mat src);
	struct Object
	{
		cv::Point center;
		cv::Rect bound;
		int pos;
	};
	Object& getObject();
	int getTarget();

protected:
	//int text_recong(cv::Mat src);
	int getCamera();
	void setCamera(int camera);
	void setTarget(char target);
	void setObject(Object object);

	//Target target;
private:
	char _target = 0;
	int _camera = 0;
	Object _object;

	int vedio_subs, block_size;
};

class Task1 : public Task
{
public:
	Task1(int camera);
	void execute();
private:
	
};


int text_train();
//int text_recong(Target &target);

int track_obj();

extern int maxValue, blockSize, C;
void on_change(int, void*);