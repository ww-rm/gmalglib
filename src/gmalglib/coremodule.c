#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <gmalglib/sm3.h>
#include <gmalglib/sm4.h>
#include <gmalglib/zuc.h>

typedef struct _PySM3Object {
    PyObject_HEAD
    SM3 sm3;
} PySM3Object;

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
    PySM3Object* other = PyObject_New(PySM3Object, Py_TYPE(self));
    if (!other)
        return NULL;

    other->sm3 = self->sm3;
    return (PyObject*)other;
}

static PyMethodDef py_methods_def_SM3[] = {
    {"update", (PyCFunction)PySM3_update, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Update internal state with data stream.")},
    {"digest", (PyCFunction)PySM3_digest, METH_NOARGS, PyDoc_STR("Get digest.")},
    {"reset", (PyCFunction)PySM3_reset, METH_NOARGS, PyDoc_STR("Reset internal state to empty.")},
    {"copy", (PyCFunction)PySM3_copy, METH_NOARGS, PyDoc_STR("Copy state to a new object.")},
    {NULL}
};

static PyTypeObject py_type_SM3 = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "gmalglib.core.SM3",
    .tp_doc = PyDoc_STR("SM3 Object."),
    .tp_basicsize = sizeof(PySM3Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySM3_init,
    .tp_methods = py_methods_def_SM3,
};

static int PyModule_AddSM3(PyObject* py_module) {
    PyObject* py_long_SM3_MAX_MSG_BITLEN = NULL;
    PyObject* py_long_SM3_DIGEST_LENGTH = NULL;

    if (PyType_Ready(&py_type_SM3) < 0)
        return 0;

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

    return 1;

error:
    Py_XDECREF(py_long_SM3_DIGEST_LENGTH);
    Py_XDECREF(py_long_SM3_MAX_MSG_BITLEN);
    Py_DECREF(&py_type_SM3);
    return 0;
}


typedef struct _PySM4Object {
    PyObject_HEAD
    SM4 sm4;
} PySM4Object;

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

static PyMethodDef py_methods_def_SM4[] = {
    {"encrypt", (PyCFunction)PySM4_encrypt, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Encrypt block data.")},
    {"decrypt", (PyCFunction)PySM4_decrypt, METH_VARARGS | METH_KEYWORDS, PyDoc_STR("Decrypt block data.")},
    {NULL}
};

static PyTypeObject py_type_SM4 = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "gmalglib.core.SM4",
    .tp_doc = PyDoc_STR("SM4 Object."),
    .tp_basicsize = sizeof(PySM4Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PySM4_init,
    .tp_methods = py_methods_def_SM4,
};

static int PyModule_AddSM4(PyObject* py_module) {
    PyObject* py_long_SM4_KEY_LENGTH = NULL;
    PyObject* py_long_SM4_BLOCK_LENGTH = NULL;

    if (PyType_Ready(&py_type_SM4) < 0)
        return 0;

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

    return 1;

error:
    Py_XDECREF(py_long_SM4_BLOCK_LENGTH);
    Py_XDECREF(py_long_SM4_KEY_LENGTH);
    Py_DECREF(&py_type_SM4);
    return 0;
}


typedef struct _PyZUCObject {
    PyObject_HEAD
    ZUC zuc;
} PyZUCObject;

static int PyZUC_init(PyZUCObject* self, PyObject* args, PyObject* kwargs)
{
    char* keys[] = { "key", "iv", NULL};
    Py_buffer py_buffer_key = { 0 };
    Py_buffer py_buffer_iv = { 0 };
    int ret = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*y*:__init__", keys, &py_buffer_key, &py_buffer_iv))
        return -1;

    if (py_buffer_key.len != ZUC_KEY_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "Incorrect key length.");
    }
    else if (py_buffer_iv.len != ZUC_IV_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "Incorrect iv length.");
    }
    else
    {
        ZUC_Init(&self->zuc, py_buffer_key.buf, py_buffer_iv.buf);
        ret = 0;
    }

    PyBuffer_Release(&py_buffer_key);
    PyBuffer_Release(&py_buffer_iv);
    return ret;
}

static PyObject* PyZUC_generate(PyZUCObject* self, PyObject* Py_UNUSED(args))
{
    uint8_t word[ZUC_WORD_LENGTH] = { 0 };
    ZUC_Generate(&self->zuc, word);

    return PyBytes_FromStringAndSize((char*)word, ZUC_WORD_LENGTH);
}

static PyObject* PyZUC_copy(PyZUCObject* self, PyObject* Py_UNUSED(args))
{
    PyZUCObject* other = PyObject_New(PyZUCObject, Py_TYPE(self));
    if (!other)
        return NULL;

    other->zuc = self->zuc;
    return (PyObject*)other;
}

static PyMethodDef py_methods_def_ZUC[] = {
    {"generate", (PyCFunction)PyZUC_generate, METH_NOARGS, PyDoc_STR("Generate a word (4 bytes).")},
    {"copy", (PyCFunction)PyZUC_copy, METH_NOARGS, PyDoc_STR("Copy state to a new object.")},
    {NULL}
};

static PyTypeObject py_type_ZUC = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "gmalglib.core.ZUC",
    .tp_doc = PyDoc_STR("ZUC Object."),
    .tp_basicsize = sizeof(PyZUCObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)PyZUC_init,
    .tp_methods = py_methods_def_ZUC,
};

static int PyModule_AddZUC(PyObject* py_module) {
    PyObject* py_long_ZUC_KEY_LENGTH = NULL;
    PyObject* py_long_ZUC_IV_LENGTH = NULL;
    PyObject* py_long_ZUC_WORD_LENGTH = NULL;

    if (PyType_Ready(&py_type_ZUC) < 0)
        return 0;

    Py_INCREF(&py_type_ZUC);
    if (PyModule_AddObject(py_module, "ZUC", (PyObject*)&py_type_ZUC) < 0)
        goto error;

    if (!(py_long_ZUC_KEY_LENGTH = PyLong_FromUnsignedLongLong(ZUC_KEY_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "ZUC_KEY_LENGTH", py_long_ZUC_KEY_LENGTH) < 0)
        goto error;

    if (!(py_long_ZUC_IV_LENGTH = PyLong_FromUnsignedLongLong(ZUC_IV_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "ZUC_IV_LENGTH", py_long_ZUC_IV_LENGTH) < 0)
        goto error;

    if (!(py_long_ZUC_WORD_LENGTH = PyLong_FromUnsignedLongLong(ZUC_WORD_LENGTH)))
        goto error;

    if (PyModule_AddObject(py_module, "ZUC_WORD_LENGTH", py_long_ZUC_WORD_LENGTH) < 0)
        goto error;

    return 1;

error:
    Py_XDECREF(py_long_ZUC_WORD_LENGTH);
    Py_XDECREF(py_long_ZUC_IV_LENGTH);
    Py_XDECREF(py_long_ZUC_KEY_LENGTH);
    Py_DECREF(&py_type_ZUC);
    return 0;
}


static PyModuleDef py_module_def_core = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "gmalglib.core",
    .m_doc = PyDoc_STR("GM Algorithm Implemented in C."),
    .m_size = 0,
};

PyMODINIT_FUNC PyInit_core() {
    PyObject* py_module = NULL;

    if (PyType_Ready(&py_type_SM3) < 0)
        return NULL;

    if (!(py_module = PyModule_Create(&py_module_def_core)))
        return NULL;

    if (!PyModule_AddSM3(py_module))
        goto error;

    if (!PyModule_AddSM4(py_module))
        goto error;

    if (!PyModule_AddZUC(py_module))
        goto error;

    return py_module;

error:
    Py_DECREF(py_module);
    return NULL;
}
