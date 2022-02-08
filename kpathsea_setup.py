from distutils.core import setup, Extension

kpathsea_module = Extension('pykpathsea', sources = ['pykpathsea.c'], libraries=["kpathsea"])

setup(name='pykpathsea',
      version='0.2.1',
      description='Kpathsea',
      ext_modules=[kpathsea_module])

