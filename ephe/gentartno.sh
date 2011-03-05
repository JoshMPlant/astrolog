#!/bin/sh
cd ../exteph
for i in `../ephe/alltno.sh`
# fwd/part2.sh`
do
if test $i -lt 10000
then
  outname=0$i
else 
  outname=$i
fi
tar rvf ../ephe/sxfiles.tar sx$outname.sx1
done
cd ../ephe
gzip sxfiles.tar
mv sxfiles.tar.gz sxfiles.tgz
