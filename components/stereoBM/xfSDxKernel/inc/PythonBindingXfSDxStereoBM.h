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

#include "xfSDxStereoBM.h"

struct pyopencv_StereoBM_t
{
    PyObject_HEAD
    Ptr<xF::StereoBM> v;
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

template<> PyObject* pyopencv_from(const Ptr<xF::StereoBM>& r)
{
    pyopencv_StereoBM_t *m = PyObject_NEW(pyopencv_StereoBM_t, &pyopencv_StereoBM_Type);
    new (&(m->v)) Ptr<xF::StereoBM>(); // init Ptr with placement new
    m->v = r;
    return (PyObject*)m;
}

template<> bool pyopencv_to(PyObject* src, Ptr<xF::StereoBM>& dst, const char* name)
{
    if( src == NULL || src == Py_None )
        return true;
    if(!PyObject_TypeCheck(src, &pyopencv_StereoBM_Type))
    {
        failmsg("Expected cv::StereoBM for argument '%s'", name);
        return false;
    }
    dst = ((pyopencv_StereoBM_t*)src)->v.dynamicCast<xF::StereoBM>();
    return true;
}


static PyObject* pyopencv_cv_StereoBM_create(PyObject* , PyObject* args, PyObject* kw)
{
    using namespace cv;

    int numDisparities=0;
    int blockSize=21;
    Ptr<xF::StereoBM> retval;

    const char* keywords[] = { "numDisparities", "blockSize", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "|ii:StereoBM_create", (char**)keywords, &numDisparities, &blockSize) )
    {
        //ERRWRAP2(retval = cv::StereoBM::create(numDisparities, blockSize));
		ERRWRAP2(retval = xF::StereoBM::create(numDisparities, blockSize));
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoMatcher_compute(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'xF::StereoBM')");
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

    return NULL;
}

static PyObject* pyopencv_cv_StereoMatcher_getMinDisparity(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoMatcher' or its derivative)");
    int retval;

    if(PyObject_Size(args) == 0 && (kw == NULL || PyObject_Size(kw) == 0))
    {
        ERRWRAP2(retval = _self_->getMinDisparity());
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoMatcher_setMinDisparity(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoMatcher' or its derivative)");
    int minDisparity=0;

    const char* keywords[] = { "minDisparity", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "i:StereoMatcher.setMinDisparity", (char**)keywords, &minDisparity) )
    {
        ERRWRAP2(_self_->setMinDisparity(minDisparity));
        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_getPreFilterCap(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int retval;

    if(PyObject_Size(args) == 0 && (kw == NULL || PyObject_Size(kw) == 0))
    {
        ERRWRAP2(retval = _self_->getPreFilterCap());
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_getTextureThreshold(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int retval;

    if(PyObject_Size(args) == 0 && (kw == NULL || PyObject_Size(kw) == 0))
    {
        ERRWRAP2(retval = _self_->getTextureThreshold());
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_getUniquenessRatio(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int retval;

    if(PyObject_Size(args) == 0 && (kw == NULL || PyObject_Size(kw) == 0))
    {
        ERRWRAP2(retval = _self_->getUniquenessRatio());
        return pyopencv_from(retval);
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_setPreFilterCap(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int preFilterCap=0;

    const char* keywords[] = { "preFilterCap", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "i:StereoBM.setPreFilterCap", (char**)keywords, &preFilterCap) )
    {
        ERRWRAP2(_self_->setPreFilterCap(preFilterCap));
        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_setTextureThreshold(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int textureThreshold=0;

    const char* keywords[] = { "textureThreshold", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "i:StereoBM.setTextureThreshold", (char**)keywords, &textureThreshold) )
    {
        ERRWRAP2(_self_->setTextureThreshold(textureThreshold));
        Py_RETURN_NONE;
    }

    return NULL;
}

static PyObject* pyopencv_cv_StereoBM_setUniquenessRatio(PyObject* self, PyObject* args, PyObject* kw)
{
    using namespace cv;

    xF::StereoBM* _self_ = NULL;
    if(PyObject_TypeCheck(self, &pyopencv_StereoBM_Type))
        _self_ = dynamic_cast<xF::StereoBM*>(((pyopencv_StereoBM_t*)self)->v.get());
    if (_self_ == NULL)
        return failmsgp("Incorrect type of self (must be 'StereoBM' or its derivative)");
    int uniquenessRatio=0;

    const char* keywords[] = { "uniquenessRatio", NULL };
    if( PyArg_ParseTupleAndKeywords(args, kw, "i:StereoBM.setUniquenessRatio", (char**)keywords, &uniquenessRatio) )
    {
        ERRWRAP2(_self_->setUniquenessRatio(uniquenessRatio));
        Py_RETURN_NONE;
    }

    return NULL;
}

static PyMethodDef pyopencv_StereoBM_methods[] =
{
    {"compute", (PyCFunction)pyopencv_cv_StereoMatcher_compute, METH_VARARGS | METH_KEYWORDS, "compute(left, right[, disparity]) -> disparity"},
	{"getMinDisparity", (PyCFunction)pyopencv_cv_StereoMatcher_getMinDisparity, METH_VARARGS | METH_KEYWORDS, "getMinDisparity() -> retval\n."},
	{"setMinDisparity", (PyCFunction)pyopencv_cv_StereoMatcher_setMinDisparity, METH_VARARGS | METH_KEYWORDS, "setMinDisparity(minDisparity) -> None\n."},
	{"getPreFilterCap", (PyCFunction)pyopencv_cv_StereoBM_getPreFilterCap, METH_VARARGS | METH_KEYWORDS, "getPreFilterCap() -> retval\n."},
	{"getTextureThreshold", (PyCFunction)pyopencv_cv_StereoBM_getTextureThreshold, METH_VARARGS | METH_KEYWORDS, "getTextureThreshold() -> retval\n."},
	{"getUniquenessRatio", (PyCFunction)pyopencv_cv_StereoBM_getUniquenessRatio, METH_VARARGS | METH_KEYWORDS, "getUniquenessRatio() -> retval\n."},
	{"setPreFilterCap", (PyCFunction)pyopencv_cv_StereoBM_setPreFilterCap, METH_VARARGS | METH_KEYWORDS, "setPreFilterCap(preFilterCap) -> None\n."},
	{"setTextureThreshold", (PyCFunction)pyopencv_cv_StereoBM_setTextureThreshold, METH_VARARGS | METH_KEYWORDS, "setTextureThreshold(textureThreshold) -> None\n."},
	{"setUniquenessRatio", (PyCFunction)pyopencv_cv_StereoBM_setUniquenessRatio, METH_VARARGS | METH_KEYWORDS, "setUniquenessRatio(uniquenessRatio) -> None\n."}
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
