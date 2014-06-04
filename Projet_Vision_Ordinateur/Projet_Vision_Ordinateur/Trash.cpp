//#include <cmath>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <functional>
//
//
//#define epsilon 5
//cv::Rect roi;
//cv::Point start(-1, -1);
//
//cv::Mat image;
//cv::Mat img;
//cv::Mat render;
//
//std::vector<cv::Vec2f> lines1;
//std::vector<cv::Vec4i> lines2;
//
//std::vector<cv::Vec2i> corners;
//int i = 0;
//
//cv::Point gp1;
//cv::Point gp2;
//cv::Point gp3;
//cv::Point gp4;
//
//void CallBackFunc(int event, int x, int y, int flags, void* userData);
//void draw();
//int EndingLineOnThisOne(cv::Point p1, cv::Point p2, std::vector<cv::Vec4i> lines);
//bool belongSameLine(cv::Vec4i l1, cv::Vec4i l2);
//cv::Vec4i fuuuuuusion(cv::Vec4i l1, cv::Vec4i l2);
//void selectGrid(cv::Mat image, std::vector<cv::Vec4i> lines);
//void detectLines();
//std::vector<cv::Vec4i> affineLines(std::vector<cv::Vec4i> lines);
//float distanceBetweenPoints(cv::Point p1, cv::Point p2);
//
//int main(int argc, char** argv)
//{
//	const char* filename = argc >= 2 ? argv[1] : "000.jpg";
//	cv::Mat plop1 = cv::imread(filename);
//	cv::cvtColor(plop1, image, cv::COLOR_BGR2GRAY);
//	if(image.empty())
//	{
//		std::cout << "can not open " << filename << std::endl;
//		return -1;
//	}
//
//	cv::Size size(3, 3);
//	cv::GaussianBlur(image, image, size, 0);
//	int threshold = 50;
//	cv::adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 7);
//	//cv::Canny(img, render, threshold, threshold * 3, 3);
//
//	HoughLinesP(image, lines2, 1, CV_PI / 180, 50, 30, 10);
//	for(size_t i = 0; i < lines2.size() && i < 200; i++)
//	{
//		cv::Vec4i l = lines2[i];
//		//cv::line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1);
//	}
//
//	//std::vector<cv::Vec4i> plop = affineLines(lines2);
//	std::vector<cv::Vec4i> plop = lines2;
//
//	cv::cvtColor(image, render, cv::COLOR_GRAY2BGR);
//
//	for(size_t i = 0; i < plop.size() && i < 200; i++)
//	{
//		cv::Vec4i l = plop[i];
//		cv::line(plop1, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 1);
//	}
//	selectGrid(plop1, plop);
//
//	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
//	//cv::setMouseCallback("Display window", CallBackFunc, NULL);
//	//draw();
//	cv::imshow("Display window", plop1);
//
//	cv::waitKey(0);
//	return 0;
//}
//
//int EndingLineOnThisOne(cv::Point p1, cv::Point p2, std::vector<cv::Vec4i> lines)
//{
//	int count = 0;
//	float a = ((float) p2.y - (float) p1.y) / ((float) p2.x - (float) p2.x);
//	float b = (float) p1.y - a * (float) p1.x;
//
//	float almostB = 0;
//	float ratio = 0;
//	for(size_t i = 0; i < lines.size() && i < 200; i++)
//	{
//		cv::Vec4i l = lines[i];
//		cv::Point p1 = cv::Point(l[0], l[1]);
//		cv::Point p2 = cv::Point(l[2], l[3]);
//
//		almostB = p1.x * a + b;
//		ratio = abs(almostB - p1.y);
//		if(ratio < epsilon)
//			count++;
//
//		almostB = p2.x * a + b;
//		ratio = abs(almostB - p2.y);
//		if(ratio < epsilon)
//			count++;
//	}
//	return count++;
//}
//
//void selectGrid(cv::Mat image, std::vector<cv::Vec4i> lines)
//{
//
//	std::vector<int> countPerLine;
//	for(size_t i = 0; i < lines.size() && i < 200; i++)
//	{
//		cv::Vec4i l = lines[i];
//		cv::Point p1 = cv::Point(l[0], l[1]);
//		cv::Point p2 = cv::Point(l[2], l[3]);
//		int temp = EndingLineOnThisOne(p1, p2, lines);
//		countPerLine.push_back(temp);
//	}
//
//	std::sort(countPerLine.begin(), countPerLine.end(), std::greater<int>());
//	for(size_t i = 0; i < lines2.size() && i < 4; i++)
//	{
//		cv::Vec4i l = lines2[i];
//		cv::line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 255), 1);
//	}
//}
//
//bool belongSameLine(cv::Vec4i l1, cv::Vec4i l2)
//{
//	cv::Point p1 = cv::Point(l1[0], l1[1]);
//	cv::Point p2 = cv::Point(l1[2], l1[3]);
//
//	cv::Point p3 = cv::Point(l2[0], l2[1]);
//	cv::Point p4 = cv::Point(l2[2], l2[3]);
//
//	float a = (float) p2.y - (float) p1.y;
//	float b = (float) p1.x - (float) p2.x;
//	float c = -a * p1.x - b * p1.y;
//
//	float distance = (abs(a * p3.x + b *p3.y + c) / sqrt(a*a + b*b));
//	if(distance < epsilon)
//	{
//		distance = (abs(a * p4.x + b *p4.y + c) / sqrt(a*a + b*b));
//		if(distance < epsilon)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//cv::Vec4i fuuuuuusion(cv::Vec4i l1, cv::Vec4i l2)
//{
//	cv::Point p1 = cv::Point(l1[0], l1[1]);
//	cv::Point p2 = cv::Point(l1[2], l1[3]);
//
//	cv::Point p3 = cv::Point(l2[0], l2[1]);
//	cv::Point p4 = cv::Point(l2[2], l2[3]);
//
//	float dist[6];
//
//	dist[0] = distanceBetweenPoints(p1, p2);
//	dist[1] = distanceBetweenPoints(p1, p3);
//	dist[2] = distanceBetweenPoints(p1, p4);
//	dist[3] = distanceBetweenPoints(p2, p3);
//	dist[4] = distanceBetweenPoints(p2, p4);
//	dist[5] = distanceBetweenPoints(p3, p4);
//
//	int index = 0;
//	for(size_t i = 1; i < 6; i++)
//	{
//		if(dist[i - 1] < dist[i])
//			index = i;
//	}
//
//	cv::Vec4i newLine;
//	switch(index)
//	{
//		case(0) :
//			newLine = cv::Vec4i(p1.x, p1.y, p2.x, p2.y);
//			break;
//		case(1) :
//			newLine = cv::Vec4i(p1.x, p1.y, p3.x, p3.y);
//			break;
//		case(2) :
//			newLine = cv::Vec4i(p1.x, p1.y, p4.x, p4.y);
//			break;
//		case(3) :
//			newLine = cv::Vec4i(p2.x, p2.y, p3.x, p3.y);
//			break;
//		case(4) :
//			newLine = cv::Vec4i(p2.x, p2.y, p3.x, p3.y);
//			break;
//		case(5) :
//			newLine = cv::Vec4i(p3.x, p3.y, p4.x, p4.y);
//			break;
//		default:
//			break;
//	}
//	return newLine;
//}
//
//float distanceBetweenPoints(cv::Point p1, cv::Point p2)
//{
//	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
//}
//
//std::vector<cv::Vec4i> affineLines(std::vector<cv::Vec4i> lines)
//{
//	std::vector<cv::Vec4i> output;
//	for(size_t i = 0; i < lines.size() && i < 200; i++)
//	{
//		bool fusion = false;
//		for(size_t j = 0; j < output.size() && j < 200; j++)
//		{
//			if(belongSameLine(lines[i], output[j]))
//			{
//				output[j] = fuuuuuusion(lines[i], output[j]);
//				fusion = true;
//				break;
//			}
//			else
//				fusion = false;
//		}
//		if(!fusion)
//			output.push_back(lines[i]);
//	}
//	return output;
//}
//
//
////void draw()
////{
////
////		for(size_t i = 0; i < lines.size() && i < 200; i++)
////		{
////			cv::Vec4i l = lines[i];
////			cv::line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1);
////		}
////}
////
////
////void CallBackFunc(int event, int x, int y, int flags, void* userdata)
////{
////	if(event == cv::EVENT_LBUTTONDOWN)
////	{
////		if(i <= 3)
////		{
////			corners.push_back(cv::Point(x, y));
////			i++;
////		}
////
////		if(corners.size() == 4)
////		{
////			cv::Rect plop = cv::Rect(corners[0], corners[1]);
////			cv::rectangle(image, plop, cv::Scalar(255, 0, 0), 1);
////		}
////	}
////
////	draw();
////	cv::imshow("Display window", image);
////}
////
////void detectLines()
////{
////	cv::Mat cannyImage;
////	Canny(image, cannyImage, 50, 200, 3);
////	double r = 1;
////	int threshold = 50;
////	cv::Mat imageGrey;
////	cv::cvtColor(cannyImage, imageGrey, cv::COLOR_GRAY2BGR);
////	cv::HoughLinesP(imageGrey, lines, r, CV_PI / 180, threshold, 50, 50);
////}
