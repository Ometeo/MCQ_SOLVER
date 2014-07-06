#ifndef DRAW_H
#define DRAW_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void drawCorner(cv::Mat img, std::vector<cv::Point2f> grid);

void drawMCQGrid(cv::Mat quad, int horizontalPadding, int verticalPadding);

void drawAllPoints(cv::Mat img, std::vector<cv::Point> grid);

void drawCorrectROI(cv::Mat img, std::vector<cv::Point2i> points, int nbROI);
#endif

