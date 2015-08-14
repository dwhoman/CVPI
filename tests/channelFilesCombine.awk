#!/bin/awk -f

# channelFilesCombine.awk [ -v default_aval=""] [-v g_file="<file_name>" [-v b_file="<file_name>" [-v a_file="<file_name>"]]] r_file_name
BEGIN {
    FS = " "
    
    g_file_p = length(g_file)
    b_file_p = length(b_file)
    a_file_p = length(a_file)

    if(length(default_aval) == 0) {
	default_aval = "255"
    }
}

function array_initialize(alen, aval) {
    for(x = 1; x <= alen; x++) {
	arr[x] = aval
    }
    return arr
}

/^\s*$/ {
    if(g_file_p) {
	"cat " g_file | getline green_line
	if(length(sub(/^\s*$/,"",green_line)) > 0) {
	    print("Error: " r_file_name " and " g_file " are different lengths.\n")
	    exit 1
	}
    }
    
    if(b_file_p) {
	"cat " b_file | getline blue_line
	if(length(sub(/^\s*$/,"",blue_line)) > 0) {
	    print("Error: " r_file_name " and " b_file " are different lengths.\n")
	    exit 1
	}
    }

    if(a_file_p) {
	"cat " a_file | getline alpha_line
	if(length(sub(/^\s*$/,"",alpha_line)) > 0) {
	    print("Error: " r_file_name " and " a_file " are different lengths.\n")
	    exit 1
	}
    }
}

{    
    if(length(red_length) == 0) {
	red_length = NF
    } else {
	if(red_length != NF) {
	    print "Error: Red file, inconsistent number of columns\n"
	    exit 1
	}
    }

    if(g_file_p) {
	gl = ("cat " g_file | getline green_line)
	#print("green_file: " g_file "\n")
	#print("green_ln: " gl "\n")
	#print("green_line: " green_line "\n")
	green_length = split(green_line, green_array, " ")
    } else {
	if(length(green_length) == 0) {
	    green_array = array_initialize(red_length, default_aval)
	    green_length = red_length
	}
    }
    
    if(b_file_p) {
	"cat " b_file | getline blue_line
	blue_length = split(blue_line, blue_array, " ")
    } else {
	if(length(blue_length) == 0) {
	    blue_array = array_initialize(red_length, default_aval)
	    blue_length = red_length
	}
    }
    if(a_file_p) {
	"cat " a_file | getline alpha_line
	alpha_length = split(alpha_line, alpha_array, " ")
    } else {
	if(length(alpha_length) == 0) {
	    alpha_array = array_initialize(red_length, default_aval)
	    alpha_length = red_length
	}
    }
    
    if(red_length != green_length || green_length != blue_length || blue_length != alpha_length) {
	print("Error: File data are different sizes. " red_length " " green_length " " blue_length " " alpha_length "\n")
	exit 1
    }
    
    for (i = 1; i <= red_length ; i++) {
	printf("%d ", $i)
	printf("%d ", green_array[i])
	printf("%d ", blue_array[i])
	printf("%d ", alpha_array[i])
    }

}
