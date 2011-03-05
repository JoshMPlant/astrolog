#!/bin/sh


mjd=`egrep "^.20000 '" astorb.cat |cut -f3 -d"'"|cut -f1 -d.`
sed "2s/%MJD%/$mjd/g" inbaricsun.opt.in > inbaricsun.opt 
inbarsun.x

