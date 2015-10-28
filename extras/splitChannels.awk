#!/bin/awk -f

# splitChannels -v start="<number>" jump="<number> file

# if there are n columns and the user wants the n+m, m>0 column
BEGIN {
    if(length(start) == 0 || start < 1) {
	print("start variable must be specified and be greater than 0.\n") > "/dev/stderr"
	exit 1
    }
    
    if(length(jump) == 0 || jump < 1) {
	print("jump variable must be specified and be greater than 0.\n") > "/dev/stderr"
	exit 1
    }
}

{
    if(start <= NF) {
	filtered = $start
	for(i = start + jump; i <= NF; i += jump) {
	    filtered = filtered " " $i
	}
	print filtered
    }
}
