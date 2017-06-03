/*
*   Author: Davide Tosatto
*
*   Python wrapper module for hx711.c
*/

#include <Python.h>
#include <wiringPi.h>
#include "hx711.h"

static PyObject * hx711_getReading (PyObject *self, PyObject *args);
static PyObject * hx711_getRawReading (PyObject *self, PyObject *args);
static PyObject * hx711_getAverageReadingTime (PyObject *self, PyObject *args);
static PyObject * hx711_initialize (PyObject *self, PyObject *args);
static PyObject * hx711_setup (PyObject *self, PyObject *args);
static PyObject * hx711_getDropCount (PyObject *self, PyObject *args);

// Exported functions table
static PyMethodDef module_methods[] = {
    {"getReading", hx711_getReading, METH_VARARGS,  "Returns read wheight"},
    {"getRawReading", hx711_getRawReading, METH_VARARGS, "Returns read wheight, raw mode"},
    {"getDropCount", hx711_getDropCount, METH_VARARGS,  "Returns the number of samples dropped from the beginning"},
    {"getAverageReadingTime", hx711_getAverageReadingTime, METH_VARARGS, "Returns average reading time"},
    {"initialize", hx711_initialize, METH_VARARGS, "Initializes the module"},
    {"setup", hx711_setup, METH_VARARGS, "Sets the sensor calibration"},
    // Terminator
    {NULL, NULL, 0, NULL}
};

//Mandatory function that initializes the module
PyMODINIT_FUNC
inithx711(void)
{
    PyObject *m = Py_InitModule3("hx711", module_methods, "Packet for controlling the ADC HX711");
    if (m == NULL)
        return;
}


//----------EXPORTED_FUNCTIONS-------------------

static PyObject * hx711_getReading (PyObject *self, PyObject *args)
{
    return PyFloat_FromDouble(getReading());
}

static PyObject * hx711_getRawReading (PyObject *self, PyObject *args)
{
    return PyInt_FromLong((long)getRawReading());
}

static PyObject * hx711_getDropCount (PyObject *self, PyObject *args)
{
    return PyInt_FromLong((long)getDropCount());
}

static PyObject * hx711_getAverageReadingTime (PyObject *self, PyObject *args)
{
    return PyInt_FromLong((long)getAverageReadingTime());
}

// Initializes the sensor
static PyObject * hx711_initialize (PyObject *self, PyObject *args)
{
    int dtPin, sckPin, offset;
    double div;

    if (!PyArg_ParseTuple(args, "iiid", &dtPin, &sckPin, &offset, &div))
        return PyInt_FromLong(1);

    wiringPiSetupGpio();

    initHX711(dtPin, sckPin, offset, div);

    return PyInt_FromLong(0);
}

static PyObject * hx711_setup (PyObject *self, PyObject *args)
{
    int offset;
    double div;

    if (!PyArg_ParseTuple(args, "id", &offset, &div))
        return PyInt_FromLong(1);

    setupHX711(offset, div);

    return PyInt_FromLong(0);
}