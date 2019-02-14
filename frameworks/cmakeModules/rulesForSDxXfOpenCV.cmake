###############################################################################
#  Copyright (c) 2019, Xilinx, Inc.
#  All rights reserved.
# 
#  Redistribution and use in source and binary forms, with or without 
#  modification, are permitted provided that the following conditions are met:
#
#  1.  Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#
#  2.  Redistributions in binary form must reproduce the above copyright 
#      notice, this list of conditions and the following disclaimer in the 
#      documentation and/or other materials provided with the distribution.
#
#  3.  Neither the name of the copyright holder nor the names of its 
#      contributors may be used to endorse or promote products derived from 
#      this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################

#     Author: Kristof Denolf <kristof@xilinx.com>
#     Date:   2017/12/05

cmake_minimum_required(VERSION 2.8)
include (rulesForSDxTargets)

macro (setXfOpenCVDefinitions)
set(XF_BORDER_CONSTANT  0)
set(XF_BORDER_REPLICATE 1)

set(XF_INTERPOLATION_NN 0)
set(XF_INTERPOLATION_BILINEAR 1)
set(XF_INTERPOLATION_AREA 2) 

set(XF_NPPC1   1)
set(XF_NPPC2   2)
set(XF_NPPC4   4)
set(XF_NPPC8   8)
set(XF_NPPC16  16)
set(XF_NPPC32  32)

set(XF_8UC1   0)
set(XF_16UC1  1)
set(XF_16SC1  2)
set(XF_32UC1  3)
set(XF_32FC1  4)
set(XF_32SC1  5)
set(XF_8UC2   6)
set(XF_8UC4   7)
set(XF_2UC1   8)
set(XF_8UC3   9)
set(XF_16UC3  10)

set(XF_L1NORM 0)
set(XF_L2NORM 1)

set(XF_CONVERT_POLICY_SATURATE 0)
set(XF_CONVERT_POLICY_TRUNCATE 1)

set(XF_RADIANS 0)
set(XF_DEGREES 1)

#Threshold
set(XF_THRESHOLD_TYPE_BINARY 0)
set(XF_THRESHOLD_TYPE_RANGE 1) 

endmacro()

macro (setDefaultParameters1In1OutModule componentNameCap)
if(NOT DEFINED maxWidthCMakeParam${componentNameCap})
	set(maxWidthCMakeParam${componentNameCap} ${maxWidthCMakeParam} CACHE STRING "maximum width")
endif()

if(NOT DEFINED maxHeightCMakeParam${componentNameCap})
	set(maxHeightCMakeParam${componentNameCap} ${maxHeightCMakeParam} CACHE STRING "maximum height")
endif()

if(NOT DEFINED srcTypeCMakeParam${componentNameCap})
	set(srcTypeCMakeParam${componentNameCap} ${srcTypeCMakeParam} CACHE STRING "src type")
endif()

if(NOT DEFINED dstTypeCMakeParam${componentNameCap})
	set(dstTypeCMakeParam${componentNameCap} ${dstTypeCMakeParam} CACHE STRING "dst type")
endif()

if(NOT DEFINED NPCCMakeParam${componentNameCap})
	set(NPCCMakeParam${componentNameCap} ${NPCCMakeParam} CACHE STRING "number of pixels per clock")
endif()

if(NOT DEFINED borderTypeCMakeParam${componentNameCap})
	set(borderTypeCMakeParam${componentNameCap} ${borderTypeCMakeParam} CACHE STRING "border type")
endif()
endmacro(setDefaultParameters1In1OutModule) 

macro (setDefaultInstantiationParameters)
setXfOpenCVDefinitions()

#overall defaults
if(NOT DEFINED borderTypeCMakeParam)
	set(borderTypeCMakeParam ${XF_BORDER_CONSTANT} CACHE STRING "border type")
endif()

if(NOT DEFINED maxWidthCMakeParam)
	set(maxWidthCMakeParam 1920 CACHE STRING "maximum width")
endif()

if(NOT DEFINED maxHeightCMakeParam)
	set(maxHeightCMakeParam 1080 CACHE STRING "maximum height")
endif()

if(NOT DEFINED srcTypeCMakeParam)
	set(srcTypeCMakeParam ${XF_8UC1} CACHE STRING "src type")
endif()

if(NOT DEFINED dstTypeCMakeParam)
	set(dstTypeCMakeParam ${XF_8UC1} CACHE STRING "dst type")
endif()

if(NOT DEFINED NPCCMakeParam)
	set(NPCCMakeParam ${XF_NPPC1} CACHE STRING "number of pixels per clock")
endif()

if(NOT DEFINED interpolationTypeCMakeParam)
	set(interpolationTypeCMakeParam ${XF_INTERPOLATION_BILINEAR})
endif()

#filter2D
set(componentNameCapLocalForRule "Filter2D")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED kernelSizeCMakeParam${componentNameCapLocalForRule})
	set(kernelSizeCMakeParam${componentNameCapLocalForRule} 3 CACHE STRING "kernel size")
endif()

if(NOT DEFINED XSHIFTCMakeParam${componentNameCapLocalForRule})
	set(XSHIFTCMakeParam${componentNameCapLocalForRule} 13 CACHE STRING "shift for fixed point precision")
endif()

#Dilate 
set(componentNameCapLocalForRule "Dilate")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Erode 
set(componentNameCapLocalForRule "Erode")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

# boxFilter 
set(componentNameCapLocalForRule "BoxFilter")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

  
if(NOT DEFINED kernelSizeCMakeParam${componentNameCapLocalForRule})
	set(kernelSizeCMakeParam${componentNameCapLocalForRule} 3 CACHE STRING "box filter size 3,5,7")
endif()

#remap
set(componentNameCapLocalForRule "Remap")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED mapTypeCMakeParam${componentNameCapLocalForRule})
	set(mapTypeCMakeParam${componentNameCapLocalForRule} ${XF_32FC1} CACHE STRING "map type")
endif()

if(NOT DEFINED windowRowsCMakeParam${componentNameCapLocalForRule})
	set(windowRowsCMakeParam${componentNameCapLocalForRule} 64 CACHE STRING "linebuffer size")
endif()

if(NOT DEFINED interpolationTypeCMakeParam${componentNameCapLocalForRule})
	set(interpolationTypeCMakeParam${componentNameCapLocalForRule} ${interpolationTypeCMakeParam})
endif()

#StereoBM
set(componentNameCapLocalForRule "StereoBM")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED disparityTypeCMakeParam${componentNameCapLocalForRule})
	set(disparityTypeCMakeParam${componentNameCapLocalForRule} ${XF_16UC1} CACHE STRING "disparity type")
endif()

if(NOT DEFINED blockSizeCMakeParam${componentNameCapLocalForRule})
	set(blockSizeCMakeParam${componentNameCapLocalForRule} 19 CACHE STRING "stereoBM block size")
endif()

if(NOT DEFINED numberOfDisparitiesCMakeParam${componentNameCapLocalForRule})
	set(numberOfDisparitiesCMakeParam${componentNameCapLocalForRule} 64 CACHE STRING "number of disparities")
endif()

if(NOT DEFINED numberOfDisparityUnitsCMakeParam${componentNameCapLocalForRule})
	set(numberOfDisparityUnitsCMakeParam${componentNameCapLocalForRule} 2 CACHE STRING "number of disparity units")
endif()

#initUndistortRectifyMap
set(componentNameCapLocalForRule "InitUndistortRectifyMap")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED cameraMatrixSizeCMakeParam${componentNameCapLocalForRule})
	set(cameraMatrixSizeCMakeParam${componentNameCapLocalForRule} 9 CACHE STRING "camera matrix size")
endif()

if(NOT DEFINED distortionCoefficientSizeCMakeParam${componentNameCapLocalForRule})
	set(distortionCoefficientSizeCMakeParam${componentNameCapLocalForRule} 5 CACHE STRING "distortion coefficiets size")
endif()

if(NOT DEFINED mapTypeCMakeParam${componentNameCapLocalForRule})
	set(mapTypeCMakeParam${componentNameCapLocalForRule} ${XF_32FC1} CACHE STRING "map type")
endif()

if(NOT DEFINED parameterTypeCMakeParam${componentNameCapLocalForRule})
	set(parameterTypeCMakeParam${componentNameCapLocalForRule} ap_fixed<32,12> CACHE STRING "parameter type")
endif()

#calcOpticalFlowDenseNonPyrLK 

set(componentNameCapLocalForRule "CalcOpticalFlowDenseNonPyrLK")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED flowTypeCMakeParam${componentNameCapLocalForRule})
	set(flowTypeCMakeParam${componentNameCapLocalForRule} ${XF_32FC1} CACHE STRING "flow type")
endif()

if(NOT DEFINED windowSizeCMakeParam${componentNameCapLocalForRule})
	set(windowSizeCMakeParam${componentNameCapLocalForRule} 25 CACHE STRING "window size")
endif()


#bitwise_and
set(componentNameCapLocalForRule "Bitwise_and")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})
#bitwise_or
set(componentNameCapLocalForRule "Bitwise_or")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})
#bitwise_xor
set(componentNameCapLocalForRule "Bitwise_xor")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})
#bitwise_not
set(componentNameCapLocalForRule "Bitwise_not")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Threshold
set(componentNameCapLocalForRule "Threshold")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED thresholdTypeCMakeParam${componentNameCapLocalForRule})
	set(thresholdTypeCMakeParam${componentNameCapLocalForRule} ${XF_THRESHOLD_TYPE_BINARY} CACHE STRING "Threshold Type")
endif()

#Subtract
set(componentNameCapLocalForRule "Subtract")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED policyTypeCMakeParam${componentNameCapLocalForRule})
	set(policyTypeCMakeParam${componentNameCapLocalForRule} ${XF_CONVERT_POLICY_SATURATE} CACHE STRING "overflow")
endif()

#Absdiff
set(componentNameCapLocalForRule "Absdiff")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#MedianBlur
set(componentNameCapLocalForRule "MedianBlur")

if(NOT DEFINED borderTypeCMakeParam${componentNameCapLocalForRule})
	set(borderTypeCMakeParam${componentNameCapLocalForRule} ${XF_BORDER_REPLICATE} CACHE STRING "border type")
endif()

setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED filterSizeCMakeParam${componentNameCapLocalForRule})
	set(filterSizeCMakeParam${componentNameCapLocalForRule} 3 CACHE STRING "filter size")
endif()



#CalcHist 
set(componentNameCapLocalForRule "CalcHist")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Accumulate 
if(NOT DEFINED dstTypeCMakeParamAccumulate)
	set(dstTypeCMakeParamAccumulate ${XF_16UC1} CACHE STRING "Accumulate output pixel type")
endif()
set(componentNameCapLocalForRule "Accumulate")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#AccumulateSquare
if(NOT DEFINED dstTypeCMakeParamAccumulateSquare)
	set(dstTypeCMakeParamAccumulateSquare ${XF_16UC1} CACHE STRING "AccumulateSquare output pixel type")
endif()
set(componentNameCapLocalForRule "AccumulateSquare")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#AccumulateWeighted
if(NOT DEFINED dstTypeCMakeParamAccumulateWeighted)
	set(dstTypeCMakeParamAccumulateWeighted ${XF_16UC1} CACHE STRING "AccumulateWeighted output pixel type")
endif()
set(componentNameCapLocalForRule "AccumulateWeighted")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#CornerHarris
if(NOT DEFINED filterSizeCMakeParamCornerHarris)
	set(filterSizeCMakeParamCornerHarris 3 CACHE STRING "Filter size (3,5 and 7 supported)")
endif()
if(NOT DEFINED blockWidthCMakeParamCornerHarris)
	set(blockWidthCMakeParamCornerHarris 3 CACHE STRING "Block width (3,5 and 7 supported)")
endif()
if(NOT DEFINED NMSRadiusCMakeParamCornerHarris)
	set(NMSRadiusCMakeParamCornerHarris 1 CACHE STRING "Radius for non-maximum suppression (1 and 2 supported)")
endif()
set(componentNameCapLocalForRule "CornerHarris")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})


#EqualizeHist 
set(componentNameCapLocalForRule "EqualizeHist")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Fast 
if(NOT DEFINED NMSCMakeParamFast)
	set(NMSCMakeParamFast 1 CACHE STRING "Non-maximum suppression")
endif()
set(componentNameCapLocalForRule "Fast")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})


#Integral
set(componentNameCapLocalForRule "Integral")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#LUT
set(componentNameCapLocalForRule "LUT")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Magnitude
if(NOT DEFINED normTypeCMakeParamMagnitude)
	set(normTypeCMakeParamMagnitude ${XF_L1NORM} CACHE STRING "Normalization type")
endif()
if(NOT DEFINED srcTypeCMakeParamMagnitude)
	set(srcTypeCMakeParamMagnitude ${XF_16SC1} CACHE STRING "Input pixel type")
endif()
if(NOT DEFINED dstTypeCMakeParamMagnitude)
	set(dstTypeCMakeParamMagnitude ${XF_16SC1} CACHE STRING "Output pixel type")
endif()
set(componentNameCapLocalForRule "Magnitude")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Phase
if(NOT DEFINED retTypeCMakeParamPhase)
	set(retTypeCMakeParamPhase ${XF_DEGREES} CACHE STRING "Angle format")
endif()
if(NOT DEFINED srcTypeCMakeParamPhase)
	set(srcTypeCMakeParamPhase ${XF_16SC1} CACHE STRING "Input pixel type")
endif()
if(NOT DEFINED dstTypeCMakeParamPhase)
	set(dstTypeCMakeParamPhase ${XF_16SC1} CACHE STRING "Output pixel type")
endif()
set(componentNameCapLocalForRule "Phase")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Merge  
if(NOT DEFINED dstTypeCMakeParamMerge)
	set(dstTypeCMakeParamMerge ${XF_8UC4} CACHE STRING "Output pixel type")
endif()
set(componentNameCapLocalForRule "Merge")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#MeanStdDev
set(componentNameCapLocalForRule "MeanStdDev")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#MinMaxLoc
set(componentNameCapLocalForRule "MinMaxLoc")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#PyrDown
set(componentNameCapLocalForRule "PyrDown")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#PyrDown
set(componentNameCapLocalForRule "PyrUp")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

#Canny
set(componentNameCapLocalForRule "Canny")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})
 
if(NOT DEFINED apertureSizeCMakeParamCanny)
	set(apertureSizeCMakeParamCanny 3 CACHE STRING "Sobel aperture size")
endif()

if(NOT DEFINED L2gradientCMakeParamCanny)
	set(L2gradientCMakeParamCanny 0 CACHE STRING "L2 gradient switch")
endif()

if(NOT DEFINED srcNPCCMakeParamCanny)
	set(srcNPCCMakeParamCanny ${XF_NPPC1} CACHE STRING "canny in number of pixels per clock")
endif()

if(NOT DEFINED dstNPCCMakeParamCanny)
	set(dstNPCCMakeParamCanny ${XF_NPPC8} CACHE STRING "edge trace out number of pixels per clock")
endif()

if(NOT DEFINED dstIntermedTypeCMakeParamCanny)
	set(dstIntermedTypeCMakeParamCanny ${XF_2UC1} CACHE STRING "canny intermediate dst type")
endif()

if(NOT DEFINED srcIntermedTypeCMakeParamCanny)
	set(srcIntermedTypeCMakeParamCanny ${XF_2UC1} CACHE STRING "edge tracing intermediate src type")
endif()

if(NOT DEFINED dstIntermedNPCCMakeParamCanny)
	set(dstIntermedNPCCMakeParamCanny ${XF_NPPC4} CACHE STRING "canny intermediate dst number of pixels per clock")
endif()

if(NOT DEFINED srcIntermedNPCCMakeParamCanny)
	set(srcIntermedNPCCMakeParamCanny ${XF_NPPC32} CACHE STRING "edge tracing intermediate src number of pixels per clock")
endif()
if(NOT DEFINED normTypeCMakeParam)
	set(normTypeCMakeParam ${XF_L1NORM} CACHE STRING "normalization type")
endif()

if(NOT DEFINED NMSCMakeParam)
	set(NMSCMakeParam 1 CACHE STRING "non-maximum suppression")
endif()

#resize
set(componentNameCapLocalForRule "Resize")
setDefaultParameters1In1OutModule(${componentNameCapLocalForRule})

if(NOT DEFINED srcMaxWidthCMakeParam${componentNameCapLocalForRule})
	set(srcMaxWidthCMakeParam${componentNameCapLocalForRule} ${maxWidthCMakeParam} CACHE STRING "maximum width")
endif()

if(NOT DEFINED srcMaxHeightCMakeParam${componentNameCapLocalForRule})
	set(srcMaxHeightCMakeParam${componentNameCapLocalForRule} ${maxHeightCMakeParam} CACHE STRING "maximum height")
endif()

if(NOT DEFINED dstMaxWidthCMakeParam${componentNameCapLocalForRule})
	set(dstMaxWidthCMakeParam${componentNameCapLocalForRule} ${maxWidthCMakeParam} CACHE STRING "maximum width")
endif()

if(NOT DEFINED dstMaxHeightCMakeParam${componentNameCapLocalForRule})
	set(dstMaxHeightCMakeParam${componentNameCapLocalForRule} ${maxHeightCMakeParam} CACHE STRING "maximum height")
endif()

if(NOT DEFINED interpolationTypeCMakeParam${componentNameCapLocalForRule})
	set(interpolationTypeCMakeParam${componentNameCapLocalForRule} ${interpolationTypeCMakeParam} CACHE STRING "Interpolation type")
endif()

if(NOT DEFINED maxDownScaleCMakeParam${componentNameCapLocalForRule})
	set(maxDownScaleCMakeParam${componentNameCapLocalForRule} 2 CACHE STRING "maximum height")
endif()


#Normalization
if(NOT DEFINED ID0)
	set(ID0 0 CACHE STRING "normalization type")
endif()
if(NOT DEFINED ID1)
	set(ID1 1 CACHE STRING "normalization type")
endif()

# wrapAffine 
if(NOT DEFINED interpolationTypeCMakeParam)
	set(interpolationTypeCMakeParam 0 CACHE STRING "interpolation Type") # NN or Bilinear
endif()
 
  
#arithmatic ops
set(policyTypeCMakeParam ${XF_CONVERT_POLICY_SATURATE})

endmacro()

function(buildSDxCompilerFlags componentList SDxCompileFlags)
	#set(SDxCompileFlagsLocal "-fpic -sds-sys-config linux -sds-proc linux")
	set(SDxCompileFlagsLocal "-fpic") #TBD: extract sys-config and proc for spfm xml file
	
	foreach(componentNameLocal ${componentList})
		message(STATUS "working on flags for component ${componentNameLocal}")
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		if (${componentNameLocal} STREQUAL "filter2D")
			message(STATUS "generating flags for filter2D")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParamFilter2D},${kernelSizeCMakeParamFilter2D},${kernelSizeCMakeParamFilter2D},${srcTypeCMakeParamFilter2D},${dstTypeCMakeParamFilter2D},${maxHeightCMakeParamFilter2D},${maxWidthCMakeParamFilter2D},${NPCCMakeParamFilter2D}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_custom_convolution.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "dilate")
			message(STATUS "generating flags for dilate")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParamDilate},${srcTypeCMakeParamDilate},${maxHeightCMakeParamDilate},${maxWidthCMakeParamDilate},${NPCCMakeParamDilate}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_dilation.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "erode")
			message(STATUS "generating flags for erode")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParamErode},${srcTypeCMakeParamErode},${maxHeightCMakeParamErode},${maxWidthCMakeParamErode},${NPCCMakeParamErode}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_erosion.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "remap")
			message(STATUS "generating flags for remap")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${windowRowsCMakeParamRemap},${interpolationTypeCMakeParamRemap},${srcTypeCMakeParamRemap},${mapTypeCMakeParamRemap},${srcTypeCMakeParamRemap},${maxHeightCMakeParamRemap},${maxWidthCMakeParamRemap},${NPCCMakeParamRemap}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_remap.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "stereoBM")
			message(STATUS "generating flags for stereoBM")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${blockSizeCMakeParamStereoBM},${numberOfDisparitiesCMakeParamStereoBM},${numberOfDisparityUnitsCMakeParamStereoBM},${srcTypeCMakeParamStereoBM},${disparityTypeCMakeParamStereoBM},${maxHeightCMakeParamStereoBM},${maxWidthCMakeParamStereoBM},${NPCCMakeParamStereoBM}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_stereoBM.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "initUndistortRectifyMap")
			message(STATUS "generating flags for initUndistortRectifyMap")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::InitUndistortRectifyMapInverse<${cameraMatrixSizeCMakeParamInitUndistortRectifyMap}, ${distortionCoefficientSizeCMakeParamInitUndistortRectifyMap}, ${mapTypeCMakeParamInitUndistortRectifyMap}, ${maxHeightCMakeParamInitUndistortRectifyMap}, ${maxWidthCMakeParamInitUndistortRectifyMap}, ${NPCCMakeParamInitUndistortRectifyMap}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_stereo_pipeline.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "calcOpticalFlowDenseNonPyrLK")
			message(STATUS "generating flags for calcOpticalFlowDenseNonPyrLK")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::DenseNonPyrLKOpticalFlow<${windowSizeCMakeParamCalcOpticalFlowDenseNonPyrLK}, ${srcTypeCMakeParamCalcOpticalFlowDenseNonPyrLK}, ${maxHeightCMakeParamCalcOpticalFlowDenseNonPyrLK}, ${maxWidthCMakeParamCalcOpticalFlowDenseNonPyrLK}, ${NPCCMakeParamCalcOpticalFlowDenseNonPyrLK}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_dense_npyr_optical_flow.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "canny")
			message(STATUS "generating flags for canny")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${apertureSizeCMakeParamCanny},${L2gradientCMakeParamCanny},${srcTypeCMakeParamCanny},${dstIntermedTypeCMakeParamCanny},${maxHeightCMakeParamCanny},${maxWidthCMakeParamCanny},${srcNPCCMakeParamCanny},${dstIntermedNPCCMakeParamCanny}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_canny.hpp -clkid ${SDxClockID} -sds-end -sds-hw \"xf::EdgeTracing<${srcIntermedTypeCMakeParamCanny},${dstTypeCMakeParamCanny},${maxHeightCMakeParamCanny},${maxWidthCMakeParamCanny},${srcIntermedNPCCMakeParamCanny},${dstNPCCMakeParamCanny}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_edge_tracing.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "threshold")
			message(STATUS "generating flags for threshold")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${thresholdTypeCMakeParamThreshold},${srcTypeCMakeParamThreshold},${maxHeightCMakeParamThreshold},${maxWidthCMakeParamThreshold},${NPCCMakeParamThreshold}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_threshold.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "absdiff")
			message(STATUS "generating flags for absdiff")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamAbsdiff},${maxHeightCMakeParamAbsdiff},${maxWidthCMakeParamAbsdiff},${NPCCMakeParamAbsdiff}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "subtract")
			message(STATUS "generating flags for subtract")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${policyTypeCMakeParamSubtract},${srcTypeCMakeParamSubtract},${maxHeightCMakeParamSubtract},${maxWidthCMakeParamSubtract},${NPCCMakeParamSubtract}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "bitwise_and")
			message(STATUS "generating flags for bitwise_and")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamBitwise_and},${maxHeightCMakeParamBitwise_and},${maxWidthCMakeParamBitwise_and},${NPCCMakeParamBitwise_and}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "bitwise_or")
			message(STATUS "generating flags for bitwise_or")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamBitwise_or},${maxHeightCMakeParamBitwise_or},${maxWidthCMakeParamBitwise_or},${NPCCMakeParamBitwise_or}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "bitwise_xor")
			message(STATUS "generating flags for bitwise_xor")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamBitwise_xor},${maxHeightCMakeParamBitwise_xor},${maxWidthCMakeParamBitwise_xor},${NPCCMakeParamBitwise_xor}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "bitwise_not")
			message(STATUS "generating flags for bitwise_not")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamBitwise_not},${maxHeightCMakeParamBitwise_not},${maxWidthCMakeParamBitwise_not},${NPCCMakeParamBitwise_not}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "medianBlur")
			message(STATUS "generating flags for medianBlur")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${filterSizeCMakeParamMedianBlur},${borderTypeCMakeParamMedianBlur},${srcTypeCMakeParamMedianBlur},${maxHeightCMakeParamMedianBlur},${maxWidthCMakeParamMedianBlur},${NPCCMakeParamMedianBlur}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_median_blur.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "calcHist")
			message(STATUS "generating flags for calcHist") 		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamCalcHist},${maxHeightCMakeParamCalcHist},${maxWidthCMakeParamCalcHist},${NPCCMakeParamCalcHist}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_histogram.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "accumulate")
			message(STATUS "generating flags for accumulate") 
 			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamAccumulate},${dstTypeCMakeParamAccumulate},${maxHeightCMakeParamAccumulate},${maxWidthCMakeParamAccumulate},${NPCCMakeParamAccumulate}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_accumulate_image.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")			
		elseif (${componentNameLocal} STREQUAL "accumulateSquare")
			message(STATUS "generating flags for accumulateSquare") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamAccumulateSquare},${dstTypeCMakeParamAccumulateSquare},${maxHeightCMakeParamAccumulateSquare},${maxWidthCMakeParamAccumulateSquare},${NPCCMakeParamAccumulateSquare}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_accumulate_squared.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}") 			
		elseif (${componentNameLocal} STREQUAL "accumulateWeighted")
			message(STATUS "generating flags for accumulateWeighted")  
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamAccumulateWeighted},${dstTypeCMakeParamAccumulateWeighted},${maxHeightCMakeParamAccumulateWeighted},${maxWidthCMakeParamAccumulateWeighted},${NPCCMakeParamAccumulateWeighted}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_accumulate_weighted.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")			
		elseif (${componentNameLocal} STREQUAL "cornerHarris")
			message(STATUS "generating flags for cornerHarris") 		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${filterSizeCMakeParamCornerHarris},${blockWidthCMakeParamCornerHarris},${NMSRadiusCMakeParamCornerHarris},${srcTypeCMakeParamCornerHarris},${maxHeightCMakeParamCornerHarris},${maxWidthCMakeParamCornerHarris},${NPCCMakeParamCornerHarris}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/features/xf_harris.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "equalizeHist")
			message(STATUS "generating flags for equalizeHist") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamEqualizeHist},${maxHeightCMakeParamEqualizeHist},${maxWidthCMakeParamEqualizeHist},${NPCCMakeParamEqualizeHist}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_hist_equalize.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "fast")
			message(STATUS "generating flags for fast") 		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${NMSCMakeParamFast},${srcTypeCMakeParamFast},${maxHeightCMakeParamFast},${maxWidthCMakeParamFast},${NPCCMakeParamFast}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/features/xf_fast.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")			
		elseif (${componentNameLocal} STREQUAL "integral")
			message(STATUS "generating flags for integral") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamIntegral},${dstTypeCMakeParamIntegral},${maxHeightCMakeParamIntegral},${maxWidthCMakeParamIntegral},${NPCCMakeParamIntegral}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_integral_image.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "LUT")
			message(STATUS "generating flags for LUT") 		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamLUT},${maxHeightCMakeParamLUT},${maxWidthCMakeParamLUT},${NPCCMakeParamLUT}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_lut.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "magnitude")
			message(STATUS "generating flags for magnitude")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${normTypeCMakeParamMagnitude},${srcTypeCMakeParamMagnitude},${dstTypeCMakeParamMagnitude},${maxHeightCMakeParamMagnitude},${maxWidthCMakeParamMagnitude},${NPCCMakeParamMagnitude}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_magnitude.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "meanStdDev")
			message(STATUS "generating flags for meanStdDev") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamMeanStdDev},${maxHeightCMakeParamMeanStdDev},${maxWidthCMakeParamMeanStdDev},${NPCCMakeParamMeanStdDev}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_mean_stddev.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "merge")
			message(STATUS "generating flags for merge") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamMerge},${dstTypeCMakeParamMerge},${maxHeightCMakeParamMerge},${maxWidthCMakeParamMerge},${NPCCMakeParamMerge}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_channel_combine.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "minMaxLoc")
			message(STATUS "generating flags for minMaxLoc") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamMinMaxLoc},${maxHeightCMakeParamMinMaxLoc},${maxWidthCMakeParamMinMaxLoc},${NPCCMakeParamMinMaxLoc}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_min_max_loc.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
elseif (${componentNameLocal} STREQUAL "phase")
			message(STATUS "generating flags for phase") 
 			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${retTypeCMakeParamPhase},${srcTypeCMakeParamPhase},${dstTypeCMakeParamPhase},${maxHeightCMakeParamPhase},${maxWidthCMakeParamPhase},${NPCCMakeParamPhase}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_phase.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "pyrDown")
			message(STATUS "generating flags for pyrDown")   		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamPyrDown},${maxHeightCMakeParamPyrDown},${maxWidthCMakeParamPyrDown},${NPCCMakeParamPyrDown}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_pyr_down.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
 		elseif (${componentNameLocal} STREQUAL "pyrUp")
			message(STATUS "generating flags for pyrUp")   		
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParamPyrUp},${maxHeightCMakeParamPyrUp},${maxWidthCMakeParamPyrUp},${NPCCMakeParamPyrUp}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_pyr_up.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
						

		elseif (${componentNameLocal} STREQUAL "multiply")
			message(STATUS "generating flags for multiply") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${policyTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")				
 		elseif (${componentNameLocal} STREQUAL "boxFilter")
			message(STATUS "generating flags for boxFilter") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParamBoxFilter},${kernelSizeCMakeParamBoxFilter},${srcTypeCMakeParamBoxFilter},${maxHeightCMakeParamBoxFilter},${maxWidthCMakeParamBoxFilter},${NPCCMakeParamBoxFilter}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_box_filter.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}") 		
elseif (${componentNameLocal} STREQUAL "warpAffine")
			message(STATUS "generating flags for warpAffine")  
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${interpolationTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_warpaffine.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "resize")
			message(STATUS "generating flags for resize")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${interpolationTypeCMakeParamResize},${srcTypeCMakeParamResize},${srcMaxHeightCMakeParamResize},${srcMaxWidthCMakeParamResize},${dstMaxHeightCMakeParamResize},${dstMaxWidthCMakeParamResize},${NPCCMakeParamResize},${maxDownScaleCMakeParamResize}>\" xf${componentNameLocalCap}CoreForVivadoHLS.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_resize.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")

 			
	else (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") #native compilation
				
		else()
		endif()
	endforeach()

	set(${SDxCompileFlags} ${SDxCompileFlagsLocal} PARENT_SCOPE)
endfunction()

function (createWrapperCPPFiles componentList subDirLevels)
	foreach(componentNameLocal ${componentList})
	capFirstLetter(${componentNameLocal} componentNameLocalCap)
		message(STATUS "Creating xf${componentNameLocalCap}.cpp")
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentNameLocal}/xfSDxKernel/src/xf${componentNameLocalCap}.cpp.in src/xf${componentNameLocalCap}.cpp)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentNameLocal}/xfSDxKernel/src/xf${componentNameLocalCap}CoreForVivadoHLS.cpp.in src/xf${componentNameLocalCap}CoreForVivadoHLS.cpp)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentNameLocal}/xfSDxKernel/inc/xf${componentNameLocalCap}CoreForVivadoHLS.h.in inc/xf${componentNameLocalCap}CoreForVivadoHLS.h)		
	endforeach()
endfunction()

function (createWrapperCPPFilesList componentList subDirLevels wrapperCPPFileList)
	set(wrapperCPPFileListLocal "")
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		list(APPEND wrapperCPPFileListLocal src/xf${componentNameLocalCap})
		list(APPEND wrapperCPPFileListLocal src/xf${componentNameLocalCap}CoreForVivadoHLS)
	endforeach()
	
	set(${wrapperCPPFileList} ${wrapperCPPFileListLocal} PARENT_SCOPE)
endfunction()

function (createPyOpenCVModulesInOverlayHeaderFile componentList incPath)
	
	set(fileName "${incPath}/xilinx_pyopencv_modules_in_overlay.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_modules_in_overlay.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		file(APPEND ${fileName} "#include <PythonBindingXfSDx${componentNameLocalCap}.h>\n")
	endforeach()
	file(APPEND ${fileName} "\n")
	file(APPEND ${fileName} "static PyMethodDef methods_cv[] = {\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "filter2D")
			file(APPEND ${fileName} "\t{\"filter2D\", (PyCFunction)pyopencv_cv_filter2D, METH_VARARGS | METH_KEYWORDS, \"filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "dilate")
			file(APPEND ${fileName} "\t{\"dilate\", (PyCFunction)pyopencv_cv_dilate, METH_VARARGS | METH_KEYWORDS, \"dilate(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "erode")
			file(APPEND ${fileName} "\t{\"erode\", (PyCFunction)pyopencv_cv_erode, METH_VARARGS | METH_KEYWORDS, \"erode(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "remap")
			file(APPEND ${fileName} "\t{\"remap\", (PyCFunction)pyopencv_cv_remap, METH_VARARGS | METH_KEYWORDS, \"remap(src, map1, map2, interpolation[, dst[, borderMode[, borderValue]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "\t{\"StereoBM_create\", (PyCFunction)pyopencv_cv_StereoBM_create, METH_VARARGS | METH_KEYWORDS, \"StereoBM_create([, numDisparities[, blockSize]]) -> retval\"},\n")
			file(APPEND ${fileName} "\t{\"compute\", (PyCFunction)pyopencv_cv_StereoMatcher_compute, METH_VARARGS | METH_KEYWORDS, \"compute(left, right[, disparity]) -> disparity\"},\n")
		elseif (${componentNameLocal} STREQUAL "initUndistortRectifyMap")
			file(APPEND ${fileName} "\t{\"initUndistortRectifyMap\", (PyCFunction)pyopencv_cv_initUndistortRectifyMap, METH_VARARGS | METH_KEYWORDS, \"initUndistortRectifyMap(cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type[, map1[, map2]]) -> map1, map2\"},\n")
		elseif (${componentNameLocal} STREQUAL "calcOpticalFlowDenseNonPyrLK")
			file(APPEND ${fileName} "\t{\"calcOpticalFlowDenseNonPyrLK\", (PyCFunction)pyopencv_cv_calcOpticalFlowDenseNonPyrLK, METH_VARARGS | METH_KEYWORDS, \"calcOpticalFlowDenseNonPyrLK(prev, next[, flowX[ , flowY]]) -> flowX, fowY \"},\n")
		elseif (${componentNameLocal} STREQUAL "canny")
			file(APPEND ${fileName} "\t{\"Canny\", (PyCFunction)pyopencv_cv_Canny, METH_VARARGS | METH_KEYWORDS, \"Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]]) -> edges\"},\n")
		elseif (${componentNameLocal} STREQUAL "threshold")
			file(APPEND ${fileName} "\t{\"threshold\", (PyCFunction)pyopencv_cv_threshold, METH_VARARGS | METH_KEYWORDS, \"threshold(src, thresh, maxval, type[, dst]) -> retval\"},\n")
		elseif (${componentNameLocal} STREQUAL "absdiff")
			file(APPEND ${fileName} "\t{\"absdiff\", (PyCFunction)pyopencv_cv_absdiff, METH_VARARGS | METH_KEYWORDS, \"absdiff(src1, src2, dst) -> None\"},\n")
		elseif (${componentNameLocal} STREQUAL "subtract")
			file(APPEND ${fileName} "\t{\"subtract\", (PyCFunction)pyopencv_cv_subtract, METH_VARARGS | METH_KEYWORDS, \"subtract(src1, src2[, dst[, mask[, dtype]]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_and")
 			file(APPEND ${fileName} "\t{\"bitwise_and\", (PyCFunction)pyopencv_cv_bitwise_and, METH_VARARGS | METH_KEYWORDS, \"bitwise_and(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_or")
 			file(APPEND ${fileName} "\t{\"bitwise_or\",(PyCFunction)pyopencv_cv_bitwise_or, METH_VARARGS | METH_KEYWORDS, \"bitwise_or(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_xor")
 			file(APPEND ${fileName} "\t{\"bitwise_xor\",(PyCFunction)pyopencv_cv_bitwise_xor, METH_VARARGS | METH_KEYWORDS, \"bitwise_xor(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_not")
 			file(APPEND ${fileName} "\t{\"bitwise_not\",(PyCFunction)pyopencv_cv_bitwise_not, METH_VARARGS | METH_KEYWORDS, \"bitwise_not(src[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "medianBlur")
 			file(APPEND ${fileName} "\t{\"medianBlur\", (PyCFunction)pyopencv_cv_medianBlur, METH_VARARGS | METH_KEYWORDS, \"medianBlur(src, ksize[, dst]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "magnitude") 
 			file(APPEND ${fileName} "\t{\"magnitude\",(PyCFunction)pyopencv_cv_magnitude, METH_VARARGS | METH_KEYWORDS, \"magnitude(x, y[, magnitude]) -> magnitude\"},\n")
 		elseif (${componentNameLocal} STREQUAL "multiply") 
 			file(APPEND ${fileName} "\t{\"multiply\",(PyCFunction)pyopencv_cv_multiply, METH_VARARGS | METH_KEYWORDS, \"multiply(src1, src2[, dst[, scale[, dtype]]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "merge")
 			file(APPEND ${fileName} "\t{\"merge\",(PyCFunction)pyopencv_cv_merge, METH_VARARGS | METH_KEYWORDS, \"merge(mv[, dst]) -> dst\"},\n")  		
 		elseif (${componentNameLocal} STREQUAL "accumulate")
 			file(APPEND ${fileName} "\t{\"accumulate\",(PyCFunction)pyopencv_cv_accumulate, METH_VARARGS | METH_KEYWORDS, \"accumulate(src, dst[, mask]) -> dst\"},\n")  		
 		elseif (${componentNameLocal} STREQUAL "accumulateSquare")
 			file(APPEND ${fileName} "\t{\"accumulateSquare\",(PyCFunction)pyopencv_cv_accumulateSquare, METH_VARARGS | METH_KEYWORDS, \"accumulateSquare(src, dst[, mask])-> dst\"},\n") 	
 		elseif (${componentNameLocal} STREQUAL "accumulateWeighted")
 			file(APPEND ${fileName} "\t{\"accumulateWeighted\",(PyCFunction)pyopencv_cv_accumulateWeighted, METH_VARARGS | METH_KEYWORDS, \"accumulateWeighted(src, dst, alpha[, mask]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "phase")
 			file(APPEND ${fileName} "\t{\"phase\",(PyCFunction)pyopencv_cv_phase, METH_VARARGS | METH_KEYWORDS, \"phase(x, y[, angle[, angleInDegrees]]) -> angle\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "boxFilter")
 			file(APPEND ${fileName} "\t{\"boxFilter\",(PyCFunction)pyopencv_cv_boxFilter, METH_VARARGS | METH_KEYWORDS, \"boxFilter(src, ddepth, ksize[, dst[, anchor[, normalize[,borderType]]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "equalizeHist")
 			file(APPEND ${fileName} "\t{\"equalizeHist\",(PyCFunction)pyopencv_cv_equalizeHist, METH_VARARGS | METH_KEYWORDS, \"equalizeHist(src[, dst]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "integral")
 			file(APPEND ${fileName} "\t{\"integral\",(PyCFunction)pyopencv_cv_integral, METH_VARARGS | METH_KEYWORDS, \"integral(src[, sum[, sdepth]]) -> sum\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "meanStdDev")
 			file(APPEND ${fileName} "\t{\"meanStdDev\",(PyCFunction)pyopencv_cv_meanStdDev, METH_VARARGS | METH_KEYWORDS, \"meanStdDev(src[, mean[, stddev[, mask]]]) -> mean, stddev\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "minMaxLoc")
 			file(APPEND ${fileName} "\t{\"minMaxLoc\",(PyCFunction)pyopencv_cv_minMaxLoc, METH_VARARGS | METH_KEYWORDS, \"minMaxLoc(src[, mask]) -> minVal, maxVal, minLoc, maxLoc\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "warpAffine")
 			file(APPEND ${fileName} "\t{\"warpAffine\",(PyCFunction)pyopencv_cv_warpAffine, METH_VARARGS | METH_KEYWORDS, \"warpAffine(src, M, dsize[, dst[, flags[, borderMode[, borderValue]]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "resize")
 			file(APPEND ${fileName} "\t{\"resize\",(PyCFunction)pyopencv_cv_resize, METH_VARARGS | METH_KEYWORDS, \"resize(src, dsize[, dst[, fx[, fy[, interpolation]]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "pyrUp")
 			file(APPEND ${fileName} "\t{\"pyrUp\",(PyCFunction)pyopencv_cv_pyrUp, METH_VARARGS | METH_KEYWORDS, \"pyrUp(src[, dst[, dstsize[, borderType]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "pyrDown")
 			file(APPEND ${fileName} "\t{\"pyrDown\",(PyCFunction)pyopencv_cv_pyrDown, METH_VARARGS | METH_KEYWORDS, \"pyrDown(src[, dst[, dstsize[, borderType]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "LUT")
 			file(APPEND ${fileName} "\t{\"LUT\",(PyCFunction)pyopencv_cv_LUT, METH_VARARGS | METH_KEYWORDS, \"LUT(src, lut[, dst]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "fast")
 			file(APPEND ${fileName} "\t{\"fast\",(PyCFunction)pyopencv_cv_LUT, METH_VARARGS | METH_KEYWORDS, \"fast(src, lut[, dst]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "cornerHarris")
 			file(APPEND ${fileName} "\t{\"cornerHarris\",(PyCFunction)pyopencv_cv_cornerHarris, METH_VARARGS | METH_KEYWORDS, \"cornerHarris(src, blockSize, ksize, k[, dst[, borderType]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "calcHist")
 			file(APPEND ${fileName} "\t{\"calcHist\",(PyCFunction)pyopencv_cv_calcHist, METH_VARARGS | METH_KEYWORDS, \"calcHist(images, channels, mask, histSize, ranges[, hist[, accumulate]]) -> hist\"},\n") 
 			 
 			
 		else()
		endif()
    endforeach()
	file(APPEND ${fileName} "\t{NULL, NULL}\n")
	file(APPEND ${fileName} "};")

endfunction()

function (createPyOpenCVTypePublishHeaderFile componentList incPath)
	set(fileName "${incPath}/xilinx_pyopencv_generated_type_publish.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_generated_type_publish.h.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "PUBLISH_OBJECT(\"${componentNameLocalCap}\", pyopencv_${componentNameLocalCap}_Type);")
		else()
		endif()
		
	endforeach()

endfunction()

function (createPyOpenCVTypeRegHeaderFile componentList incPath)
	set(fileName "${incPath}/xilinx_pyopencv_generated_type_reg.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_generated_type_reg.h.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "MKTYPE2(${componentNameLocalCap});")
		else()
		endif()
		
	endforeach()
	
endfunction()


function(createModuleIncludeList componentList subDirLevels xfIncludeList)
	set(xfIncludeListLocal "")
	foreach(incElement ${componentList})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${incElement}/xfSDxKernel/inc/)
			list(APPEND xfIncludeListLocal ${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${incElement}/xfSDxKernel/inc/)
		endif()
	endforeach()
	
	set(${xfIncludeList} ${xfIncludeListLocal} PARENT_SCOPE)
endfunction()

function (sysrootDependentLibSDSLink targetName)
	SET (SDS_SHARED_LIB "libcma.so")
	find_path(SDS_SHARED_LIBPATH ${SDS_SHARED_LIB} PATHS ${CMAKE_INSTALL_RPATH})

	if (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
		message(STATUS "INFO: ${SDS_SHARED_LIB} not found")
	else (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
		target_link_libraries (${targetName} ${SDS_SHARED_LIBPATH}/${SDS_SHARED_LIB})
		message(STATUS "INFO: ${SDS_SHARED_LIB} found in ${SDS_SHARED_LIBPATH}, adding as link library to target: ${targetName}") 
	endif (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
endfunction ()

function (createXfOpenCVOverlayWithPythonBindings overlayName subDirLevels componentList)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/${subDirLevels}/frameworks/cmakeModules)
	
	# Define project name
	project(${overlayName})
	capFirstLetter(${overlayName} overlayNameCap)
	
	#add_subdirectory(${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/xF/PynqLib ${CMAKE_CURRENT_BINARY_DIR}/PynqLib)
	
	# Find packages and compile xF::Mat in static lib (with sds++ while generating overlay).
	requireOpenCVAndVivadoHLS()
	find_package(xfOpenCV QUIET)

	message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
	message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
	message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")
	message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
	
	#find pythonlib C++ headers for Python bindings
	find_path(PYTHON_INCLUDE_DIR "Python.h" PATH_SUFFIXES "include/python3.6m" "include/python2.7")
	find_path(NUMPY_INCLUDE_DIR "numpy/arrayobject.h" PATH_SUFFIXES "lib/python3.6/site-packages/numpy/core/include" "lib/python3/dist-packages/numpy/core/include" "lib/python2.7/site-packages/numpy/core/include")
	message(STATUS "Python    include path: ${PYTHON_INCLUDE_DIR}")
	message(STATUS "Numpy     include path: ${NUMPY_INCLUDE_DIR}")
	
	createModuleIncludeList("${componentList}" ${subDirLevels} xFIncList)
	
	include_directories(
		${xFIncList}
		${OpenCV_INCLUDE_DIRS}
		${xfOpenCV_INCLUDE_DIRS}
	)
	
	# ---- SDx overlay target ----
	if (${VivadoHLS_FOUND})
	
		SET (currentTarget ${overlayName})
		message(STATUS "ADDING ${currentTarget} target")
		
		# Define PL instantiation parameters
		setDefaultInstantiationParameters()
			
		#create wrapper cpp file
		createWrapperCPPFiles("${componentList}" ${subDirLevels})
		createWrapperCPPFilesList("${componentList}" ${subDirLevels} wrapperCPPFileList)
		message (STATUS "wrapper cpp file list ${wrapperCPPFileList}")
		
		# generate code for building Python bindings
		createPyOpenCVModulesInOverlayHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		createPyOpenCVTypePublishHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		createPyOpenCVTypeRegHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/cv_xilinx.cpp.in src/cv_xilinx.cpp)
		
		# set directives and configure compile flags
		 
		if (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
			buildSDxCompilerFlags("${componentList}" CompileFlags)
			message(STATUS "SDxCompileFlags ${CompileFlags}")
		else (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") #native compilation
			SET(CompileFlags "-O3")
		endif (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
		
		add_library(${currentTarget} SHARED
			${wrapperCPPFileList}
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/xF/PynqLib/src/pynqlib.c
			#${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/cma_utils.cpp
			${PROJECT_BINARY_DIR}/src/cv_xilinx.cpp
		)
		
		set_target_properties (${currentTarget} PROPERTIES COMPILE_FLAGS ${CompileFlags})	#FLAGS for compile only	
		#set_target_properties (xFMat PROPERTIES COMPILE_FLAGS "-fpic")
		
		compilerDependentVivadoHLSInclude(${currentTarget})
		
		target_include_directories (${currentTarget} PUBLIC
			${PROJECT_BINARY_DIR}/inc/
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/xF/PynqLib/inc
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/xF/
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/
			${PYTHON_INCLUDE_DIR}
			${NUMPY_INCLUDE_DIR}
		)
		
		#sysrootDependentLibSDSLink(${currentTarget})
		target_link_libraries(${currentTarget}
			${OpenCV_LIBS}
		)
		
	endif (${VivadoHLS_FOUND})
endfunction()


function (generateInstallRulesOverlay overlayName subDirLevels)
	#install rules
	install(FILES "${CMAKE_BINARY_DIR}/lib${overlayName}.so" DESTINATION ${CMAKE_BINARY_DIR}/lib${SDxArch} RENAME ${overlayName}.so)
	install(FILES "${CMAKE_BINARY_DIR}/lib${overlayName}.so.bit" DESTINATION ${CMAKE_BINARY_DIR}/lib${SDxArch}/ RENAME ${overlayName}.bit)
    install(CODE "execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/SDxUtils/findAndInstallHwh.sh ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/lib${SDxArch}/${overlayName}.hwh)")
endfunction()

function (createOverlayWithPythonBindings overlayName subDirLevels componentList)
	
	createXfOpenCVOverlayWithPythonBindings(${overlayName} ${subDirLevels} "${componentList}")	
	generateInstallRulesOverlay(${overlayName} ${subDirLevels})
	
endfunction()


function (createXfOpenCVUnitTestPL subDirLevels componentName)

	capFirstLetter(${componentName} componentNameCap)

	# Define project name
	project(Xf${componentName})
	
	# Find packages, add subdirectories and setup target independent include folders and libraries 
	setupForOpenCVUtilsAndHRTimerAndXFMat(${subDirLevels})
	find_package(xfOpenCV QUIET)

	include_directories(
		${xfOpenCV_INCLUDE_DIRS}
	)

	# ---- golden model ----
	SET (currentTarget goldenOpenCV${componentNameCap})
	message(STATUS "ADDING golden ${currentTarget}")

	add_executable(${currentTarget} src/goldenOpenCV${componentNameCap}.cpp)

	# ---- xFOpenCV test ----
	
	# check usePL option setting
	if (usePL)
		message(STATUS "INFO: enabling moving components to PL")
	else (usePL)
		message(STATUS "INFO: disabling moving components to PL")
	endif (usePL)
	
	createModuleIncludeList("${componentName}" ${subDirLevels} xFIncList)
	
	include_directories(
		${xFIncList}
		${OpenCV_INCLUDE_DIRS}
		${xfOpenCV_INCLUDE_DIRS}
	)
	
	# ---- SDx overlay target ----
	if (${VivadoHLS_FOUND})
		message(STATUS "ADDING ${currentTarget} target")
		
		# Define PL instantiation parameters
		setDefaultInstantiationParameters()
			
		#create include file for instantiation parameters
		configure_file(inc/PLInstantiationParameters.h.in inc/PLInstantiationParameters.h)
	
		#create wrapper cpp file
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentName}/xfSDxKernel/src/xf${componentNameCap}.cpp.in src/xf${componentNameCap}.cpp)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentName}/xfSDxKernel/src/xf${componentNameCap}CoreForVivadoHLS.cpp.in src/xf${componentNameCap}CoreForVivadoHLS.cpp)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentName}/xfSDxKernel/inc/xf${componentNameCap}CoreForVivadoHLS.h.in inc/xf${componentNameCap}CoreForVivadoHLS.h)
		
		#create build target
		SET (currentTarget testSDxXf${componentNameCap})
		message(STATUS "ADDING SDSoC target ${currentTarget}")
		
		# set directives and configure compile flags
		 
		if ((${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") AND usePL)
			buildSDxCompilerFlags("${componentName}" CompileFlags)
			message(STATUS "SDxCompileFlags ${CompileFlags}")
		else ((${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") AND usePL) #native compilation
			SET(CompileFlags "-O3")
		endif ((${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") AND usePL)
		
		add_executable(${currentTarget} src/testSDxXf${componentNameCap}.cpp 
			src/xf${componentNameCap}.cpp
			src/xf${componentNameCap}CoreForVivadoHLS.cpp
		)
		
		set_target_properties (${currentTarget} PROPERTIES COMPILE_FLAGS ${CompileFlags})	#FLAGS for compile only	
		
		target_include_directories (${currentTarget} PUBLIC
			${PROJECT_SOURCE_DIR}/${subdirLevels}/${componentFolder}/${componentName}/xfSDxKernel/inc
			${PROJECT_BINARY_DIR}/inc
		)
	
		compilerDependentVivadoHLSInclude(${currentTarget})
		
	endif (${VivadoHLS_FOUND})
endfunction()
