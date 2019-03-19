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
*     Date:   2018/09/18
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <OpenCVUtils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp> 
#include "opencv2/features2d.hpp"
#include "opencv2/video/tracking.hpp"
 
//include xF::mat prototype
#include <Mat/inc/mat.hpp>

#include <HRTimer.h>  

#include <stdlib.h> 
  
using namespace cv;


//PL instantiation parameters
#include "PLInstantiationParameters.h"

#include "xfSDxCalcOpticalFlowDenseNonPyrLK.h"

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
	HRTimer timer;
 

  // Command line parsing
	const String keys =
		"{help h usage ? |                 | print this message                   }"
		"{@image1        |                 | input image 	                      }"
		"{@image2        |                 | input image 	                      }"
		"{goldenFile gf  |                 | golden output image (from SW)        }"
		"{outFile of     |                 | output image (from PL)               }"
		"{display d      |                 | diplay result with imshow            }"
		"{iterations n   | 1               | number of iterations to measure time }"
		;

	CommandLineParser parser(argc, argv, keys.c_str());
	if (parser.has("help") || argc < 2)
	{
		parser.printMessage();
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		return 0;
	}

	String fileName1 = parser.get<String>(0);
	String fileName2 = parser.get<String>(1);

	String filenameSW; bool writeSWResult = false;
	if(parser.has("goldenFile")) {
		filenameSW = parser.get<String>("goldenFile");
    writeSWResult = true;
	}

	String filenamePL; bool writePLResult = false;
	if(parser.has("outFile")) {
		filenamePL = parser.get<String>("outFile");
    writePLResult = true;
	}

	bool imShowOn = parser.has("display");
	unsigned int numberOfIterations;
	if (parser.has("iterations"))
		numberOfIterations = parser.get<unsigned int>("iterations");

	if(!parser.check())
	{
		parser.printErrors();
		return(-1);
	}

  	/// Declare variables
  	Mat prev,next;
  	Mat prevY,nextY; 

  	// Initialize
  	initializeSingleImageTest(fileName1, prev);
  	initializeSingleImageTest(fileName2, next);
 
  	cvtColor(prev, prevY, CV_BGR2GRAY, 1);
  	cvtColor(next, nextY, CV_BGR2GRAY, 1);
  
	
	std::vector<Point2f > prevPts, nextPts;
	std::vector<uchar> status;
  	std::vector<float> err;
	prevPts = markAllPixelsAsFeaturePoints(prevY.size());

	int width = prevY.size().width;
	int height = prevY.size().height;
	// Declare variables used for HW-SW interface to achieve good performance
	xF::Mat prevHLS(height, width, CV_8UC1);	
	xF::Mat nextHLS(height, width, CV_8UC1);  
	xF::Mat flowXHLS(height, width, CV_32FC1);	
	xF::Mat flowYHLS(height, width, CV_32FC1);
	
  	cvtColor(prev, prevHLS, CV_BGR2GRAY, 1);
  	cvtColor(next, nextHLS, CV_BGR2GRAY, 1);
  	
	// Apply OpenCV reference erode
	std::cout << "running golden model" << std::endl;
	timer.StartTimer();
	for (int i = 0; i < numberOfIterations; i++){
    	cv::calcOpticalFlowPyrLK(prevY, nextY, prevPts, nextPts, status, err, cv::Size(21,21),0);;
	}
	timer.StopTimer();
	std::cout << "Elapsed time over " << numberOfIterations << "SW call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	// Call wrapper for xf::calcOpticalFlowPyrLK
	std::cout << "running hardware calcOpticalFlowPyrLK" << std::endl;
	timer.StartTimer();
	
	
	for (int i = 0; i < numberOfIterations; i++){ 
		xF::calcOpticalFlowDenseNonPyrLK(prevHLS,nextHLS,flowXHLS,flowXHLS);
	}
	timer.StopTimer();	
	std::cout << "Elapsed time over " << numberOfIterations << "PL call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	// compare results
	std::cout << "comparing HLS versus golden" << std::endl;
	int numberOfDifferences = 0;
	double errorPerPixel = 0;
	//imageCompare(dstHLS, dstSW, numberOfDifferences, errorPerPixel, true, false);
	std::cout << "number of differences: " << numberOfDifferences << " average L2 error: " << errorPerPixel << std::endl;


	
	if (imShowOn) {
		std::vector<KeyPoint> keypointsPrev, keypointsNext;
		cv::Mat nextImgCorners;
		
		for(int i=0;i<nextPts.size();i++)
		{
			if (status[i] == 1) {
				KeyPoint point(nextPts[i],1);
				keypointsNext.push_back(point);
			}		
		}
		
		/// Loop - Will filter the image with different kernel sizes each 0.5 seconds
		//drawKeypoints(prevImg, keypointsPrev, prevImgCorners, Scalar(0,0,255)); 	
		drawKeypoints(nextY, keypointsNext, nextImgCorners, Scalar(0,0,255)); 	
		imshow("input",prevY);
		//imshow("prevImgCorners",prevImgCorners);
		imshow("nextImgCorners",nextImgCorners);
		
		cv::Mat flowXtmp, flowYtmp, flowXasU, flowYasV;
		flowXtmp = flowXHLS*5+128.0;
		flowYtmp = flowYHLS*5+128.0;
		flowXtmp.convertTo(flowXasU,CV_8U);
		flowYtmp.convertTo(flowYasV,CV_8U);
		
		cv::Mat toMerge[3] = {prevY,flowXasU,flowYasV}; 
		cv::Mat outYUV;
		merge(toMerge,3,outYUV);
		cv::Mat outBGR;
		cvtColor(outYUV,outBGR,cv::COLOR_YUV2BGR);
		imshow("output",outBGR);		
		
		waitKey(0);
	}
	return 0;
}
