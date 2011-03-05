#!/bin/sh

outname=$1
sed "s/%NAME%/$1/g" fitobs.inp > fitobs.tmp
sed "s/%NAME%/$1/g" input.fop > $1.fop
fitobs.x < fitobs.tmp
sed "1,+5d;s/'$1'//g" $1.ele > .tmpfile2
days=`head -1 .tmpfile2 | cut -f1 -d.`
day2=`head -2 .tmpfile2 | tail -1 | cut -f1 -d.`
start=`expr 2400000 + $days`
start2=`expr 2400000 + $day2`
step=`expr $start2 - $start`
echo "Start at JD $start stepsize $step , pls check"
sed "1s/$days.0/$start.5/g" .tmpfile2 > .tmpfile
aastrolog -YTI -ZC .tmpfile ../exteph/sx$outname.sx1 $step 
rm -f $1.fop $1.fga $1.clo $1.pro $1.moid $1.ele $1.fou
