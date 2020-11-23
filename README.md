# Open-SSL-for-Raspberry-Pi
This contains the files I am using in a raspberry Pi4 program to use an API on a web server.   
This program scrapes the HTML response from an IoT device and then sends the results 
to the database on the web server.

To compile this program, it is necessary to link a couple libraries "-lcrypto and -lssl"
Here is a working command line to compile this program below:
gcc -Wall main.c getString.c iot_access.c -o updateWebServer -lssl -lcrypto


