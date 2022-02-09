from distutils.core import setup, Extension

kpathsea_module = Extension('pykpathsea_xetex', sources = ['pykpathsea_xetex.c'], libraries=["kpathsea_xetex"])

setup(name='pykpathsea_xetex',
      version='0.2.1',
      description='Kpathsea',
      ext_modules=[kpathsea_module])

