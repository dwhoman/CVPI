#!/bin/dash
# -*- eval: (sh-set-shell "posix") -*-

# cvpi_test.sh test_list

SCRIPT_DIR=$(dirname $0)
#TEST_DIR directory for the current test; a sub-directory of SCRIPT_DIR

TRAP_SIGS="EXIT INT TERM"
# default shell is sh, allow the user to specify another
TEST_SHELL="/bin/sh"

TEST_PASS=0
TEST_FAIL=0
TEST_SKIP=0

TEMP_FILE_LIST=
REMOVE_TEMPS_P=0

#TIMED if set tests will be timed using the time functionn
#DATED if set and TIMED is not set, tests will be timed using the date function

skip_test() {
    local test_name=$1
    TEST_SKIP=$(($TEST_SKIP + 1))
    echo "SKIPPED:" $test_name > /dev/stderr
}

run_test() {
    local result
    local time_elapsed
    local timer_start
    local timer_end

    if [ ${#DATED} -ne 0 ]
    then
	timer_start=`date '+%s%N'`
    fi

    if [ ${#TIMED} -ne 0 ]
    then
	case $TEST_SHELL in
	    *busybox*ash)
		time_elapsed=$($TEST_SHELL -c "time -p ash -c \"$@ > /dev/null; res=\\\$?; exit \\\$res\" 2>&1 >/dev/null")
		;;
	    *)
		time_elapsed=$($TEST_SHELL -c "(time $TEST_SHELL -c \"$@ > /dev/null; res=\\\$?; exit \\\$res\") 2>&1 >/dev/null | sed ':n;N;s/\\n/ /; $!{b n}'")
		result=$?
		;;
	esac
    else
	# $TEST_SHELL
	$@ > /dev/null
	result=$?
    fi

    if [ ${#DATED} -ne 0 ]
    then
	timer_end=`date '+%s%N'`
	time_elapsed=$((timer_end - timer_start))
    fi

    if [ 0 -eq $result ]
    then
	echo "PASSED:" $@
	TEST_PASS=$(($TEST_PASS + 1))
    else
	echo "FAILED:" $@
	TEST_FAIL=$(($TEST_FAIL + 1))
    fi

    if [ ${#TIMED} -ne 0 ]
    then
	echo " -- " $time_elapsed
    elif [ ${#DATED} -ne 0 ]
    then
	printf " -- %fs\n" $(printf "%09d" $time_elapsed | sed 's/\([[:digit:]]*\)\([[:digit:]]\{9\}\)/\1.\2/')
    fi
}

remove_temps() {
    trap "" $TRAP_SIGS
    local file
    if [ $REMOVE_TEMPS_P -eq 1 ]
    then
	for file in $TEMP_FILE_LIST
	do
	    if [ -e $file ] && [ -f $file ]
	    then
		rm -f $file
	    fi
	done
    fi
}

add_temp() {
    if [ ${#TEMP_FILE_LIST} -gt 0 ]
    then
	TEMP_FILE_LIST="$TEMP_FILE_LIST $1"
    else
	TEMP_FILE_LIST="$1"
    fi
}

temp_name() {
    printf "%s.XXXXXX" $(echo "$@" | sed 's|\([^ ./]*\)[ ./]*\([^ ./]*\)|\1_\2|g')
}

bin2channels_channels2bin() {
    if [ $# -lt 4 ]
    then
	echo "bin2channels_channels2bin $@: too few arguments, $#, requires 4" > /dev/stderr
	return 1
    fi

    local image_width=$1
    local height=$2
    local pixel_width=$3
    local input_image=$4

    local success=0

    local channels
    local output_image
    local temp_file

    trap "remove_temps" $TRAP_SIGS

    # create output files
    local file=1
    while [ $file -le $pixel_width ]; do
	temp_file=$(mktemp -p ${TEST_DIR}/output $(temp_name $(echo $@) 'channel'))
	channels="${channels} $temp_file "
	add_temp $temp_file
	file=$(( $file + 1 ))
    done
    output_image=$(mktemp -p $TEST_DIR"/output" $(temp_name $(echo $@) "image"))
    add_temp $output_image

    # run
    $TEST_SHELL $SCRIPT_DIR/bin2channels -g $pixel_width $image_width $input_image $channels
    success=$?
    if [ $success -gt 0 ]
    then
	echo "bin2channels failed: "$success > /dev/stderr
	return $success
    fi

    # test that channel images where created
    for file in $channels; do
	if ! { [ -e $file ] && { [ -f $file ] && [ -r $file ]; }; }
	then
	    printf "%s is not a readable file.\n" $file; > /dev/stderr
	    return 1
	fi
    done

    # echo "testing channels2bin" > /dev/stderr
    
    # test back conversion
    $TEST_SHELL $SCRIPT_DIR/channels2bin -c $output_image ${channels}
    success=$?
    if [ $success -gt 0 ]
    then
	echo "channels2bin failed: "$success > /dev/stderr
	return $success
    fi

    if ! { [ -e $output_image ] && { [ -f $output_image ] && [ -r $output_image ]; }; }
    then
	printf "%s is not a readable file.\n" $output_image; > /dev/stderr
	return 1
    fi

    cmp -s $output_image $input_image
    success=$?
    if [ $success -eq 1 ]
    then
	echo "input and output bin images differ" > /dev/stderr
    elif [ $success -eq 2 ]
    then
	echo "cmp erred" > /dev/stderr
    fi

    return $success
}

while getopts :s:tf:c OPT; do
    case $OPT in
	c|+c)
	    REMOVE_TEMPS_P=1
	    ;;
	s|+s)
	    TEST_SHELL="$OPTARG"
	    ;;
	t|+t)
	    test_for_time=$(which time 2> /dev/null > /dev/null)
	    has_time=$?

	    test_for_date=$(which date 2> /dev/null > /dev/null)
	    has_date=$?

	    if [ $has_time -eq 0 ]
	    then
		#TIMED=1
		DATED=1
	    elif [ $has_date -eq 0 ]
	    then
		DATED=1
	    else
		echo "time and date functions are unavailable, not timing.\n" > /dev/stderr
	    fi

	    ;;
	f|+f)
	    TEST_FILE="$OPTARG"
	    if ! { [ -e $TEST_FILE ] && { [ -f $TEST_FILE ] && [ -r $TEST_FILE ]; }; }
	    then
		printf "%s is not a readable file.\n" $TEST_FILE; > /dev/stderr
		return 1
	    else
		FILE_TEST=1
	    fi
	    ;;
	*)
	    echo "usage: ${0##*/} [+-f ARG] [+-s ARG] [+-t} [--] ARGS..." > /dev/stderr
	    exit 2
    esac
done

shift $(( OPTIND - 1 ))
OPTIND=1

choose_test() {
    local test_name=$1
    case $test_name in
	# bin2channels and channels2bin undo eachother, so testing
	# input == output, as well as correct intermediary
	bin2channels_channels2bin*)
	    TEST_DIR="${SCRIPT_DIR}/bin2channels_channels2bin"
	    case $test_name in
		*_1x4x2)
		    run_test bin2channels_channels2bin 1 4 2 "${TEST_DIR}/input/data_8"
		    ;;
		*_4x1x2)
		    run_test bin2channels_channels2bin 4 1 2 "${TEST_DIR}/input/data_8"
		    ;;
		*_2x4x1)
		    run_test bin2channels_channels2bin 2 4 1 "${TEST_DIR}/input/data_8"
		    ;;
		*_4x2x1)
		    run_test bin2channels_channels2bin 4 2 1 "${TEST_DIR}/input/data_8"
		    ;;
		*_2x1x4)
		    run_test bin2channels_channels2bin 2 1 4 "${TEST_DIR}/input/data_8"
		    ;;
		*_1x2x4)
		    run_test bin2channels_channels2bin 1 2 4 "${TEST_DIR}/input/data_8"
		    ;;
#		*_image_data)
#		    run_test bin2channels_channels2bin 1280 960 4 "${TEST_DIR}/input/image.yuva"
#		    ;;
		*)
		    skip_test $test_name
		    ;;
	    esac
	;;
	*)
	    skip_test $test_name
	;;
    esac    
}

# || part necessary to get the last line read
# see http://stackoverflow.com/questions/4165135/how-to-use-while-read-bash-to-read-the-last-line-in-a-file-if-there-s-no-new
if [ ${#FILE_TEST} -ne 0 ]
then
    while IFS='' read -r fun || [ -n "$fun" ]
    do
	choose_test $fun
    done < $TEST_FILE
fi
if [ $# -gt 0 ]
then
    for fun in $@; do
	choose_test $fun
    done
fi
if readlink /proc/$$/fd/0 | grep -q "^pipe:"
then
    while IFS='' read -r fun || [ -n "$fun" ]
    do
	choose_test $fun
    done
fi

printf "Tests: PASSED: %s FAILED: %s SKIPPED: %s\n" $TEST_PASS $TEST_FAIL $TEST_SKIP
