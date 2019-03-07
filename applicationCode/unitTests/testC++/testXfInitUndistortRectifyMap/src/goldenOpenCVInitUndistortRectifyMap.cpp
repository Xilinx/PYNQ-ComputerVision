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
*     Date:   2018/09/25
*
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"

#include "OpenCVUtils.h"
#include "HRTimer.h"
#include "cameraParameters.h"
using namespace cv;

void initializeCameraParameters(Mat &cameraMatrixLeft, Mat &cameraMatrixRight, Mat &distCoeffsLeft, Mat &distCoeffsRight, Mat &R, Mat &newCameraMatrixLeft, Mat &newCameraMatrixRight)
{
	// set up the camera parameters
	cameraMatrixLeft = Mat(3, 3, CV_64FC1, &cameraMA_l);
	cameraMatrixRight = Mat(3, 3, CV_64FC1, &cameraMA_r);
	distCoeffsLeft = Mat(1, 5, CV_64FC1, &distC_l);
	distCoeffsRight = Mat(1, 5, CV_64FC1, &distC_r);
	R = Mat(3, 3, CV_64FC1);
	newCameraMatrixLeft = Mat(3, 3, CV_64FC1);
	newCameraMatrixRight = Mat(3, 3, CV_64FC1);

	Mat iRLeft = Mat(3, 3, CV_64FC1, &irA_l);
	Mat iRRight = Mat(3, 3, CV_64FC1, &irA_r);

	//hard code as identity matrix for now
	R = Mat::eye(3, 3, CV_64FC1);

	//calculate the newCameraMatrix
	newCameraMatrixLeft = iRLeft*R;
	newCameraMatrixRight = iRRight*R;

	newCameraMatrixLeft = newCameraMatrixLeft.inv(); // camara parameters only contain (newCameraMatrix*R)^-1
	newCameraMatrixRight = newCameraMatrixRight.inv(); // camara parameters only contain (newCameraMatrix*R)^-1
}

void initializeZedCameraParameters(Mat &cameraMatrixLeft, Mat &cameraMatrixRight, Mat &distCoeffsLeft, Mat &distCoeffsRight, Mat &RLeft, Mat &RRight, Mat &newCameraMatrixLeft, Mat &newCameraMatrixRight, Mat &Q, int width, int height)
{
	// set up the camera parameters VGA
	if (width < 720) { //VGA parameters
		cameraMatrixLeft = Mat(3, 3, CV_64FC1, &zed_vga_cameraMA_l);
		cameraMatrixRight = Mat(3, 3, CV_64FC1, &zed_vga_cameraMA_r);
		distCoeffsLeft = Mat(1, 5, CV_64FC1, &zed_vga_distC_l);
		distCoeffsRight = Mat(1, 5, CV_64FC1, &zed_vga_distC_r);
	}
	else //1080p
	{
		cameraMatrixLeft = Mat(3, 3, CV_64FC1, &zed_1080p_cameraMA_l);
		cameraMatrixRight = Mat(3, 3, CV_64FC1, &zed_1080p_cameraMA_r);
		distCoeffsLeft = Mat(1, 5, CV_64FC1, &zed_1080p_distC_l);
		distCoeffsRight = Mat(1, 5, CV_64FC1, &zed_1080p_distC_r);
	}

	// Rotation matrix
	double rx = 0.00136952;  //in ZED conf file: [STEREO] RX_
	double ry = 0.0041293;   //in ZED conf file: [STEREO] CV_
	double rz = 0.000577991; //in ZED conf file: [STEREO] RZ_
	double rot[3] = { rx,ry,rz };
	Mat rotMat(3, 1, CV_64FC1, rot);
	Mat R(3, 3, CV_64FC1);
	Rodrigues(rotMat, R);

	// Translation matrix
	//double trans[3] = { -120.0,0.0,0.0 }; //in ZED conf file: [STEREO] Baseline
	double trans[3] = { -0.12,0.0,0.0 }; //in ZED conf file: [STEREO] Baseline
	Mat T(3, 1, CV_64FC1, trans);

	stereoRectify(cameraMatrixLeft, distCoeffsLeft, cameraMatrixRight, distCoeffsRight, Size(width,height), R, T, RLeft, RRight, newCameraMatrixLeft, newCameraMatrixRight, Q);
	//std::cout << "Q: " << Q << std::endl;
}

int main(int argc, char** argv)
{
	HRTimer timer;
	Mat cameraMatrixLeft, cameraMatrixRight, distCoeffsLeft, distCoeffsRight, RLeft, RRight, iRLeft, iRRight, newCameraMatrixLeft, newCameraMatrixRight;

	// Command line parsing
	const String keys =
		"{help h usage ? |                 | print this message                   }"
		"{zed z          |                 | use zed camera matrices              }"
		"{width          | 1920            | map width                            }"
		"{height         | 1080            | map height                           }"
		"{iterations n   | 1               | number of iterations to measure time }"
		;

	CommandLineParser parser(argc, argv, keys.c_str());
	if (parser.has("help"))
	{
		parser.printMessage();
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		return 0;
	}

	bool useZedParameters = parser.has("zed");
	unsigned int numberOfIterations;
	if (parser.has("iterations"))
		numberOfIterations = parser.get<unsigned int>("iterations");
	int width;
	if (parser.has("width"))
		width = parser.get<unsigned int>("width");	
	int height;
	if (parser.has("height"))
		height = parser.get<unsigned int>("height");

	if(!parser.check())
	{
		parser.printErrors();
		return(-1);
	}
	
	int mapType = MapTypeInitUndistortRecitfyMapD;

	Mat mapX(Size(width,height), mapType);
	Mat mapY(Size(width,height), mapType);

	if(useZedParameters) {
		Mat Q;
		initializeZedCameraParameters(cameraMatrixLeft, cameraMatrixRight, distCoeffsLeft, distCoeffsRight, RLeft, RRight, newCameraMatrixLeft, newCameraMatrixRight, Q, (int)width, (int)height);
	}
	else
	{
		initializeCameraParameters(cameraMatrixLeft, cameraMatrixRight, distCoeffsLeft, distCoeffsRight, RLeft, newCameraMatrixLeft, newCameraMatrixRight);
		RLeft.copyTo(RRight);
	}
	
	// Golden Rectification and Disparity calculation
	std::cout << "calling sw component" << std::endl;
	timer.StartTimer();
	
	for (int i = 0; i < numberOfIterations; i++){
		cv::initUndistortRectifyMap(cameraMatrixLeft, distCoeffsLeft, RLeft, newCameraMatrixLeft, Size(width,height), mapType, mapX, mapY);
	}
	
	timer.StopTimer();
	std::cout << "Elapsed time over " << numberOfIterations << " SW call(s): " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;

	return 0;
}
