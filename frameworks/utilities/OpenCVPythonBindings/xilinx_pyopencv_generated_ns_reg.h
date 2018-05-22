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

static ConstDef consts_cv[] = {
    {"ACCESS_FAST", cv::ACCESS_FAST},
    {"ACCESS_MASK", cv::ACCESS_MASK},
    {"ACCESS_READ", cv::ACCESS_READ},
    {"ACCESS_RW", cv::ACCESS_RW},
    {"ACCESS_WRITE", cv::ACCESS_WRITE},
    {NULL, 0}
};

static PyMethodDef methods_Error[] = {
    {NULL, NULL}
};

static ConstDef consts_Error[] = {
    {"BadAlign", cv::Error::BadAlign},
    {"BAD_ALIGN", cv::Error::BadAlign},
    {"BadAlphaChannel", cv::Error::BadAlphaChannel},
    {"BAD_ALPHA_CHANNEL", cv::Error::BadAlphaChannel},
    {"BadCOI", cv::Error::BadCOI},
    {"BAD_COI", cv::Error::BadCOI},
    {"BadCallBack", cv::Error::BadCallBack},
    {"BAD_CALL_BACK", cv::Error::BadCallBack},
    {"BadDataPtr", cv::Error::BadDataPtr},
    {"BAD_DATA_PTR", cv::Error::BadDataPtr},
    {"BadDepth", cv::Error::BadDepth},
    {"BAD_DEPTH", cv::Error::BadDepth},
    {"BadImageSize", cv::Error::BadImageSize},
    {"BAD_IMAGE_SIZE", cv::Error::BadImageSize},
    {"BadModelOrChSeq", cv::Error::BadModelOrChSeq},
    {"BAD_MODEL_OR_CH_SEQ", cv::Error::BadModelOrChSeq},
    {"BadNumChannel1U", cv::Error::BadNumChannel1U},
    {"BAD_NUM_CHANNEL1U", cv::Error::BadNumChannel1U},
    {"BadNumChannels", cv::Error::BadNumChannels},
    {"BAD_NUM_CHANNELS", cv::Error::BadNumChannels},
    {"BadOffset", cv::Error::BadOffset},
    {"BAD_OFFSET", cv::Error::BadOffset},
    {"BadOrder", cv::Error::BadOrder},
    {"BAD_ORDER", cv::Error::BadOrder},
    {"BadOrigin", cv::Error::BadOrigin},
    {"BAD_ORIGIN", cv::Error::BadOrigin},
    {"BadROISize", cv::Error::BadROISize},
    {"BAD_ROISIZE", cv::Error::BadROISize},
    {"BadStep", cv::Error::BadStep},
    {"BAD_STEP", cv::Error::BadStep},
    {"BadTileSize", cv::Error::BadTileSize},
    {"BAD_TILE_SIZE", cv::Error::BadTileSize},
    {"GpuApiCallError", cv::Error::GpuApiCallError},
    {"GPU_API_CALL_ERROR", cv::Error::GpuApiCallError},
    {"GpuNotSupported", cv::Error::GpuNotSupported},
    {"GPU_NOT_SUPPORTED", cv::Error::GpuNotSupported},
    {"HeaderIsNull", cv::Error::HeaderIsNull},
    {"HEADER_IS_NULL", cv::Error::HeaderIsNull},
    {"MaskIsTiled", cv::Error::MaskIsTiled},
    {"MASK_IS_TILED", cv::Error::MaskIsTiled},
    {"OpenCLApiCallError", cv::Error::OpenCLApiCallError},
    {"OPEN_CLAPI_CALL_ERROR", cv::Error::OpenCLApiCallError},
    {"OpenCLDoubleNotSupported", cv::Error::OpenCLDoubleNotSupported},
    {"OPEN_CLDOUBLE_NOT_SUPPORTED", cv::Error::OpenCLDoubleNotSupported},
    {"OpenCLInitError", cv::Error::OpenCLInitError},
    {"OPEN_CLINIT_ERROR", cv::Error::OpenCLInitError},
    {"OpenCLNoAMDBlasFft", cv::Error::OpenCLNoAMDBlasFft},
    {"OPEN_CLNO_AMDBLAS_FFT", cv::Error::OpenCLNoAMDBlasFft},
    {"OpenGlApiCallError", cv::Error::OpenGlApiCallError},
    {"OPEN_GL_API_CALL_ERROR", cv::Error::OpenGlApiCallError},
    {"OpenGlNotSupported", cv::Error::OpenGlNotSupported},
    {"OPEN_GL_NOT_SUPPORTED", cv::Error::OpenGlNotSupported},
    {"StsAssert", cv::Error::StsAssert},
    {"STS_ASSERT", cv::Error::StsAssert},
    {"StsAutoTrace", cv::Error::StsAutoTrace},
    {"STS_AUTO_TRACE", cv::Error::StsAutoTrace},
    {"StsBackTrace", cv::Error::StsBackTrace},
    {"STS_BACK_TRACE", cv::Error::StsBackTrace},
    {"StsBadArg", cv::Error::StsBadArg},
    {"STS_BAD_ARG", cv::Error::StsBadArg},
    {"StsBadFlag", cv::Error::StsBadFlag},
    {"STS_BAD_FLAG", cv::Error::StsBadFlag},
    {"StsBadFunc", cv::Error::StsBadFunc},
    {"STS_BAD_FUNC", cv::Error::StsBadFunc},
    {"StsBadMask", cv::Error::StsBadMask},
    {"STS_BAD_MASK", cv::Error::StsBadMask},
    {"StsBadMemBlock", cv::Error::StsBadMemBlock},
    {"STS_BAD_MEM_BLOCK", cv::Error::StsBadMemBlock},
    {"StsBadPoint", cv::Error::StsBadPoint},
    {"STS_BAD_POINT", cv::Error::StsBadPoint},
    {"StsBadSize", cv::Error::StsBadSize},
    {"STS_BAD_SIZE", cv::Error::StsBadSize},
    {"StsDivByZero", cv::Error::StsDivByZero},
    {"STS_DIV_BY_ZERO", cv::Error::StsDivByZero},
    {"StsError", cv::Error::StsError},
    {"STS_ERROR", cv::Error::StsError},
    {"StsFilterOffsetErr", cv::Error::StsFilterOffsetErr},
    {"STS_FILTER_OFFSET_ERR", cv::Error::StsFilterOffsetErr},
    {"StsFilterStructContentErr", cv::Error::StsFilterStructContentErr},
    {"STS_FILTER_STRUCT_CONTENT_ERR", cv::Error::StsFilterStructContentErr},
    {"StsInplaceNotSupported", cv::Error::StsInplaceNotSupported},
    {"STS_INPLACE_NOT_SUPPORTED", cv::Error::StsInplaceNotSupported},
    {"StsInternal", cv::Error::StsInternal},
    {"STS_INTERNAL", cv::Error::StsInternal},
    {"StsKernelStructContentErr", cv::Error::StsKernelStructContentErr},
    {"STS_KERNEL_STRUCT_CONTENT_ERR", cv::Error::StsKernelStructContentErr},
    {"StsNoConv", cv::Error::StsNoConv},
    {"STS_NO_CONV", cv::Error::StsNoConv},
    {"StsNoMem", cv::Error::StsNoMem},
    {"STS_NO_MEM", cv::Error::StsNoMem},
    {"StsNotImplemented", cv::Error::StsNotImplemented},
    {"STS_NOT_IMPLEMENTED", cv::Error::StsNotImplemented},
    {"StsNullPtr", cv::Error::StsNullPtr},
    {"STS_NULL_PTR", cv::Error::StsNullPtr},
    {"StsObjectNotFound", cv::Error::StsObjectNotFound},
    {"STS_OBJECT_NOT_FOUND", cv::Error::StsObjectNotFound},
    {"StsOk", cv::Error::StsOk},
    {"STS_OK", cv::Error::StsOk},
    {"StsOutOfRange", cv::Error::StsOutOfRange},
    {"STS_OUT_OF_RANGE", cv::Error::StsOutOfRange},
    {"StsParseError", cv::Error::StsParseError},
    {"STS_PARSE_ERROR", cv::Error::StsParseError},
    {"StsUnmatchedFormats", cv::Error::StsUnmatchedFormats},
    {"STS_UNMATCHED_FORMATS", cv::Error::StsUnmatchedFormats},
    {"StsUnmatchedSizes", cv::Error::StsUnmatchedSizes},
    {"STS_UNMATCHED_SIZES", cv::Error::StsUnmatchedSizes},
    {"StsUnsupportedFormat", cv::Error::StsUnsupportedFormat},
    {"STS_UNSUPPORTED_FORMAT", cv::Error::StsUnsupportedFormat},
    {"StsVecLengthErr", cv::Error::StsVecLengthErr},
    {"STS_VEC_LENGTH_ERR", cv::Error::StsVecLengthErr},
    {NULL, 0}
};

static void init_submodules(PyObject * root) 
{
  init_submodule(root, MODULESTR"", methods_cv, consts_cv);
  init_submodule(root, MODULESTR".Error", methods_Error, consts_Error);
};
