#!/bin/sh

#if test -d "$1"
#then
#echo "Doing directory $1 part $2" 
cd ast1/fwd
#if test -x "part$2.sh"
#then
../../alltno.sh > orb9.sec2
#fi
cat orb9.sec? > orb9.opt
echo "Starting integration forward in time"
orbit9.x
echo "Converting results"
echo 0 | conv9.x

cd ../rev
#if test -x "part$2.sh"
#then
../../alltno.sh > orb9.sec2
#fi
cat orb9.sec? > orb9.opt
echo "Starting integration backward in time"
orbit9.x
echo "Converting results"
echo 0 | conv9.x
cd ..
#else
#echo "$0 astxx [1,2]"
#echo "does subdirectory part 1 or 2"
#fi
