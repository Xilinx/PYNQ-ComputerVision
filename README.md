# PYNQ - Computer Vision

All PYNQ releases ship with the popular [OpenCV](https://opencv.org/) library pre-installed. The PYNQ computer vision overlays enable accelerating OpenCV components in Programmable Logic (PL). These overlays expose a subset Xilinx' [xfOpenCV](https://github.com/Xilinx/xfopencv) library (a part of Xilinx' [reVISION solution](https://www.xilinx.com/products/design-tools/embedded-vision-zone.html)) at the Python level, combined with the support for HDMI input/output (Pynq-Z1 and Pynq-Z2 only). Webcam, stream or file based input/output remains available through the pre-installed SW OpenCV (on all Pynq boards).

Currently this package is compatible with [PYNQ image v2.4](http://www.pynq.io/board).

![](./block_diagram.png)

A number of prebuilt overlays are available for your PYNQ-enabled board. For example:
  + Pynq-Z1/ Pynq-Z2:
    + 2D filter & dilate: accelerated 3x3 2D filter and 3x3 dilate. See  [filter2d_and_dilate.ipynb](./boards/Pynq-Z1/notebooks/filter2d_and_dilate.ipynb).
    + 2D filter & remap: accelerated 3x3 2D filter and remap. See [filter2d_and_remap.ipynb](./boards/Pynq-Z1/notebooks/filter2d_and_remap.ipynb). 
  + Ultra96:
    + 2D filter & dilate % absdiff. See [2__Overlays-filter2d_and_dilate.ipynb](./boards/Ultra96/notebooks/2__Overlays-filter2d_and_dilate.ipynb).
    + remap & stereoBM: See [3a_Overlays-stereo_block_matching.ipynb](./boards/Ultra96/notebooks/3a_Overlays-stereo_block_matching.ipynb).
    + optical flow: See [4__Overlays-opticalflow.ipynb](./boards/Ultra96/notebooks/4__Overlays-opticalflow.ipynb).

## Quick Start

To install the computer vision overlay example on your board, open a terminal and run:

   ```bash
   $ sudo pip3 install --upgrade git+https://github.com/Xilinx/PYNQ-ComputerVision.git
   ```
   
After the setup, new Jupyter notebooks will be added under the pynqOpenCV folder, ready to try out, no additional steps are needed. 

## Building Your Own Overlay

When after profiling, the critical OpenCV modules in an application are identified, the generation of a tailored overlay is easily done by specifying those modules as a list in a CMake project file. After that, building the tailored overlay only requires running cmake to generate the Makefiles followed by make to build the overlay.

For detailed instructions:  [Building xfOpenCV Overlays for Pynq: CMake based sds++ cross-compilation](overlays/README.md). Currently, this only supports building on the Ultra96 with 2018.3 Xilinx tools.

## License

**Source files** : [3-Clause BSD License](LICENSE)

**SDSoC Platform** : [Multiple License File](https://www.xilinx.com/member/forms/download/xef.html?filename=pynq-ultra96-dsa-license-ca.txt) 

**Sysroot**
  + License and Copyrights Info [TAR/GZIP](https://www.xilinx.com/member/forms/download/xef.html?filename=pynq-v2.3-license.tar.gz)
  + Open Components Source Code [TAR/GZIP](https://www.xilinx.com/member/forms/download/xef.html?filename=pynq-v2.3-open_components.tar.gz)
