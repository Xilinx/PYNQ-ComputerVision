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
*     Date:   2018/02/01
*
*****************************************************************************/

#ifndef PYOPENCV_CV_STEREOBM
#define PYOPENCV_CV_STEREOBM

#include <iostream>

#include "xfSDxStereoBM.h"

struct pyopencv_StereoBM_t
{
    PyObject_HEAD
    Ptr<xF_StereoBM> v;
};

static PyTypeObject pyopencv_StereoBM_Type =
{
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    MODULESTR".StereoBM",
    sizeof(pyopencv_StereoBM_t),
};

static void pyopencv_StereoBM_dealloc(PyObject* self)
{
    ((pyopencv_StereoBM_t*)self)->v.release();
    PyObject_Del(self);
}

template<> PyObject* pyopencv_from(const Ptr<xF_StereoBM>& r)
{
    pyopencv_StereoBM_t *m = PyObject_NEW(pyopencv_StereoBM_t, &pyopencv_StereoBM_Type);
    new (&(m->v)) Ptr<xF_StereoBM>(); // init Ptr with placement new
    m->v = r;
    return (PyObject*)m;
}

template<> bool pyopencv_to(PyObject* src, Ptr<xF_StereoBM>& dst, const char* name)
{
    if( src == NULL || src == Py_None )
        return true;
    if(!PyObject_TypeCheck(src, &pyopencv_StereoBM_Type))
    {
        failmsg("Expected cv::StereoBM for argument '%s'", name);
        return false;
    }
    dst = ((pyopencv_StereoBM_t*)src)->v.dynamicCast<xF_StereoBM>();
    return true;
}


static PyObject* pyopencv_cv_StereoBM_create(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    int numDisparities=0;
    int blockSize=21;
    Ptr<xF_StereoBM> retval;
	
	std::cout << "creating xf stereo bm object" << std::endl;

    const char* keywords[] = { "numDisparities", "blockSize", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "|ii:StereoBM_create", (char**)keywords, &numDisparities, &blockSize) )
    {
        //ERRWRAP2(retval = cv::StereoBM::create(numDisparities, blockSize));
		ERRWRAP2(retval = xF_StereoBM::create(numDisparities, blockSize));
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoMatcher_compute(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;
	
	std::cout << "calling xf stereo BM python binding" << std::endl;

    xF_StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF_StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'xF_StereoBM')");
    {
    PyObject* pyobj_left = NULL;
    Mat left;
    PyObject* pyobj_right = NULL;
    Mat right;
    PyObject* pyobj_disparity = NULL;
    Mat disparity;

    const char* keywords[] = { "left", "right", "disparity", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "OO|O:StereoMatcher.compute", (char**)keywords, &pyobj_left, &pyobj_right, &pyobj_disparity) &&
        pyopencv_to(pyobj_left, left, ArgInfo("left", 0)) &&
        pyopencv_to(pyobj_right, right, ArgInfo("right", 0)) &&
        pyopencv_to(pyobj_disparity, disparity, ArgInfo("disparity", 1)) )
    {
        ERRWRAP2(_self_->compute(left, right, disparity));
        return pyopencv_from(disparity);
    }
    }
    PyErr_Clear();

    /*{
    PyObject* pyobj_left = NULL;
    UMat left;
    PyObject* pyobj_right = NULL;
    UMat right;
    PyObject* pyobj_disparity = NULL;
    UMat disparity;

    const char* keywords[] = { "left", "right", "disparity", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "OO|O:StereoMatcher.compute", (char**)keywords, &pyobj_left, &pyobj_right, &pyobj_disparity) &&
        pyopencv_to(pyobj_left, left, ArgInfo("left", 0)) &&
        pyopencv_to(pyobj_right, right, ArgInfo("right", 0)) &&
        pyopencv_to(pyobj_disparity, disparity, ArgInfo("disparity", 1)) )
    {
        ERRWRAP2(_self_->compute(left, right, disparity));
        return pyopencv_from(disparity);
    }
    }*/

    return NULL;
}

static PyMethodDef pyopencv_StereoBM_methods[] =
{
    {"compute", (PyCFunction)pyopencv_cv_StereoMatcher_compute, METH_VARARGS | METH_KEYWORDS, "compute(left, right[, disparity]) -> disparity"},
	
};

static void pyopencv_StereoBM_specials(void)
{
    //pyopencv_StereoBM_Type.tp_base = &pyopencv_StereoMatcher_Type;
    pyopencv_StereoBM_Type.tp_dealloc = pyopencv_StereoBM_dealloc;
    //pyopencv_StereoBM_Type.tp_repr = pyopencv_StereoBM_repr;
    //pyopencv_StereoBM_Type.tp_getset = pyopencv_StereoBM_getseters;
    pyopencv_StereoBM_Type.tp_init = (initproc)0;
    pyopencv_StereoBM_Type.tp_methods = pyopencv_StereoBM_methods;
}

#endif
