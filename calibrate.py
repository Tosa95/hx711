import RPi.GPIO as GPIO
import time
import Queue
import threading
import hx711

SAMPLES = 300

def getData (samples):
    
    print "Now reading %d samples \n" % samples

    res = []

    for i in range(samples):
        val = hx711.getRawReading()
        res.append(val)
        print "%4d: %d, %d" % (i+1, val, hx711.getAverageReadingTime())
        time.sleep(0.1)

    return res


def getReadAvg (data):
    
    acc = 0

    

    for d in data:
        val = hx711.getRawReading()
        acc = acc + d
       
    
    return acc/len(data);

def getMaxDistance (data, div):
    
    max = 0.0

    for i in range(len(data)):
        for j in range(len(data)):
            
            #print "%d - %d = %d \n" % (data[i], data[j], data[i]-data[j]) 

            dist = abs(data[i]-data[j])

            if (dist > max):
                max = dist
    
    print "MAX DIFFERENCE: %d \n" % max

    return max/div

def main():
    hx711.initialize(21, 20, -43000, 200000.0)

    print "Sensor calibration script.\n\nIt's important not to move or touch nor the sensor nor the surface on which it lies while calibrating.\n\n\n"

    raw_input("Put nothing on the sensor and press enter to start calibrating: ")
    
    offset = -getReadAvg(getData(SAMPLES))

    raw_input("Now put something of which you know the weight on the sensor and press enter: ")
    
    data = getData(SAMPLES)

    wAvg = getReadAvg(data)

    fixedWeight = float(raw_input("Insert now the weight of the object in Kg: "))

    div = (wAvg+offset)/fixedWeight

    print "Offset: %d,  div: %f, precision: %f Kg \n" % (offset, div, getMaxDistance(data, div))

if __name__ == '__main__':
    main()
