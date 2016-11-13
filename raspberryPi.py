from devicehub import Sensor, Device, Project
from time import sleep
from random import randint
import serial

PROJECT_ID      = '8311'
DEVICE_UUID     = 'c5007706-3970-4efe-93e7-13f1bcf6d6fc'
API_KEY         = '9069dc54-f38e-426f-833d-9637fbc04dc1'

SENSOR_NAMES    = ['Temperature', 'Humidity', 'Pressure', 'Light', 'IR', 'UV', 'Remote_Presence', 'Remote_Temperature']
SENSOR_VALUES   = []
SENSORS         = []

def add_sensor(type, name, dev):
        AN = Sensor(type, name)
        dev.addSensor(AN)
        return AN

def analog_input(dev, sensor, value):
        sensor.addValue(float(value))
        dev.send()
        return


ser=serial.Serial('/dev/ttyUSB0', 9600)
ser.flushInput()
ser.flushOutput()
line = ser.readline()
line = ser.readline()
print 'first', line

project = Project(PROJECT_ID, ssl_verify=False)
device = Device(project, DEVICE_UUID, API_KEY)

for s in SENSOR_NAMES:
        num = add_sensor(Sensor.ANALOG, s, device)
        SENSORS.append(num)


while True:
        try:

                SENSOR_VALUES = []
                line = ser.readline()
                print line
                print len(line)
                fields = line[:-1].split(',')
                #if( len(line) > 35 and fields != None):
                if(fields != None):
                        for f in fields:
                                SENSOR_VALUES.append(f)

                        for i in range(0, 8):
                                print SENSOR_NAMES[i], SENSOR_VALUES[i]
                                analog_input(device, SENSORS[i], SENSOR_VALUES[i])
                        print 'Data Sent OK!'

        except:
                print 'Something went wrong, retrying...'
        sleep(8.0)
