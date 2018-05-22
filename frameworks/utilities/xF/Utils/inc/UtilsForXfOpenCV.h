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
*             Jack Lo <jack.lo@xilinx.com>
*     Date:   2018/03/05
*
*****************************************************************************/

#ifndef __XF__UTILS_FOR_XFOPENCV
#define __XF__UTILS_FOR_XFOPENCV

#include "common/xf_common.h"
#ifdef __SDSCC__
#undef __ARM_NEON__
#undef __ARM_NEON
#include <opencv2/core/core.hpp>
#define __ARM_NEON__
#define __ARM_NEON
#else
#include <opencv2/core/core.hpp>
#endif

#include <hls_video.h> // TODO needed for HLS_USRTYPE1?
#include "imgproc/xf_custom_convolution.hpp"

template<int type, int channels> struct TypeAndChannels     { static const int xf_typeName = -1; };
template<> struct TypeAndChannels<XF_8UP,1>   { static const int xf_typeName = XF_8UC1; };
template<> struct TypeAndChannels<XF_16UP,1>  { static const int xf_typeName = XF_16UC1; };
template<> struct TypeAndChannels<XF_16SP,1>  { static const int xf_typeName = XF_16SC1; };
template<> struct TypeAndChannels<XF_32SP,1>  { static const int xf_typeName = XF_32SC1; };
template<> struct TypeAndChannels<XF_32FP,1>  { static const int xf_typeName = XF_32FC1; };
template<> struct TypeAndChannels<XF_32UP,1>  { static const int xf_typeName = XF_32UC1; };
#define XF_MAKETYPE(t,c) TypeAndChannels<t,c>::xf_typeName


template<int xfDepth> struct xfDepth2cvDepth {static const int cvDepth = -1; };
template<> struct xfDepth2cvDepth<XF_8UP> {static const int cvDepth = CV_8U; };
template<> struct xfDepth2cvDepth<XF_8SP> {static const int cvDepth = CV_8S; };
template<> struct xfDepth2cvDepth<XF_16UP> {static const int cvDepth = CV_16U; };
template<> struct xfDepth2cvDepth<XF_16SP> {static const int cvDepth = CV_16S; };
template<> struct xfDepth2cvDepth<XF_32SP> {static const int cvDepth = CV_32S; };
template<> struct xfDepth2cvDepth<XF_32FP> {static const int cvDepth = CV_32F; };
//template<> struct xfDepth2cvDepth<XF_64FP> {static const int cvDepth = CV_64F; };

#define XF_XFDEPTH2CVDEPTH(d) xfDepth2cvDepth<d>::cvDepth
	
template<int srcTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP>
void cvToXfSource(cv::Mat &src, xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>* &imgInput)
{
	// Extract depth and number of channels from xF type
	const int scrDepthTP = XF_DEPTH(srcTypeTP,NPCTP);
	const int srcChannelsTP = XF_CHANNELS(srcTypeTP,NPCTP);
	
	// Check for matching data depths and number of channels in src cv::Mat and xf::Mat and perform sw depth conversion if necessary
	if (src.depth() == XF_XFDEPTH2CVDEPTH(scrDepthTP) && src.channels() == srcChannelsTP /* &&src.channels() < HLS_USRTYPE1*/) { // shallow copy only possible if types match (and are native C types)
		//std::cout << "provided src type matches instantiated core type" << std::endl;
		imgInput =  new xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *)src.data); // allocate xf::Mat (xFsrc) reusing cv::Mata data buffer (shallow copy)
	}
	else { // if types do not match, perform SW conversion
		std::cout << "provided src type does not match instantiated core type, applying SW conversion" << std::endl;
		cv::Mat tmpMat; // temporary opencv Mat
		src.convertTo(tmpMat, XF_XFDEPTH2CVDEPTH(scrDepthTP)); // SW convert src data to correct depth and copy into tmpMat
		imgInput = new xf::Mat<srcTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols); // allocate xf::Mat (xFsrc)
		std::cout << "conversion done, memory allocated, copying data" << std::endl;
		imgInput->copyTo(tmpMat.data); // copy converted data to xf::Mat 
		std::cout << "done copying data" << std::endl;
	}
}

template<int dstTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP>
bool cvToXfDestination(cv::Mat &dst, xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>* &imgOutput, cv::Mat &src, int ddepth = -1)
{
	// Extract depth and number of channels from xF type
	const int dstDepthTP = XF_DEPTH(dstTypeTP,NPCTP); 
	const int dstChannelsTP = XF_CHANNELS(dstTypeTP,NPCTP);
	
	// perform some checks on the dst type
	ddepth = (ddepth == -1) ? src.depth() : ddepth;
		
	//check in dst Mat was already allocated and matches ddepth
	if (dst.empty())
	{
		std::cout << "dst not yet allocated" << std::endl;
		dst = cv::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));		
	} else if (dst.depth() != ddepth || dst.channels() != src.channels()) { // if dst cv::Mat has wrong depth, or has a different number of channels than the source re-allocate it 
		std::cout << "dst allocated does not match ddepth, reallocating" << std::endl;
		dst = cv::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));
	}
	
	// Check for matching data depths in dst Mat and perform set flag for post conversion
	bool dstPostConversion = false;
	if (dst.depth() == XF_XFDEPTH2CVDEPTH(dstDepthTP) && dst.channels() == dstChannelsTP /* && ddepth < HLS_USRTYPE1 */) { // shallow copy only possible if types match (and are native C types)
		//std::cout << "provided dst type matches instantiated core type" << std::endl;
		imgOutput = new xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *) dst.data);
	}
	else // if depths or channels do no match, allocate Mat of depth dstTypeTP and perform SW post conversion later
	{
		imgOutput = new xf::Mat<dstTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols);
		std::cout << "provided output does not match, need SW post conversion" << std::endl;
		dstPostConversion = true;
	}

	return dstPostConversion;
}

template<int disparityTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP>
bool cvToXfDisparity(cv::Mat &disparity, xf::Mat<disparityTypeTP, maxHeightTP, maxWidthTP, NPCTP>* &xfDisparity, cv::Mat &src, int ddepth = XF_XFDEPTH2CVDEPTH(XF_DEPTH(disparityTypeTP,NPCTP)))
{
	// Extract depth and number of channels from xF type
	const int disparityDepthTP = XF_DEPTH(disparityTypeTP,NPCTP); 
	const int disparityChannelsTP = XF_CHANNELS(disparityTypeTP,NPCTP);
		
	//check in disparity Mat was already allocated and matches ddepth
	if (disparity.empty())
	{
		std::cout << "disparity not yet allocated" << std::endl;
		disparity = cv::Mat(src.size(),CV_MAKE_TYPE(ddepth,src.channels()));		
	} else if (!(disparity.depth() == CV_16U || disparity.depth() == CV_32F)) { //OpenCV only specifies CV_16U or CV32F
		std::cout << "disparity allocated does not match OpenCV supported type, reallocating to CV_16U" << std::endl;
		disparity = cv::Mat(src.size(),CV_MAKE_TYPE(CV_16U,1));
	}
	
	// Check for matching data depths in disparity Mat and perform set flag for post conversion
	bool dstPostConversion = false;
	if (disparity.depth() == XF_XFDEPTH2CVDEPTH(disparityDepthTP) && disparity.channels() == disparityChannelsTP /* && ddepth < HLS_USRTYPE1 */) { // shallow copy only possible if types match (and are native C types)
		//std::cout << "provided disparity type matches instantiated core type" << std::endl;
		xfDisparity = new xf::Mat<disparityTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols,(void *) disparity.data);
	}
	else // if depths or channels do no match, allocate Mat of depth disparityTypeTP and perform SW post conversion later
	{
		xfDisparity = new xf::Mat<disparityTypeTP, maxHeightTP, maxWidthTP, NPCTP>(src.rows,src.cols);
		std::cout << "provided output does not match, need SW post conversion" << std::endl;
		dstPostConversion = true;
	}

	return dstPostConversion;
}


// Performs depth conversion on dst buffer
// User should check if this is needed outside of this function
template<int dstTypeTP, int maxHeightTP, int maxWidthTP, int NPCTP>
int xfMatDepthPostConversion(cv::Mat &dst, xf::Mat<dstTypeTP,maxHeightTP,maxWidthTP,NPCTP> *xFdst) 
{
	const int dstDepthTP = XF_DEPTH(dstTypeTP,NPCTP);

	// If SW post conversion required, we do it here
	//if (dstPostConversion) {
		std::cout << "Dst type does not match, performing SW conversion" << std::endl;
		cv::Mat tmpMat2(dst.rows,dst.cols,CV_MAKE_TYPE(XF_XFDEPTH2CVDEPTH(dstDepthTP),dst.channels()),xFdst->copyFrom()); //Declare opencv temporary Mat and copy data from xF Mat (xFdst)
		tmpMat2.convertTo(dst, dst.type()); // Convert data from depth to requested ddepth
	//}
	return 1;
}


#endif
