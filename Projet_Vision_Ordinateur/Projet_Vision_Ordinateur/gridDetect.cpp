#include "gridDetect.hpp"

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
	int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
	int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

	if(float d = ((float) (x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
	{
		cv::Point2f pt;
		pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
		pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
		if(pointOnLine(a, pt) || pointOnLine(b, pt))
			return pt;
	}
	else
		return cv::Point2f(-1, -1);
}

bool pointOnLine(cv::Vec4i l, cv::Point2f p)
{
	cv::Point p1 = cv::Point(l[0], l[1]);
	cv::Point p2 = cv::Point(l[2], l[3]);
	
	return true;
}

float dot(float x1, float x2, float y1, float y2)
{
	return x1* x2 + y1 * y2;
}

bool belongSameLine(cv::Vec4i l1, cv::Vec4i l2)
{
	cv::Point p1 = cv::Point(l1[0], l1[1]);
	cv::Point p2 = cv::Point(l1[2], l1[3]);

	cv::Point p3 = cv::Point(l2[0], l2[1]);
	cv::Point p4 = cv::Point(l2[2], l2[3]);

	float a = (float) p2.y - (float) p1.y;
	float b = (float) p1.x - (float) p2.x;
	float c = -a * p1.x - b * p1.y;

	float distance = (abs(a * p3.x + b *p3.y + c) / sqrt(a*a + b*b));
	if(distance < epsilon)
	{
		distance = (abs(a * p4.x + b *p4.y + c) / sqrt(a*a + b*b));
		if(distance < epsilon)
		{
			return true;
		}
	}
	return false;
}

cv::Vec4i fuuuuuusion(cv::Vec4i l1, cv::Vec4i l2)
{
	cv::Point p1 = cv::Point(l1[0], l1[1]);
	cv::Point p2 = cv::Point(l1[2], l1[3]);

	cv::Point p3 = cv::Point(l2[0], l2[1]);
	cv::Point p4 = cv::Point(l2[2], l2[3]);

	float dist[6];

	dist[0] = distanceBetweenPoints(p1, p2);
	dist[1] = distanceBetweenPoints(p1, p3);
	dist[2] = distanceBetweenPoints(p1, p4);
	dist[3] = distanceBetweenPoints(p2, p3);
	dist[4] = distanceBetweenPoints(p2, p4);
	dist[5] = distanceBetweenPoints(p3, p4);

	int index = 0;
	for(size_t i = 1; i < 6; i++)
	{
		if(dist[i - 1] < dist[i])
			index = i;
	}

	cv::Vec4i newLine;
	switch(index)
	{
		case(0) :
			newLine = cv::Vec4i(p1.x, p1.y, p2.x, p2.y);
			break;
		case(1) :
			newLine = cv::Vec4i(p1.x, p1.y, p3.x, p3.y);
			break;
		case(2) :
			newLine = cv::Vec4i(p1.x, p1.y, p4.x, p4.y);
			break;
		case(3) :
			newLine = cv::Vec4i(p2.x, p2.y, p3.x, p3.y);
			break;
		case(4) :
			newLine = cv::Vec4i(p2.x, p2.y, p4.x, p4.y);
			break;
		case(5) :
			newLine = cv::Vec4i(p3.x, p3.y, p4.x, p4.y);
			break;
		default:
			break;
	}
	return newLine;
}

std::vector<cv::Vec4i> affineLines(std::vector<cv::Vec4i> lines)
{
	std::vector<cv::Vec4i> output;
	for(size_t i = 0; i < lines.size(); i++)
	{
		bool fusion = false;
		for(size_t j = 0; j < output.size(); j++)
		{
			if(belongSameLine(lines[i], output[j]))
			{
				output[j] = fuuuuuusion(lines[i], output[j]);
				fusion = true;
				break;
			}
			else
				fusion = false;
		}
		if(!fusion)
			output.push_back(lines[i]);
	}
	return output;
}

float segmentNorm(cv::Vec4i line)
{
	cv::Point p1 = cv::Point(line[0], line[1]);
	cv::Point p2 = cv::Point(line[2], line[3]);
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}


void detectGrid(cv::Mat image, cv::Mat plop, std::vector<cv::Vec4i> lines)
{
	cv::Size size(3, 3);
	cv::GaussianBlur(image, image, size, 0);
	cv::adaptiveThreshold(image, image, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 7);
	//cv::bitwise_not(image, image);

	//cv::Canny(image, image, 50, 100, 3);
	
	cv::HoughLinesP(image, lines, 1, CV_PI / 180, 80, 100, 10);

	std::vector<cv::Vec4i> lines2 = affineLines(lines);
	
	//for(size_t i = 0; i < lines.size() && i < 200; i++)
	//{
	//	cv::Vec4i l = lines[i];
	//	cv::line(plop, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1);
	//}

	for(size_t i = 0; i < lines2.size() && i < 200; i++)
	{
		cv::Vec4i l = lines2[i];
		if(segmentNorm(l) > 100 && segmentNorm(l) < 500)
			cv::line(plop, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 1);
	}

	std::vector<cv::Point2f> corners;
	for(int i = 0; i < lines2.size(); i++)
	{
		for(int j = i + 1; j < lines2.size(); j++)
		{
			cv::Point2f pt = computeIntersect(lines2[i], lines2[j]);
			if(pt.x >= 0 && pt.y >= 0)
				corners.push_back(pt);
		}
	}

	for each (cv::Point2f  corner in  corners)
	{
		cv::Point2i c = corner;
		cv::line(plop, cv::Point(c.x, c.y), cv::Point(c.x, c.y), cv::Scalar(255, 0, 0), 3, CV_AA);
	}

}