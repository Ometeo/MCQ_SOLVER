#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include "gridDetect.hpp"

cv::Mat image;
std::vector<cv::Vec4i> lines;



//std::vector<cv::Point2i> Corners(std::vector<cv::Vec4i> in)
//{
//	std::vector<cv::Point2i> corners;
//
//	for(int i = 0; i < in.size(); i++)
//	{
//		for(int j = i + 1; j < in.size(); j++)
//		{
//			cv::Point2i pt = Connected(in[i], in[j]);
//			if(pt.x >= 0 && pt.y >= 0)
//			{
//				corners.push_back(pt);
//			}
//		}
//	}
//	return corners;
//}





int main(int argc, char** argv)
{	
	const char* filename = argc >= 2 ? argv[1] : "000.jpg";
	cv::Mat plop = cv::imread(filename);
	cv::cvtColor(plop, image, cv::COLOR_BGR2GRAY);
	if(image.empty())
	{
		std::cout << "can not open " << filename << std::endl;
		return -1;
	}


	detectGrid(image, plop, lines);

	
	cv::imshow("fuck", image);
	cv::imshow("result", plop);
	cv::waitKey(0);
	return 0;
}