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

using namespace cv;

int threshVal = 10;
RNG rng(12345);

int main() {

	//VideoCapture cap(1);
	Mat frame;
	frame = imread("../../images/eye.jpg");
	//char key;
	namedWindow("Eye Frame", WINDOW_AUTOSIZE);
	
	//while(1) {
		
		//cap >> frame;
		imshow("Eye Frame", frame);
		//key = waitKey(1);
		//if(key == 'c') {
		//	break;
		//}
		
		Mat ycbcr;
		cvtColor(frame, ycbcr, CV_BGR2YCrCb);
		
		Mat chan[3];
		split(ycbcr,chan);
		Mat y = chan[0];
		
		equalizeHist(y, y);
		threshold(y, y, threshVal, 255, THRESH_BINARY_INV);
		
		//Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// Detect edges using canny
		//Canny(y, canny_output, 200, 255, 5);
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
		std::cout<<maxArea<<"lol"<<std::endl;
		drawContours(frame, contours, maxIndex, Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point());

		// Show in a window
		namedWindow("Detected Pupil", CV_WINDOW_AUTOSIZE);
		imshow("Detected Pupil", frame);
		
		waitKey(0);
	//}
	
	return 0;
}
