/*****************************************************************************
 *
 *     Author: Xilinx, Inc.
 *
 *     This text contains proprietary, confidential information of
 *     Xilinx, Inc. , is distributed by under license from Xilinx,
 *     Inc., and may be used, copied and/or disclosed only pursuant to
 *     the terms of a valid license agreement with Xilinx, Inc.
 *
 *     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
 *     AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
 *     SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
 *     OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
 *     APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
 *     THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
 *     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
 *     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
 *     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
 *     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
 *     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
 *     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE.
 *
 *     Xilinx products are not intended for use in life support appliances,
 *     devices, or systems. Use in such applications is expressly prohibited.
 *
 *     (c) Copyright 2011 Xilinx Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "SDSOCCornerHarris.h"

#include "hls_video.h"

//include xF::mat prototype
#include <Mat/inc/mat.hpp>

//PL instatiation parameters
#define maxHeight 1080
#define maxWidth 1920
#define srcType HLS_8U
#define srcChannels 1
#define dstType HLS_32F
#define dstChannels 1

typedef float kernelType;


#pragma SDS data access_pattern(img_in:SEQUENTIAL, img_out:SEQUENTIAL)
#pragma SDS data copy(img_in[0:rows*cols], img_out[0:rows*cols])
#pragma SDS data sys_port(img_in: AFI, img_out: AFI)
void sdsocCornerHarris(HLS_TNAME(srcType)* img_in, HLS_TNAME(dstType)* img_out, int rows, int cols, int stride, kernelType K)
{
	hls::Mat<maxHeight, maxWidth, HLS_MAKE_TYPE(srcType,srcChannels)> srcHW(rows, cols);
	hls::Mat<maxHeight, maxWidth, HLS_MAKE_TYPE(dstType,dstChannels)> dstHW(rows, cols);

	#pragma HLS dataflow
	/* copy data into hls::Mat (Array -> hls::Mat)*/
	hls::Array2Mat<maxWidth>(img_in, stride, srcHW);

	/* executing filter function */
	hls::CornerHarris<blockSizeD,kSizeD,kernelType>(srcHW, dstHW, K);

	/* copy data out of hls::Mat (hls::Mat -> Array) */
	hls::Mat2Array<maxWidth>(dstHW, img_out, stride);
	
	return;
}

#ifndef __SDSVHLS__

void sdsoc_CornerHarris(cv::Mat &src, cv::Mat &dst, int blockSize, int kSize, double k, int borderType)
{
	assert(srcChannels == 1 && src.channels() == 1);
	assert(blockSize == blockSizeD);
	assert(kSize == kSizeD);
	int ddepth = CV_32F;
	
	cv::Mat srcHLS, dstHLS;
	kernelType K = (kernelType)k;

	// perform some checks on the src type
	if (src.depth() == srcType && src.depth() < HLS_USRTYPE1) { // no conversion needed if types match and are native C types
		std::cout << "provided src type matches instantiated core type" << std::endl;
		srcHLS = src;
	}
	else { // if types do not match, perform SW conversion
		std::cout << "provided src type does not match instantiated core type, applying SW conversion" << std::endl;
		src.convertTo(srcHLS, srcType);
	}

	// perform some checks on the dst type
	ddepth = (ddepth == -1) ? src.depth() : ddepth;
		
	bool dstPostConversion = false;
	if (dst.empty())
	{
		std::cout << "dst not yet allocated" << std::endl;
		dst = xF::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));		
	} else if (dst.type() != ddepth) {
		std::cout << "dst allocated does not match ddepth, reallocating" << std::endl;
		dst = xF::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));
	}
		
	if (ddepth == dstType && ddepth < HLS_USRTYPE1) { // no conversion needed if types match and are native C types
		std::cout << "provided dst type matches instantiated core type" << std::endl;
		dstHLS = dst;
	}
	else
	{
		dstHLS = cv::Mat(src.size(),CV_MAKE_TYPE(dstType,1));
		std::cout << "provided output does not match, need SW post conversion" << std::endl;
		dstPostConversion = true;
	}


	// call BilateralFilter respecting SDSoC array limitation;
	sdsocCornerHarris((HLS_TNAME(srcType)*) srcHLS.data, (HLS_TNAME(dstType)*) dstHLS.data, srcHLS.size().height, srcHLS.size().width, srcHLS.step/srcHLS.elemSize(), K);

	
	
	return;

}

#endif
