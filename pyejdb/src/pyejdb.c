#include "pyejdb.h"

/* PEJDB */
typedef struct {
    PyObject_HEAD
    EJDB *ejdb;
} PEJDB; 

#include "EJDB.c"

PyDoc_STRVAR(ejdb_m_doc, "EJDB http://ejdb.org");
PyDoc_STRVAR(ejdb_version_doc, "version() -> str\n\nReturns the version string of the underlying EJDB library.");

static PyObject* ejdb_version(PyObject *module) {
    return PyString_FromString(tcversion);
}

/* cabinet_module.m_methods */
static PyMethodDef pyejdb_m_methods[] = {
    {"version", (PyCFunction) ejdb_version, METH_NOARGS, ejdb_version_doc},
    {NULL} /* Sentinel */
};

/* pyejdb_module */
static PyModuleDef pyejdb_module = {
    PyModuleDef_HEAD_INIT,
    "_pyejdb", /*m_name*/
    ejdb_m_doc, /*m_doc*/
    -1, /*m_size*/
    pyejdb_m_methods, /*m_methods*/
};

PyObject* init_pyejdb(void) {
    PyObject *pyejdb;
    if (PyType_Ready(&EJDBType)) {
        return NULL;
    }

    pyejdb = PyModule_Create(&pyejdb_module);

    if (!pyejdb) {
        return NULL;
    }
    Error = PyErr_NewException("_pyejdb.Error", NULL, NULL);
    Py_XINCREF(Error);
    if (!Error || PyModule_AddObject(pyejdb, "Error", Error)) {
        Py_XDECREF(Error);
        goto fail;
    }

    /* adding types and constants */
    if (
            PyModule_AddType(pyejdb, "EJDB", &EJDBType) ||

            PyModule_AddIntMacro(pyejdb, JBOREADER) ||
            PyModule_AddIntMacro(pyejdb, JBOWRITER) ||
            PyModule_AddIntMacro(pyejdb, JBOCREAT) ||
            PyModule_AddIntMacro(pyejdb, JBOTRUNC) ||
            PyModule_AddIntMacro(pyejdb, JBONOLCK) ||
            PyModule_AddIntMacro(pyejdb, JBOLCKNB) ||
            PyModule_AddIntMacro(pyejdb, JBOTSYNC) ||

            PyModule_AddIntMacro(pyejdb, JBIDXDROP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXDROPALL) ||
            PyModule_AddIntMacro(pyejdb, JBIDXOP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXOP) ||
            PyModule_AddIntMacro(pyejdb, JBIDXREBLD) ||
            PyModule_AddIntMacro(pyejdb, JBIDXNUM) ||
            PyModule_AddIntMacro(pyejdb, JBIDXSTR) ||
            PyModule_AddIntMacro(pyejdb, JBIDXARR) ||
            PyModule_AddIntMacro(pyejdb, JBIDXISTR) ||

            PyModule_AddIntMacro(pyejdb, JBQRYCOUNT)
            ) {

        goto fail;
    }

    return pyejdb;

fail:
    Py_DECREF(pyejdb);
    return NULL;
}

PyMODINIT_FUNC PyInit__pyejdb(void) {
    return init_pyejdb();
}
