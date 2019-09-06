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
*     Date:   2016/06/02
*
*****************************************************************************/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "OpenCVUtils.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void readImage(const std::string &fileName, Mat &image, int flags)
{
	// Load an image
	image = imread(fileName, flags);
	if (!image.data) {
		std::stringstream errorMessage;
		errorMessage << "Failed to load image " << fileName;
		CV_Error(Error::StsBadArg, errorMessage.str());
	}
}

void writeImage(const std::string &fileName, Mat &image)
{
	//Write an image
	if (!image.data) {
		std::stringstream errorMessage;
		errorMessage << "Failed to write image " << fileName;
		CV_Error(Error::StsBadArg, errorMessage.str());
	}
	imwrite(fileName, image);
}

void writeImage(const std::string &fileName, Mat &image, const std::vector<int> &params)
{
	//Write an image
	if (!image.data) {
		std::stringstream errorMessage;
		errorMessage << "Failed to write image " << fileName;
		CV_Error(Error::StsBadArg, errorMessage.str());
	}
	imwrite(fileName, image, params);
}

template<typename T>
void listFirstDifferenceTwoMatrices(Mat &test, Mat &golden, double epsilon)
{
	bool foundFirstDifference = false;

	for (int i = 0; i < golden.rows; i++) {
		for (int j = 0; j < golden.cols; j++) {
			T *pGolden = golden.ptr<T>(i, j);
			T *pTest = test.ptr<T>(i, j);
			for (int k = 0; k < golden.channels(); k++) {
				double goldenValue = (double)pGolden[k];
				double testValue = (double)pTest[k];
				if (abs(goldenValue - testValue) > epsilon) {
					std::cout << "Mismatach at (" << i << "," << j << ") channel: " << k << " golden: " << goldenValue << " test: " << testValue << std::endl;
					foundFirstDifference = true;
				}
			}
			if (foundFirstDifference)
				break;
		}
		if (foundFirstDifference)
			break;
	}
}

bool imageCompare(Mat &test, Mat &golden, int &numberOfDifferences, double &error, bool listPositionFirstDifference, bool displayResult, double epsilon)
{
	bool identical = true;
	numberOfDifferences = -1;
	error = -1;

	if(test.rows != golden.rows || test.cols != golden.cols || test.channels() != golden.channels() || test.depth() != golden.depth()) {
		identical = false;
		std::cerr << "Error: image sizes do not match, golden: " << golden.cols << "x" << golden.rows << " test: " << test.cols << "x" << test.rows <<" golden channels " << golden.channels() << " test channels " << test.channels() << " golden depth " << golden.depth() << " test depth " << test.depth() <<  std::endl;
	}
	else {
		Mat difference = Mat(golden.size(), golden.type());

		error = norm(test, golden, NORM_L1);
		error /= (double)(test.rows*test.cols);
		absdiff(test, golden, difference);

		numberOfDifferences = 0;
		for (int k = 0; k < golden.channels(); k++) {
			Mat differenceChannel;
			extractChannel(difference, differenceChannel, k);
			numberOfDifferences += countNonZero(differenceChannel);
		}

		if (numberOfDifferences != 0)
			identical = false;

		if (displayResult)
		{
			const char* differenceWindowName = "difference";
			namedWindow(differenceWindowName, WINDOW_AUTOSIZE);
			imshow(differenceWindowName, difference);
		}

		if (listPositionFirstDifference && !identical) {
			switch (golden.depth()) {
				case CV_8U:
					listFirstDifferenceTwoMatrices<uchar>(test, golden, epsilon);
					break;
				case CV_8S:
					listFirstDifferenceTwoMatrices<char>(test, golden, epsilon);
				case CV_16U:
					listFirstDifferenceTwoMatrices<ushort>(test, golden, epsilon);
					break;
				case CV_16S:
					listFirstDifferenceTwoMatrices<short>(test, golden, epsilon);
					break;
				case CV_32S:
					listFirstDifferenceTwoMatrices<int>(test, golden, epsilon);
					break;
				case CV_32F:
					listFirstDifferenceTwoMatrices<float>(test, golden, epsilon);
					break;
				case CV_64F:
					listFirstDifferenceTwoMatrices<double>(test, golden, epsilon);
					break;
				default:
					std::cerr << "unexpected CV type" << std::endl;
			}
		}
	}

	return identical;
}

bool compareKeypointPoints (std::vector<KeyPoint> test, std::vector<KeyPoint> golden)
{
	bool returnValue = true;
	
	if (golden.size() == test.size())
	{
		std::cout << "Same number of keypoints found, starting elementwise comparison" << std::endl;
		
		for (int i = 0; i < golden.size(); i++ )
		{
			auto keyGolden = golden[i];
			auto keyTest = test[i];
			
			if (keyGolden.pt != keyTest.pt)
			{
				std::cout << "keypoint differs" << i << " : golden: " << keyGolden.pt << " , test: " << keyTest.pt << std::endl;
				returnValue = false;
			}
		}
		
		if (returnValue)
			std::cout << "keypoint comparison succesfull" << std::endl; 
	}
	else
	{
		std::cout << "Different number of keypoints: golden: " << golden.size() << ", test: " << test.size() << std::endl;
		returnValue = false;
	}
		
	return returnValue;
	
}

template<typename srcTypeTP, typename dstTypeTP>
void deepSlowConvertFixedPointToCvMat(cv::Mat &src, cv::Mat &dst, unsigned int scaleFactor)
{
	const int channels = 1; 
	
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			srcTypeTP *pSrc = src.ptr<srcTypeTP>(i, j);
			dstTypeTP *pDst = dst.ptr<dstTypeTP>(i, j);
			for (int k = 0; k < channels; k++) {
				float tmpFloat = (float) pSrc[k];
				tmpFloat = tmpFloat/(1 << scaleFactor);
				pDst[k] = (dstTypeTP) tmpFloat;
			}
		}
	}
}

void fixedPointToCvConversion(cv::Mat &src, cv::Mat &dst, unsigned int scaleFactor)
{
	if (src.depth()==CV_16S){
		
		switch (dst.depth()) {
		case CV_8U:
			deepSlowConvertFixedPointToCvMat<short,uchar>(src, dst, scaleFactor);
			break;
		case CV_8S:
			deepSlowConvertFixedPointToCvMat<short,char>(src, dst, scaleFactor);
		case CV_16U:
			deepSlowConvertFixedPointToCvMat<short,ushort>(src, dst, scaleFactor);
			break;
		case CV_16S:
			deepSlowConvertFixedPointToCvMat<short,short>(src, dst, scaleFactor);
			break;
		case CV_32S:
			deepSlowConvertFixedPointToCvMat<short,int>(src, dst, scaleFactor);
			break;
		case CV_32F:
			deepSlowConvertFixedPointToCvMat<short,float>(src, dst, scaleFactor);
			break;
		case CV_64F:
			deepSlowConvertFixedPointToCvMat<short,double>(src, dst, scaleFactor);
			break;
		default:
			std::cerr << "unexpected CV type" << std::endl;
		}
	}
	else if (src.depth() == CV_16U) {
		switch (dst.depth()) {
		case CV_8U:
			deepSlowConvertFixedPointToCvMat<ushort,uchar>(src, dst, scaleFactor);
			break;
		case CV_8S:
			deepSlowConvertFixedPointToCvMat<ushort,char>(src, dst, scaleFactor);
		case CV_16U:
			deepSlowConvertFixedPointToCvMat<ushort,ushort>(src, dst, scaleFactor);
			break;
		case CV_16S:
			deepSlowConvertFixedPointToCvMat<ushort,short>(src, dst, scaleFactor);
			break;
		case CV_32S:
			deepSlowConvertFixedPointToCvMat<ushort,int>(src, dst, scaleFactor);
			break;
		case CV_32F:
			deepSlowConvertFixedPointToCvMat<ushort,float>(src, dst, scaleFactor);
			break;
		case CV_64F:
			deepSlowConvertFixedPointToCvMat<ushort,double>(src, dst, scaleFactor);
			break;
		default:
			std::cerr << "unexpected CV type" << std::endl;
		}		
	}
	else
		std::cerr << "unexpected CV src type" << std::endl;
}

template<typename T, typename Tcast>
void writeImageAsTextFileT(Mat in, std::ofstream &file)
{
	for (int i = 0; i < in.rows; i++) {
		for (int j = 0; j < in.cols; j++) {
			T *pIn = in.ptr<T>(i, j);
			for (int k = 0; k < in.channels(); k++) {
				T inValue = pIn[k];
				file << (Tcast) inValue << " ";
			}
			file << "\n";
		}
	}
}

void writeImageAsTextFile(Mat in, std::ofstream &file){
	if(!file.is_open()){
		std::stringstream errorMessage;
		errorMessage << "Output file not open";
		CV_Error(Error::StsBadArg, errorMessage.str());
	}

	switch (in.depth()) {
		case CV_8U:
		writeImageAsTextFileT<uchar,int>(in, file);
		break;
		case CV_8S:
		writeImageAsTextFileT<char,int>(in, file);
		case CV_16U:
		writeImageAsTextFileT<ushort,int>(in, file);
		break;
		case CV_16S:
		writeImageAsTextFileT<short,short>(in, file);
		break;
		case CV_32S:
		writeImageAsTextFileT<int,int>(in, file);
		break;
		case CV_32F:
		writeImageAsTextFileT<float,float>(in, file);
		break;
		case CV_64F:
		writeImageAsTextFileT<double,double>(in, file);
		break;
		default:
		std::stringstream errorMessage;
		errorMessage << "unexpected CV type";
		CV_Error(Error::StsBadArg, errorMessage.str());
	}
}


std::string type2str(int type) {
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int imageTypeString2ImageType (std::string typeName)
{
	int channels = 1;
	std::string typeNameNoChannels;
	
	std::size_t found = typeName.find_first_of('C', 1);
	if (found !=std::string::npos) { //channels defined
		std::cout << "pos: " << found << " channels: " << typeName.substr(found+1,found+1) << std::endl;
		channels = std::stoi(typeName.substr(found+1,found+1));
		typeNameNoChannels = typeName.substr(0,found);
	} else
		typeNameNoChannels = typeName;
	std::cout << "channels: " << channels << std::endl;
	std::cout << "typeNameNoChannels: " << typeNameNoChannels << std::endl;
	
	int depth = 0;
	if (typeNameNoChannels == std::string("CV_8U")) depth = CV_8U; 
	else if (typeNameNoChannels == std::string("CV_8S")) depth = CV_8S;
	else if (typeNameNoChannels == std::string("CV_16U")) depth = CV_16U;
	else if (typeNameNoChannels == std::string("CV_16S")) depth = CV_16S;
	else if (typeNameNoChannels == std::string("CV_32S")) depth = CV_32S;
	else if (typeNameNoChannels == std::string("CV_32F")) depth = CV_32F;
	else if (typeNameNoChannels == std::string("CV_64F")) depth = CV_64F;
	else depth = -1; // unknown type
	
	return CV_MAKETYPE(depth,channels);
	
}

int borderTypeString2BorderType (std::string typeName)
{
	int borderType = -1;
	if (typeName == std::string("BORDER_CONSTANT")) borderType = BORDER_CONSTANT;
	else if (typeName == std::string("BORDER_REPLICATE")) borderType = BORDER_REPLICATE;
	else if (typeName == std::string("BORDER_WRAP")) borderType = BORDER_WRAP;
	else if (typeName == std::string("BORDER_REFLECT_101")) borderType = BORDER_REFLECT_101;
	else if (typeName == std::string("BORDER_REFLECT101")) borderType = BORDER_REFLECT101;
	else if (typeName == std::string("BORDER_DEFAULT")) borderType = BORDER_DEFAULT;
	else if (typeName == std::string("BORDER_ISOLATED")) borderType = BORDER_ISOLATED;
	else borderType = BORDER_DEFAULT; // unkown type, fall back to default;
	return borderType;
}

void initializeSingleGrayImageTest(int argc, char ** argv, Mat &src)
{
	// check program argument and try to open input files
	try
	{
		if (argc != 2) {
			std::stringstream errorMessage;
			errorMessage << argv[0] << " <inputImage>";
			std::cerr << errorMessage.str();
			throw std::invalid_argument(errorMessage.str());
		}

		// Load an image
		readImage(argv[1], src, IMREAD_GRAYSCALE );
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		exit(-1);
	}
}

void initializeSingleGrayImageTest(std::string fileName, Mat &src)
{
	// check program argument and try to open input files
	try
	{
		// Load an image
		readImage(fileName, src, IMREAD_GRAYSCALE );
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		exit(-1);
	}
}

void initializeSingleImageTest(int argc, char ** argv, Mat &src)
{
	// check program argument and try to open input files
	try
	{
		if (argc != 2) {
			std::stringstream errorMessage;
			errorMessage << argv[0] << " <inputImage>";
			std::cerr << errorMessage.str();
			throw std::invalid_argument(errorMessage.str());
		}

		// Load an image
		readImage(argv[1], src, IMREAD_COLOR );
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		exit(-1);
	}
}

void initializeSingleImageTest(std::string fileName, Mat &src)
{
	// check program argument and try to open input files
	try
	{
		// Load an image
		readImage(fileName, src, IMREAD_COLOR );
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		exit(-1);
	}
}

void initializeDualImageTest(std::string fileName1, std::string fileName2, Mat &src1, Mat &src2)
{
	// try to open input files
	try
	{
		// Load an image
		readImage(fileName1, src1, IMREAD_COLOR );
		readImage(fileName2, src2, IMREAD_COLOR );
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;
		std::cout << "\nhit enter to quit...";
		std::cin.get();
		exit(-1);
	}
}

std::vector<cv::Rect> mergeIntersectingBoundingBoxes(std::vector<cv::Rect> listOfBoxes) // recursive algorithm
{
	if (listOfBoxes.size() < 1)
	 	return listOfBoxes;

	int offset = 0;
  std::vector<cv::Rect> tmpList, returnList;

  while ((listOfBoxes.size() > 1) && (offset < (listOfBoxes.size()-1))) // check for overlapping boundingboxes
  {
    std::cout << "processing bounding box list: size: " << listOfBoxes.size() << " offset: " << offset << std::endl;
    for (int i=0; i < listOfBoxes.size(); i++)
      std::cout << i << ": " << listOfBoxes[i] << std::endl;
    Rect rect1 = listOfBoxes[offset];
    Rect rect2 = listOfBoxes[offset+1];
		Rect tmpRect1 = rect1;
		Rect tmpRect2 = rect2;

		//enlarged bounding box (1 pixel) to enable touching bounding boxes
		tmpRect1.x = (rect1.x > 0) ? rect1.x-1 : 0;
		tmpRect1.width = (rect1.x > 0) ? rect1.width+2 :rect1.width+1;
		tmpRect1.y = (rect1.y > 0) ? rect1.y-1 : 0;
		tmpRect1.height = (rect1.y > 0) ? rect1.height+2 : rect1.height+1;

		tmpRect2.x = (rect2.x > 0) ? rect2.x-1 : 0;
		tmpRect2.width = (rect2.x > 0) ? rect2.width+2 :rect2.width+1;
		tmpRect2.y = (rect2.y > 0) ? rect2.y-1 : 0;
		tmpRect2.height = (rect2.y > 0) ? rect2.height+2 : rect2.height+1;

    if ((rect1 & rect2) == rect2 ) // rect2 completely inside rect1
    {
      listOfBoxes.erase(listOfBoxes.begin()+offset+1); //remove rect 2
      offset = 0;
    }
    else if ((rect1 & rect2) == rect1) // rect1 completely inside rect2
    {
      listOfBoxes.erase(listOfBoxes.begin()+offset); //remove rect 1
      offset = 0;
    }
		else if((tmpRect1 & tmpRect2).area() > 0) // intersecting our touching boundingboxes, merge
    {
      cv::Rect newrect = rect1 | rect2;
      listOfBoxes.erase(listOfBoxes.begin()+offset);
      listOfBoxes.erase(listOfBoxes.begin()+offset);
      listOfBoxes.push_back(newrect);
      offset = 0;
    }
    else
      offset++;

    if ((listOfBoxes.size() > 1) && (offset == (listOfBoxes.size()-1))) {
      std::vector<cv::Rect> poppedFirstElementList(listOfBoxes);
      poppedFirstElementList.erase(poppedFirstElementList.begin());
      tmpList = mergeIntersectingBoundingBoxes(poppedFirstElementList);//recursive call
    }
  }

  returnList.push_back(listOfBoxes[0]);
  if (tmpList.size() > 0)
    returnList.insert(returnList.end(),tmpList.begin(),tmpList.end());
  return returnList;
}
