#ifndef _H_GRIDDETECT
#define _H_GRIDDETECT

#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#define epsilon 10

void detectGrid(cv::Mat image, cv::Mat plop, std::vector<cv::Vec4i> lines);
bool belongSameLine(cv::Vec4i l1, cv::Vec4i l2);
bool pointOnLine(cv::Vec4i l, cv::Point2f p);
inline float distanceBetweenPoints(cv::Point p1, cv::Point p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}
#endif // !1
