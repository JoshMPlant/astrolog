#!/bin/sh
rm .tmpfile

#astdir="ast`expr $1 / 1000`"
astdir=ast1
years=`tail -1 $astdir/rev/v$1.eclr | cut -f1 -d.`
tenstep=`head -11 $astdir/fwd/v$1.eclf | tail -1 | cut -f1 -d.`
if test $tenstep -eq 2
then
 step=73
fi
if test $tenstep -eq 5
then
 step=183
fi
if test $tenstep -eq 1
then
 step=37
fi
if test $tenstep -eq 4
then
 step=146
fi
if test $1 -lt 10000
then
 outname=0$1
else
 outname=$1
fi
echo "Initial Condition of integration MJD 55600, pls check"
start=`expr 2455600 + 36525 \* $years / 100`
echo -n $start >> .tmpfile
sort -k1 -n $astdir/rev/v$1.eclr | sed "s/-/ -/g;$d" | sed "1s/^ *[-\.0-9]* / /g;$d"  >> .tmpfile
sort -k1 -n $astdir/fwd/v$1.eclf | sed "1d;s/-/ -/g;" >> .tmpfile

#| sed "1s/^..[-\.0-9]* //g;$d"  >> .tmpfile

aastrolog.exe -YTI -ZC .tmpfile ../exteph/sx$outname.sx1 $step 
