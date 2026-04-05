#!/bin/bash


awk '{l=length($0); for(i=1; i<=length($0); i++) { if(substr($0,i,1)=="1"){a[i]++}}} END {for(i=1; i<=l; i++) { if(a[i]>NR/2) {g+=(2^(l-i))} else {e+=(2^(l-i))} }; print g*e}' $1