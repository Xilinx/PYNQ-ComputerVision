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
*             
*     Date:   2018/02/09
*
*****************************************************************************/

#ifndef PYOPENCV_CV_CANNY
#define PYOPENCV_CV_CANNY

#include "xfSDxFast.h"
/*
static PyObject* pyopencv_cv_FastFeatureDetector_create(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    int threshold=10;
    bool nonmaxSuppression=true;
    int type=FastFeatureDetector::TYPE_9_16;
    Ptr<FastFeatureDetector> retval;

    const char* keywords[] = { "threshold", "nonmaxSuppression", "type", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "|ibi:FastFeatureDetector_create", (char**)keywords, &threshold, &nonmaxSuppression, &type) )
    {
        ERRWRAP2(retval = cv::FastFeatureDetector::create(threshold, nonmaxSuppression, type));
        return pyopencv_from(retval);
    }

    return NULL;
}*/

#endif
