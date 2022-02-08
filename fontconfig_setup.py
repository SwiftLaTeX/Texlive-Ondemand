from distutils.core import setup, Extension

fontconfig_module = Extension('pyfontconfig', sources = ['pyfontconfig.c'], libraries=["fontconfig"])

setup(name='pyfontconfig',
      version='0.1.0',
      description='pyfontconfig',
      ext_modules=[fontconfig_module])