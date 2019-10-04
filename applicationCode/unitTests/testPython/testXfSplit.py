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

print("Running testXfSplit.py ...")
print("Loading overlay")
from pynq import Overlay
bs = Overlay("/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2split.bit")
bs.download()

print("Loading xlnk")
from pynq import Xlnk
Xlnk.set_allocator_library('/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2split.so')
mem_manager = Xlnk()

import pynq_cv.overlays.xv2split as xv2
import numpy as np
import cv2
import time

import OpenCVUtils as cvu

print("Loading image ../images/bigBunny_1080.png")
img = cv2.imread('../images/bigBunny_1080.png')
#imgY = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
imgRGBA = cv2.cvtColor(img,cv2.COLOR_BGR2RGBA)

print("Size of imgRGBA is ",imgRGBA.shape);
height, width, channels = imgRGBA.shape

numberOfIterations=10
print("Number of loop iterations: "+str(numberOfIterations))

#imgChannels = cv2.split(imgRGBA)

#xFimgChannels  = mem_manager.cma_array((4,height,width),np.uint8) #allocated physically contiguous numpy array 
#xFimgChannels[:] = imgChannels[:] # copy source data

xFimgRGBA    = mem_manager.cma_array((height,width,4),np.uint8) #allocated physically contiguous numpy array 
xFimgRGBA[:] = imgRGBA[:] # copy source data

#dstSW = np.ones((height,width,4),np.uint8);
#xFdst  = mem_manager.cma_array((height,width,4),np.uint8) #allocated physically contiguous numpy array

dstSW = np.ones((4,height,width),np.uint8);
xFdst  = mem_manager.cma_array((4,height,width),np.uint8) #allocated physically contiguous numpy array

print("Start SW loop")
startSW=time.time()
for i in range(numberOfIterations):
    cv2.split(imgRGBA,mv=dstSW) #split on ARM
stopSW=time.time()


print("Start HW loop")
startPL=time.time()
for i in range(numberOfIterations):
    xv2.split(xFimgRGBA,mv=xFdst) #split offloaded to PL, working on physically continuous numpy arrays
stopPL=time.time()
    
print("SW frames per second: ", ((numberOfIterations) / (stopSW - startSW)))
print("PL frames per second: ", ((numberOfIterations) / (stopPL - startPL)))

print("Checking SW and HW results match")
channels = dstSW.shape[0]
for k in range(channels):
    numberOfDifferences,errorPerPixel = cvu.imageCompare(xFdst[k],dstSW[k],True,False,0.0)
    print("number of differences: "+str(numberOfDifferences)+", average L1 error: "+str(errorPerPixel))

print("Done")
