#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "FileUtils.hpp"

cv::Point2i Connected(cv::Vec4i a, cv::Vec4i b);
std::vector<cv::Point2i> Corners(std::vector<cv::Vec4i> in);
void detectROI(cv::Mat& img, std::vector<cv::Vec4i> grid, bool response[20][4], std::string filePath, int fileIndex);
bool correctROI(cv::Mat img, cv::Mat drawImage, std::vector<cv::Point2i> points, int nbROI);

void correctGrid(cv::Mat& img, cv::Mat drawImage, bool response[20][4], std::vector<cv::Point2i> points);
