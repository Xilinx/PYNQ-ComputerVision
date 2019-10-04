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

print("Running testXfCalcOpticalFlowDenseNonPyrLK.py ...")
print("Loading overlay")
from pynq import Overlay
bs = Overlay("/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2calcOpticalFlowDenseNonPyrLK.bit")
bs.download()

print("Loading xlnk")
from pynq import Xlnk
Xlnk.set_allocator_library('/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2calcOpticalFlowDenseNonPyrLK.so')
mem_manager = Xlnk()

import pynq_cv.overlays.xv2calcOpticalFlowDenseNonPyrLK as xv2
import numpy as np
import cv2
import time

import OpenCVUtils as cvu

numberOfIterations=1
print("Number of loop iterations: "+str(numberOfIterations))

print("Loading image ../images/000005_10_L.png")
prev = cv2.imread('../images/000005_10_L.png')
prevY = cv2.cvtColor(prev,cv2.COLOR_BGR2GRAY)

print("Size of prevY is ",prevY.shape);
height, width, channels = prev.shape

print("Loading image ../images/000005_10_R.png")
next_ = cv2.imread('../images/000005_10_R.png')
nextY = cv2.cvtColor(next_,cv2.COLOR_BGR2GRAY)

xFflowX = mem_manager.cma_array((height,width),np.float32) #allocated physically contiguous numpy array
xFflowY = mem_manager.cma_array((height,width),np.float32) #allocated physically contiguous numpy array

xFprevY = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array
xFnextY = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array

print("Size of nextY is ",nextY.shape);

#prevPts = cv2.goodFeaturesToTrack(prevY, maxCorners = 100, qualityLevel=0.01, minDistance = 1, mask = None)
prevPts=np.empty((height*width, 1, 2), np.float32)
for h in range(height):
    for w in range(width):
        xFprevY[h,w] = prevY[h,w]
        xFnextY[h,w] = nextY[h,w]
        prevPts[height*h + w, :, :] = [float(h), float(w)]



print("Start SW loop")
startSW=time.time()
for i in range(numberOfIterations):
     nextPts, status, err = cv2.calcOpticalFlowPyrLK(prevY, nextY, prevPts, None, winSize=(25, 25), maxLevel=0) # on ARM
stopSW=time.time()
print("SW loop finished")

print("Start HW loop")
startPL=time.time()
for i in range(numberOfIterations):
    xFflowX, xFflowY = xv2.calcOpticalFlowDenseNonPyrLK(xFprevY, xFnextY, xFflowX, xFflowY) #absdiff offloaded to PL, working on physically continuous numpy arrays
stopPL=time.time()
print("HW loop finished")

print("SW frames per second: ", ((numberOfIterations) / (stopSW - startSW)))
print("PL frames per second: ", ((numberOfIterations) / (stopPL - startPL)))

#print("Checking SW and HW results match")
#numberOfDifferences,errorPerPixel = cvu.imageCompare(xFdst,dstSW,True,False,0.0)
#print("number of differences: "+str(numberOfDifferences)+", average L1 error: "+str(errorPerPixel))


print("Done")

