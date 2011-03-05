#!/bin/sh

if test -d "$1"
then
mjd=$1
else
mjd=`egrep "^.20000 '" astorb.cat |cut -f3 -d"'"|cut -f1 -d.`
fi
sed "2s/%MJD%/$mjd/g" inbaricsun.opt.in > inbaricsun.opt 
inbarsun.x

