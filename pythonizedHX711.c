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

// Exported functions table
static PyMethodDef module_methods[] = {
    {"getReading", hx711_getReading, METH_VARARGS,  "Returns read wheight"},
    {"getRawReading", hx711_getRawReading, METH_VARARGS, "Returns read wheight, raw mode"},
    {"getAverageReadingTime", hx711_getAverageReadingTime, METH_VARARGS, "Returns average reading time"},
    {"initialize", hx711_initialize, METH_VARARGS, "Initializes the module"},
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

static PyObject * hx711_getAverageReadingTime (PyObject *self, PyObject *args)
{
    return PyInt_FromLong((long)getAverageReadingTime());
}

// Funzione che inizializza il sensore. Se ritorna 0: ok. Se ritorna 1: errore
static PyObject * hx711_initialize (PyObject *self, PyObject *args)
{
    int dtPin, sckPin, offset;
    double div;

    //Serve per leggere due inter (ii) dagli argomenti passati
    if (!PyArg_ParseTuple(args, "iiid", &dtPin, &sckPin, &offset, &div))
        return PyInt_FromLong(1);

    wiringPiSetupGpio();

    initHX711(dtPin, sckPin, offset, div);

    return PyInt_FromLong(0);
}
