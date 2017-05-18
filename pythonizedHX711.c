/*
*   Autori: Tosatto Davide, Grespan Riccardo
*
*   Wrapper del modulo di controllo del sensore di prossimita attraverso le
*   primitive di interoperabilità Python-C
*
*/

#include <Python.h>
#include <wiringPi.h>
#include "hx711.h"

static PyObject * proxsensor_getReading (PyObject *self, PyObject *args);
static PyObject * proxsensor_initialize (PyObject *self, PyObject *args);

// Tabella delle funzioni esportate
static PyMethodDef module_methods[] = {
    {"getReading", proxsensor_getReading, METH_VARARGS, "Ritorna il peso letto"},
    {"initialize", proxsensor_initialize, METH_VARARGS, "Inizializza il modulo"},
    // Terminatore
    {NULL, NULL, 0, NULL}
};

//Funzione che inizializza il modulo. Obbligatoria per permettere il funzionamento dello stesso
PyMODINIT_FUNC
inithx711(void)
{
    PyObject *m = Py_InitModule3("hx711", module_methods, "Packet for controlling the ADC HX711");
    if (m == NULL)
        return;
}


//----------FUNZIONI ESPORTATE-------------------

static PyObject * proxsensor_getReading (PyObject *self, PyObject *args)
{
    return PyFloat_FromDouble(getReading());
}

// Funzione che inizializza il sensore. Se ritorna 0: ok. Se ritorna 1: errore
static PyObject * proxsensor_initialize (PyObject *self, PyObject *args)
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
