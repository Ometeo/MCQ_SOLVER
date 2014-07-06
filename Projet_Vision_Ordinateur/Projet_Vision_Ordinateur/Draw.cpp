#include "Draw.hpp"

void drawCorner(cv::Mat img, std::vector<cv::Point2f> grid)
{
	cv::line(img, cv::Point(grid[0].x, grid[0].y), cv::Point(grid[0].x, grid[0].y), cv::Scalar(0, 0, 0), 5, CV_AA);
	cv::line(img, cv::Point(grid[1].x, grid[1].y), cv::Point(grid[1].x, grid[1].y), cv::Scalar(255, 0, 0), 5, CV_AA);
	cv::line(img, cv::Point(grid[2].x, grid[2].y), cv::Point(grid[2].x, grid[2].y), cv::Scalar(0, 255, 0), 5, CV_AA);
	cv::line(img, cv::Point(grid[3].x, grid[3].y), cv::Point(grid[3].x, grid[3].y), cv::Scalar(255, 255, 255), 5, CV_AA);
}

void drawAllPoints(cv::Mat img, std::vector<cv::Point> grid)
{
	for(int i = 0; i < grid.size(); i++)
	{
		cv::line(img, cv::Point(grid[i].x, grid[i].y), cv::Point(grid[i].x, grid[i].y), cv::Scalar(0, 0, 255), 5, CV_AA);
	}
}

void drawMCQGrid(cv::Mat quad, int horizontalPadding, int verticalPadding)
{
	for(int i = 0; i < 21; i++)
	{
		cv::line(quad, cv::Point(0, quad.size().height - i*horizontalPadding), cv::Point(quad.size().width, quad.size().height - i*horizontalPadding), cv::Scalar(0, 0, 0), 3, CV_AA);
	}

	for(int i = 0; i < 5; i++)
	{
		cv::line(quad, cv::Point(quad.size().width - i * verticalPadding, 0), cv::Point(quad.size().width - i * verticalPadding, quad.size().height), cv::Scalar(0, 0, 0), 3, CV_AA);
	}
}

void drawCorrectROI(cv::Mat img, std::vector<cv::Point2i> points, int nbROI)
{
	cv::Mat trans;
	img.copyTo(trans);

	cv::line(trans, points[nbROI], points[nbROI + 6], cv::Scalar(0, 0, 255), 3, CV_AA);
	cv::line(trans, points[nbROI + 1], points[nbROI + 5], cv::Scalar(0, 0, 255), 3, CV_AA);

	double opacity = 0.4;

	cv::addWeighted(trans, opacity, img, 1 - opacity, 0, img);
}
