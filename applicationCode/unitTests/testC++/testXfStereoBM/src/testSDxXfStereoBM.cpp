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
*     Date:   2018/01/30
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <OpenCVUtils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"

#include "xfSDxStereoBM.h"

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
	Mat dstSW;

  // Command line parsing
	const String keys =
		"{help h usage ? |                 | print this message                   }"
		"{@left          |                 | left input image 	                  }"
		"{@right         |                 | right output image 	              }"
		"{goldenFile gf  |                 | golden output image (from SW)        }"
		"{outFile of     |                 | output image (from PL)               }"
		"{display d      |                 | display result with imshow           }"
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

	String filenameLeft = parser.get<String>(0);
	String filenameRight = parser.get<String>(1);

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
	Mat srcLeft, srcRight, disparitySW;
	Mat srcLeftY, srcRightY;
	initializeDualImageTest(filenameLeft, filenameRight, srcLeft, srcRight);

	int width = srcLeft.size().width;
	int height = srcLeft.size().height;
	
	// Declare variables used for HW-SW interface to achieve good performance
	xF::Mat leftHLS(height, width, CV_8UC1);
	xF::Mat rightHLS(height, width, CV_8UC1);
	xF::Mat disparityHLS(height, width, CV_16UC1);
	
	//cv::Mat srcIn16; srcIn.convertTo(srcIn16,CV_16U);
	
	//convert 3-channel image into 1-channel image
	cvtColor(srcLeft, leftHLS, COLOR_BGR2GRAY, 1);
	cvtColor(srcRight, rightHLS, COLOR_BGR2GRAY, 1);
	cvtColor(srcLeft, srcLeftY, COLOR_BGR2GRAY, 1);
	cvtColor(srcRight, srcRightY, COLOR_BGR2GRAY, 1);

	// Apply OpenCV reference stereoBM
	std::cout << "running golden model" << std::endl;
	Ptr<cv::StereoBM> sbm = cv::StereoBM::create(numberOfDisparitiesD, blockSizeD);
	sbm->setPreFilterCap(31);
	sbm->setUniquenessRatio(15);
	sbm->setTextureThreshold(20);
	sbm->setMinDisparity(0);
	
	timer.StartTimer();
	for (int i = 0; i < numberOfIterations; i++){
	   sbm->compute(srcLeftY, srcRightY, disparitySW);
	}
	timer.StopTimer();
	std::cout << "Elapsed time over " << numberOfIterations << "SW call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;
	
	// Call wrapper for xf::stereoBM
	std::cout << "running hardware stereoBM" << std::endl;
	Ptr<xF::StereoBM> xfsbm = xF::StereoBM::create(numberOfDisparitiesD, blockSizeD);
	xfsbm->setPreFilterCap(31);
	xfsbm->setUniquenessRatio(15);
	xfsbm->setTextureThreshold(20);
	xfsbm->setMinDisparity(0);
	timer.StartTimer();
	for (int i = 0; i < numberOfIterations; i++){
		xfsbm->compute(leftHLS, rightHLS, disparityHLS);
	}
	timer.StopTimer();	
	std::cout << "Elapsed time over " << numberOfIterations << "PL call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	// compare results
	std::cout << "comparing HLS versus golden" << std::endl;
	int numberOfDifferences = 0;
	double errorPerPixel = 0;
	
	cv::Mat disparitySWInFloat(disparitySW.size(),CV_32F);
	fixedPointToCvConversion(disparitySW,disparitySWInFloat,4); //OPenCV uses fixed point 12.4 format
	
	cv::Mat disparityHLSInFloat(disparityHLS.size(),CV_32F);
	fixedPointToCvConversion(disparityHLS,disparityHLSInFloat,4); // xfOpenCV uses fixed point 12.4 format
	
	imageCompare(disparityHLSInFloat, disparitySWInFloat, numberOfDifferences, errorPerPixel, true, false);
	std::cout << "number of differences: " << numberOfDifferences << " average L1 error: " << errorPerPixel << std::endl;

	//write back images in files
	if (writeSWResult)
		writeImage(filenameSW, disparitySW);
	if (writePLResult)
		writeImage(filenamePL, disparityHLS);

	// Output input and filter output
	if (imShowOn) {
		imshow("Input left", leftHLS);
		imshow("Input right", rightHLS);
		
		double minVal; double maxVal;
		minMaxLoc(disparitySW, &minVal, &maxVal);
		
		Mat tmpShowDisparitySW, showDisparitySW;
		disparitySW.convertTo(tmpShowDisparitySW,CV_8U,255.0/maxVal); // use maxVal for nicer colors instead of (numberOfDisparitiesD*16.0) disparity in 16U is 12.4 format, so we need *16
		applyColorMap(tmpShowDisparitySW,showDisparitySW,COLORMAP_JET); 		
		imshow("Processed (SW)", showDisparitySW);
		
		Mat tmpShowDisparityHLS, showDisparityHLS;
		disparityHLS.convertTo(tmpShowDisparityHLS,CV_8U,255.0/maxVal); // use maxVal for nicer colors instead of (numberOfDisparitiesD*16.0) disparity in 16U is 12.4 format, so we need *16
		applyColorMap(tmpShowDisparityHLS,showDisparityHLS,COLORMAP_JET);
		imshow("Processed (PL)", showDisparityHLS);
		waitKey(0);
	}

	return 0;
}
