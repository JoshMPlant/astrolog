#!/bin/sh

mkdir $1
mkdir $1/fwd
mkdir $1/rev
cp -p ast1/fwd/filter.100 $1/fwd
cp -p ast1/rev/filter.100 $1/rev
cp -p ast1/fwd/orb9.sec? $1/fwd
cp -p ast1/rev/orb9.sec? $1/rev
cp -p ast1/fwd/part?.sh $1/fwd
cp -p ast1/rev/part?.sh $1/rev
cp -p ast1/fwd/conv9.inp $1/fwd
cp -p ast1/rev/conv9.inp $1/rev
