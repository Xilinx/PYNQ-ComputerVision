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
*     Date:   2017/11/22
*
*****************************************************************************/

#ifndef _XFSDXFILTER2D_H_
#define _XFSDXFILTER2D_H_

///SDx temporal fix for Clang issue
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

/*
#include "common/xf_common.h"
#include <hls_video.h>
#include "imgproc/xf_custom_convolution.hpp"
#include <Utils/inc/UtilsForXfOpenCV.h>
*/

namespace xF { 

//---------------------------------------------------------- Declaration ----------------------------------------------------------
//Template specialization not working yet in SDx
/*
template<int borderTypeTP, int kernelColsTP, int kernelRowsTP, int srcTypeTP, int dstTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP = XF_NPPC1>
void xF_filter2D(cv::Mat &src, cv::Mat &dst, int ddepth, cv::Mat &kernel, cv::Point anchor, double delta, int borderType = cv::BORDER_CONSTANT);
*/

void filter2D(cv::Mat &src, cv::Mat &dst, int ddepth, cv::Mat &kernel, cv::Point anchor=cv::Point( -1, -1 ), double delta=0, int borderType = cv::BORDER_CONSTANT);
//----------------------------------------------------------  Definition  ----------------------------------------------------------

//Template specialization not working yet in SDx
/*
//PL instatiation parameters
#define XFSHIFT		15  //NOTE XFSHIFT should be a template parameter as well
typedef short xfkernelType; //xfOpenCV forces coefficients as XF_16SP

template<int borderTypeTP, int kernelColsTP, int kernelRowsTP, int srcTypeTP, int dstTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP>
void xF_filter2D(cv::Mat &src, cv::Mat &dst, int ddepth, cv::Mat &kernel, cv::Point anchor, double delta, int borderType)
{
	assert(src.channels() == 1);
	assert(dst.channels() == 1);
	assert(borderTypeTP == XF_BORDER_CONSTANT); //XF only supports border constant
	assert(borderType == borderTypeTP);
	
	cv::Mat srcHLS, dstHLS;
	//cv::Mat tmpMat;
	
	const int scrDepthTP = XF_DEPTH(srcTypeTP,NPCTP);
	const int dstDepthTP = XF_DEPTH(dstTypeTP,NPCTP);
	
	xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>* imgInput;
	xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>* imgOutput;

	// perform some checks on the src type
	if (src.depth() == scrDepthTP && src.depth() < HLS_USRTYPE1) { // no conversion needed if types match and are native C types
		std::cout << "provided src type matches instantiated core type" << std::endl;
		imgInput =  new xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *)src.data);
	}
	else { // if types do not match, perform SW conversion
		std::cout << "provided src type does not match instantiated core type, applying SW conversion" << std::endl;
		cv::Mat tmpMat;
		src.convertTo(tmpMat, XF_XFDEPTH2CVDEPTH(scrDepthTP));
		imgInput = new xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols);
		std::cout << "conversion done, memory allocated, copying data" << std::endl;
		imgInput->copyTo(tmpMat.data);
		std::cout << "done copying data" << std::endl;
	}
	
	// perform some checks on the dst type
	ddepth = (ddepth == -1) ? src.depth() : ddepth;
		
	//check in dst Mat was already allocated and matches ddepth
	if (dst.empty())
	{
		std::cout << "dst not yet allocated" << std::endl;
		dst = cv::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));		
	} else if (dst.depth() != ddepth) {
		std::cout << "dst allocated does not match ddepth, reallocating" << std::endl;
		dst = cv::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));
	}
	
	bool dstPostConversion = false;
	if (ddepth == dstDepthTP && ddepth < HLS_USRTYPE1) { // no conversion needed if types match and are native C types
		std::cout << "provided dst type matches instantiated core type" << std::endl;
		imgOutput = new xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *) dst.data);
	}
	else
	{
		imgOutput = new xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols);
		std::cout << "provided output does not match, need SW post conversion" << std::endl;
		dstPostConversion = true;
	}

	//prepare kernel coefficients as array with kernelType fixed to XF_16SP
	
	xfkernelType kernelWindow[kernelRowsTP*kernelColsTP];
	for(int i=0; i<kernelRowsTP; i++)
		for(int j=0; j<kernelColsTP; j++)
		{
			kernelWindow[i*kernelRowsTP+j] = (xfkernelType) (kernel.at<float>(i,j) * (float)(1 << XFSHIFT));
		}

	// call xf::filter2D 
	xf::filter2D<borderTypeTP,kernelColsTP,kernelRowsTP,srcTypeTP,dstTypeTP,maxHeightTP, maxWidthTP,NPCTP>(*imgInput,*imgOutput,kernelWindow,XFSHIFT);

	//perform checks on the dst type
	if (dstPostConversion) {
		std::cout << "Dst type does not match, performing SW conversion" << std::endl;
		cv::Mat tmpMat2(src.rows,src.cols,CV_MAKE_TYPE(XF_XFDEPTH2CVDEPTH(dstDepthTP),src.channels()),imgOutput->copyFrom());
		tmpMat2.convertTo(dst, dst.type());
	}
	
	delete imgInput;
	delete imgOutput;
	
	return;
}
*/

} // namespace xF

#endif
