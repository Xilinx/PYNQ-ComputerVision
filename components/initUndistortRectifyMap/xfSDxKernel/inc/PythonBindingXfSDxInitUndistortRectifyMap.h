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
*     Date:   2018/09/25
*
*****************************************************************************/

#ifndef PYOPENCV_CV_INITUNDISTORTRECTIFYMAP
#define PYOPENCV_CV_INITUNDISTORTRECTIFYMAP

#include "xfSDxInitUndistortRectifyMap.h"

static PyObject* pyopencv_cv_initUndistortRectifyMap(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    {
    PyObject* pyobj_cameraMatrix = NULL;
    Mat cameraMatrix;
    PyObject* pyobj_distCoeffs = NULL;
    Mat distCoeffs;
    PyObject* pyobj_R = NULL;
    Mat R;
    PyObject* pyobj_newCameraMatrix = NULL;
    Mat newCameraMatrix;
    PyObject* pyobj_size = NULL;
    Size size;
    int m1type=0;
    PyObject* pyobj_map1 = NULL;
    Mat map1;
    PyObject* pyobj_map2 = NULL;
    Mat map2;

    const char* keywords[] = { "cameraMatrix", "distCoeffs", "R", "newCameraMatrix", "size", "m1type", "map1", "map2", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "OOOOOi|OO:initUndistortRectifyMap", (char**)keywords, &pyobj_cameraMatrix, &pyobj_distCoeffs, &pyobj_R, &pyobj_newCameraMatrix, &pyobj_size, &m1type, &pyobj_map1, &pyobj_map2) &&
        pyopencv_to(pyobj_cameraMatrix, cameraMatrix, ArgInfo("cameraMatrix", 0)) &&
        pyopencv_to(pyobj_distCoeffs, distCoeffs, ArgInfo("distCoeffs", 0)) &&
        pyopencv_to(pyobj_R, R, ArgInfo("R", 0)) &&
        pyopencv_to(pyobj_newCameraMatrix, newCameraMatrix, ArgInfo("newCameraMatrix", 0)) &&
        pyopencv_to(pyobj_size, size, ArgInfo("size", 0)) &&
		pyopencv_to(pyobj_size, size, ArgInfo("m1type", 0)) &&
        pyopencv_to(pyobj_map1, map1, ArgInfo("map1", 1)) &&
        pyopencv_to(pyobj_map2, map2, ArgInfo("map2", 1)) )
    {
        ERRWRAP2(xF::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type, map1, map2));
        return Py_BuildValue("(NN)", pyopencv_from(map1), pyopencv_from(map2));
    }
    }
    PyErr_Clear();

 
    return NULL;
}

#endif
