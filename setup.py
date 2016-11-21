from distutils.core import setup, Extension
import os.path, sys

rle_c_lib = 'rle_python_interface/librle_c.so'
if not os.path.isfile(rle_c_lib):
  print 'ERROR: Unable to find required library: %s. Please ensure you\'ve '\
    'built RLE using CMake.'%(rle_c_lib)
  sys.exit()

module1 = Extension('rle_python_interface.rle_c_wrapper',
                    libraries = ['rle_c'],
                    include_dirs = ['src'],
                    library_dirs = ['rle_python_interface'],
                    extra_compile_args=['-D__STDC_CONSTANT_MACROS', '-std=c++11'],
                    sources=['rle_python_interface/rle_c_wrapper.cpp'])
setup(name = 'rle_python_interface',
      version='0.1.0',
      description = 'Arcade Learning Environment Python Interface',
      url='github.com/nadavbh12/Retro-Learning-Environment',
      author='Ben Goodrich', # Modified by Nadav Bhonker
      license='GPL',
      ext_modules = [module1],
      packages=['rle_python_interface'],
      package_dir={'rle_python_interface': 'rle_python_interface'},
      package_data={'rle_python_interface': ['librle_c.so']})
