#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <gmalglib/sm4.h>

typedef struct _PySM4Object {
    PyObject_HEAD
    SM4 sm4;
} PySM4Object;

static int PySM4_init(PySM4Object* self, PyObject* args, PyObject* kwargs);
static PyObject* PySM4_encrypt(PySM4Object* self, PyObject* args, PyObject* kwargs);
static PyObject* PySM4_decrypt(PySM4Object* self, PyObject* args, PyObject* kwargs);

static PyMethodDef py_methods_def_SM4[] = {
    {"encrypt", (PyCFunction)PySM4_encrypt, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Encrypt block data.")},
    {"decrypt", (PyCFunction)PySM4_decrypt, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Decrypt block data.")},
    {NULL}
};

static PyTypeObject py_type_SM4 = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "sm4.SM4",
    .tp_doc = PyDoc_STR("SM4 Object."),
    .tp_basicsize = sizeof(PySM4Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySM4_init,
    .tp_methods = py_methods_def_SM4,
};

static PyModuleDef py_module_def_sm4 = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "sm4",
    .m_doc = PyDoc_STR("SM4 Algorithm Implemented in C."),
    .m_size = 0,
};

static int PySM4_init(PySM4Object* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "key", NULL };
    Py_buffer py_buffer_key = { 0 };
    int ret = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*:__init__", keys, &py_buffer_key))
        return -1;

    if (py_buffer_key.len != SM4_KEY_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "Incorrect key length.");
        ret = -1;
    }
    else
    {
        SM4_Init(&self->sm4, py_buffer_key.buf);
    }

    PyBuffer_Release(&py_buffer_key);
    return ret;
}

static PyObject* PySM4_encrypt(PySM4Object* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "block", NULL };
    Py_buffer py_buffer_block = { 0 };
    uint8_t block_out[SM4_BLOCK_LENGTH] = { 0 };
    PyObject* ret = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*:encrypt", keys, &py_buffer_block))
        return NULL;

    if (py_buffer_block.len != SM4_BLOCK_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "Incorrect block length.");
    }
    else
    {
        SM4_Encrypt(&self->sm4, py_buffer_block.buf, block_out);
        ret = PyBytes_FromStringAndSize((char*)block_out, SM4_BLOCK_LENGTH);
    }

    PyBuffer_Release(&py_buffer_block);
    return ret;
}

static PyObject* PySM4_decrypt(PySM4Object* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "block", NULL };
    Py_buffer py_buffer_block = { 0 };
    uint8_t block_out[SM4_BLOCK_LENGTH] = { 0 };
    PyObject* ret = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*:decrypt", keys, &py_buffer_block))
        return NULL;

    if (py_buffer_block.len != SM4_BLOCK_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "Incorrect block length.");
    }
    else
    {
        SM4_Decrypt(&self->sm4, py_buffer_block.buf, block_out);
        ret = PyBytes_FromStringAndSize((char*)block_out, SM4_BLOCK_LENGTH);
    }

    PyBuffer_Release(&py_buffer_block);
    return ret;
}

PyMODINIT_FUNC PyInit_sm4() {
    PyObject* py_module = NULL;
    PyObject* py_long_SM4_KEY_LENGTH = NULL;
    PyObject* py_long_SM4_BLOCK_LENGTH = NULL;

    if (PyType_Ready(&py_type_SM4) < 0)
        return NULL;

    if (!(py_module = PyModule_Create(&py_module_def_sm4)))
        return NULL;

    Py_INCREF(&py_type_SM4);
    if (PyModule_AddObject(py_module, "SM4", (PyObject*)&py_type_SM4) < 0)
        goto error;

    if (!(py_long_SM4_KEY_LENGTH = PyLong_FromUnsignedLongLong(SM4_KEY_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "SM4_KEY_LENGTH", py_long_SM4_KEY_LENGTH) < 0)
        goto error;


    if (!(py_long_SM4_BLOCK_LENGTH = PyLong_FromUnsignedLongLong(SM4_BLOCK_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "SM4_BLOCK_LENGTH", py_long_SM4_BLOCK_LENGTH) < 0)
        goto error;

    return py_module;

error:
    Py_XDECREF(py_long_SM4_BLOCK_LENGTH);
    Py_XDECREF(py_long_SM4_KEY_LENGTH);
    Py_DECREF(&py_type_SM4);
    Py_DECREF(py_module);
    return NULL;
}