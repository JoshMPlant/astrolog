#!/bin/bash

rm ../exteph/newtno.fle
for obj in 91554  1999CF119 1999CY118 1999CZ118 2000PF30 54520 \
118702  2008SJ236 2008QD4 181902 60608 2007VK305 2007UM126 24835 \
47171 148209 87269 2007RH283 2007RW10 2005VJ119 48639 \
2000PH30 2005RO43 82155 82158 119951 2002MS4 208996 \
84522  65489 2004PG115 2004TF282 2003YQ179 
do
sed "s/%NAME%/$obj/g" fitobs.inp.riyalfle > fitobs.tmp
sed "s/%NAME%/$obj/g" input.fop > $obj.fop
fitobs.x < fitobs.tmp
sed "1,+5d;s/'$obj'//g" $obj.ele > .tmpfile2
days=`head -1 .tmpfile2 | cut -f1 -d.`
day2=`head -2 .tmpfile2 | tail -1 | cut -f1 -d.`
start=`expr 2400000 + $days`
start2=`expr 2400000 + $day2`
step=`expr "$start2" - "$start"`
echo "Start at JD $start stepsize $step , pls check"
sed "1s/$days.0/$start.5/g" .tmpfile2 > .tmpfile
aastrolog -ZC .tmpfile ../exteph/fle$obj.sx1 $step 0
cat ../exteph/fle$obj.sx1 >> ../exteph/newtno.fle
rm -f $obj.fop $obj.fga $obj.clo $obj.pro $obj.moid $obj.ele $obj.fou
done
