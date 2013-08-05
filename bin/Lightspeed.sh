#!/bin/bash
# Shell script to run Lightspee

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin/
./bin/Lightspeed $@
