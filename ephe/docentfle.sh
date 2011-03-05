#!/bin/bash

rm ../exteph/newcent.fle
for obj in 1998XY95 1999RU214 1999RZ214 1999RJ215 2000CQ105 1994TA 2000PE30 \
2000SR331 49036 2000YY1 52975 2001FM194 2001KG76 \
 2001KG77 54598 121725 55576 83982 44594 63252 87555 2000FZ53 \
2001OM109 2001QX322 55565 55636 55637 2003LA7 50000 2001XQ254 2002GA32 \
52872 60558 2000GM137 2000CO104 73480 119315 2002DH5 120061 95626 \
2001XZ255 148975 88269 1999HD12 \
1999JV127 1995SN55 1996RX33 1996AR20 1996AS20 2002CA249 250112 2002PQ152 \
2002FY36 2002TK301 2002VG131 119976 2002QX47 2000SN331


do
#if test $obj -eq 250112
#then
sed "s/%NAME%/$obj/g" fitobs.inp.riyalfle > fitobs.tmp
sed "s/%NAME%/$obj/g" input.fop > $obj.fop
fitobs.x < fitobs.tmp
sed "1,+5d;s/'$obj'//g" $obj.ele > .tmpfile2
#astdir="ast`expr $1 / 1000`"
days=`head -1 .tmpfile2 | cut -f1 -d.`
day2=`head -2 .tmpfile2 | tail -1 | cut -f1 -d.`
start=`expr 2400000 + $days`
start2=`expr 2400000 + $day2`
step=`expr "$start2" - "$start"`
echo "Start at JD $start stepsize $step , pls check"
sed "1s/$days.0/$start.5/g" .tmpfile2 > .tmpfile
aastrolog -ZC .tmpfile ../exteph/fle$obj.sx1 $step 0
#fi
cat ../exteph/fle$obj.sx1 >> ../exteph/newcent.fle
rm -f $obj.fop $obj.fga $obj.clo $obj.pro $obj.moid $obj.ele $obj.fou
done
