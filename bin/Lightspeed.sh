#!/bin/bash
# Shell script to run Lightspeed

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin/
./bin/Lightspeed $@
