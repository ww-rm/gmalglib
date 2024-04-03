#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <gmalglib/sm3.h>

typedef struct _PySM3Object {
    PyObject_HEAD
    SM3 sm3;
} PySM3Object;

static int PySM3_init(PySM3Object* self, PyObject* args, PyObject* kwargs);
static PyObject* PySM3_update(PySM3Object* self, PyObject* args, PyObject* kwargs);
static PyObject* PySM3_digest(PySM3Object* self, PyObject* Py_UNUSED(args));
static PyObject* PySM3_reset(PySM3Object* self, PyObject* Py_UNUSED(args));
static PyObject* PySM3_copy(PySM3Object* self, PyObject* Py_UNUSED(args));

static PyMethodDef py_methods_def_SM3[] = {
    {"update", (PyCFunction)PySM3_update, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Update internal state with data stream.")},
    {"digest", (PyCFunction)PySM3_digest, METH_NOARGS, PyDoc_STR("Get digest.")},
    {"reset", (PyCFunction)PySM3_reset, METH_NOARGS, PyDoc_STR("Reset internal state to empty.")},
    {"copy", (PyCFunction)PySM3_copy, METH_NOARGS, PyDoc_STR("Copy state to a new object.")},
    {NULL}
};

static PyTypeObject py_type_SM3 = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "sm3.SM3",
    .tp_doc = PyDoc_STR("SM3 Object."),
    .tp_basicsize = sizeof(PySM3Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySM3_init,
    .tp_methods = py_methods_def_SM3,
};

static PyModuleDef py_module_def_sm3 = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "sm3",
    .m_doc = PyDoc_STR("SM3 Algorithm Implemented in C."),
    .m_size = 0,
};

static int PySM3_init(PySM3Object* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "data", NULL };
    Py_buffer py_buffer_data = { 0 };
    int ret = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*:__init__", keys, &py_buffer_data))
        return -1;

    SM3_Init(&self->sm3);

    ret = SM3_Update(&self->sm3, py_buffer_data.buf, py_buffer_data.len);
    PyBuffer_Release(&py_buffer_data);

    if (ret == SM3_ERR_OVERFLOW)
    {
        PyErr_SetString(PyExc_OverflowError, "Data too long.");
        return -1;
    }
    return 0;
}

static PyObject* PySM3_update(PySM3Object* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "data", NULL };
    Py_buffer py_buffer_data = { 0 };
    int ret = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*:update", keys, &py_buffer_data))
        return NULL;

    ret = SM3_Update(&self->sm3, py_buffer_data.buf, py_buffer_data.len);
    PyBuffer_Release(&py_buffer_data);

    if (ret == SM3_ERR_OVERFLOW)
    {
        PyErr_SetString(PyExc_OverflowError, "Data too long.");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* PySM3_digest(PySM3Object* self, PyObject* Py_UNUSED(args))
{
    uint8_t digest[SM3_DIGEST_LENGTH] = { 0 };
    SM3_Digest(&self->sm3, digest);

    return PyBytes_FromStringAndSize((char*)digest, SM3_DIGEST_LENGTH);
}

static PyObject* PySM3_reset(PySM3Object* self, PyObject* Py_UNUSED(args))
{
    SM3_Init(&self->sm3);
    Py_RETURN_NONE;
}

static PyObject* PySM3_copy(PySM3Object* self, PyObject* Py_UNUSED(args))
{
    PySM3Object* other = PyObject_New(PySM3Object, &py_type_SM3);
    if (!other)
        return NULL;

    other->sm3 = self->sm3;
    return (PyObject*)other;
}

PyMODINIT_FUNC PyInit_sm3() {
    PyObject* py_module = NULL;
    PyObject* py_long_SM3_MAX_MSG_BITLEN = NULL;
    PyObject* py_long_SM3_DIGEST_LENGTH = NULL;

    if (PyType_Ready(&py_type_SM3) < 0)
        return NULL;

    if (!(py_module = PyModule_Create(&py_module_def_sm3)))
        return NULL;

    Py_INCREF(&py_type_SM3);
    if (PyModule_AddObject(py_module, "SM3", (PyObject*)&py_type_SM3) < 0)
        goto error;

    if (!(py_long_SM3_MAX_MSG_BITLEN = PyLong_FromUnsignedLongLong(SM3_MAX_MSG_BITLEN)))
        goto error;

    if (PyModule_AddObject(py_module, "SM3_MAX_MSG_BITLEN", py_long_SM3_MAX_MSG_BITLEN) < 0)
        goto error;

    if (!(py_long_SM3_DIGEST_LENGTH = PyLong_FromUnsignedLongLong(SM3_DIGEST_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "SM3_DIGEST_LENGTH", py_long_SM3_DIGEST_LENGTH) < 0)
        goto error;

    return py_module;

error:
    Py_XDECREF(py_long_SM3_DIGEST_LENGTH);
    Py_XDECREF(py_long_SM3_MAX_MSG_BITLEN);
    Py_DECREF(&py_type_SM3);
    Py_DECREF(py_module);
    return NULL;
}
