#include "GridDetection.hpp"
#include <iostream>
#include "Draw.hpp"

int returnGreatestContour(std::vector<std::vector<cv::Point>> contours)
{
	std::vector<cv::Point> greatestContour = contours[0];
	int geatestContourIndex = 0;

	for(int i = 1; i < contours.size(); i++)
	{
		if(cv::contourArea(contours[i]) > cv::contourArea(greatestContour))
		{
			greatestContour = contours[i];
			geatestContourIndex = i;
		}
	}

	return geatestContourIndex;
}

void cornersDetection(cv::Mat img, std::vector<cv::Point> points, std::vector<cv::Point2f> &corners)
{
	if(points.size() == 4)
	{
		corners.push_back(points[0]);
		corners.push_back(points[1]);
		corners.push_back(points[2]);
		corners.push_back(points[3]);
	}
	else
	{
		corners.push_back(cv::Point2f(img.size().width, img.size().height));
		corners.push_back(cv::Point2f(0, img.size().height));
		corners.push_back(cv::Point2f(0, 0));
		corners.push_back(cv::Point2f(img.size().width, 0));

		for(int i = 0; i < points.size(); i++)
		{
			cornerDetection(points[i], corners);
		}
	}
}

void cornerDetection(cv::Point pt, std::vector<cv::Point2f> &corners)
{
	if(pt.y < corners[0].y && pt.x < corners[0].x)
		corners[0] = pt;

	if(pt.y < corners[1].y && pt.x > corners[1].x)
		corners[1] = pt;

	if(pt.x > corners[2].x && pt.y > corners[2].y)
		corners[2] = pt;

	if(pt.x < corners[3].x && pt.y > corners[3].y)
		corners[3] = pt;
}

bool detectGoodCorners(std::vector<cv::Point2f> corners)
{
	for(int i = 0; i < corners.size(); i++)
	{
		for(int j = i + 1; j < corners.size(); j++)
		{
			if(corners[i].x == corners[j].x && corners[i].y == corners[j].y)
				return false;
		}
	}

	return true;
}

cv::Point2f getMassCenter(std::vector<cv::Point2f> corners)
{
	cv::Point2f center(0, 0);
	for(int i = 0; i < corners.size(); i++)
		center += corners[i];

	center *= (1. / corners.size());
	return center;
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

	cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}

std::vector<cv::Vec4i> createGrid(cv::Mat quad, int horizontalPadding, int verticalPadding)
{
	std::vector<cv::Vec4i> grid;

	for(int i = 0; i < 21; i++)
	{
		cv::Vec4i temp = cv::Vec4i(0, quad.size().height - i*horizontalPadding, quad.size().width, quad.size().height - i*horizontalPadding);
		grid.push_back(temp);
	}

	for(int i = 0; i < 5; i++)
	{
		cv::Vec4i temp = cv::Vec4i(quad.size().width - i * verticalPadding, 0, quad.size().width - i * verticalPadding, quad.size().height);
		grid.push_back(temp);
	}

	return grid;
}

bool goodOrientation(cv::Mat img)
{
	cv::Mat imageToCheck;

	img.copyTo(imageToCheck);
	cv::Point checkerTopLeft(0, 0);
	cv::Point checkerBottomRight(135, 45);

	cv::Mat grayImg;
	cv::cvtColor(imageToCheck, grayImg, CV_BGR2GRAY);
	cv::adaptiveThreshold(grayImg, grayImg, 200, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 4);

	cv::Rect plop(checkerTopLeft, checkerBottomRight);
	cv::rectangle(grayImg, plop, cv::Scalar(255, 0, 0), 3, CV_AA);

	cv::Mat submat(grayImg, plop);
	double p = (double) countNonZero(submat) / (submat.size().width*submat.size().height);
	if(p < 0.3)
	{
		cv::line(grayImg, checkerTopLeft, checkerBottomRight, cv::Scalar(255, 255, 255), 3, CV_AA);
		return true;
	}

	return false;
}

void rotateImage(std::vector<cv::Point2f> &corners)
{
	std::vector<cv::Point2f> temp = corners;

	corners.clear();
	corners.push_back(temp[3]);
	corners.push_back(temp[0]);
	corners.push_back(temp[1]);
	corners.push_back(temp[2]);
}

std::vector<std::vector<cv::Point>> findContour(cv::Mat image, std::vector<cv::Vec4i> &hierarchy)
{
	cv::adaptiveThreshold(image, image, 200, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 4);

	std::vector<std::vector<cv::Point> > contours;


	cv::findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	for(int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 50, true);
	}

	return contours_poly;
}

void straightenAndCorrect(cv::Mat redimImage, std::vector<cv::Point2f> corners, cv::Mat img, bool response[20][4], std::string filePath, int fileIndex)
{
	redimImage = straightenGrid(corners, img);

	correctGrid(redimImage, response, filePath, fileIndex);

	cv::imshow("Result", redimImage);
}

cv::Mat straightenGrid(std::vector<cv::Point2f> corners, cv::Mat imgToStraighten)
{
	cv::Mat redimImage = cv::Mat::zeros(800, 300, CV_8UC3);
	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(redimImage.cols, 0));
	quad_pts.push_back(cv::Point2f(redimImage.cols, redimImage.rows));
	quad_pts.push_back(cv::Point2f(0, redimImage.rows));

	changePerspective(corners, quad_pts, imgToStraighten, redimImage);

	int count = 0;
	while(!goodOrientation(redimImage) && count < 4)
	{
		rotateImage(corners);
		changePerspective(corners, quad_pts, imgToStraighten, redimImage);
		count++;
	}

	return redimImage;
}

void changePerspective(std::vector<cv::Point2f> corners, std::vector<cv::Point2f> quad_pts, cv::Mat imgToStraighten, cv::Mat redimImage)
{
	cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
	cv::warpPerspective(imgToStraighten, redimImage, transmtx, redimImage.size());
}


void correctGrid(cv::Mat redimImage, bool response[20][4], std::string filePath, int fileIndex)
{
	
	int horizontalPadding = 800 / 21;
	int verticalPadding = 300 / 7;

	std::vector<cv::Vec4i> grid = createGrid(redimImage, horizontalPadding, verticalPadding);

	detectROI(redimImage, grid, response, filePath, fileIndex);

	drawMCQGrid(redimImage, horizontalPadding, verticalPadding);
}