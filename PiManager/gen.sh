#!/bin/bash

# Generates a makefile for the PiManagerApp
#Code Borrowd and modified from josh rahms original generator
obs=()
src=()
libs=()

BINARY_NAME="Manager"

# Iterate through and find the
# c++ source files 
for i in $(find /home/nbroeking/Documents/Framework/classes/ | egrep '.*\.c(pp|xx)?$') ; do
	# add this file to the list of
	# sources
	deps="$( echo $i | sed 's/^src\/*//')"

	# add the resulting object file to
	# the objects
	src[$cnt]=$deps
	obs+=("obs/`basename $i | sed -r 's/\.(cpp|cxx)/.o/g'`")
	cnt=$[cnt + 1]
done

# remove the Makefile if it exists
rm -f raspberrypi.mk || true

# open Makefile
exec 3<> raspberrypi.mk


# some commonly used files to generate
echo "CC=$CC">&3
echo "CFLAGS=$CFLAGS">&3
echo "LDFLAGS=$LDFLAGS">&3
echo 'OBJECTS='${obs[@]}>&3
echo 'BINARY='$BINARY_NAME>&3
echo '.PHONY: all clean\n'>&3

echo -e \
'all: obs/main.o $(OBJECTS)
	$(CC) $(OBJECTS) $< $(LDFLAGS) -o $(BINARY) \n'>&3

echo -e \
'obs/main.o: main.cpp
	$(CC) $< $(CFLAGS) -c -o $@\n'>&3


# iterate through all of the objects and 
# add a rule for the binary
for ((i=0;i<${#obs[@]};i++)) ; do
    echo "Object file: ${obs[$i]}"
	echo -e "${obs[$i]}: ${src[$i]}\n\t"'$(CC) $< -c $(CFLAGS) -o $@\n'>&3
done

# close Makefile
exec 3>&-
