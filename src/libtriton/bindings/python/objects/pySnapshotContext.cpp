//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <triton/pythonObjects.hpp>
#include <triton/pythonUtils.hpp>
#include <triton/pythonXFunctions.hpp>
#include <triton/exceptions.hpp>
#include <triton/snapshotEngine.hpp>

namespace triton {
  namespace bindings {
    namespace python {


      //! SolverModel destructor.
      void ContextSnapshot_dealloc(PyObject* self) {
        std::cout << std::flush;
        delete PyContextSnapshot_AsSnapshotEngine(self);
        Py_TYPE(self)->tp_free((PyObject*)self);
      }


      static int ContextSnapshot_print(PyObject* self) {
        std::cout << PyContextSnapshot_AsSnapshotEngine(self);
        return 0;
      }


      static PyObject* ContextSnapshot_str(PyObject* self) {
        try {
          std::stringstream str;
          str << PyContextSnapshot_AsSnapshotEngine(self);
          return PyString_FromFormat("%s", str.str().c_str());
        }
        catch (const triton::exceptions::Exception& e) {
          return PyErr_Format(PyExc_TypeError, "%s", e.what());
        }
      }


      //! SolverModel methods.
      PyMethodDef ContextSnapshot_callbacks[] = {
        {nullptr,     nullptr,                0,              nullptr}
      };


      PyTypeObject ContextSnapshot_Type = {
        PyObject_HEAD_INIT(&PyType_Type)
        0,                                          /* ob_size */
        "ContextSnapshot",                          /* tp_name */
        sizeof(ContextSnapshot_Object),             /* tp_basicsize */
        0,                                          /* tp_itemsize */
        (destructor)ContextSnapshot_dealloc,        /* tp_dealloc */
        (printfunc)ContextSnapshot_print,           /* tp_print */
        0,                                          /* tp_getattr */
        0,                                          /* tp_setattr */
        0,                                          /* tp_compare */
        0,                                          /* tp_repr */
        0,                                          /* tp_as_number */
        0,                                          /* tp_as_sequence */
        0,                                          /* tp_as_mapping */
        0,                                          /* tp_hash */
        0,                                          /* tp_call */
        (reprfunc)ContextSnapshot_str,              /* tp_str */
        0,                                          /* tp_getattro */
        0,                                          /* tp_setattro */
        0,                                          /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,                         /* tp_flags */
        "ContextSnapshot objects",                  /* tp_doc */
        0,                                          /* tp_traverse */
        0,                                          /* tp_clear */
        0,                                          /* tp_richcompare */
        0,                                          /* tp_weaklistoffset */
        0,                                          /* tp_iter */
        0,                                          /* tp_iternext */
        ContextSnapshot_callbacks,                  /* tp_methods */
        0,                                          /* tp_members */
        0,                                          /* tp_getset */
        0,                                          /* tp_base */
        0,                                          /* tp_dict */
        0,                                          /* tp_descr_get */
        0,                                          /* tp_descr_set */
        0,                                          /* tp_dictoffset */
        0,                                          /* tp_init */
        0,                                          /* tp_alloc */
        0,                                          /* tp_new */
        0,                                          /* tp_free */
        0,                                          /* tp_is_gc */
        0,                                          /* tp_bases */
        0,                                          /* tp_mro */
        0,                                          /* tp_cache */
        0,                                          /* tp_subclasses */
        0,                                          /* tp_weaklist */
        (destructor)ContextSnapshot_dealloc,        /* tp_del */
        0                                           /* tp_version_tag */
      };


      PyObject* PyContextSnapshot(const triton::engines::snapshot::SnapshotEngine& snapshot) {
        ContextSnapshot_Object* object;

        PyType_Ready(&ContextSnapshot_Type);
        object = PyObject_NEW(ContextSnapshot_Object, &ContextSnapshot_Type);
        if (object != NULL)
          object->snapshot = new triton::engines::snapshot::SnapshotEngine(snapshot);

        return (PyObject*)object;
      }

    }; /* python namespace */
  }; /* bindings namespace */
}; /* triton namespace */
