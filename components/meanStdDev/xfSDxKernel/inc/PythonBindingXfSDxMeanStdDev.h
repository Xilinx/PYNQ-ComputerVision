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
*             Jack Lo <jackl@xilinx.com>
*     Date:   2018/01/25
*
*****************************************************************************/

#ifndef PYOPENCV_CV_REMAP
#define PYOPENCV_CV_REMAP

#include "xfSDxMeanStdDev.h"

static PyObject* pyopencv_cv_meanStdDev(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    {
    PyObject* pyobj_src = NULL;
    Mat src;
    PyObject* pyobj_mean = NULL;
    Mat mean;
    PyObject* pyobj_stddev = NULL;
    Mat stddev;
    PyObject* pyobj_mask = NULL;
    Mat mask;

    const char* keywords[] = { "src", "mean", "stddev", "mask", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "O|OOO:meanStdDev", (char**)keywords, &pyobj_src, &pyobj_mean, &pyobj_stddev, &pyobj_mask) &&
        pyopencv_to(pyobj_src, src, ArgInfo("src", 0)) &&
        pyopencv_to(pyobj_mean, mean, ArgInfo("mean", 1)) &&
        pyopencv_to(pyobj_stddev, stddev, ArgInfo("stddev", 1)) &&
        pyopencv_to(pyobj_mask, mask, ArgInfo("mask", 0)) )
    {
        ERRWRAP2(xF::meanStdDev(src, mean, stddev, mask));
        return Py_BuildValue("(NN)", pyopencv_from(mean), pyopencv_from(stddev));
    }
    }
    PyErr_Clear(); 
    return NULL;
}
#endif
