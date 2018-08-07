# Building xfOpenCV Overlays for Pynq: CMake based sds++ cross-compilation

## List of Supported Components 

| Image Arithmetic      | Filters       |   Geometric Transform | Flow and Depts|   Features    | Input Processing	| Analysis 	|
| ---------             | ---------     |   ---------           |    ---------  |    ---------  |  ---------  		|--------- 	|
| bitwise_and           |   filter2D   	|         remap         |   stereoBM    |   canny       | 					|			|
| bitwise_or            |   erode       |         resize        |               |               |					|			|
| bitwise_xor           |   dilate      |                       |               |               |					|			|
| threshold             |   medianBlur  |                       |               |               |					|			|
| subtract              |   boxFilter   |                       |               |               |					|			|
## Setup Environment on Host

  + clone [Pynq-ComputerVision](https://github.com/Xilinx/PYNQ-ComputerVision) repository:
    ```commandline
    $ git clone https://github.com/Xilinx/PYNQ-ComputerVision.git /your_pynqCV_folder
    ``` 
  + clone [xfOpenCV](https://github.com/Xilinx/xfopencv) repository and checkout the 2017.4 version:
    ```commandline
    $ git clone https://github.com/Xilinx/xfopencv.git /your_xfOpenCV_folder
    $ cd /your_xfOpenCV_folder
    $ git checkout 2017.4_release
    ``` 
  + Prepare the PYNQ-Z1 Video platform package in /your_PynqPlatform_folder. 
    + download [bare_hdmi.tar.gz](https://www.xilinx.com/member/forms/download/xef.html?filename=bare_hdmi.tar.gz) in /your_PynqPlatform_folder
    + untar the package. This will create a bare_hdmi subfolder in /your_PynqPlatform_folder
      ```commandline
      $ tar -zxvf bare_hdmi.tar.gz
      ```
    + download [sysroot_2018.4.9.tar.gz](https://www.xilinx.com/member/forms/download/xef.html?filename=sysroot_2018.4.9.tar.gz) in /your_PynqPlatform_folder/bare_hdmi/sw
    + untar the package. This will create a sysroot subfolder in /your_PynqPlatform_folder/bare_hdmi/sw
     ```commandline
      $ tar -zxvf sysroot_2018.4.9.tar.gz
      ```
    + Note that you can adapt /your_PynqPlatform_folder to your preference, but the deepest subfolder should match the platform name, in this case 'bare_hdmi'. 
  + set an environmental variable to xfOpenCV: setenv XFOPENCV_PATH /your_xfOpenCV_folder
  + set up Xilinx SDx tools, version 2017.4 by running its setup script


### Building your Overlay
  + create an overlay folder in /your_pynqCV_folder/overlays, for instance myFirstOverlay
  + Copy and adapt the [/your_pynqCV_folder/overlays/cvXfUserSpecific/CMakeLists.txt](./cvXfUserSpecific/CMakeLists.txt) to /your_pynqCV_folder/overlays/myFirstOverlay
    + line 41, choose your overlay name, for instance xv2MyFirstOverlay
    + line 45, choose the CV components offloaded to PL in your overlay, restricted to a subset of filter2D, remap, dilate, stereoBM and canny
    + lines 47-49: optionally overwrite some of the default instantiation paramaters (defined in the [setDefaultInstantiationParameters CMake macro](../frameworks/cmakeModules/rulesForSDxXfOpenCV.cmake#L37)) by user specific ones 
  + create a build folder in /your_pynqCV_folder/overlays/myFirstOverlay run CMake:
    ```commandline
    $ cd /your_pynqCV_folder/overlays/myFirstOverlay
    $ mkdir build; cd build
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../frameworks/cmakeModules/toolchain_sdx2017.4.cmake -DSDxPlatform=/your_PynqPlatform_folder/bare_hdmi -DSDxClockID=1 -DSDxArch=arm32
    ```
  + run make for the target with your chosen overlay name
    ```commandline
    $ make xv2MyFirstOverlay
    ```
  + run make install. This will copy three files (xv2MyFirstOverlay.tcl, xv2MyFirstOverlay.so, xv2MyFirstOverlay.bit) to /your_pynqCV_folder/overlays/myFirstOverlay/libarm32 
    ```commandline
    $ make install
    ```
  + copy the content of /your_pynqCV_folder/overlays/myFirstOverlay/build/libarm32 to a test folder (for instance ~/proj/test) on your pynq board:
    ```commandline
    $  scp xv2MyFirstOverlay.* xilinx@<pynq-board-ip>:/home/xilinx/proj/test
    ```
    
 ## Using PL offloaded CV Modules
 
 Your Python test code first needs to load the overlay, then import the python bindings for the PL offloaded CV modules and then call the methods from those bindings. Allocating numpy arrays in physically contiguous memory maximizes the efficiency of DRAM data movement.
 
 An example python script for filter 2D is available in: [applicationCode/overlayTests/testPython/testXfFilter2D.py](../applicationCode/overlayTests/testPython/testXfFilter2D.py)
 
 ### Loading the Overlay
 
 Before you can load the overlay, we must first install the base video overlay which includes HDMI support. This can be done by following the pip install instructions for the pynq-computervision extension found [here](https://github.com/Xilinx/PYNQ-ComputerVision). From the command line on your pynq board, you need to run:
 
```bash
$ sudo -H pip3.6 install --upgrade git+https://github.com/Xilinx/PYNQ-ComputerVision.git
```
Then to load the overlay, you add the following to your python code:

 ```python
from pynq_computervision import BareHDMIOverlay
base = BareHDMIOverlay("/home/xilinx/proj/test/xv2MyFirstOverlay.bit")
```   

### Import Overlay Python Bindings

```python
import xv2MyFirstOverlay as xv2
```

### Call Methods for Offloaded modules

Suppose filter2D has been offloaded to PL, then calling its methods simply involves changing cv2 in xv2

```python
cv2.filter2D(imgY,-1,kernel,dst=dst,borderType=cv2.BORDER_CONSTANT) #filter2D on ARM
xv2.filter2D(imgY,-1,kernel,dst=dst,borderType=cv2.BORDER_CONSTANT) #filter2D offloaded to PL
```

### Pysically Contiguous Memory

```python
from pynq import Xlnk
mem_manager = Xlnk()

xFimgY  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array 
xFimgY[:] = imgY[:] # copy source data

xFdst  = mem_manager.cma_array((height,width),np.uint8) #allocated physically contiguous numpy array

xv2.filter2D(xFimgY,-1,kernel,dst=xFdst,borderType=cv2.BORDER_CONSTANT) #filter2D offloaded to PL, working on physically continuous numpy arrays
```

### Running an filter2D Python Example

The [example python script for filter 2D (testXfFilter2D.py)](../applicationCode/overlayTests/testPython/testXfFilter2D.py) on the repo assumes your xv2MyFirstOverlay listed filter2D as one of the modules. If necessary, adapt the .bit filename of line 6 and library name on line 14 to the overlay name of your choice (xv2MyFirstOverlay in the steps above). Currently the overlay name is set to xv2Filter2D.

  + To run testXfFilter2D.py (/your_pynqCV_folder/applicationCode/overlayTests/testPython), copy filter2d python test script (with the correct overlay name on lines 6 and 14) to your test folder on the board.
    ```commandline
    $ scp testXfFilter2D.py. xilinx@<pynq-board-ip>:/home/xilinx/proj/test
    ```

  + Create a folder with test images, for instance /proj/images and copy over some test images (the testXfFilter2D.py will try to open ../images/bigBunny_1080.png).
  + ssh into your Pynq board enabling X11:
    ```commandline
    $ ssh -X xilinx@<pynq-board-ip> // password: xilinx
    ```
  + run the filter2D python test script with sudo:
    ```commandline
    $ sudo ./testXfFilter2D.py 
    ```
    you should see ~60fps measured result as well as an outline of big bunny pop up on your terminal.
 


