EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:SF2016-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +15V #PWR2
U 1 1 58490510
P 1000 1150
F 0 "#PWR2" H 1000 1000 50  0001 C CNN
F 1 "+15V" H 1000 1290 50  0000 C CNN
F 2 "" H 1000 1150 50  0000 C CNN
F 3 "" H 1000 1150 50  0000 C CNN
	1    1000 1150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 58490524
P 1000 1500
F 0 "#PWR3" H 1000 1250 50  0001 C CNN
F 1 "GND" H 1000 1350 50  0000 C CNN
F 2 "" H 1000 1500 50  0000 C CNN
F 3 "" H 1000 1500 50  0000 C CNN
	1    1000 1500
	1    0    0    -1  
$EndComp
Wire Notes Line
	850  800  1450 800 
Wire Notes Line
	1450 800  1450 1750
Wire Notes Line
	1450 1750 850  1750
Wire Notes Line
	850  1750 850  800 
$Comp
L M_receptacle PWR1
U 1 1 5849095D
P 2250 1400
F 0 "PWR1" H 2250 1400 60  0000 C CNN
F 1 "M_receptacle" H 2250 1700 60  0000 C CNN
F 2 "" H 2250 1400 60  0001 C CNN
F 3 "" H 2250 1400 60  0001 C CNN
	1    2250 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1150 1750 1150
Wire Wire Line
	1000 1500 1000 1350
Wire Wire Line
	1000 1350 1750 1350
Text Notes 850  1300 0    60   ~ 0
15V DC 0.56A
$Comp
L GND #PWR5
U 1 1 584909F8
P 2950 1850
F 0 "#PWR5" H 2950 1600 50  0001 C CNN
F 1 "GND" H 2950 1700 50  0000 C CNN
F 2 "" H 2950 1850 50  0000 C CNN
F 3 "" H 2950 1850 50  0000 C CNN
	1    2950 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1850 2950 1200
Wire Wire Line
	2950 1200 2700 1200
Text Notes 2250 1600 0    60   ~ 0
+15V DC
$Comp
L TLC555 U1
U 1 1 58490A96
P 4100 2950
F 0 "U1" H 4000 3375 50  0000 R CNN
F 1 "TLC555" H 4000 3300 50  0000 R CNN
F 2 "" H 4150 2700 50  0000 L CNN
F 3 "" H 4750 2400 50  0000 C CNN
	1    4100 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 1300 2700 2350
Wire Wire Line
	1700 2350 4750 2350
Wire Wire Line
	4100 2350 4100 2550
Wire Wire Line
	4750 2350 4750 2750
Wire Wire Line
	4750 2750 4600 2750
Connection ~ 4100 2350
$Comp
L R R1
U 1 1 58490D91
P 2150 2750
F 0 "R1" V 2230 2750 50  0000 C CNN
F 1 "10K" V 2150 2750 50  0000 C CNN
F 2 "" V 2080 2750 50  0000 C CNN
F 3 "" H 2150 2750 50  0000 C CNN
	1    2150 2750
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 58490DD0
P 2600 3000
F 0 "R2" V 2680 3000 50  0000 C CNN
F 1 "10K" V 2600 3000 50  0000 C CNN
F 2 "" V 2530 3000 50  0000 C CNN
F 3 "" H 2600 3000 50  0000 C CNN
	1    2600 3000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR6
U 1 1 58491043
P 4100 3750
F 0 "#PWR6" H 4100 3500 50  0001 C CNN
F 1 "GND" H 4100 3600 50  0000 C CNN
F 2 "" H 4100 3750 50  0000 C CNN
F 3 "" H 4100 3750 50  0000 C CNN
	1    4100 3750
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5849105F
P 2950 3400
F 0 "C1" H 2975 3500 50  0000 L CNN
F 1 "820pF" H 2975 3300 50  0000 L CNN
F 2 "" H 2988 3250 50  0000 C CNN
F 3 "" H 2950 3400 50  0000 C CNN
	1    2950 3400
	1    0    0    -1  
$EndComp
NoConn ~ 4600 3050
Text Notes 1650 3600 0    60   ~ 0
Ceramic capacitor, 50V
Wire Wire Line
	1700 2350 1700 2750
Wire Wire Line
	1700 2750 2000 2750
Connection ~ 2700 2350
Wire Wire Line
	2300 2750 3600 2750
Wire Wire Line
	2350 2750 2350 3000
Wire Wire Line
	2350 3000 2450 3000
Connection ~ 2350 2750
Wire Wire Line
	2750 3000 3450 3000
Wire Wire Line
	3450 2950 3450 3050
Wire Wire Line
	3450 2950 3600 2950
Wire Wire Line
	3450 3050 3600 3050
Connection ~ 3450 3000
Wire Wire Line
	2950 3000 2950 3250
Connection ~ 2950 3000
Wire Wire Line
	4100 3250 4100 3750
Wire Wire Line
	2950 3550 2950 3650
Wire Wire Line
	2950 3650 4100 3650
Connection ~ 4100 3650
$Comp
L SP-4 T1
U 1 1 58495517
P 2100 4950
F 0 "T1" H 2150 4500 50  0000 L CNN
F 1 "SP-4" H 2100 5400 50  0000 C CNN
F 2 "" H 2100 4950 50  0000 C CNN
F 3 "" H 2100 4950 50  0000 C CNN
	1    2100 4950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4600 2950 4700 2950
Wire Wire Line
	4700 2950 4700 4100
Wire Wire Line
	4700 4100 800  4100
Wire Wire Line
	800  4100 800  4750
Wire Wire Line
	800  4750 1700 4750
$Comp
L GND #PWR1
U 1 1 58495755
P 800 5550
F 0 "#PWR1" H 800 5300 50  0001 C CNN
F 1 "GND" H 800 5400 50  0000 C CNN
F 2 "" H 800 5550 50  0000 C CNN
F 3 "" H 800 5550 50  0000 C CNN
	1    800  5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5150 800  5150
Wire Wire Line
	800  5150 800  5550
NoConn ~ 1700 4950
NoConn ~ 2500 4950
Text Notes 2450 4400 0    60   ~ 0
All capacitors ceramic, 200V\nAll diodes general purpose 200V
$Comp
L C C2
U 1 1 58495A73
P 2650 4750
F 0 "C2" H 2675 4850 50  0000 L CNN
F 1 "C" H 2675 4650 50  0000 L CNN
F 2 "" H 2688 4600 50  0000 C CNN
F 3 "" H 2650 4750 50  0000 C CNN
	1    2650 4750
	0    1    1    0   
$EndComp
$Comp
L C C3
U 1 1 58495E39
P 3050 5200
F 0 "C3" H 3075 5300 50  0000 L CNN
F 1 "C" H 3075 5100 50  0000 L CNN
F 2 "" H 3088 5050 50  0000 C CNN
F 3 "" H 3050 5200 50  0000 C CNN
	1    3050 5200
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 5851FB6C
P 3500 4750
F 0 "C4" H 3525 4850 50  0000 L CNN
F 1 "C" H 3525 4650 50  0000 L CNN
F 2 "" H 3538 4600 50  0000 C CNN
F 3 "" H 3500 4750 50  0000 C CNN
	1    3500 4750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR4
U 1 1 5852069F
P 2600 5550
F 0 "#PWR4" H 2600 5300 50  0001 C CNN
F 1 "GND" H 2600 5400 50  0000 C CNN
F 2 "" H 2600 5550 50  0000 C CNN
F 3 "" H 2600 5550 50  0000 C CNN
	1    2600 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 5150 2600 5150
Wire Wire Line
	2600 5150 2600 5550
$Comp
L DD D1
U 1 1 58520856
P 2800 5000
F 0 "D1" H 2750 5150 50  0000 C CNN
F 1 "DD" H 2850 4950 50  0001 C CNN
F 2 "" H 2800 5000 50  0000 C CNN
F 3 "" H 2800 5000 50  0000 C CNN
	1    2800 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D2
U 1 1 5852158C
P 3250 4950
F 0 "D2" H 3200 5100 50  0000 C CNN
F 1 "DD" H 3300 4900 50  0001 C CNN
F 2 "" H 3250 4950 50  0000 C CNN
F 3 "" H 3250 4950 50  0000 C CNN
	1    3250 4950
	0    1    1    0   
$EndComp
$Comp
L DD D3
U 1 1 585215F7
P 3700 5000
F 0 "D3" H 3650 5150 50  0000 C CNN
F 1 "DD" H 3750 4950 50  0001 C CNN
F 2 "" H 3700 5000 50  0000 C CNN
F 3 "" H 3700 5000 50  0000 C CNN
	1    3700 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D4
U 1 1 58521690
P 4150 4950
F 0 "D4" H 4100 5100 50  0000 C CNN
F 1 "DD" H 4200 4900 50  0001 C CNN
F 2 "" H 4150 4950 50  0000 C CNN
F 3 "" H 4150 4950 50  0000 C CNN
	1    4150 4950
	0    1    1    0   
$EndComp
Text Notes 3500 5400 3    60   ~ 0
Stage 1
$Comp
L DD D5
U 1 1 58521782
P 4600 5000
F 0 "D5" H 4550 5150 50  0000 C CNN
F 1 "DD" H 4650 4950 50  0001 C CNN
F 2 "" H 4600 5000 50  0000 C CNN
F 3 "" H 4600 5000 50  0000 C CNN
	1    4600 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D6
U 1 1 585217CD
P 5050 4950
F 0 "D6" H 5000 5100 50  0000 C CNN
F 1 "DD" H 5100 4900 50  0001 C CNN
F 2 "" H 5050 4950 50  0000 C CNN
F 3 "" H 5050 4950 50  0000 C CNN
	1    5050 4950
	0    1    1    0   
$EndComp
$Comp
L DD D7
U 1 1 5852184A
P 5500 5000
F 0 "D7" H 5450 5150 50  0000 C CNN
F 1 "DD" H 5550 4950 50  0001 C CNN
F 2 "" H 5500 5000 50  0000 C CNN
F 3 "" H 5500 5000 50  0000 C CNN
	1    5500 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D8
U 1 1 585218B1
P 5950 4950
F 0 "D8" H 5900 5100 50  0000 C CNN
F 1 "DD" H 6000 4900 50  0001 C CNN
F 2 "" H 5950 4950 50  0000 C CNN
F 3 "" H 5950 4950 50  0000 C CNN
	1    5950 4950
	0    1    1    0   
$EndComp
$Comp
L DD D9
U 1 1 58521944
P 6400 5000
F 0 "D9" H 6350 5150 50  0000 C CNN
F 1 "DD" H 6450 4950 50  0001 C CNN
F 2 "" H 6400 5000 50  0000 C CNN
F 3 "" H 6400 5000 50  0000 C CNN
	1    6400 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D10
U 1 1 5852198F
P 6850 4950
F 0 "D10" H 6800 5100 50  0000 C CNN
F 1 "DD" H 6900 4900 50  0001 C CNN
F 2 "" H 6850 4950 50  0000 C CNN
F 3 "" H 6850 4950 50  0000 C CNN
	1    6850 4950
	0    1    1    0   
$EndComp
$Comp
L DD D11
U 1 1 585219E0
P 7300 5000
F 0 "D11" H 7250 5150 50  0000 C CNN
F 1 "DD" H 7350 4950 50  0001 C CNN
F 2 "" H 7300 5000 50  0000 C CNN
F 3 "" H 7300 5000 50  0000 C CNN
	1    7300 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D12
U 1 1 58521A39
P 7750 4950
F 0 "D12" H 7700 5100 50  0000 C CNN
F 1 "DD" H 7800 4900 50  0001 C CNN
F 2 "" H 7750 4950 50  0000 C CNN
F 3 "" H 7750 4950 50  0000 C CNN
	1    7750 4950
	0    1    1    0   
$EndComp
$Comp
L DD D13
U 1 1 58521A8E
P 8200 5000
F 0 "D13" H 8150 5150 50  0000 C CNN
F 1 "DD" H 8250 4950 50  0001 C CNN
F 2 "" H 8200 5000 50  0000 C CNN
F 3 "" H 8200 5000 50  0000 C CNN
	1    8200 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D14
U 1 1 58521AE1
P 8650 4950
F 0 "D14" H 8600 5100 50  0000 C CNN
F 1 "DD" H 8700 4900 50  0001 C CNN
F 2 "" H 8650 4950 50  0000 C CNN
F 3 "" H 8650 4950 50  0000 C CNN
	1    8650 4950
	0    1    1    0   
$EndComp
$Comp
L DD D15
U 1 1 58521B40
P 9100 5000
F 0 "D15" H 9050 5150 50  0000 C CNN
F 1 "DD" H 9150 4950 50  0001 C CNN
F 2 "" H 9100 5000 50  0000 C CNN
F 3 "" H 9100 5000 50  0000 C CNN
	1    9100 5000
	1    0    0    -1  
$EndComp
$Comp
L DD D16
U 1 1 58521B9D
P 9550 4950
F 0 "D16" H 9500 5100 50  0000 C CNN
F 1 "DD" H 9600 4900 50  0001 C CNN
F 2 "" H 9550 4950 50  0000 C CNN
F 3 "" H 9550 4950 50  0000 C CNN
	1    9550 4950
	0    1    1    0   
$EndComp
$Comp
L DD D17
U 1 1 58521BF8
P 10200 5000
F 0 "D17" H 10150 5150 50  0000 C CNN
F 1 "DD" H 10250 4950 50  0001 C CNN
F 2 "" H 10200 5000 50  0000 C CNN
F 3 "" H 10200 5000 50  0000 C CNN
	1    10200 5000
	1    0    0    -1  
$EndComp
Text Notes 4400 5400 3    60   ~ 0
Stage 2
Text Notes 5350 5400 3    60   ~ 0
Stage 3
Text Notes 6250 5400 3    60   ~ 0
Stage 4
Text Notes 7150 5400 3    60   ~ 0
Stage 5
Text Notes 8050 5400 3    60   ~ 0
Stage 6
Text Notes 8950 5400 3    60   ~ 0
Stage 7
Text Notes 9650 5300 3    60   ~ 0
Stage 8
$Comp
L DD D18
U 1 1 5852319E
P 10650 4950
F 0 "D18" H 10600 5100 50  0000 C CNN
F 1 "DD" H 10700 4900 50  0001 C CNN
F 2 "" H 10650 4950 50  0000 C CNN
F 3 "" H 10650 4950 50  0000 C CNN
	1    10650 4950
	0    1    1    0   
$EndComp
Text Notes 11050 5150 3    60   ~ 0
Stage 12
$Comp
L C C5
U 1 1 585238FA
P 3950 5200
F 0 "C5" H 3975 5300 50  0000 L CNN
F 1 "C" H 3975 5100 50  0000 L CNN
F 2 "" H 3988 5050 50  0000 C CNN
F 3 "" H 3950 5200 50  0000 C CNN
	1    3950 5200
	0    1    1    0   
$EndComp
$Comp
L C C6
U 1 1 58523957
P 4400 4750
F 0 "C6" H 4425 4850 50  0000 L CNN
F 1 "C" H 4425 4650 50  0000 L CNN
F 2 "" H 4438 4600 50  0000 C CNN
F 3 "" H 4400 4750 50  0000 C CNN
	1    4400 4750
	0    1    1    0   
$EndComp
$Comp
L C C7
U 1 1 585239AA
P 4850 5200
F 0 "C7" H 4875 5300 50  0000 L CNN
F 1 "C" H 4875 5100 50  0000 L CNN
F 2 "" H 4888 5050 50  0000 C CNN
F 3 "" H 4850 5200 50  0000 C CNN
	1    4850 5200
	0    1    1    0   
$EndComp
$Comp
L C C8
U 1 1 585239FD
P 5300 4750
F 0 "C8" H 5325 4850 50  0000 L CNN
F 1 "C" H 5325 4650 50  0000 L CNN
F 2 "" H 5338 4600 50  0000 C CNN
F 3 "" H 5300 4750 50  0000 C CNN
	1    5300 4750
	0    1    1    0   
$EndComp
$Comp
L C C9
U 1 1 58523A52
P 5750 5200
F 0 "C9" H 5775 5300 50  0000 L CNN
F 1 "C" H 5775 5100 50  0000 L CNN
F 2 "" H 5788 5050 50  0000 C CNN
F 3 "" H 5750 5200 50  0000 C CNN
	1    5750 5200
	0    1    1    0   
$EndComp
$Comp
L C C10
U 1 1 58523B17
P 6200 4750
F 0 "C10" H 6225 4850 50  0000 L CNN
F 1 "C" H 6225 4650 50  0000 L CNN
F 2 "" H 6238 4600 50  0000 C CNN
F 3 "" H 6200 4750 50  0000 C CNN
	1    6200 4750
	0    1    1    0   
$EndComp
$Comp
L C C11
U 1 1 58523B70
P 6650 5200
F 0 "C11" H 6675 5300 50  0000 L CNN
F 1 "C" H 6675 5100 50  0000 L CNN
F 2 "" H 6688 5050 50  0000 C CNN
F 3 "" H 6650 5200 50  0000 C CNN
	1    6650 5200
	0    1    1    0   
$EndComp
$Comp
L C C12
U 1 1 58523BCB
P 7100 4750
F 0 "C12" H 7125 4850 50  0000 L CNN
F 1 "C" H 7125 4650 50  0000 L CNN
F 2 "" H 7138 4600 50  0000 C CNN
F 3 "" H 7100 4750 50  0000 C CNN
	1    7100 4750
	0    1    1    0   
$EndComp
$Comp
L C C13
U 1 1 58524367
P 7550 5200
F 0 "C13" H 7575 5300 50  0000 L CNN
F 1 "C" H 7575 5100 50  0000 L CNN
F 2 "" H 7588 5050 50  0000 C CNN
F 3 "" H 7550 5200 50  0000 C CNN
	1    7550 5200
	0    1    1    0   
$EndComp
$Comp
L C C14
U 1 1 585243C6
P 8000 4750
F 0 "C14" H 8025 4850 50  0000 L CNN
F 1 "C" H 8025 4650 50  0000 L CNN
F 2 "" H 8038 4600 50  0000 C CNN
F 3 "" H 8000 4750 50  0000 C CNN
	1    8000 4750
	0    1    1    0   
$EndComp
$Comp
L C C15
U 1 1 58524427
P 8450 5200
F 0 "C15" H 8475 5300 50  0000 L CNN
F 1 "C" H 8475 5100 50  0000 L CNN
F 2 "" H 8488 5050 50  0000 C CNN
F 3 "" H 8450 5200 50  0000 C CNN
	1    8450 5200
	0    1    1    0   
$EndComp
$Comp
L C C16
U 1 1 5852448A
P 8900 4750
F 0 "C16" H 8925 4850 50  0000 L CNN
F 1 "C" H 8925 4650 50  0000 L CNN
F 2 "" H 8938 4600 50  0000 C CNN
F 3 "" H 8900 4750 50  0000 C CNN
	1    8900 4750
	0    1    1    0   
$EndComp
$Comp
L C C17
U 1 1 585244EF
P 9350 5200
F 0 "C17" H 9375 5300 50  0000 L CNN
F 1 "C" H 9375 5100 50  0000 L CNN
F 2 "" H 9388 5050 50  0000 C CNN
F 3 "" H 9350 5200 50  0000 C CNN
	1    9350 5200
	0    1    1    0   
$EndComp
$Comp
L C C18
U 1 1 58524556
P 10100 4750
F 0 "C18" H 10125 4850 50  0000 L CNN
F 1 "C" H 10125 4650 50  0000 L CNN
F 2 "" H 10138 4600 50  0000 C CNN
F 3 "" H 10100 4750 50  0000 C CNN
	1    10100 4750
	0    1    1    0   
$EndComp
$Comp
L C C19
U 1 1 585245BF
P 10450 5200
F 0 "C19" H 10475 5300 50  0000 L CNN
F 1 "C" H 10475 5100 50  0000 L CNN
F 2 "" H 10488 5050 50  0000 C CNN
F 3 "" H 10450 5200 50  0000 C CNN
	1    10450 5200
	0    1    1    0   
$EndComp
Wire Wire Line
	9050 4750 9950 4750
Wire Wire Line
	10250 4750 10450 4750
Wire Wire Line
	9500 5200 10300 5200
Connection ~ 10000 5200
Wire Wire Line
	10600 5200 10900 5200
Connection ~ 9800 5200
Wire Wire Line
	8600 5200 9200 5200
Connection ~ 8900 5200
Wire Wire Line
	7700 5200 8300 5200
Wire Wire Line
	8150 4750 8750 4750
Connection ~ 9350 4750
Wire Notes Line
	9900 4500 9900 5350
Wire Notes Line
	9900 5350 10900 5350
Wire Notes Line
	10900 5350 10900 4500
Wire Notes Line
	10900 4500 9900 4500
Text Notes 10000 4450 0    60   ~ 0
Repeat for stages\n9, 10, 11, 12
Wire Wire Line
	10900 5200 10900 6300
Text Notes 11050 5750 3    60   ~ 0
To top electrode
Wire Wire Line
	9800 5200 9800 6250
Text Notes 9950 5600 3    60   ~ 0
To middle electrode
Wire Wire Line
	7250 4750 7850 4750
Connection ~ 8450 4750
Connection ~ 8000 5200
Wire Wire Line
	6800 5200 7400 5200
Connection ~ 7550 4750
Wire Wire Line
	6350 4750 6950 4750
Connection ~ 7100 5200
Wire Wire Line
	5900 5200 6500 5200
Connection ~ 6650 4750
Wire Wire Line
	5450 4750 6050 4750
Connection ~ 6200 5200
Wire Wire Line
	5000 5200 5600 5200
Connection ~ 5300 5200
Wire Wire Line
	4100 5200 4700 5200
Wire Wire Line
	4550 4750 5150 4750
Connection ~ 5750 4750
Connection ~ 4850 4750
Wire Wire Line
	3650 4750 4250 4750
Connection ~ 4400 5200
Wire Wire Line
	3200 5200 3800 5200
Wire Wire Line
	2600 5200 2900 5200
Connection ~ 2600 5200
Connection ~ 3500 5200
Wire Wire Line
	2800 4750 3350 4750
Connection ~ 3950 4750
Connection ~ 3050 4750
Text Notes 7350 7500 0    60   Italic 12
 Cockcroft-Walton Assembly
$EndSCHEMATC
