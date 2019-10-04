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

print("Running testXfInitUndistortRectifyMap.py ...")
print("Loading overlay")
from pynq import Overlay
bs = Overlay("/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2initUndistortRectifyMap.bit")
bs.download()

print("Loading xlnk")
from pynq import Xlnk
Xlnk.set_allocator_library('/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2initUndistortRectifyMap.so')
mem_manager = Xlnk()

import pynq_cv.overlays.xv2initUndistortRectifyMap as xv2
import numpy as np
import cv2
import time

import OpenCVUtils as cvu

print("Loading image ../images/bigBunny_1080.png")
img1 = cv2.imread('../images/bigBunny_1080.png')
imgY1 = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)

print("Size of imgY1 is ",imgY1.shape);
height, width, channels = img1.shape

numberOfIterations=1
print("Number of loop iterations: "+str(numberOfIterations))

cameraMatrixLeft = np.array([[1000.0, 0.0, 0.0], [0.0, 1000.0, 0.0], [950.0, 950.0, 1.0]], np.float64)
cameraMatrixRight = np.array([[1000.0, 0.0, 0.0], [0.0, 1000.0, 0.0], [950.0, 950.0, 1.0]], np.float64)

distCoeffsLeft = np.zeros([1,5], np.float64)
distCoeffsRight = np.zeros([1,5], np.float64)

R = np.eye(3, dtype=np.float64)

iRLeft = np.array([[0.001, 0.0, 0.0], [0.0, 0.001, 0.0], [-0.95, -0.95, 1.0]], np.float64)
iRRight = np.array([[0.001, 0.0, 0.0], [0.0, 0.001, 0.0], [-0.95, -0.95, 1.0]], np.float64)

newCameraMatrixLeft = iRLeft*R
newCameraMatrixRight = iRRight*R

newCameraMatrixLeft = np.linalg.inv(newCameraMatrixLeft)
newCameraMatrixRight = np.linalg.inv(newCameraMatrixRight)

Rleft = R.copy()
RRight = R.copy()

mapX = np.zeros((height, width), np.float32)
mapY = np.zeros((height, width), np.float32)

xFmapX = np.zeros((height, width), np.float32)
xFmapY = np.zeros((height, width), np.float32)

print("Start SW loop")
startSW=time.time()
for i in range(numberOfIterations):
    cv2.initUndistortRectifyMap(cameraMatrixLeft, distCoeffsLeft, Rleft, newCameraMatrixLeft, (width, height), cv2.CV_32FC1, mapX, mapY) # on ARM
stopSW=time.time()
print("SW loop finished")

print("Start HW loop")
startPL=time.time()
for i in range(numberOfIterations):
    xv2.initUndistortRectifyMap(cameraMatrixLeft, distCoeffsLeft, Rleft, newCameraMatrixLeft, (width, height), cv2.CV_32FC1, xFmapX, xFmapY)# offloaded to PL, working on physically continuous numpy arrays
stopPL=time.time()
print("HW loop finished")

print("SW frames per second: ", ((numberOfIterations) / (stopSW - startSW)))
print("PL frames per second: ", ((numberOfIterations) / (stopPL - startPL)))

print("Checking SW and HW results match")
print("Checking mapX")
numberOfDifferences,errorPerPixel = cvu.imageCompare(xFmapX,mapX,True,False,20)
print("mapX: number of differences: "+str(numberOfDifferences)+", average L1 error: "+str(errorPerPixel))

print("Checking mapY")
numberOfDifferences,errorPerPixel = cvu.imageCompare(xFmapY,mapY,True,False,20)
print("mapY: number of differences: "+str(numberOfDifferences)+", average L1 error: "+str(errorPerPixel))

print("Done")
