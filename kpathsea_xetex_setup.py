from distutils.core import setup, Extension

xetex_module = Extension('pykpathsea_xetex', sources = ['pykpathsea_xetex.c'], libraries=["kpathsea"])

setup(name='pykpathsea_xetex',
      version='0.2.1',
      description='Kpathsea_xetex',
      ext_modules=[xetex_module])

