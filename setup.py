from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys
import multiprocessing


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        # test env variable to determine whether to build in debug
        if os.environ.get("DEBUG") is not None:
            cfg = 'Debug'
        else:
            cfg = 'Release'
        build_args = ['--config', cfg]
        env = os.environ.copy()

        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]

        cpu_count = max(2, multiprocessing.cpu_count() // 2)
        build_args += ['--', '-j{0}'.format(cpu_count)]

        python_path = sys.executable
        cmake_args += ['-DPYTHON_EXECUTABLE:FILEPATH=' + python_path]

        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp, env=env)

        subprocess.check_call(
            ['cmake', '--build', '.', "--target", "maestro"] + build_args,
            cwd=self.build_temp)


long_description = f"""Disclaimer: I only wrote python binding parts of this package for my own research.
If you find any bugs (which I did a lot) from the Maestro source code, please contact the maintainer of Maestro project. 
I am happy to get any feedback on python binding."
"""

setup(
    name='maestro',
    version='0.0.1',
    author='Taeyoung Kong',
    author_email='kongty@stanford.edu',
    description="Python bindings for Maestro. Original C++ code: https://github.com/maestro-project/maestro",
    long_description=long_description,
    long_description_content_type='text/x-rst',
    python_requires=">=3.6",
    cmdclass={"build_ext": CMakeBuild},
    ext_modules=[CMakeExtension('maestro')],
)
