import RPi.GPIO as GPIO
import time
import Queue
import threading
import hx711

DT = 40
SCK = 38

def main():
    hx711.initialize(21, 20, -40337, 2010924.528302)

    while True:
        print "%.5f Kg, read time: %d, drops: %d" % (hx711.getReading(), hx711.getAverageReadingTime(), hx711.getDropCount())
        time.sleep(0.1)

def data_ready (pin):
    
    if(not GPIO.input(DT)):

        count = 0

        for j in range(25):
            GPIO.output(SCK, 1)
            count = count * 2
            GPIO.output(SCK, 0)

            if (GPIO.input(DT)):
                count = count + 1
                #print "c%d" % j

        print count

if __name__ == '__main__':
    main()
