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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "../inc/mat.hpp"

#if (CV_MAJOR_VERSION < 4)
namespace cv {
typedef int AccessFlag;
}
#endif

namespace xF {
class MatAllocator : public cv::MatAllocator
{
public:
    cv::UMatData* allocate(int dims, const int* sizes, int type, void* data0, size_t* step, cv::AccessFlag /*flags*/, cv::UMatUsageFlags /*usageFlags*/) const
    {
		//std::cout << "SDX allocator" << std::endl;
		size_t total = CV_ELEM_SIZE(type);
        for( int i = dims-1; i >= 0; i-- )
        {
            if( step )
            {
                if( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                else
                    step[i] = total;
            }
            total *= sizes[i];
        }
				#ifndef __SDSCC__
        uchar* data = data0 ? (uchar*)data0 : (uchar*)cv::fastMalloc(total);
				#else
				uchar* data = data0 ? (uchar*)data0 : (uchar*)sds_alloc_non_cacheable(total);
				#endif
        cv::UMatData* u = new cv::UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
        if(data0)
            u->flags |= cv::UMatData::USER_ALLOCATED;

        return u;
    }

    bool allocate(cv::UMatData* u, cv::AccessFlag /*accessFlags*/, cv::UMatUsageFlags /*usageFlags*/) const
    {
        if(!u) return false;
        return true;
    }

    void deallocate(cv::UMatData* u) const
    {
        if(!u)
            return;

        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0);
        if( !(u->flags & cv::UMatData::USER_ALLOCATED) )
        {
						#ifndef __SDSCC__
						cv::fastFree(u->origdata);
						#else
						sds_free(u->origdata);
						#endif
            u->origdata = 0;
        }
        delete u;
    }
};

// copied over some helper opencv functions that are not available in library
#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_VALUE) \
     static TYPE* volatile instance = NULL; \
     if (instance == NULL) \
     { \
         if (instance == NULL) \
             instance = INITIALIZER; \
     } \
     return RET_VALUE;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)

static inline void setSize( cv::Mat& m, int _dims, const int* _sz,
                            const size_t* _steps, bool autoSteps=false )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m.step.p != m.step.buf )
        {
            cv::fastFree(m.step.p);
            m.step.p = m.step.buf;
            m.size.p = &m.rows;
        }
        if( _dims > 2 )
        {
            m.step.p = (size_t*)cv::fastMalloc(_dims*sizeof(m.step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m.step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m.rows = m.cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CV_ELEM_SIZE(m.flags), esz1 = CV_ELEM_SIZE1(m.flags), total = esz;
    int i;
    for( i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
        CV_Assert( s >= 0 );
        m.size.p[i] = s;

        if( _steps )
        {
            if (_steps[i] % esz1 != 0)
            {
                CV_Error(cv::Error::BadStep, "Step must be a multiple of esz1");
            }

            m.step.p[i] = i < _dims-1 ? _steps[i] : esz;
        }
        else if( autoSteps )
        {
            m.step.p[i] = total;
            int64 total1 = (int64)total*s;
            if( (uint64)total1 != (size_t)total1 )
                CV_Error( CV_StsOutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 )
    {
        m.dims = 2;
        m.cols = 1;
        m.step[1] = esz;
    }
}

static void updateContinuityFlag(cv::Mat& m)
{
    int i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m.step[j]*m.size[j] < m.step[j-1] )
            break;
    }

    uint64 t = (uint64)m.step[0]*m.size[0];
    if( j <= i && t == (size_t)t )
        m.flags |= cv::Mat::CONTINUOUS_FLAG;
    else
        m.flags &= ~cv::Mat::CONTINUOUS_FLAG;
}

static void finalizeHdr(cv::Mat& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m.rows = m.cols = -1;
    if(m.u)
        m.datastart = m.data = m.u->data;
    if( m.data )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        if( m.size[0] > 0 )
        {
            m.dataend = m.ptr() + m.size[d-1]*m.step[d-1];
            for( int i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        else
            m.dataend = m.datalimit;
    }
    else
        m.dataend = m.datalimit = 0;
}

// XMat class declaration and definition, inheriting from cv::Mat



cv::MatAllocator* Mat::getStdAllocator()
{
	CV_SINGLETON_LAZY_INIT(cv::MatAllocator, new xF::MatAllocator())
}


void Mat::deallocate()
{
    if(u)
        (u->currAllocator ? u->currAllocator : allocator ? allocator : getStdAllocator())->unmap(u);
    u = NULL;
}

#if (CV_MAJOR_VERSION < 4) 
void Mat::create(int d, const int* _sizes, int _type)
{
    int i;
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
        cv::MatAllocator *a = allocator, *a0 = getStdAllocator();
#ifdef HAVE_TGPU
        if( !a || a == tegra::getAllocator() )
            a = tegra::getAllocator(d, _sizes, _type);
#endif
        if(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, 0, cv::USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        catch(...)
        {
            if(a != a0)
                u = a0->allocate(dims, size, _type, 0, step.p, 0, cv::USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
}
#else
void Mat::create(int d, const int* _sizes, int _type)
{
    int i;
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    int _sizes_backup[CV_MAX_DIM]; // #5991
    if (_sizes == (this->size.p))
    {
        for(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
        cv::MatAllocator *a = allocator, *a0 = getDefaultAllocator();
#ifdef HAVE_TGPU
        if( !a || a == tegra::getAllocator() )
            a = tegra::getAllocator(d, _sizes, _type);
#endif
        if(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, cv::ACCESS_RW /* ignored */, cv::USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        catch (...)
        {
            if (a == a0)
                throw;
            u = a0->allocate(dims, size, _type, 0, step.p, cv::ACCESS_RW /* ignored */, cv::USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
}
#endif

template<typename T>
void deepSlowCopyMatToMat(cv::Mat &src, cv::Mat &dst)
{
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			T *pSrc = src.ptr<T>(i, j);
			T *pDst = dst.ptr<T>(i, j);
			for (int k = 0; k < src.channels(); k++) {
				pDst[k] = pSrc[k];
			}
		}
	}
}

//to enforce version with strict alignment needed by sds++
void Mat::copyFromCvMat(cv::Mat &src) {

	if( src.empty() )
    {
        this->release();
        return;
    }
	
	switch (src.depth()) {
		case CV_8U:
			deepSlowCopyMatToMat<uchar>(src, *this);
			break;
		case CV_8S:
			deepSlowCopyMatToMat<char>(src, *this);
		case CV_16U:
			deepSlowCopyMatToMat<ushort>(src, *this);
			break;
		case CV_16S:
			deepSlowCopyMatToMat<short>(src, *this);
			break;
		case CV_32S:
			deepSlowCopyMatToMat<int>(src, *this);
			break;
		case CV_32F:
			deepSlowCopyMatToMat<float>(src, *this);
			break;
		case CV_64F:
			deepSlowCopyMatToMat<double>(src, *this);
			break;
		default:
			std::cerr << "unexpected CV type" << std::endl;		
	}
	
}

//to enforce version with strict alignment needed by sds++
void Mat::copyToCvMat(cv::Mat &dst) {   

	if( this->empty() )
    {
        dst.release();
        return;
    }
	
	switch (this->depth()) {
		case CV_8U:
			deepSlowCopyMatToMat<uchar>(*this, dst);
			break;
		case CV_8S:
			deepSlowCopyMatToMat<char>(*this, dst);
		case CV_16U:
			deepSlowCopyMatToMat<ushort>(*this, dst);
			break;
		case CV_16S:
			deepSlowCopyMatToMat<short>(*this, dst);
			break;
		case CV_32S:
			deepSlowCopyMatToMat<int>(*this, dst);
			break;
		case CV_32F:
			deepSlowCopyMatToMat<float>(*this, dst);
			break;
		case CV_64F:
			deepSlowCopyMatToMat<double>(*this, dst);
			break;
		default:
			std::cerr << "unexpected CV type" << std::endl;		
	}
}

} //xF
