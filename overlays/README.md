# Building xfOpenCV Overlays for PYNQ: CMake based sds++ cross-compilation

## List of Supported Components 

| Image Arithmetic      | Filters       |   Geometric Transform |    Features    | Input Processing	| Analysis 	   | Flow and Depts|
| ---------             | ---------     |   ---------           |    ---------   |    ---------     |  ---------   | --------- 	   |
| absdiff               |  filter2D     |   remap               |      canny     | * split 	 	      |	calcHist     | stereoBM      |  
| bitwise_and           |  dilate    	  |   resize         | cornerHarris |	* merge |	equalizeHist | calcOpticalFlowDenseNonPyrLK  |
| bitwise_or            |   erode       |   warpAffine          |     fast       |	      	        | integral	   |               |
| bitwise_xor           |   medianBlur  |  warpPerspective      |                |			            | meanStdDev	 |               | 
| bitwise_not           |   boxFilter   |                       |                |					        | minMaxLoc		 |               |   
| threshold             |   pyrUp       |                       |                |                  | LUT          |               |
| add                   |   pyrDown     |                       |                |                  |              |               | 
| subtract              |||||||
| multiply              |||||||
| accumulate            |||||||
| accumulateWeighted    |||||||
| accumulateSquare      |||||||
| magnitude             |||||||
| phase                 |||||||

\* These designs require a patch to xfopencv (tag: 2018.3_release) in order to build properly. Please clone https://github.com/denolf/xfopencv.git and checkout the v2018.2Fixed branch to pick up a temporary fix.

## Setup Environment on Host

  + clone [Pynq-ComputerVision](https://github.com/Xilinx/PYNQ-ComputerVision) repository:
    ```commandline
    $ git clone https://github.com/Xilinx/PYNQ-ComputerVision.git <your_pynqcv_folder>
    ``` 
  + clone [xfOpenCV](https://github.com/Xilinx/xfopencv) repository and checkout the correct version (e.g. 2018.2_release):
    ```commandline
    $ git clone https://github.com/Xilinx/xfopencv.git <your_xfopencv_folder>
    $ cd <your_xfopencv_folder>
    $ git checkout <release_number>
    ``` 
  + Prepare the Ultra96 bare platform package in /your_PynqPlatform_folder. 
    + download [ultra96_platform_sysroot_2018.3.tar.gz](https://www.xilinx.com/member/forms/download/xef.html?filename=ultra96_platform_sysroot_2018.3.tar.gz) in /your_PynqPlatform_folder
    + untar the package. This will create an ultra subfolder in /your_PynqPlatform_folder
      ```commandline
      $ tar -zxvf ultra96_platform_sysroot_2018.3.tar.gz
      ```
    + Note that you can adapt /your_PynqPlatform_folder to your preference, but the deepest subfolder should match the platform name, in this case 'ultra'. 
  + set an environmental variable to xfOpenCV
    ```commandline
    $ setenv XFOPENCV_PATH <your_xfopencv_folder>
    ```
  + set up Xilinx SDx tools, version 2018.3 by running its setup script


### Building your Overlay
  + create an overlay folder in /<your_pynqcv_folder>/overlays, for instance myFirstOverlay
  + Copy and adapt the [/<your_pynqcv_folder>/overlays/cvXfUserSpecific/CMakeLists.txt](./cvXfUserSpecific/CMakeLists.txt) to /<your_pynqcv_folder>/overlays/myFirstOverlay
    + line 41, choose your overlay name, for instance xv2MyFirstOverlay
    + line 45, choose the CV components offloaded to PL in your overlay, restricted to a subset of filter2D, remap, dilate, stereoBM and canny
    + lines 47-49: optionally overwrite some of the default instantiation paramaters (defined in the [setDefaultInstantiationParameters CMake macro](../frameworks/cmakeModules/rulesForSDxXfOpenCV.cmake#L37)) by user specific ones 
  + create a build folder in /<your_pynqcv_folder>/overlays/myFirstOverlay run CMake:
    ```commandline
    $ cd /<your_pynqcv_folder>/overlays/myFirstOverlay
    $ mkdir build; cd build
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../frameworks/cmakeModules/cmakeModulesXilinx/toolchain_sds.cmake -DSDxPlatform=/your_PynqPlatform_folder/ultra -DSDxClockID=1 -DusePL=ON -DnoBitstream=OFF -DnoSDCardImage=ON -DSDxArch=arm64
    ```
  + Note that the clock ID for the platform selects the desired clock frequency for the overlay design. In the case of the Ultra96 platform, the following IDs are available: 0=100MHz, 1=150MHz, 2=250MHz, 3=300MHz.
  + run make for the target with your chosen overlay name
    ```commandline
    $ make xv2MyFirstOverlay
    ```
  + run make install. This will copy three files (xv2MyFirstOverlay.tcl, xv2MyFirstOverlay.so, xv2MyFirstOverlay.bit) to /<your_pynqcv_folder>/overlays/myFirstOverlay/libarm64 
    ```commandline
    $ make install
    ```
  + copy the content of /<your_pynqcv_folder>/overlays/myFirstOverlay/build/libarm64 to a test folder (for instance ~/proj/test) on your pynq board:
    ```commandline
    $  scp xv2MyFirstOverlay.* xilinx@<pynq-board-ip>:/home/xilinx/proj/test
    ```
    
 ## Using PL offloaded CV Modules
 
 Your Python test code first needs to load the overlay, then import the python bindings for the PL offloaded CV modules and then call the methods from those bindings. Allocating numpy arrays in physically contiguous memory maximizes the efficiency of DRAM data movement.
 
 An example python script for filter 2D is available in: [applicationCode/overlayTests/testPython/testXfFilter2D.py](../applicationCode/overlayTests/testPython/testXfFilter2D.py)
 
 ### Loading the Overlay
 
 Before we load the overlay, it is a good idea to first install the PYNQ computer vision extensions. This can be done by following the pip install instructions for the pynq-computervision extension found [here](https://github.com/Xilinx/PYNQ-ComputerVision). From the command line on your pynq board, you need to run:
 
```bash
$ sudo -H pip3 install --upgrade git+https://github.com/Xilinx/PYNQ-ComputerVision.git
```
Then to load the overlay, you add the following to your python code:

 ```python
print("Loading overlay") 
from pynq import Bitstream
bs = Bitstream("/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2MyFirstOverlay.bit")
bs.download()

print("Loading xlnk")
from pynq import Xlnk
Xlnk.set_allocator_library('/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/xv2MyFirstOverlay.so')
mem_manager = Xlnk()
```   

The function Xlnk.set_allocator_library is required if we ever need to allocate continguous memory buffers. Note that we should put the install files (.bit,.so,.hwh) in the overlays folder (/usr/local/lib/python3.6/dist-packages/pynq_cv/overlays). This is needed so the we can find and import the library file in the next step.

### Import Overlay Python Bindings

```python
import pynq_cv.overlays.xv2MyFirstOverlay as xv2
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

The [example python script for filter 2D (testXfFilter2D.py)](../applicationCode/overlayTests/testPython/testXfFilter2D.py) on the repo assumes your xv2MyFirstOverlay listed filter2D as one of the modules. If necessary, adapt the .bit filename of line 37 and library name on line 45 to the overlay name of your choice (xv2MyFirstOverlay in the steps above). Currently the overlay name is set to xv2Filter2DDilate which is installed as part of the pynq-computervision extension.

  + To run testXfFilter2D.py (/<your_pynqcv_folder>/applicationCode/overlayTests/testPython), copy filter2d python test script (with the correct overlay name on lines 37 and 45) to your test folder on the board.
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
    you should see ~140fps measured result as well as an outline of big bunny pop up on your terminal.
 
## Running sample Python unit tests

Additional example unit tests for all supported kernels can be found [here](../applicationCode/unitTests/testPython). If valid overlays are built using the python build script ([buildUnitOverlays.py](../overlays/buildUnitOverlays.py)), then we can execute all the python tests on the board using the script [runUnitTests.py](../applicationCode/unitTests/testPython/runUnitTests.py). 

First we run the build script on the host:
```commandline
$ cd ../applicationCode/overlays
$ ./buildUnitOverlays.py |& tee build.log
```
Each kernel will be made into an overlay and the relevant files (.so, .bit, .hwh) will be stored in the build area: ../applicationCode/overlays/unitOverlays/overlayFiles.

We then copy the files over to the board's overlays folder:
```commandline
$ cd ../applicationCode/overlays/unitOverlays/overlayFiles
$ scp * xilinx@192.168.3.1:/home/xilinx/tmp
$ ssh xilinx@192.168.3.1
$ cd tmp
$ sudo cp tmp/* /usr/local/lib/python3.6/dist-packages/pynq_cv/overlays/.
```
Then an images sub-folder should be created under ../applicationCode/unitTests/images which has the 3 image files used in the tests (bigBunny_1080.png, bbb-splash_1080.png, 000005_10_L.png, 000005_10_R.png). 
```commandline
$ cd /home/xilinx/proj/PYNQ-ComputerVision/applicationCode/unitTests
$ mkdir images
$ <cp image files to this sub-folder>
```
Finally, the test script can be run.
```commandline
$ cd /home/xilinx/proj/PYNQ-ComputerVision/applicationCode/unitTests/testPython
$ ./runUnitTests.py |& tee run.log
```


