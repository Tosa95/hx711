/*
*   Autori: Tosatto Davide, Grespan Riccardo
*
*   Modulo di controllo del sensore ad ultrasuoni per la rilevazione della distanza.
*
*   Si è deciso di scrivere questo modulo in C perche, per leggere la distanza
*   e necessario misurare la durata di un'impulso, cosa che, fatta attraverso polling,
*   va ad utilizzare troppe risorse di calcolo, in quanto non si possono inserire
*   sleep nel ciclo per mantenere una precisione accettabile.
*
*   Abbiamo pertanto utilizzato wiringPi, che, nella sua versione C, permette di
*   mettersi in ascolto di variazioni sui pin di I/O attraverso il meccanismo degli
*   interrupt e quindi senza spreco di risorse di calcolo
*
*   In questo file abbiamo il modulo in C puro. Nel file pythonizedProxSensor.c
*/

#include "proxSensor.h"
#include <time.h>
#include <wiringPi.h>
#include <stdio.h>

static int DT_PIN = 0;
static int SCK_PIN = 0;

static int readValue = 0;
static volatile int reading = 0;

unsigned long int t1,t2;
const int SOUND_CONST = 171.50;

static float getMillisDiff (clock_t t1, clock_t t2);
static float getDistFromTime (float timeDiff);

static void edge ();

void initProxSensor (int dtPin, int sckPin)
{
    DT_PIN = dtPin;
    SCK_PIN = sckPin;
    pinMode(DT_PIN, INPUT);
    pinMode(SCK_PIN, OUTPUT);
    wiringPiISR (DT_PIN, INT_EDGE_FALLING,  edge);
}

int getReading ()
{
    return readValue;
}

//Funzione callback richiamata al rilevamento di un fronte sul piedino ECHO_PIN
static void edge ()
{
    if (!reading)
    {
        reading = 1;

        read = 0;

        for (int i = 0; i < 25; i++)
        {
            digitalWrite(SCK_PIN,HIGH);
            read = read << 1;
            digitalWrite(SCK_PIN,LOW);

            if (digitalRead(DT_PIN))
                read++;
        }

        readValue = read;

        reading = 0;
    }

    
}
