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
*     Author: Kristof Denolf <kristof@xilinx.com>
*     Date:   2018/02/09
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <OpenCVUtils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "xfSDxCanny.h"

//include xF::mat prototype
#include <Mat/inc/mat.hpp>

#include <HRTimer.h>

using namespace cv;


//PL instantiation parameters
#include "PLInstantiationParameters.h"


/** @function main */
int main ( int argc, char** argv )
{
	HRTimer timer;

	// Declare SW variables
	double threshold1 = 30;
	double threshold2 = 64;
	int apertureSize  = 3; // Sobel filter size
	bool L2gradient   = 0; //enable L2 norm: off

  // Command line parsing
	const String keys =
		"{help h usage ? |                 | print this message                   }"
		"{@image         |                 | input image 	                      }"
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

	String filenameIn = parser.get<String>(0);

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

	// Initialize
	Mat srcIn, dstSW, srcInY;
	initializeSingleImageTest(filenameIn, srcIn);

	int width = srcIn.size().width;
	int height = srcIn.size().height;
	
	// Declare variables used for HW-SW interface to achieve good performance
	xF::Mat srcHLS(height, width, CV_8UC1);
	xF::Mat dstHLS(height, width, CV_8UC1);
	
	//convert 3-channel image into 1-channel image
	cvtColor(srcIn, srcHLS, COLOR_BGR2GRAY, 1);
	cvtColor(srcIn, srcInY, COLOR_BGR2GRAY, 1);

	// Apply OpenCV reference canny
	std::cout << "running golden model" << std::endl;
	timer.StartTimer();
	for (int i = 0; i < numberOfIterations; i++){
	   cv::Canny(srcInY, dstSW, threshold1, threshold2, apertureSize, L2gradient);
	}
	timer.StopTimer();
	std::cout << "Elapsed time over " << numberOfIterations << "SW call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	// Call wrapper for xf::canny
	std::cout << "running hardware Canny" << std::endl;
	timer.StartTimer();
	for (int i = 0; i < numberOfIterations; i++){
		xF::Canny(srcHLS, dstHLS, threshold1, threshold2, apertureSize, L2gradient);
	}
	timer.StopTimer();	
	std::cout << "Elapsed time over " << numberOfIterations << "PL call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	// compare results
	std::cout << "comparing HLS versus golden" << std::endl;
	int numberOfDifferences = 0;
	double errorPerPixel = 0;
	imageCompare(dstHLS, dstSW, numberOfDifferences, errorPerPixel, true, false);
	std::cout << "number of differences: " << numberOfDifferences << " average L1 error: " << errorPerPixel << std::endl;

	//write back images in files
	if (writeSWResult)
		writeImage(filenameSW, dstSW);
	if (writePLResult)
		writeImage(filenamePL, dstHLS);

	// Output input and filter output
	if (imShowOn) {
		imshow("Input image", srcHLS);
		imshow("Processed (SW)", dstSW);
		imshow("Processed (PL)", dstHLS);
		waitKey(0);
	}

	return 0;
}
