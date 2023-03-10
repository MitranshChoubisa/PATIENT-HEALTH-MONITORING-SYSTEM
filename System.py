
import MySQLdb
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)

#define pins
define HEARTRATE_PIN_SIG 0
define PIR_PIN_SIG 4
define MCP3008_PIN_CS 8


########
GPIO.setup(0, GPIO.IN)
GPIO.setup(4, GPIO.IN)
GPIO.setup(8, GPIO.IN)

########
# Settings for database connection
hostname = '172.0.1.2'
username = 'patient'
password = 'patient'
database = 'INFO'

########
# Query to insert data into table
def insert_record( temp, ecg, bodymovement ):
	query = "INSERT INTO temps3 (temp, ecg, bodymovement) " \
                "VALUES (%s,%s,%s,%s)"
    	args = (temp, ecg, bodymovement)

    	try:
        	conn = MySQLdb.connect( host=hostname, user=username, passwd=password, db=database )
		cursor = conn.cursor()
        	cursor.execute(query, args)
		conn.commit()

    	except Exception as error:
        	print(error)

    	finally:
        	cursor.close()
        	conn.close()
