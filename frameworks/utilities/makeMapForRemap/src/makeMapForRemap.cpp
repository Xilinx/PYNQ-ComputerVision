#include <makeMapForRemap.h>
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

void makeMapCircleZoom(int width, int height, int cx, int cy, int radius, float zoom, Mat &map1, Mat &map2)
{
	Mat mapX, mapY;
	
	makeMapXYCircleZoom(width, height, cx, cy, radius, zoom, mapX, mapY);
	
	convertMaps(mapX, mapY, map1, map2, CV_16SC2);	
}

void makeMapXYCircleZoom(int width, int height, int cx, int cy, int radius, float zoom, Mat &mapX, Mat &mapY)
{
	mapX.create(Size(width, height), CV_32FC1);
	mapY.create(Size(width, height), CV_32FC1);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			mapX.at<float>(j, i) = i;
			mapY.at<float>(j, i) = j;


			float x = i - cx;
			float y = j - cy;

			if (sqrt(x*x + y*y) < radius) {
				mapX.at<float>(j, i) = cx + x / zoom;
				mapY.at<float>(j, i) = cy + y / zoom;
			}
		}
	}
}

void makeMapXYFlipHor(int width, int height, Mat &mapX, Mat &mapY)
{
	mapX.create(Size(width, height), CV_32FC1);
	mapY.create(Size(width, height), CV_32FC1);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			mapX.at<float>(j, i) = width - i - 1;
			mapY.at<float>(j, i) = j;
		}
	}
}

void makeMapFlipHor(int width, int height, Mat &map1, Mat &map2)
{
	Mat mapX, mapY;
	
	makeMapXYFlipHor(width, height, mapX, mapY);
	
	convertMaps(mapX, mapY, map1, map2, CV_16SC2);
}

void makeCircleMap(int width, int height, Mat &map1, Mat &map2) { //Based on http://sidekick.windforwings.com/2012/12/opencv-fun-with-remap.html
    
	double rad = (height < width ? height : width)/2;
	double diag_rad = sqrt(height*height + width*width)/2;
	printf("radius = %d (rows: %d, cols: %d)\n", (int)rad, height, width);

	Mat map_x, map_y;
	map_x.create(Size(width,height), CV_32FC1);
	map_y.create(Size(width,height), CV_32FC1);
	
	// the center 
	double c_x = (double)width/2;
	double c_y = (double)height/2;

	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			// shift the coordinates space to center
			double x = i-c_x;
			double y = j-c_y;

			// handle the 0 and pi/2 angles separately as we are doing atan
			if(0 == x) {
				double ratio = 2*rad/height;
				map_y.at<float>(j,i) = y/ratio + c_y;
				map_x.at<float>(j,i) = c_x;
			}
			else if(0 == y) {
				double ratio = 2*rad/width;
				map_x.at<float>(j,i) = x/ratio + c_x;
				map_y.at<float>(j,i) = c_y;
			}	
			else {
			// get the r and theta
			double r = sqrt(y*y + x*x);
			double theta = atan(y/x);
			// get the length of line at theta touching the rectangle border
			double diag = min(fabs(c_x/cos(theta)), fabs(c_y/sin(theta)));

			// scale r
			double ratio = rad/diag;
			r = r/ratio;

			// remap the point
			if(x > 0)       map_x.at<float>(j,i) = r*cos(fabs(theta)) + c_x;
			else            map_x.at<float>(j,i) = c_x - r*cos(fabs(theta));

			if(y > 0)       map_y.at<float>(j,i) = r*sin(fabs(theta)) + c_y;
			else            map_y.at<float>(j,i) = c_y - r*sin(fabs(theta));
			}
		}
	}
	
	convertMaps(map_x, map_y, map1, map2, CV_16SC2);
}