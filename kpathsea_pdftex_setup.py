from distutils.core import setup, Extension

pdftex_module = Extension('pykpathsea_pdftex', sources = ['pykpathsea_pdftex.c'], libraries=["kpathsea"])

setup(name='pykpathsea_pdftex',
      version='0.1.0',
      description='Kpathsea_pdftex',
      ext_modules=[pdftex_module])