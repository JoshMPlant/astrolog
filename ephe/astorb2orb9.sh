#!/bin/sh
# Edit epoch MJD manually 
cat << EOH
format  = 'OEF1.1'       ! file format
rectype = '1L'           ! record type (1L/ML)
elem    = 'KEP'          ! type of orbital elements
refsys  = ECLM J2000     ! default reference system
END_OF_HEADER
! Name, Epoch(MJD), a, e, i, long. node, arg. peric., mean anomaly
EOH

awk ' {
print substr($0,116,11),
substr($0,127,10),
substr($0,138,10),
substr($0,148,10),
substr($0,158,11),
substr($0,169,13),
substr($0,107,8), 
substr($0,44,5),
substr($0,49,5),
substr($0,1,7), 
gensub(" ","",1,substr($0,8,18))
}' | awk ' { printf("%s%-6s%s   %16s  %-14s %-14s %-12s %-11s %-12s %-12s %-5s %-5s \n", "'\''",$10,"'\''","55500.000000",$6,$5,$4,$3,$2,$1,$8,$9)}' 
