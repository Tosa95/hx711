/*
*   Author: Tosatto Davide
*
*   Module that controls sample reading from the HX711 sensor.
*
*   Due to process switches, it can happen that a reading fails sometimes (on average one over 70).
*
*   Since to avoid this kernel mode operation is needed but I have no time to write down a device driver,
*   I simply throw away bad readings, identified by a reading time higher than average. Algorithm below.
*/

#include "hx711.h"
#include <time.h>
#include <wiringPi.h>
#include <stdio.h>

//#define DEBUG //Uncomment to activate debug printfs

static int DT_PIN = 0;
static int SCK_PIN = 0;

static int min, max, min_id, max_id;
static int readValue = 0;
static int samples = 0;
static int _offset = 0;
static double _div = 1;
static int _length = 200;

static volatile int reading = 0; //Keeps the read data

static int readings[200]; //Keeps a rolling log of up to 200 points of read data

static int readTotal = 0;

static long int readAverage = 70; //Keeps the average reading time of a single sample

static float getMillisDiff (clock_t t1, clock_t t2);
static float getDistFromTime (float timeDiff);

static void edge ();

void initHX711 (int dtPin, int sckPin, int offset, double div)
{

    DT_PIN = dtPin;
    SCK_PIN = sckPin;
    _offset = offset;               // No gain from having the separate setupHX711 method, makes code harder to follow
    _div = div;                     // Set global variables based on user argument input
    pinMode(DT_PIN, INPUT);
    pinMode(SCK_PIN, OUTPUT);
    wiringPiISR (DT_PIN, INT_EDGE_FALLING,  edge);

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

void removeMinMax(int orig[], int size)
{
    max = min = orig[0];
    int i;
    for (i = 0; i < size; i++)
    {
        if (orig[i] > max)
        {
            max = orig[i];
            max_id = i;
        }
        else if (orig[i] < min)
        {
            min = orig[i];
            min_id = i;
        }
    }
    orig[max_id] = orig[size-1];
    orig[min_id] = orig[size-2];
    // Now the original array contains everything but the min and max. Simply ignore the last two elements of the array.
    // You could swap the min & max values to the last two indeces to be ignored, but it would be pointless.
}

double getFilteredData (int length)
{
    _length = length;
    removeMinMax(readings, _length);
    int i;
    readTotal = 0;
    for (i = 0; i < (_length-2); i++)
    {
        readTotal += readings[i];
        #ifdef DEBUG
            //printf("%d|", readings[i]);
        #endif
    }
    #ifdef DEBUG
        //printf("%d|", readings[_length-2]);
        //printf("%d|", readings[_length-1]);
        //printf("\n");
    #endif
    return extendSign(readTotal / (_length-2));
}

long getAverageReadingTime ()
{
    return readAverage;
}

static void delayFor ()
{
    int j = 0;

    
}



//Callback function called at falling edge on DT_PIN
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

            if (digitalRead(DT_PIN)){
                #ifdef DEBUG
                    //printf ("%d ", i);
                #endif
                read++;}
        }

        

        digitalWrite(SCK_PIN,HIGH);
        delayMicroseconds (1);
        digitalWrite(SCK_PIN,LOW);


        //Calculates the reading time for this sample
        long int readTime = micros()-microS;

        //Calculates a weighted average through shifts to waste less time
        //It's like doing (readAverage*7 + readTime)/8
        //Probably the copiler would do this as well, but for being sure I used shifts
        readAverage = ((readAverage << 2)  + (readAverage << 1) + readAverage + readTime)>>3;

        //This sample is valid if its reading time is less than average + 25% of the average
        int valid = !(readTime > (readAverage + (readAverage>>2)));

        if (valid)
        {
            int i;
            for (i = 0; i < (_length-1); i++)
            {
                readings[i] = readings[i+1];
                #ifdef DEBUG
                    //printf("%d|", readings[i]);
                #endif
            }
            readings[_length-1] = read;
            readValue = read;
            #ifdef DEBUG
                //printf("%d|", readings[_length-1]);
                //printf("\n");
            #endif
        }
        reading = 0;
    }
}

#ifdef DEBUG

    int main(void)
    {
        wiringPiSetupGpio() ;

        initHX711(2, 3, -455040, 1);
        delay(3000);
        int i;
        for (i = 0; i < 50; i++)
        {
            printf ("%2d : %7.0f %7d  Avg: %3ld\n", i+1, getFilteredData(30), getRawReading(), readAverage);
            delay(100);
        }
    }

#endif