#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

cv::Mat image;
std::vector<cv::Vec4i> lines;

cv::Point2i Connected(cv::Vec4i a, cv::Vec4i b)
{

	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	int epsilon = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

	if(int d = epsilon)
	{
		cv::Point2i pt;
		pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
		pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;

		if(pt.x<cv::min(x1, x2) - 10 || pt.x>cv::max(x1, x2) + 10 || pt.y<cv::min(y1, y2) - 10 || pt.y>cv::max(y1, y2) + 10)
		{
			return cv::Point2i(-1, -1);
		}
		if(pt.x<cv::min(x3, x4) - 10 || pt.x>cv::max(x3, x4) + 10 || pt.y<cv::min(y3, y4) - 10 || pt.y>cv::max(y3, y4) + 10)
		{
			return cv::Point2i(-1, -1);
		}

		return pt;
	}
	else
	{
		cv::Point2i pt;
		pt.x = -1;
		pt.y = -1;
		return pt;
	}

}

std::vector<cv::Point2i> Corners(std::vector<cv::Vec4i> in)
{
	std::vector<cv::Point2i> corners;

	for(int i = 0; i < in.size(); i++)
	{
		for(int j = i + 1; j < in.size(); j++)
		{
			cv::Point2i pt = Connected(in[i], in[j]);
			if(pt.x >= 0 && pt.y >= 0)
			{
				corners.push_back(pt);
			}
		}
	}
	return corners;
}

bool comparator(cv::Point2f a, cv::Point2f b)
{
	return a.x<b.x;
}

void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{
	std::vector<cv::Point2f> top, bot;
	for(int i = 0; i < corners.size(); i++)
	{
		if(corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}
	sort(top.begin(), top.end(), comparator);
	sort(bot.begin(), bot.end(), comparator);
	cv::Point2f tl = top[0];
	cv::Point2f tr = top[top.size() - 1];
	cv::Point2f bl = bot[0];
	cv::Point2f br = bot[bot.size() - 1];
	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}
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

	cv::Size size(3, 3);
	cv::GaussianBlur(image, image, size, 0);
	cv::adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 7);
	//cv::bitwise_not(image, image);

	cv::HoughLinesP(image, lines, 1,CV_PI / 180, 80, 100, 10);

	std::vector<cv::Point2i> points = Corners(lines);
		for(size_t i = 0; i < lines.size() && i < 200; i++)
		{
			cv::Vec4i l = lines[i];
			cv::line(plop, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1);
		}

		for each (cv::Point2i  point in  points)
		{
			cv::Point2i p = point;
			cv::line(plop, cv::Point(p.x, p.y), cv::Point(p.x, p.y), cv::Scalar(255, 0, 0), 3, CV_AA);
		}
		

	cv::imshow("fuck", image);
	cv::imshow("result", plop);
	cv::waitKey(0);
	return 0;
}