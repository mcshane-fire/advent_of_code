awk '{if(NR>1 && $0>last){n++}; last=$0} END {print n}' $1

awk '{if(NR>3 && a[0]+a[1]+a[2]>last){n++}; last=a[0]+a[1]+a[2]; a[NR%3]=$0} END {print n}' $1