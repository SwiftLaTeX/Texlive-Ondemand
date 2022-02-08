/* kpsewhich -- standalone path lookup and variable expansion for Kpathsea.
   Ideas from Thomas Esser, Pierre MacKay, and many others.

   Copyright 1995-2016 Karl Berry & Olaf Weber.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, see <http://www.gnu.org/licenses/>.  */

#include <Python.h>
#include <fontconfig/fontconfig.h>
#include <string.h>

static char* searchFont(const char* fontname, int requiredBold, int requiredItalic)
{
  FcConfig* config = FcInitLoadConfigAndFonts(); //Most convenient of all the alternatives

  //does not necessarily has to be a specific name.  You could put anything here and Fontconfig WILL find a font for you
  FcPattern* pat = FcNameParse((const FcChar8*)fontname);
  if (requiredItalic) {
    FcPatternAddInteger(pat, FC_SLANT, FC_SLANT_ITALIC);
  }
  
  if (requiredBold) {
    FcPatternAddInteger(pat, FC_WEIGHT, FC_WEIGHT_BOLD);
  }
  
  FcConfigSubstitute(config, pat, FcMatchPattern);//NECESSARY; it increases the scope of possible fonts
  FcDefaultSubstitute(pat);//NECESSARY; it increases the scope of possible fonts

  char* fontFile = NULL;
  FcResult result;

  FcPattern* font = FcFontMatch(config, pat, &result);

  if (font)
  {
    //The pointer stored in 'file' is tied to 'font'; therefore, when 'font' is freed, this pointer is freed automatically.
    //If you want to return the filename of the selected font, pass a buffer and copy the file name into that buffer
    FcChar8* file = NULL; 
    if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
    {
      int lens = strlen((const char *)(file)) + 1;
      fontFile = malloc(lens);
      strcpy(fontFile, (const char *)file);
    }
  }
  FcPatternDestroy(font);//needs to be called for every pattern created; in this case, 'fontFile' / 'file' is also freed
  FcPatternDestroy(pat);//needs to be called for every pattern created
  FcConfigDestroy(config);//needs to be called for every config created
  return fontFile;
}


static PyObject *py_fontconfig_find_font(PyObject *self, PyObject *args) {
  char *filename;
  char *completefilename;
  int requiredBold = 0;
  int requiredItalic = 0;
  PyObject *returnvalue;

  if (PyArg_ParseTuple(args, "sii", &filename, &requiredBold, &requiredItalic)) {
    completefilename = searchFont(filename, requiredBold, requiredItalic);
    returnvalue = Py_BuildValue("s", completefilename);
    if (completefilename != NULL) {
      free(completefilename);
    }
    return returnvalue;
  }
  return NULL;
}

/* exported methods */

static PyMethodDef pykpathsea_methods[] = {
    {"find_font", (PyCFunction)py_fontconfig_find_font, METH_VARARGS, NULL},
    {NULL, NULL}};

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT,
                                       "pyfontconfig",
                                       NULL,
                                       -1,
                                       pykpathsea_methods,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL};

PyMODINIT_FUNC PyInit_pyfontconfig(void) {

  

  PyObject *module = PyModule_Create(&moduledef);
  if (module == NULL)
    return NULL;

  FcInit(); // initializes Fontconfig

  return module;
}