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

#ifndef PYOPENCV_CV_PHASE
#define PYOPENCV_CV_PHASE

#include "xfSDxPhase.h"

static PyObject* pyopencv_cv_phase(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    {
    PyObject* pyobj_x = NULL;
    Mat x;
    PyObject* pyobj_y = NULL;
    Mat y;
    PyObject* pyobj_angle = NULL;
    Mat angle;
    bool angleInDegrees=false;

    const char* keywords[] = { "x", "y", "angle", "angleInDegrees", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "OO|Ob:phase", (char**)keywords, &pyobj_x, &pyobj_y, &pyobj_angle, &angleInDegrees) &&
        pyopencv_to(pyobj_x, x, ArgInfo("x", 0)) &&
        pyopencv_to(pyobj_y, y, ArgInfo("y", 0)) &&
        pyopencv_to(pyobj_angle, angle, ArgInfo("angle", 1)) )
    {
        ERRWRAP2(xF::phase(x, y, angle, angleInDegrees));
        return pyopencv_from(angle);
    }
    }
    PyErr_Clear(); 
    return NULL;
}
#endif
