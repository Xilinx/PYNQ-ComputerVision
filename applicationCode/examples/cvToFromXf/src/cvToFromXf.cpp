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
*     Date:   2018/03/27
*
*****************************************************************************/

#include "common/xf_common.h"
#include "imgproc/xf_custom_convolution.hpp"
#include <Utils/inc/UtilsForXfOpenCV.h>
#include <OpenCVUtils.h>

int main ( int argc, char** argv )
{

	//define instantiation parameters
	const int maxWidthTP = 1920;   //maximum width
	const int maxHeightTP = 1080;  //maximum height
	const int srcTypeTP = XF_8UC1; //pixel type input, 1 channel, uint8
	const int dstTypeTP = XF_8UC1; //pixel type output, 1 channel, uint8
	const int NPCTP = XF_NPPC1;    //number of pixels per clock
	
	//declare cv::Mat source and destination
	cv::Mat src(maxHeightTP,maxWidthTP,CV_8UC1); 
	cv::Mat dst(maxHeightTP,maxWidthTP,CV_8UC1);
	cv::randu(src, 0, 255); //initialize source with random values
	
	//declare equivalent xf::Mat source and destination and extract depth
	xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>* imgInput;
	xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>* imgOutput;
	const int scrDepthTP = XF_DEPTH(srcTypeTP,NPCTP); const int srcChannelsTP = XF_CHANNELS(srcTypeTP,NPCTP);
	const int dstDepthTP = XF_DEPTH(dstTypeTP,NPCTP); const int dstChannelsTP = XF_CHANNELS(dstTypeTP,NPCTP);
	
	
	try {
		// check if the source data types match
		if (src.depth() == scrDepthTP && src.channels() == srcChannelsTP) { // shallow copy only possible if types match
			std::cout << "provided src type matches instantiated core type" << std::endl;
			imgInput =  new xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *)src.data);
		}
		else { // if types do not match, deal with it according to your preference, for now, throw an error
			throw std::invalid_argument("provided cv:: src type (depth or channels) does not match instantiated xf:: type");
		}
	
		// check if the destination data types match
		if (dst.depth() == dstDepthTP && dst.channels() == dstChannelsTP) { // shallow copy only possible if types match
			std::cout << "provided dst type matches instantiated core type" << std::endl;
			imgOutput =  new xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *)dst.data);
		}
		else { // if types do not match, deal with it according to your preference, for now, throw an error
			throw std::invalid_argument("provided cv:: dst type (depth or channels) does not match instantiated xf:: type");
		}
		
		//call the xfOpenCV module of interest, for example filter2D with a 1x1 kernel to mimic a data copy
		const int XFSHIFT = 12;
		short kernelWindow[1*1]; kernelWindow[0] = (short) (1 << XFSHIFT);
		xf::filter2D<XF_BORDER_CONSTANT,1,1,srcTypeTP,dstTypeTP,maxHeightTP, maxWidthTP,NPCTP>(*imgInput,*imgOutput,kernelWindow,XFSHIFT);

		//check if content src equal dst at cv:: level
		int numberOfDifferences = 0; double errorPerPixel = 0;
		imageCompare(src, dst, numberOfDifferences, errorPerPixel, true, false);
		std::cout << "number of differences: " << numberOfDifferences << " average error per pixel: " << errorPerPixel << std::endl;
	
	}
	catch (std::exception &e)
	{
		const char* errorMessage = e.what();
		std::cerr << "Exception caught: " << errorMessage << std::endl;	
	}
	catch (const char *e) {
		std::cerr << "Exception caught: " << e << std::endl;
	}
	
	std::cout << "\nhit enter to quit...";
	std::cin.get();
	return 0;

}