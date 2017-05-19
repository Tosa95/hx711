import RPi.GPIO as GPIO
import time
import Queue
import threading
import hx711

DT = 40
SCK = 38

def getReadAvg (samples):
    
    acc = 0

    print "Now reading %d samples for taking the average\n" % samples

    for i in range(samples):
        val = hx711.getRawReading()
        print "%d, %d" % (val, hx711.getAverageReadingTime())
        acc = acc + val

        time.sleep(0.1)
    
    return acc/samples;

def main():
    hx711.initialize(21, 20, -43000, 200000.0)

    raw_input("Put nothing on sensor and press enter to start calibrating: ")
    
    offset = -getReadAvg(100)

    raw_input("Now put something of which you kow the weight and press enter: ")
    
    wAvg = getReadAvg(100)

    fixedWeight = float(raw_input("Insert now the weight of the object in Kg: "))

    div = (wAvg+offset)/fixedWeight

    print "Offset: %d,  div: %f\n" % (offset, div)

if __name__ == '__main__':
    main()
