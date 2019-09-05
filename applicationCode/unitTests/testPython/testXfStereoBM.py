#!/usr/bin/python3.6

###############################################################################
#  Copyright (c) 2018, Xilinx, Inc.
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

print("Running testXfStereoBM.py ...")
print("Loading overlay")
from pynq import Overlay
bs = Overlay("/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2stereoBM.bit")
bs.download()

print("Loading xlnk")
from pynq import Xlnk
Xlnk.set_allocator_library('/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2stereoBM.so')
mem_manager = Xlnk()

import pynq_cv.overlays.xv2stereoBM as xv2
import numpy as np
import cv2
import time

import OpenCVUtils as cvu

print("Loading left image ../images/000005_10_L.png")
imgL = cv2.imread('../images/000005_10_L.png')
imgLY = cv2.cvtColor(imgL,cv2.COLOR_BGR2GRAY)

print("Size of imgLY is ",imgLY.shape);
height, width, channels = imgL.shape

print("Loading right image ../images/000005_10_R.png")
imgR = cv2.imread('../images/000005_10_R.png')
imgRY = cv2.cvtColor(imgR,cv2.COLOR_BGR2GRAY)

print("Size of imgRY is ",imgRY.shape);

kernel = np.array([[1.0,2.0,1.0],[0.0,0.0,0.0],[-1.0,-2.0,-1.0]],np.float32) # Sobel Horizontal Edges

numberOfIterations=10
print("Number of loop iterations: "+str(numberOfIterations))

dstSW = np.ones((height,width),np.float32);

#xFimgLY  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array 
#xFimgRY  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array 
#xFimgLY[:] = imgLY[:] # copy source data
#xFimgRY[:] = imgRY[:] # copy source data

xFdst  = mem_manager.cma_array((height,width),np.uint16) #allocated physically contiguous numpy array

print("Start SW loop")
cv2Stereo = cv2.StereoBM_create(numDisparities=64, blockSize=19)
startSW=time.time()
for i in range(numberOfIterations):
    dstSW = cv2Stereo.compute(imgLY,imgRY) #stereoBM on ARM
stopSW=time.time()


print("Start HW loop")
xv2Stereo = xv2.StereoBM_create(numDisparities=64, blockSize=19)
startPL=time.time()
for i in range(numberOfIterations):
    xv2Stereo.compute(imgLY,imgRY,xFdst) #stereoBM offloaded to PL, working on physically continuous numpy arrays 
#    xv2Stereo.compute(xFimgLY,xFimgRY,xFdst) #stereoBM offloaded to PL, working on physically continuous numpy arrays 
stopPL=time.time()
    
print("SW frames per second: ", ((numberOfIterations) / (stopSW - startSW)))
print("PL frames per second: ", ((numberOfIterations) / (stopPL - startPL)))

#tmpSW = np.clip(np.int16(dstSW),0,255)
tmpSW = np.clip(np.int16(dstSW),0,None)

print("Checking SW and HW results match")
tmpSW_uint16 = np.ones((height,width),np.uint16)
tmpSW_uint16[:] = tmpSW[:]
numberOfDifferences,errorPerPixel = cvu.imageCompare(xFdst,tmpSW_uint16,True,False,0.0)
print("number of differences: "+str(numberOfDifferences)+", average L1 error: "+str(errorPerPixel))

print("Writing SW and HW results to image files.")
disp_sw = cvu.colorizeDisparity(dstSW)
cv2.imwrite("stereobm_sw.png",disp_sw)
disp_hw = cvu.colorizeDisparity(xFdst)
cv2.imwrite("stereobm_hw.png",disp_hw)

print("Done")
