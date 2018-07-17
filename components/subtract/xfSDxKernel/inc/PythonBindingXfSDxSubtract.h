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
*     Author: Murad Qasaimeh <muradq@xilinx.com> <kristof@xilinx.com>
*     Date:   2018/06/22
*
*****************************************************************************/

#ifndef PYOPENCV_CV_SUBTRACT
#define PYOPENCV_CV_SUBTRACT

#include "xfSDxSubtract.h"

static PyObject* pyopencv_cv_subtract(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;
 
    PyObject* pyobj_src1 = NULL;
    Mat src1;
    PyObject* pyobj_src2 = NULL;
    Mat src2;
    PyObject* pyobj_dst = NULL;
    Mat dst;
    PyObject* pyobj_mask = NULL;
    Mat mask;
    int dtype=-1;

    const char* keywords[] = { "src1", "src2", "dst", "mask", "dtype", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "OO|OOi:subtract", (char**)keywords, &pyobj_src1, &pyobj_src2, &pyobj_dst, &pyobj_mask, &dtype) &&
        pyopencv_to(pyobj_src1, src1, ArgInfo("src1", 0)) &&
        pyopencv_to(pyobj_src2, src2, ArgInfo("src2", 0)) &&
        pyopencv_to(pyobj_dst, dst, ArgInfo("dst", 1)) &&
        pyopencv_to(pyobj_mask, mask, ArgInfo("mask", 0)) )
    {
        ERRWRAP2(xF_subtract(src1, src2, dst, mask));
        return pyopencv_from(dst);
    } 
    PyErr_Clear();
 
    return NULL;
}

#endif
