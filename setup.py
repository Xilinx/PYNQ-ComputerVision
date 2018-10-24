#   Copyright (c) 2017, Xilinx, Inc.
#   All rights reserved.
# 
#   Redistribution and use in source and binary forms, with or without 
#   modification, are permitted provided that the following conditions are met:
#
#   1.  Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#
#   2.  Redistributions in binary form must reproduce the above copyright 
#       notice, this list of conditions and the following disclaimer in the 
#       documentation and/or other materials provided with the distribution.
#
#   3.  Neither the name of the copyright holder nor the names of its 
#       contributors may be used to endorse or promote products derived from 
#       this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


from setuptools import setup, find_packages
from distutils.dir_util import copy_tree
import os
import shutil


__author__ = "Jack Lo"
__copyright__ = "Copyright 2017, Xilinx"
__email__ = "jackl@xilinx.com"


# global variables
pynqcv_module_name = 'pynq_cv'
board = os.environ['BOARD']
repo_board_folder = 'boards/{}/'.format(board)
board_notebooks_dir = os.environ['PYNQ_JUPYTER_NOTEBOOKS']
pynqcv_data_files = []


# check whether board is supported
def check_env():
    if not os.path.isdir(repo_board_folder):
        raise ValueError("Board {} is not supported.".format(board))
    if not os.path.isdir(board_notebooks_dir):
        raise ValueError("Directory {} does not exist.".format(board_notebooks_dir))


# copy overlays to python package
def copy_overlays():
    src_ol_dir = os.path.join(repo_board_folder, 'overlays')
    dst_ol_dir = os.path.join(pynqcv_module_name, 'overlays')
    copy_tree(src_ol_dir, dst_ol_dir)
    pynqcv_data_files.extend([os.path.join("..", dst_ol_dir, f)
                              for f in os.listdir(dst_ol_dir)])


# copy notebooks to jupyter home
def copy_notebooks():
    src_nb_dir = os.path.join(repo_board_folder, 'notebooks')
    dst_nb_dir = os.path.join(board_notebooks_dir, 'pynqOpenCV')
    if os.path.exists(dst_nb_dir):
        shutil.rmtree(dst_nb_dir)
    copy_tree(src_nb_dir, dst_nb_dir)


check_env()
copy_overlays()
copy_notebooks()


setup(name=pynqcv_module_name,
      version='2.3',
      install_requires=['pynq>=2.3'],
      description='PYNQ computer vision package',
      license='BSD 3-Clause License',
      author='Xilinx computer vision group',
      author_email='jackl@xilinx.com',
      url='https://github.com/Xilinx/PYNQ-ComputerVision',
      packages=find_packages(),
      download_url='https://github.com/Xilinx/PYNQ-ComputerVision',
      package_data={
          '': pynqcv_data_files,
      }
      )
