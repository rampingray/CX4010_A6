Author          : Burke, Jackson & Mu, Xiaotong
Created         : November 11, 2019
Last Modified   : November 11, 2019
Affiliation     : Georgia Institute of Technology

Description
-------------
This program takes in 3 arguments:
 - Runtime (e.g. 240 minutes)
 - A filename of a specifically formatted input configuration file (5 are given here)
 - The desired filename of the output (e.g. "output1.txt")

The program runs a simluation of the given system given by the configuration file until the runtime is over.
The output file contains the average queue time for each queue given by the config file, the 
average, min, and max queue time for each customer who has exited the system, and the average, min, and max
total system time for each customer who has exited the system.


Installation
------------
To install, run:
$ gcc -std=c99 main.c engine.c Random_Generator.c import.c FIFO_Queue.c Priority_Queue.c -o runsim -lm

Note: The -std=c99 flag is required on some C compilers
to allow variable declarations at arbitrary locations in a function.

Execution
-----------
Assuming your executable is called "runsim", run it using:
$ runsim 240 config1.txt output1.txt
 - Where 240 is the runtime for the simulation
 - Where config1.txt is the desired name of the config file (NOTE: Must be formatted properly to minimize errors)
 - Where output1.txt is the desired name of the output file