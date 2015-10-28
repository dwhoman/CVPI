#!/bin/sh
# -*- eval: (sh-set-shell "posix") -*-

# cvpi_test.sh test_list

SCRIPT_DIR=$(dirname $0)

extras=$(readlink -f ../extras/)

PATH="$PATH:$extras"

#TEST_DIR directory for the current test; a sub-directory of SCRIPT_DIR

. "$SCRIPT_DIR/cvpi_tests_common"
# source

TEST_PASS=0
TEST_FAIL=0
TEST_SKIP=0

REMOVE_TEMPS_P=0

#DATED if set, tests will be timed using the date function

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

    sh $@ > /dev/null
    result=$?


    if [ ${#DATED} -ne 0 ]
    then
	timer_end=`date '+%s%N'`
	time_elapsed=$((timer_end - timer_start))
    fi

    if [ 0 -eq $result ]
    then
	echo "PASSED:" $@
	TEST_PASS=$(($TEST_PASS + 1))
    elif [ 1 -eq $result ]
    then
	echo "SKIPPED:" $@
	TEST_SKIP=$(($TEST_SKIP + 1))
    else
	echo "FAILED:" $@
	TEST_FAIL=$(($TEST_FAIL + 1))
    fi

    if [ ${#DATED} -ne 0 ]
    then
	printf " -- %fs\n" $(printf "%09d" $time_elapsed | sed 's/\([[:digit:]]*\)\([[:digit:]]\{9\}\)/\1.\2/')
    fi
}

while getopts :s:tf:c OPT; do
    case $OPT in
	c|+c)
	    REMOVE_TEMPS_P=1
	    ;;
	t|+t)
	    # not use time function since it differs between shells
	    test_for_date=$(which date 2> /dev/null > /dev/null)
	    has_date=$?

	    if [ $has_date -eq 0 ]
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
	    ;;
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
	    run_test $SCRIPT_DIR/test_extras/test_bin2channels_channels2bin $test_name
	;;
	*)
	    skip_test $test_name
	;;
    esac    
}

# || part necessary to get the last line read
# see http://stackoverflow.com/questions/4165135/how-to-use-while-read-bash-to-read-the-last-line-in-a-file-if-there-s-no-new

export REMOVE_TEMPS_P
export PATH

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

printf "Tests: PASSED:%d FAILED:%d SKIPPED:%d\n" $TEST_PASS $TEST_FAIL $TEST_SKIP
