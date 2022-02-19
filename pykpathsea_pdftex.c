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
#include <kpathsea/c-ctype.h>
#include <kpathsea/c-pathch.h>
#include <kpathsea/config.h>
#include <kpathsea/expand.h>
#include <kpathsea/getopt.h>
#include <kpathsea/line.h>
#include <kpathsea/pathsearch.h>
#include <kpathsea/proginit.h>
#include <kpathsea/str-list.h>
#include <kpathsea/tex-file.h>
#include <kpathsea/tex-glyph.h>
#include <kpathsea/variable.h>
#include <kpathsea/version.h>
#include <string.h>

static kpathsea kpse = NULL;
static unsigned int DPI = 600;

static PyObject *py_kpse_find_pk(PyObject *self, PyObject *args) {
  char *filename;
  int dpi;
  char *completefilename;
  PyObject *returnvalue;
  if (PyArg_ParseTuple(args, "si", &filename, &dpi)) {
    kpse_glyph_file_type font_file_ex;
    completefilename = kpathsea_find_glyph(kpse, filename, dpi, kpse_pk_format, &font_file_ex);
    returnvalue = Py_BuildValue("s", completefilename);
    if (completefilename != NULL)
      free(completefilename);
    return returnvalue;
  }
  return NULL;
}

static PyObject *py_kpse_find_file(PyObject *self, PyObject *args) {
  char *filename;
  int fmt;
  char *completefilename;
  PyObject *returnvalue;
  if (PyArg_ParseTuple(args, "si", &filename, &fmt)) {
    completefilename = kpathsea_find_file(kpse, filename, fmt, 0);
    returnvalue = Py_BuildValue("s", completefilename);
    if (completefilename != NULL)
      free(completefilename);
    return returnvalue;
  }
  return NULL;
}

/* exported methods */

static PyMethodDef pykpathsea_pdftex_methods[] = {
    {"find_file", (PyCFunction)py_kpse_find_file, METH_VARARGS, NULL},
    {"find_pk", (PyCFunction)py_kpse_find_pk, METH_VARARGS, NULL},
    {NULL, NULL}};

static struct PyModuleDef moduledef = {PyModuleDef_HEAD_INIT,
                                       "pykpathsea_pdftex",
                                       NULL,
                                       -1,
                                       pykpathsea_pdftex_methods,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL};

PyMODINIT_FUNC PyInit_pykpathsea_pdftex(void) {
  PyObject *module = PyModule_Create(&moduledef);
  if (module == NULL)
    return NULL;

  kpse = kpathsea_new();

  kpathsea_set_program_name(kpse, "pdflatex", "pdflatex");

  kpathsea_set_program_enabled(kpse, kpse_pk_format, true,
                               kpse_src_cmdline - 1);

  kpathsea_set_program_enabled(kpse, kpse_tfm_format, true,
                               kpse_src_cmdline - 1);

  kpathsea_xputenv(kpse, "engine", "pdftex");

  kpathsea_init_prog(kpse, uppercasify(kpse->program_name), DPI, NULL, NULL);

  return module;
}