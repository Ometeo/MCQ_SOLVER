#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	cv::Mat image;
	image = cv::imread("000.jpg");

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);

	cv::imshow("Display window", image);

	cv::waitKey(0);
	return 0;
}

