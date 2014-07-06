#ifndef GRIDDETECTION_H
#define GRIDDETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GridCorrection.hpp"

int returnGreatestContour(std::vector<std::vector<cv::Point>> contours);
void cornersDetection(cv::Mat img, std::vector<cv::Point> points, std::vector<cv::Point2f> &corners);
void cornerDetection(cv::Point pt, std::vector<cv::Point2f> &corners);
bool detectGoodCorners(std::vector<cv::Point2f> corners);
cv::Point2f getMassCenter(std::vector<cv::Point2f> corners);
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);

std::vector<cv::Vec4i> createGrid(cv::Mat quad, int horizontalPadding, int verticalPadding);

bool goodOrientation(cv::Mat img);

void rotateImage(std::vector<cv::Point2f> &corners);


std::vector<std::vector<cv::Point>> findCountour(cv::Mat image, std::vector<cv::Vec4i> &hierarchy);


cv::Mat straightenGrid(std::vector<cv::Point2f> corners, cv::Mat imgToStraighten);

void changePerspective(std::vector<cv::Point2f> corners, std::vector<cv::Point2f> quad_pts, cv::Mat imgToStraighten, cv::Mat redimImage);
void correctGrid(cv::Mat redimImage, bool response[20][4], std::string filePath, int fileIndex);

void straightenAndCorrect(cv::Mat redimImage, std::vector<cv::Point2f> corners, cv::Mat img, bool response[20][4], std::string filePath, int fileIndex);
#endif