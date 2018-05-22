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

#ifndef _OPENCVUTILS_H_
#define _OPENCVUTILS_H_

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

bool imageCompare(cv::Mat &test, cv::Mat &golden, int &numberOfDifferences, double &error, bool listPositionFirstDifference = false, bool displayResult = false, double epsilon = 0.01);
void readImage(const std::string &fileName, cv::Mat &image, int flags = 1);
void writeImage(const std::string &fileName, cv::Mat &image);
void writeImage(const std::string &fileName, cv::Mat &image, const std::vector<int> &params);
void writeImageAsTextFile(cv::Mat in, std::ofstream &file);
std::string type2str(int type);
int imageTypeString2ImageType (std::string typeName);
int borderTypeString2BorderType (std::string typeName);

void initializeSingleGrayImageTest(std::string fileName, cv::Mat &src);
void initializeSingleGrayImageTest(int argc, char ** argv, cv::Mat &src);
void initializeSingleImageTest(int argc, char ** argv, cv::Mat &src);
void initializeSingleImageTest(std::string fileName, cv::Mat &src);
void initializeDualImageTest(std::string fileName1, std::string fileName2, cv::Mat &src1, cv::Mat &src2);

std::vector<cv::Rect> mergeIntersectingBoundingBoxes(std::vector<cv::Rect> listOfBoxes);
#endif
