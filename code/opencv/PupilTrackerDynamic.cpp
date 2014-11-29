/************************************************************************
* Measure and plot pupil parameters from a live feed
* Main file
*
* Author: Alankar Kotwal <alankarkotwal13@gmail.com>
************************************************************************/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace cv;

int threshVal = 10;
RNG rng(12345);

int main() {

	//VideoCapture cap(1);
	Mat frame;
	Mat frameCopy = frame.clone();
	frame = imread("../../images/eye.jpg");
	char key;
	namedWindow("Eye Frame", WINDOW_AUTOSIZE);
	namedWindow("Output", WINDOW_AUTOSIZE);
	
	ofstream results("../../results/data");
	time_t start = time(0);
	
	while(key != 'c') {
		
		//cap >> frame;
		imshow("Eye Frame", frame);
		key = waitKey(1);
		
		Mat ycbcr;
		cvtColor(frame, ycbcr, CV_BGR2YCrCb);
		
		Mat chan[3];
		split(ycbcr,chan);
		Mat y = chan[0];
		
		equalizeHist(y, y);
		threshold(y, y, threshVal, 255, THRESH_BINARY_INV);
		imshow("Output", y);
		
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// Find contours
		findContours(y, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
		
		int maxArea = 0;
		int maxIndex = 0;
		for(int i = 0; i<contours.size(); i++) {
			int ar = contourArea(contours[i]);
			std::cout<<ar<<std::endl;
			if(ar>maxArea) {
				maxIndex = i;
				maxArea = ar;
			}
		}
		drawContours(frame, contours, maxIndex, Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point());

		// Fit ellipses
		RotatedRect fittedEllipse = fitEllipse(Mat(contours[maxIndex]));
		ellipse(frameCopy, fittedEllipse, Scalar(0, 255, 0), 2, 8);
		
		std::cout<<"Ellipse height: "<<fittedEllipse.size.height<<" width: "<<fittedEllipse.size.width<<std::endl;
		
		myfile<<difftime(time(0), start)<<","<<fittedEllipse.size.height<<","<<fittedEllipse.size.width<<std::endl;

		// Show in a window
		namedWindow("Contours", CV_WINDOW_AUTOSIZE);
		imshow("Contours", frame);
		
		waitKey(1);
	}
	
	return 0;
}
