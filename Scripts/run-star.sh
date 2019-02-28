#!/bin/sh
echo "Running STAR..."
./Scripts/setup-online.sh 2 512 128
./Scripts/run-online.sh star_program 512 128
