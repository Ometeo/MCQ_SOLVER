#include "GridCorrection.hpp"
#include "Draw.hpp"

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

void detectROI(cv::Mat& img, std::vector<cv::Vec4i> grid, bool response[20][4], std::string filePath, int fileIndex)
{
	cv::Mat localImage;
	img.copyTo(localImage);
	std::vector<cv::Vec4i> lines;

	//Detect the corners of the grid.
	std::vector<cv::Point2i> points = Corners(grid);

	for each (cv::Point2i  point in  points)
	{
		cv::Point2i p = point;
	}

	cv::Mat grayImg;
	cv::cvtColor(localImage, grayImg, CV_BGR2GRAY);
	cv::adaptiveThreshold(grayImg, grayImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 4);

	//Detect the selected response.
	correctGrid(grayImg, img, response, points);

	//Return the correction on a file.
	writeResultOnFile(fileIndex, filePath, response);
}

void correctGrid(cv::Mat& img, cv::Mat drawImage, bool response[20][4], std::vector<cv::Point2i> points)
{
	int j = 0;
	int tabY = 3;
	int tabX = 19;
	for(int i = 0; i < 100; i++)
	{
		if(j != 4)
		{
			response[tabX][tabY] = correctROI(img, drawImage, points, i);
			tabY--;
			j++;
		}
		else
		{
			tabX--;
			tabY = 3;
			j = 0;
		}
	}
}

bool correctROI(cv::Mat img, cv::Mat drawImage, std::vector<cv::Point2i> points, int nbROI)
{
	cv::Rect ROI(points[nbROI], points[nbROI + 6]);
	cv::Mat submat(img, ROI);

	double p = (double) countNonZero(submat) / (submat.size().width*submat.size().height);
	if(p >= 0.3)
	{
		drawCorrectROI(drawImage, points, nbROI);
		return true;
	}
	else
		return false;
}