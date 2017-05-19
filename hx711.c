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

#include "hx711.h"
#include <time.h>
#include <wiringPi.h>
#include <stdio.h>

#define DEBUG

static int DT_PIN = 0;
static int SCK_PIN = 0;

static int readValue = 0;
static int _offset = 0;
static double _div = 0;

static volatile int reading = 0;

static long int readAverage = 70;

unsigned long int t1,t2;
const int SOUND_CONST = 171.50;

static float getMillisDiff (clock_t t1, clock_t t2);
static float getDistFromTime (float timeDiff);

static void edge ();

void initHX711 (int dtPin, int sckPin, int offset, double div)
{

    DT_PIN = dtPin;
    SCK_PIN = sckPin;
    pinMode(DT_PIN, INPUT);
    pinMode(SCK_PIN, OUTPUT);
    wiringPiISR (DT_PIN, INT_EDGE_FALLING,  edge);

    _offset = offset;
    _div = div;
}


int extendSign (int val)
{
    if ((val & (1<<23))>0)
    {
        val += 0xFF<<24;
    } 

    return val;
}

double getReading ()
{
    int val = extendSign(readValue);
    
    return (val + _offset)/_div;
}

int getRawReading ()
{
    return extendSign(readValue);
}

long getAverageReadingTime ()
{
    return readAverage;
}

static void delayFor ()
{
    int j = 0;

    
}



//Funzione callback richiamata al rilevamento di un fronte sul piedino ECHO_PIN
static void edge ()
{
    if (!reading && !digitalRead(DT_PIN))
    {
        //printf("Reading \n");

        long int microS = micros();

        reading = 1;

        int read = 0;

        int i;

        int j = 0;

        delayMicroseconds (1);

        for (i = 0; i < 24; i++)
        {
            digitalWrite(SCK_PIN,HIGH);
            read <<= 1;

            
            delayMicroseconds (1);

            digitalWrite(SCK_PIN,LOW);
            //delayMicroseconds (10);

            if (digitalRead(DT_PIN)){
                //printf ("%d ", i);
                read++;}
        }

        

        digitalWrite(SCK_PIN,HIGH);
        delayMicroseconds (1);
        digitalWrite(SCK_PIN,LOW);


        // long int readTimeDiff = 0;

        // if (prevReadUS > 0)
        // {
        //     readTimeDiff = micros() - prevReadUS;
        // }

        // prevReadUS = micros();

        long int readTime = micros()-microS;

        readAverage = ((readAverage << 2)  + (readAverage << 1) + readAverage + readTime)>>3;

        int valid = !(readTime > (readAverage + ((readAverage<<2)>>4)));


        // printf ("Read: %ld  Time: %ld  Avg: %ld", read, readTime, readAverage);

        // if (!valid)
        // {
        //     printf (" INVALID!\n");
        // } else {
        //     printf ("\n");
        // }


        if (valid)
            readValue = read;


        reading = 0;
    }

    
}

#ifdef DEBUG

    int main(void)
    {
        wiringPiSetupGpio() ;

        initHX711(21, 20, -43000, 200000.0);

        delay(100);

        int i;

        for (i = 0; i < 3000; i++)
        {
            printf ("%5.2f %d Avg: %3ld\n", getReading(), getRawReading(), readAverage);
            delay(100);
        }
    }

#endif