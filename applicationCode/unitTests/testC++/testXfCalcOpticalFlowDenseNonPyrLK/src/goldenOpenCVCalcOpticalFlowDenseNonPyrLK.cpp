/******************************************************************************
 *  Copyright (c) 2018, Xilinx, Inc.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in the 
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its 
 *      contributors may be used to endorse or promote products derived from 
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*****************************************************************************
*
*     Author: Kristof Denolf <kristof@xilinx.com>
*     Date:   2018/09/12
*
*****************************************************************************/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include "opencv2/features2d.hpp"
#include "opencv2/video/tracking.hpp"
#include "OpenCVUtils.h"
#include "HRTimer.h"

using namespace cv;

std::vector<Point2f> markAllPixelsAsFeaturePoints(cv::Size size)
{
	std::vector<Point2f> allPointsAsFeatures;
	
	for (int i = 0; i < size.height; i++)
		for (int j = 0; j < size.width; j++)
		{
			Point2f p(j,i);
			allPointsAsFeatures.push_back(p);
		}
	
	return allPointsAsFeatures;
}



/** @function main */
int main ( int argc, char** argv )
{
  //command line parsing
  const String keys =
  "{help h usage ? |                 | print this message                   }"
  "{@image1	       |                 | in image 	                        }"
  "{@image2	       |                 | in image 	                        }"
  "{display d      |                 | diplay result with imshow            }"
  "{interations n  | 1               | number of iterations to measure time }"
  ;

  CommandLineParser parser(argc, argv, keys);
  if (parser.has("help") || argc < 2)
  {
    parser.printMessage();
    std::cout << "\nhit enter to quit...";
    std::cin.get();
    return 0;
  }

  String fileName1 = parser.get<String>(0);
  String fileName2 = parser.get<String>(1);

  unsigned int numberOfIterations = 0;
  if (parser.has("interations"))
  numberOfIterations = parser.get<unsigned int>("interations");

  bool imShowOn = parser.has("display");

  /// Declare variables
  Mat prevImg,nextImg;
  Mat prevImgGray,nextImgGray;
  Mat dstSW;

  // Initialize
  initializeSingleImageTest(fileName1, prevImg);
  initializeSingleImageTest(fileName2, nextImg);
 
  cvtColor(prevImg, prevImgGray, COLOR_BGR2GRAY, 1);
  cvtColor(nextImg, nextImgGray, COLOR_BGR2GRAY, 1);
  
  
  // Initialize arguments for Fast detector
  Mat prevImgCorners,nextImgCorners;
  int threshold=15;
  bool nonMaxSupression= true;
  std::vector<KeyPoint> keypointsPrev, keypointsNext;  
  cv::FAST (prevImgGray, keypointsPrev, threshold, nonMaxSupression);

  std::vector<Point2f > prevPts, nextPts;
  /*for(int i=0;i<keypointsPrev.size();i++)
  {
  	prevPts.push_back(keypointsPrev[i].pt);	
  
  }*/
  
  prevPts = markAllPixelsAsFeaturePoints(prevImgGray.size());
  
  
  std::vector<uchar> status;
  std::vector<float> err;
  
  if (numberOfIterations > 0) {
  
    HRTimer timer;

	std::cout << "starting time measurement: " << numberOfIterations << std::endl;
    timer.StartTimer();
	
    for (unsigned int i = 0; i < numberOfIterations; i++) {
    	cv::calcOpticalFlowPyrLK(prevImgGray, nextImgGray, prevPts, nextPts, status, err, cv::Size(21,21),0);
	}

    timer.StopTimer();

    std::cout << "Elapsed time over " << numberOfIterations << " calls: " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;
    std::cout << "Framerate: " << (float)numberOfIterations / ((float)timer.GetElapsedUs() / 1e6) << std::endl;
  }
  
  for(int i=0;i<nextPts.size();i++)
  {
  	if (status[i] == 1) {
		KeyPoint point(nextPts[i],1);
		keypointsNext.push_back(point);
	}		
  }
  if (imShowOn) {
    /// Loop - Will filter the image with different kernel sizes each 0.5 seconds
    drawKeypoints(prevImg, keypointsPrev, prevImgCorners, Scalar(0,0,255)); 	
    drawKeypoints(nextImg, keypointsNext, nextImgCorners, Scalar(0,0,255)); 	
	imshow("input",prevImg);
	imshow("prevImgCorners",prevImgCorners);
	imshow("nextImgCorners",nextImgCorners);
	waitKey(0);
  }

  return 0;
}
