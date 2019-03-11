#!/usr/local/bin/python
import csv
from random import randint
import sys
import operator
import numpy as np

def generate_zodiac_data(filename, nRows):
	with open(filename, 'w+') as f:
	    wr = csv.writer(f, delimiter =' ')
	    # wr.writerow(['Aquarius', 'Pisces', 'Aries', 
	    # 	'Taurus', ' Gemini', ' Cancer', 'Leo', 'Virgo', 
	    # 	'Libra', 'Scorpio', 'Sagittarius','Capricorn', 'Extra-1', 'Extra-2','Extra-3','Extra-4','Extra-5', More-1, More-2
	    # 	])


	    # generate random rows
	    for i in range(0, nRows):
	       	extraBits = [0] * 5
	       	extra = randint(0, 4)
	       	extraBits[extra] = 1

	       	moreBits = [0] * 2
	       	more = randint(0, 1)
	       	moreBits[more] = 1

	       	zodiacBits = [0]*12
	       	zodiac = randint(0, 11)
	       	zodiacBits[zodiac] = 1

	        row = zodiacBits + extraBits + moreBits

	        wr.writerow(row)

def generate_patient_data(filename, nRows):
	with open(filename, 'w+') as f:
	    wr = csv.writer(f, delimiter =' ')
	    # wr.writerow(['Age', 'Height', 'Weight', 'Blood Pressure-Systolic', 'Blood Pressure-Diastolic' ])

	    ageRangeMin = 12
	    ageRangeMax = 110

	    heightRangeMin = 48
	    heightRangeMax = 80
	    
	    weightRangeMin = 84
	    weightRangeMax = 140 # higher in dataset bc influenced by height

	    bpSystolicRangeMin = 100
	    bpSystolicRangeMax = 160

	    bpDiastolicRangeMin = 44
	    bpDiastolicRangeMax = 60 # higher in dataset bc influenced by Systolic BP

	    # generate random rows
	    for i in range(0, nRows):
	       	age = randint(ageRangeMin, ageRangeMax)
	        height = randint(heightRangeMin, heightRangeMax)
	        weight = randint(weightRangeMin, weightRangeMax) + int((height/float(heightRangeMax))*randint(weightRangeMin, weightRangeMax))
	        bps = randint(bpSystolicRangeMin, bpSystolicRangeMax)
	        bpd = randint(bpDiastolicRangeMin, bpDiastolicRangeMax) + int((bps/float(bpSystolicRangeMax))*randint(bpDiastolicRangeMin, bpDiastolicRangeMax))


	        row = [age, height, weight, bps, bpd]

	        wr.writerow(row)

if __name__ == "__main__":
    if (len(sys.argv) < 4 or int(sys.argv[2]) == 0):
        print("Usage: ./datagen.py filename n <cont | cat (for continous or categorical datasets)>")
        sys.exit(1)

    filename = ""
    n = 0

    filename = sys.argv[1]
    n = int(sys.argv[2])

    if sys.argv[3] == 'cont':
    	generate_patient_data(filename, n)
    elif sys.argv[3] == 'cat':
    	generate_zodiac_data(filename, n)
    else:
    	print("specify continous or categorical dataset!")
    	sys.exit(1)
