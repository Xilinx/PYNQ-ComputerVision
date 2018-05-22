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
 *     Author: Kristof Denolf, Xilinx, Inc.
 *     Date: 2016/12/14
 *
 *****************************************************************************/
 
#ifndef _XF_MAT_HPP_
#define _XF_MAT_HPP_

//SDx temporal fix for Clang issue
#ifdef __SDSCC__
#undef __ARM_NEON__
#undef __ARM_NEON
#include <opencv2/core/utility.hpp>
#define __ARM_NEON__
#define __ARM_NEON
#else
#include <opencv2/core/utility.hpp>
#endif
//#include <opencv2/core/utility.hpp>

#ifdef __SDSCC__
#include <sds_lib.h>
#endif

namespace xF {

// xF::Mat class declaration
class Mat : public cv::Mat {

public:
	Mat():cv::Mat(){};
	Mat(int rows, int cols, int type);
	Mat(cv::Size size, int type);

	void create(int ndims, const int* sizes, int type);
	void create(int rows, int cols, int type);

	void deallocate();

	static cv::MatAllocator* getStdAllocator();

};


//////////////////////////////////////////// Mat inline //////////////////////////////////////////


inline
void Mat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type);
}

inline
Mat::Mat(int _rows, int _cols, int _type) : cv::Mat()/*, flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
			datalimit(0), allocator(0), u(0), size(&rows)*/
{
	create(_rows, _cols, _type);
}

inline
Mat::Mat(cv::Size _size, int _type) : cv::Mat()/*, flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
			datalimit(0), allocator(0), u(0), size(&rows)*/
{
	create(_size.height, _size.width, _type);
}

} //xF

#endif //_XF_MAT_HPP_
