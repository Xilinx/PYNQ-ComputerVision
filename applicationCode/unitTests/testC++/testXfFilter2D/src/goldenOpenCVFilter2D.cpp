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
*     Date:   2015/12/05
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <OpenCVUtils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <HRTimer.h>

using namespace cv;

/** @function main */
int main ( int argc, char** argv )
{
	//command line parsing
	const String keys =
	"{help h usage ? |                 | print this message                   }"
	"{@image	     |                 | in image 	                          }"
	"{display d      |                 | diplay result with imshow            }"
	"{interations n  | 1               | number of iterations to measure time }"
	"{goldenFile g   |                 | golden output image (from SW)        }"
	"{goldenText t   |                 | golden output in text (yaml format)  }"
	"{kernelFile k   |                 | filter kernel input file             }"
	"{blurDemo       |                 | blur demo (increasing filter size)   }"
	"{pixelDepth p   |                 | pixel depth as CV_ type string       }"
	"{borderType b   |                 | border type as CV::BorderTypes       }"
	;

	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help") || argc < 2)
	{
		parser.printMessage();
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		return 0;
	}

	String fileName = parser.get<String>(0);

	unsigned int numberOfIterations = 0;
	if (parser.has("interations"))
	numberOfIterations = parser.get<unsigned int>("interations");

	bool imShowOn = parser.has("display");

	String imageFilenameSW; bool writeSWResultImage = false;
	if(parser.has("goldenFile")) {
		imageFilenameSW = parser.get<String>("goldenFile");
		writeSWResultImage = true;
		std::cout << "need to write golden image: " << imageFilenameSW  << std::endl;
	}
	
	String textFilenameSW; bool writeSWResultText = false;
	if(parser.has("goldenText")) {
		textFilenameSW = parser.get<String>("goldenText");
		writeSWResultText = true;
		std::cout << "need to write golden text: " << textFilenameSW  << std::endl;
	}
	
	String kernelFilename; bool readKernel = false;
	if(parser.has("kernelFile")) {
		kernelFilename = parser.get<String>("kernelFile");
		readKernel = true;
		std::cout << "need to read kernel: " << kernelFilename  << std::endl;
	}
	
	bool enableBlurDemo = parser.has("blurDemo");
	int ddepth = -1;
	if(parser.has("pixelDepth")) {
		ddepth = imageTypeString2ImageType(parser.get<String>("pixelDepth"));
	}
	
	int borderType = BORDER_DEFAULT;
	if(parser.has("borderType")) {
		borderType = borderTypeString2BorderType(parser.get<String>("borderType"));
	}	
	 

	/// Declare variables
	Mat src, dst;

	Mat kernel;
	Point anchor;
	double delta;
	
	int kernelSize;
	int c;

	// Initialize
	initializeSingleImageTest(fileName, src);

	/// Initialize arguments for the filter
	anchor = Point( -1, -1 );
	delta = 0;
	kernelSize = 3;
	if (readKernel)
	{
		//Use Yaml file instead
		try {
			FileStorage kernelTxtFile(kernelFilename, FileStorage::READ);
			kernelTxtFile["kernel"] >> kernel;
			std::cout << "Kernel from file, kernelSize: " << kernelSize << " values: " << kernel << std::endl;
		}
		/*try {
			std::ifstream kernelTxtFile;
			kernelTxtFile.open(kernelFilename.c_str());
			kernelTxtFile >> kernelSize;
			std::vector<float> kernelVector;
			float tmpFloat;
			while (kernelTxtFile >> tmpFloat) {
				kernelVector.push_back(tmpFloat);
			}
			kernel = Mat(kernelSize,kernelSize,CV_32F);
			memcpy(kernel.data,kernelVector.data(),kernelSize*kernelSize*sizeof(float));
			std::cout << "Kernel from file, kernelSize: " << kernelSize << " values: " << kernel << std::endl;
		}*/ 
		catch (std::exception &e) {
			const char* errorMessage = e.what();
			std::cerr << "Exception caught: " << errorMessage << std::endl;
			std::cout << "\nhit enter to quit...";
			std::cin.get();
			exit(-1);
		}
	}
	else
		kernel = Mat::ones(kernelSize, kernelSize, CV_32F) / (float)(kernelSize*kernelSize);
	
	Mat srcY, dstY;
	std::cout << "starting time measurement: " << numberOfIterations << std::endl;
	if (numberOfIterations > 0) {
		HRTimer timer;

		cvtColor(src, srcY, COLOR_BGR2GRAY);
		
		timer.StartTimer();
		for (unsigned int i = 0; i < numberOfIterations; i++)
			filter2D(srcY, dstY, ddepth, kernel, anchor, delta, borderType);
		timer.StopTimer();

		std::cout << "Elapsed time over " << numberOfIterations << " calls: " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;
		std::cout << "Framerate: " << (float)numberOfIterations / ((float)timer.GetElapsedUs() / 1e6) << std::endl;
		
		if (writeSWResultImage)
			writeImage(imageFilenameSW, dstY);
		
		if (writeSWResultText) {
			FileStorage fs(textFilenameSW, FileStorage::WRITE);
			fs << "image" << dstY;
		}
	}
	
	if (imShowOn) {
		imshow("input",srcY);
		imshow("filtered",dstY);
		waitKey();
	}

	if (enableBlurDemo) {
		/// Loop - Will filter the image with different kernel sizes each 0.5 seconds
		int ind = 0;
		while( true )
		{
			c = waitKey(500);
			/// Press 'ESC' to exit the program
			if( (char)c == 27 )
			{
				break;
			}

			/// Update kernel size for a normalized box filter
			kernelSize = 3 + 2*( ind%5 );
			kernel = Mat::ones( kernelSize, kernelSize, CV_32F )/ (float)(kernelSize*kernelSize);

			/// Apply filter
			filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
			imshow("Filter OpenCV", dst );
			ind++;
		}
	}

	return 0;
}
