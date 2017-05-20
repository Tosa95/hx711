/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   proxSensor.h
 * Author: davide
 *
 * Created on February 16, 2017, 11:33 AM
 */

#ifndef PROXSENSOR_H
#define PROXSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

    void initHX711 (int dtPin, int sckPin, int offset, double div);
    double getReading ();
    int getRawReading ();
    long getAverageReadingTime ();
    void setupHX711 (int offset, double div);

#ifdef __cplusplus
}
#endif

#endif /* PROXSENSOR_H */

