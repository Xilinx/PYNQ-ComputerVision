#!/opt/python3.6/bin/python3.6

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

print("Loading overlay")

from pynq.overlays.bare_hdmi import BareHDMIOverlay
base = BareHDMIOverlay("/home/xilinx/proj/test/xv2Filter2D.bit")

print("Loading xlnk")
from pynq import Xlnk
mem_manager = Xlnk()

import numpy as np
import cv2
import xv2Filter2D as xv2
import time

img = cv2.imread('../images/bigBunny_1080.png')
imgY = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

print("Size of imgY is ",imgY.shape);
height, width, channels = img.shape

#kernel = np.ones((3,3),np.float32)/9 #averaging filter
#kernel = np.array([[0.0, 1.0, 0],[1.0, -4, 1.0],[0, 1.0, 0.0]],np.float32) # Laplacian high-pass
#kernel = np.array([[0.0625,0.125,0.0625],[0.125,0.25,0.125],[0.0625,0.125,0.0625]],np.float32) #Gaussian blur
kernel = np.array([[1.0,0.0,-1.0],[2.0,0.0,-2.0],[1.0,0.0,-1.0]],np.float32) # Sobel Vertical Edges
kernel = np.array([[1.0,2.0,1.0],[0.0,0.0,0.0],[-1.0,-2.0,-1.0]],np.float32) # Sobel Horizontal Edges

numberOfIterations=10

dstSW = np.ones((height,width),np.uint8);

xFimgY  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array 
xFimgY[:] = imgY[:] # copy source data

xFdst  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array


print("Start SW loop")
startSW=time.time()
for i in range(numberOfIterations):
    cv2.filter2D(imgY,-1,kernel,dst=dstSW,borderType=cv2.BORDER_CONSTANT) #filter2D on ARM
stopSW=time.time()


print("Start HW loop")
startPL=time.time()
for i in range(numberOfIterations):
    xv2.filter2D(xFimgY,-1,kernel,dst=xFdst,borderType=cv2.BORDER_CONSTANT) #filter2D offloaded to PL, working on physically continuous numpy arrays
stopPL=time.time()
    
print("SW frames per second: ", ((numberOfIterations) / (stopSW - startSW)))
print("PL frames per second: ", ((numberOfIterations) / (stopPL - startPL)))

cv2.imshow('image',xFimgY)
cv2.imshow('filteredSW',dstSW)
cv2.imshow('filteredHW',xFdst)
cv2.waitKey(0)
cv2.destroyAllWindows()

print("Done")
