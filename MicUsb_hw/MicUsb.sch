EESchema Schematic File Version 2
LIBS:Connectors_kl
LIBS:pcb_details
LIBS:power
LIBS:Power_kl
LIBS:Tittar_kl
LIBS:st_kl
LIBS:Transistors_kl
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
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
L STM32L151CxTx DD?
U 1 1 56451687
P 7700 3300
F 0 "DD?" H 7200 4650 60  0000 C CNN
F 1 "STM32L151CxTx" H 7850 4650 60  0000 C CNN
F 2 "" H 7700 3300 60  0000 C CNN
F 3 "" H 7700 3300 60  0000 C CNN
	1    7700 3300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 564518B6
P 6200 4850
F 0 "C?" H 6100 4750 50  0000 L CNN
F 1 "C" H 6100 4950 50  0000 L CNN
F 2 "CAP_0603" V 6300 4700 28  0000 C BNN
F 3 "" H 6200 4850 60  0000 C CNN
	1    6200 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5645190F
P 5900 4850
F 0 "C?" H 5800 4750 50  0000 L CNN
F 1 "C" H 5800 4950 50  0000 L CNN
F 2 "CAP_0603" V 6000 4700 28  0000 C BNN
F 3 "" H 5900 4850 60  0000 C CNN
	1    5900 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4450 6400 4450
Wire Wire Line
	5900 4450 5900 4650
Wire Wire Line
	6200 4300 6200 4650
Connection ~ 6200 4450
Wire Wire Line
	6400 4300 6200 4300
$Comp
L +3.3V #PWR?
U 1 1 5645196E
P 5750 4450
F 0 "#PWR?" H 5750 4410 30  0001 C CNN
F 1 "+3.3V" H 5830 4480 30  0000 C CNN
F 2 "" H 5750 4450 60  0000 C CNN
F 3 "" H 5750 4450 60  0000 C CNN
	1    5750 4450
	-1   0    0    1   
$EndComp
Connection ~ 5900 4450
$Comp
L GND #PWR?
U 1 1 56451993
P 6200 5300
F 0 "#PWR?" H 6290 5280 30  0001 C CNN
F 1 "GND" H 6200 5220 30  0001 C CNN
F 2 "" H 6200 5300 60  0000 C CNN
F 3 "" H 6200 5300 60  0000 C CNN
	1    6200 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 564519AD
P 5900 5300
F 0 "#PWR?" H 5990 5280 30  0001 C CNN
F 1 "GND" H 5900 5220 30  0001 C CNN
F 2 "" H 5900 5300 60  0000 C CNN
F 3 "" H 5900 5300 60  0000 C CNN
	1    5900 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 5050 6200 5300
Wire Wire Line
	5900 5050 5900 5300
$Comp
L C C?
U 1 1 564519DE
P 5450 4850
F 0 "C?" H 5350 4750 50  0000 L CNN
F 1 "C" H 5350 4950 50  0000 L CNN
F 2 "CAP_0603" V 5550 4700 28  0000 C BNN
F 3 "" H 5450 4850 60  0000 C CNN
	1    5450 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 56451A85
P 5150 4850
F 0 "C?" H 5050 4750 50  0000 L CNN
F 1 "C" H 5050 4950 50  0000 L CNN
F 2 "CAP_0603" V 5250 4700 28  0000 C BNN
F 3 "" H 5150 4850 60  0000 C CNN
	1    5150 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 56451AA4
P 4850 4850
F 0 "C?" H 4750 4750 50  0000 L CNN
F 1 "C" H 4750 4950 50  0000 L CNN
F 2 "CAP_0603" V 4950 4700 28  0000 C BNN
F 3 "" H 4850 4850 60  0000 C CNN
	1    4850 4850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 56451AE9
P 4550 4850
F 0 "C?" H 4450 4750 50  0000 L CNN
F 1 "C" H 4450 4950 50  0000 L CNN
F 2 "CAP_0603" V 4650 4700 28  0000 C BNN
F 3 "" H 4550 4850 60  0000 C CNN
	1    4550 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4150 6400 4150
Wire Wire Line
	5450 4150 5450 4650
Wire Wire Line
	5100 4050 6400 4050
Wire Wire Line
	5150 4050 5150 4650
Wire Wire Line
	4450 3950 6400 3950
Wire Wire Line
	4850 3950 4850 4650
Wire Wire Line
	4550 3950 4550 4650
Connection ~ 4850 3950
$Comp
L +3.3V #PWR?
U 1 1 56451B5F
P 5400 4150
F 0 "#PWR?" H 5400 4110 30  0001 C CNN
F 1 "+3.3V" H 5480 4180 30  0000 C CNN
F 2 "" H 5400 4150 60  0000 C CNN
F 3 "" H 5400 4150 60  0000 C CNN
	1    5400 4150
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 56451B81
P 5100 4050
F 0 "#PWR?" H 5100 4010 30  0001 C CNN
F 1 "+3.3V" H 5180 4080 30  0000 C CNN
F 2 "" H 5100 4050 60  0000 C CNN
F 3 "" H 5100 4050 60  0000 C CNN
	1    5100 4050
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 56451BA3
P 4450 3950
F 0 "#PWR?" H 4450 3910 30  0001 C CNN
F 1 "+3.3V" H 4530 3980 30  0000 C CNN
F 2 "" H 4450 3950 60  0000 C CNN
F 3 "" H 4450 3950 60  0000 C CNN
	1    4450 3950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 56451BC5
P 5450 5300
F 0 "#PWR?" H 5540 5280 30  0001 C CNN
F 1 "GND" H 5450 5220 30  0001 C CNN
F 2 "" H 5450 5300 60  0000 C CNN
F 3 "" H 5450 5300 60  0000 C CNN
	1    5450 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56451BE7
P 5150 5300
F 0 "#PWR?" H 5240 5280 30  0001 C CNN
F 1 "GND" H 5150 5220 30  0001 C CNN
F 2 "" H 5150 5300 60  0000 C CNN
F 3 "" H 5150 5300 60  0000 C CNN
	1    5150 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56451C09
P 4850 5300
F 0 "#PWR?" H 4940 5280 30  0001 C CNN
F 1 "GND" H 4850 5220 30  0001 C CNN
F 2 "" H 4850 5300 60  0000 C CNN
F 3 "" H 4850 5300 60  0000 C CNN
	1    4850 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56451C2B
P 4550 5300
F 0 "#PWR?" H 4640 5280 30  0001 C CNN
F 1 "GND" H 4550 5220 30  0001 C CNN
F 2 "" H 4550 5300 60  0000 C CNN
F 3 "" H 4550 5300 60  0000 C CNN
	1    4550 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5050 5450 5300
Wire Wire Line
	5150 5050 5150 5300
Wire Wire Line
	4850 5050 4850 5300
Wire Wire Line
	4550 5050 4550 5300
Connection ~ 5450 4150
Connection ~ 5150 4050
Connection ~ 4550 3950
$Comp
L GND #PWR?
U 1 1 56451DB6
P 7650 5300
F 0 "#PWR?" H 7740 5280 30  0001 C CNN
F 1 "GND" H 7650 5220 30  0001 C CNN
F 2 "" H 7650 5300 60  0000 C CNN
F 3 "" H 7650 5300 60  0000 C CNN
	1    7650 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 4750 7450 4900
Wire Wire Line
	7450 4900 7900 4900
Wire Wire Line
	7900 4900 7900 4750
Wire Wire Line
	7650 4750 7650 5300
Connection ~ 7650 4900
Wire Wire Line
	7550 4750 7550 4900
Connection ~ 7550 4900
Wire Wire Line
	7750 4750 7750 4900
Connection ~ 7750 4900
$EndSCHEMATC
