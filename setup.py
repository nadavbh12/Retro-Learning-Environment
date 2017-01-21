import sys
import os
import multiprocessing
from setuptools import setup
import os.path as op
import distutils.command.build as _build
import distutils.dir_util as dd
import distutils.spawn as ds

# script based on:
#   https://github.com/mgbellemare/Arcade-Learning-Environment/blob/master/setup.py
#   https://github.com/sim-x/simx/blob/master/setup.py
#   https://github.com/openai/atari-py/blob/master/setup.py

def run_cmake():
    if ds.find_executable('cmake') is None:
        print "CMake is required to build RLE"
        print "Please install cmake version >= 3.02 and re-run setup"
        sys.exit(-1)

    new_dir = op.join(op.split(__file__)[0], 'build')
    dd.mkpath(new_dir)
    os.chdir(new_dir)
    cmake_args = ' -DBUILD_EXAMPLES=OFF -DBUILD_PYTHON=ON'
    cores_to_use = max(1, multiprocessing.cpu_count() - 1)

    try:
        ds.spawn(['cmake', '../'] + cmake_args.split())
        ds.spawn(['make', '-j', str(cores_to_use)])
    except ds.DistutilsExecError:
        print "Error while running cmake"
        print "run 'setup.py build --help' for build options"
        print "You may also try editing the settings in CMakeLists.txt file and re-running setup"
        sys.exit(-1)

###########################
# Custom 'build' command
###########################
class Build(_build.build):
    def run(self):
        cwd = os.getcwd()
        run_cmake()
        os.chdir(cwd)

setup(name = 'rle_python_interface',
      version='1.0.3',
      description = 'Retro Learning Environment Python Interface based on Ben Goodrich\'s work',
      url='https://github.com/nadavbh12/Retro-Learning-Environment',
      download_url='https://github.com/nadavbh12/Retro-Learning-Environment/tarball/1.0.3',
      author='Nadav Bhonker, Shai Rozenberg',
      author_email='nadavbh@gmail.com;shai.roz1989@gmail.com',
      license = 'GPL',
      packages=['rle_python_interface'],
      package_dir={'rle_python_interface': 'rle_python_interface'},
      package_data={'rle_python_interface': ['librle_c.so',
                                             'stella_libretro.so',
                                             'snes9x2010_libretrolinux-x86_64.so',
                                             'genesis_plus_gx_libretro.so']},
      cmdclass={'build': Build }
      )
