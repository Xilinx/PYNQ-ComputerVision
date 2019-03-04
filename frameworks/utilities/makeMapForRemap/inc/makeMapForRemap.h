#ifndef _MAKEMAPFORREMAP_H_
#define _MAKEMAPFORREMAP_H_

//SDx temporal fix for Clang issue
#ifdef __SDSCC__
#undef __ARM_NEON__
#undef __ARM_NEON
#include <opencv2/core/core.hpp>
#define __ARM_NEON__
#define __ARM_NEON
#else
#include <opencv2/core/core.hpp>
#endif
//#include <opencv2/core/core.hpp>

void makeMapFlipHor(int width, int height, cv::Mat &map1, cv::Mat &map2);
void makeMapXYFlipHor(int width, int height, cv::Mat &mapX, cv::Mat &mapY);

void makeMapXYCircleZoom(int width, int height, int cx, int cy, int radius, float zoom, cv::Mat &mapX, cv::Mat &mapY);
void makeMapCircleZoom(int width, int height, int cx, int cy, int radius, float zoom, cv::Mat &map1, cv::Mat &map2);

#endif