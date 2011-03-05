#!/bin/bash

#if test $1 -lt 10000
#then
# outname=0$1
#else
# outname=$1
#fi
#if test $1 -lt 1000
#then
# outname=00$1
#fi
#if test $1 -lt 100
#then
# outname=000$1
#fi
#if test $1 -lt 10
#then
# outname=0000$1
#fi
rm ../exteph/newast.fle
count=0
for obj in \
2004NN8 2005SB223 2006LM1 2005TJ50 2005VD \
2006RG1 2006BZ8 1997MD10 1998WU24 1999XS35 \
2000KP65 2006EX52 1999LE31 2003UY283 2003WN188 1998QJ1 \
2000DG8 1998HO121 2004CM111 2000AB229 2000HE46 \
65407 2001QF6 2002CE10 2001YK61 2003CC22 2006RJ2  

do
if test $obj = 65407
then
sed "s/%NAME%/$obj/g" fitobs.inp.riyalfleast > fitobs.tmp
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
if ( test $count -eq 26 
#|| test $count -eq 19 || test $count -eq 20 || test $count -eq 21 
#|| test $count -eq 0 || test $count -eq 2 || test $count -eq 5 \
#|| test $count -eq 7 \
#|| test $count -eq 9 \
#|| test $count -eq 10 \
#|| test $count -eq 11 \
#|| test $count -eq 13 \
#|| test $count -eq 14 \
#|| test $count -eq 18 \
 )
then
../src/aastrolog.exe -ZC .tmpfile ../exteph/fle$obj.sx1 $step -100000000
else
../src/aastrolog.exe -ZC .tmpfile ../exteph/fle$obj.sx1 $step -10000000 
fi
fi
cat ../exteph/fle$obj.sx1 >> ../exteph/newast.fle
rm -f $obj.fop $obj.fga $obj.clo $obj.pro $obj.moid $obj.ele $obj.fou
count=`expr "$count" + 1`
done
