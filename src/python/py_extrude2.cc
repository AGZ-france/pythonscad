/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "linalg.h"
#include "GeometryUtils.h"
#include "genlang/genlang.h"
#include <Python.h>
#include "pyopenscad.h"
#include "pyfunctions.h"
#include "pyconversion.h"
#include <primitives.h>
#include <HelicoidalExtrudeNode.h>
#include <PathExtrudeNode.h>
#include <SkinNode.h>

double get_double(PyObject *kwargs, const char *name, double defaultValue)
{
    double value = defaultValue;
    PyObject *obj = PyDict_GetItemString(kwargs, name);

    if (!obj) {
        return defaultValue;
    }

    value = PyFloat_AsDouble(obj);
    if(!PyErr_Occurred())
	return value;
    return defaultValue;
}
static char *kwlist[] = {
	"angle", "step", "xOffset",
	"convexity", "xScale", "yScale",  "axeRotate", "xScaleEnd", "yScaleEnd", "zScaleEnd", "zRotate", "xRotate", "xOffsetEnd", 
	"scaleBegin", "scaleEnd", "scaleMatrix", "origin",  
	NULL};

PyObject *python_oo_helicoidal_extrude(PyObject *obj, PyObject *args, PyObject *kwargs)
{
  PyObject *origin = NULL;
  PyObject *scaleBegin  = NULL;
  PyObject *scaleEnd    = NULL;
  PyObject *scaleMatrix = NULL;
  auto discretizer = CreateCurveDiscretizer(kwargs);
  DECLARE_INSTANCE();
  std::shared_ptr<AbstractNode> child;
  auto node = std::make_shared<HelicoidalExtrudeNode>(instance, discretizer);

  PyTypeObject *type = &PyOpenSCADType;
  node->profile_func = NULL;
  node->twist_func = NULL;
  if (obj->ob_type == &PyFunction_Type) {
    Py_XINCREF(obj);  // TODO there to decref it ?
    node->profile_func = obj;
    node->profile_func_hash = python_func_hash(obj);
    auto dummy_node = std::make_shared<SquareNode>(instance);
    node->children.push_back(dummy_node);
  } else {
    PyObject *dummydict = nullptr;
    type = PyOpenSCADObjectType(obj);
    child = PyOpenSCADObjectToNodeMulti(obj, &dummydict);
    auto dummydict_owner = py_owned(dummydict);
    if (child == NULL) return propagate_or_typeerror("Invalid type for  Object in helicoidal_extrude\n");
    node->children.push_back(child);
  }

  double xScale = -1.0, yScale = -1.0;
  node->xOffsetEnd = -1.0;
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|$dddidddddddddOOOO",kwlist, 
		&node->angle, &node->step, &node->xOffset, &node->convexity, &xScale, &yScale, &node->axeRotate, &node->xScalEnd, &node->yScalEnd, &node->zScalEnd,
		&node->zRotate, &node->xRotate, &node->xOffsetEnd, &scaleBegin, &scaleEnd, &scaleMatrix, &origin)) {
    fprintf(stderr, "error during parsing in python_oo_helicoidal_extrude \n");
    return NULL;
  }
  if(node->xOffsetEnd <0)
	  node->xOffsetEnd = node->xOffset;
  node->xRotate += 90;
  node->nbRotation = fabs(node->angle / 360);
  if(xScale >0) {
	if(xScale <= 0.1) xScale = 0.1; // Limitation de reduction !
        node->xScalEnd = xScale * node->nbRotation;
  }
  if(yScale >0) {
	if(yScale <= 0.1) yScale = 0.1; // Limitation de reduction !
        node->yScalEnd = yScale * node->nbRotation;
  }
  
 // return helicoidal_extrude_core(obj, convexity, scale, angle, twist, origin, offset, v, std::move(discretizer));

/*
  if (twist != NULL) {
    if (twist->ob_type == &PyFunction_Type) {
      Py_XINCREF(twist);  // TODO there to decref it ?
      node->twist_func = twist;
      node->twist_func_hash = python_func_hash(twist);
    } else node->twist = PyFloat_AsDouble(twist);
  }
*/

  // origin/offset accept a list/tuple/NumPy 2-vector.
  double ox, oy;
  if (origin != NULL && origin != Py_None) {
    if (!python_is_sequence(origin) ||
        python_vectorval(origin, 2, 2, &ox, &oy, nullptr, nullptr, nullptr)) {
      PyErr_SetString(PyExc_TypeError, "Invalid helicoidal_extrude origin parameter");
      return NULL;
    }
    node->origin_x = ox;
    node->origin_y = oy;
  }
/*
  if (offset != NULL && offset != Py_None) {
    if (!python_is_sequence(offset) ||
        python_vectorval(offset, 2, 2, &ox, &oy, nullptr, nullptr, nullptr)) {
      PyErr_SetString(PyExc_TypeError, "Invalid helicoidal_extrude offset parameter");
      return NULL;
    }
    node->offset_x = ox;
    node->offset_y = oy;
  }
*/
/*
  double dummy;
  Vector3d v(0, 0, 0);
  if (vp != nullptr && !python_vectorval(vp, 3, 3, &v[0], &v[1], &v[2], &dummy)) {
  }
  node->v = v;
 */

  if (node->convexity <= 0) node->convexity = 2;
  if (node->scale <= 0) node->scale = 1;

  node->fn = get_double(kwargs, "fn", 100);
  node->fs = get_double(kwargs, "fs", 0);
  node->fa = get_double(kwargs, "fa", 0);

  return PyOpenSCADObjectFromNode(type, node);
}

PyObject *python_helicoidal_extrude(PyObject *self, PyObject *args, PyObject *kwargs)
{
  Py_ssize_t n_args = PyTuple_Size(args);
Py_ssize_t n_kwargs = 0;

if (kwargs != NULL) {
    n_kwargs = PyDict_Size(kwargs);
}

  PyObject *obj = PyTuple_GetItem(args, 0);

  PyObject *empty_args = PyTuple_New(0);
  PyObject *objHelicoidal = python_oo_helicoidal_extrude(obj, empty_args, kwargs);
  Py_DECREF(empty_args);
  if(objHelicoidal == NULL)
    PyErr_SetString(PyExc_TypeError, "Error in python_helicoidal_extrude(object,...)");
  return objHelicoidal;
}


