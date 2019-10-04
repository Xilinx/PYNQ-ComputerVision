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
*     Date:   2019/07/04
*
*****************************************************************************/

#ifndef PYOPENCV_CV_FAST
#define PYOPENCV_CV_FAST

#include <stdio.h>
#include <iostream>

#include "xfSDxFast.h"

struct pyopencv_FastFeatureDetector_t
{
    PyObject_HEAD
    Ptr<xF::FastFeatureDetector> v;
};

static PyTypeObject pyopencv_FastFeatureDetector_Type =
{
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    MODULESTR".FastFeatureDetector",
    sizeof(pyopencv_FastFeatureDetector_t),
};

static void pyopencv_FastFeatureDetector_dealloc(PyObject* self)
{
    ((pyopencv_FastFeatureDetector_t*)self)->v.release();
    PyObject_Del(self);
}

template<> PyObject* pyopencv_from(const Ptr<xF::FastFeatureDetector>& r)
{    
	pyopencv_FastFeatureDetector_t *m = PyObject_NEW(pyopencv_FastFeatureDetector_t, &pyopencv_FastFeatureDetector_Type);
	new (&(m->v)) Ptr<xF::FastFeatureDetector>(); // init Ptr with placement new
	m->v = r;
	return (PyObject*)m;
}

template<> bool pyopencv_to(PyObject* src, Ptr<xF::FastFeatureDetector>& dst, const char* name)
{
	if(!src || src == Py_None)
		return true;
	if(PyObject_TypeCheck(src, &pyopencv_FastFeatureDetector_Type))
	{
		dst = ((pyopencv_FastFeatureDetector_t*)src)->v.dynamicCast<xF::FastFeatureDetector>();
		return true;
	}
	
	failmsg("Expected cv::FastFeatureDetector for argument '%s'", name);
	return false;
}


static PyObject* pyopencv_cv_FastFeatureDetector_create(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    int threshold=10;
    bool nonmaxSuppression=true;
    PyObject* pyobj_type = NULL;
    int type=FastFeatureDetector::TYPE_9_16;
    Ptr<xF::FastFeatureDetector> retval;

    const char* keywords[] = { "threshold", "nonmaxSuppression", "type", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "|ibO:FastFeatureDetector_create", (char**)keywords, &threshold, &nonmaxSuppression, &pyobj_type) &&
        pyopencv_to(pyobj_type, type, ArgInfo("type", 0)) )
    {
        //ERRWRAP2(retval = cv::FastFeatureDetector::create(threshold, nonmaxSuppression, type));
		ERRWRAP2(retval = xF::FastFeatureDetector::create(threshold, nonmaxSuppression, type));
        return pyopencv_from(retval);
    }
	
    return NULL;
}

static PyObject* pyopencv_cv_Feature2D_detect(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;
	
	std::cout << "in fast detect binding" << std::endl;

    xF::FastFeatureDetector* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_FastFeatureDetector_Type))
        _self_ = dynamic_cast<xF::FastFeatureDetector*>(((pyopencv_FastFeatureDetector_t*)self)->v.get());
    if (!_self_)
        return failmsgp("Incorrect type of self (must be 'xF::FastFeatureDetector' or its derivative)");
    {
    PyObject* pyobj_image = NULL;
    Mat image;
    vector_KeyPoint keypoints;
    PyObject* pyobj_mask = NULL;
    Mat mask;

    const char* keywords[] = { "image", "mask", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "O|O:Feature2D.detect", (char**)keywords, &pyobj_image, &pyobj_mask) &&
        pyopencv_to(pyobj_image, image, ArgInfo("image", 0)) &&
        pyopencv_to(pyobj_mask, mask, ArgInfo("mask", 0)) )
    {
        std::cout << "calling C++ wrapper fast detect" << std::endl;
		ERRWRAP2(_self_->detect(image, keypoints, mask));
		std::cout << "done calling C++ wrapper fast detect" << std::endl;
        return pyopencv_from(keypoints);
    }
    }
    PyErr_Clear();

    return NULL;
}

static PyMethodDef pyopencv_FastFeatureDetector_methods[] =
{
	{"detect", (PyCFunction)pyopencv_cv_Feature2D_detect, METH_VARARGS | METH_KEYWORDS, "detect(image[, mask]) -> keypoints"}
};

static void pyopencv_FastFeatureDetector_specials(void)
{
	pyopencv_FastFeatureDetector_Type.tp_dealloc = pyopencv_FastFeatureDetector_dealloc;
	pyopencv_FastFeatureDetector_Type.tp_init = (initproc)0;
    pyopencv_FastFeatureDetector_Type.tp_methods = pyopencv_FastFeatureDetector_methods;
}

#endif
