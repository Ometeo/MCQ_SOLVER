#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "FileUtils.hpp"
#include "GridDetection.hpp"
#include "Draw.hpp"
#include "GridCorrection.hpp"

cv::Mat image;
cv::Mat redimImage;

std::string filePath;
int fileIndex;

bool response[20][4];

void on_correctMCQForImage(int, void*);

int main(int argc, char** argv)
{
	filePath = filesPath(argc, argv);

	cv::namedWindow("Select Picture");

	cv::createTrackbar("Picture", "Select Picture", &fileIndex, 68, on_correctMCQForImage);
	on_correctMCQForImage(fileIndex, 0);

	cv::waitKey(0);
	return 0;
}

void on_correctMCQForImage(int, void*)
{
	cv::destroyWindow("Result");
	
	std::string filename;
	createFilePath(filename, fileIndex, filePath);

	cv::Mat origin = cv::imread(filename);

	cv::Mat modified;
	origin.copyTo(modified);

	cv::cvtColor(modified, image, cv::COLOR_BGR2GRAY);

	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> contours_poly = findCountour(image, hierarchy);

	cv::Mat drawing = cv::Mat::zeros(image.size(), CV_8UC3);
	cv::Scalar color = cv::Scalar(0, 255, 0);
	
	int contourIndex = returnGreatestContour(contours_poly);

	drawContours(modified, contours_poly, contourIndex, color, 1, 8, hierarchy, 0, cv::Point());

	std::vector<cv::Point2f> corners;
	cornersDetection(modified, contours_poly[contourIndex], corners);

	drawCorner(modified, corners);

	if(detectGoodCorners(corners))
	{
		cv::Point2f center = getMassCenter(corners);
		sortCorners(corners, center);

		straightenAndCorrect(redimImage, corners, origin, response, filePath, fileIndex);
	}
	else
	{
		cv::RotatedRect rect = cv::minAreaRect(contours_poly[contourIndex]);
		std::vector<cv::Point2f> newCorners;

		cv::Point2f vertices[4];
		rect.points(vertices);

		newCorners.push_back(vertices[0]);
		newCorners.push_back(vertices[1]);
		newCorners.push_back(vertices[2]);
		newCorners.push_back(vertices[3]);



		for(int i = 0; i < 4; i++)
			line(modified, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 0, 255));

		straightenAndCorrect(redimImage, newCorners, origin, response, filePath, fileIndex);
	}

	imshow("Select Picture", modified);
}