#!/bin/bash

# This file is part of CVPI.
# Copyright 2017 Devin Homan
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# interactive program for running cvpi_sample_motion_detection.c

# no command line arguments necessary; interatively asks the user for
# them.

# The script creates list of resolutions supported by the camera and
# asks the user to select one, and also asks the user for the number
# of frames to capture.  The program ouputs frames in the
# ./cvpi_sample_motion_detection directory and converts them to jpg
# for use by an external image viewing program such as 'feh'.


width=640
height=480
frames=50

resolutions=$(perl cvpi_camera_resolutions.pl /dev/video0)

echo 'select image dimensions:'
select dimensions in $resolutions
do
    if [[ -n $dimensions ]]
    then
    	width=$(echo $dimensions | sed 's/x.*//')
    	height=$(echo $dimensions | sed 's/[^x]*x//')
	echo "$width x $height selected"
    else
    	echo 'Invalid, using default dimensions: 640x480.'
    fi
    break
done

min=1
num_check='^[0-9]+$'
read -p "Number of frames: " number
if  [[ $number =~ $num_check && "$number" -gt "$min" ]]
then
    frames=$number
else
    echo 'Invalid, using default frames: 50.'
fi

if [ ! -d 'cvpi_sample_motion_detect_output' ]; then
    mkdir ./cvpi_sample_motion_detect_output
fi

cd ./cvpi_sample_motion_detect_output
if [[ 'cvpi_sample_motion_detect_output' != ${PWD##*/} ]]
then
    echo 'Invalid locale.'
    exit
fi

rm *.yuv 2> /dev/null
rm *.bmp 2> /dev/null
rm *.jpg 2> /dev/null

while :
do

    trap : INT			# ignore Ctrl-c

    cd ..
    ./cvpi_sample_motion_detection $frames $width $height

    cd ./cvpi_sample_motion_detect_output
    for yuva in `ls *.yuv`
    do
	name=$(echo $yuva | sed 's/\.yuv//')
	bmp="${name}.bmp"
	jpg="${name}.jpg"
	../cvpi_sample_yuv2bmp $width $height $yuva $bmp
	convert -rotate 180 $bmp $jpg
	printf "Converted %s\n" $yuva
    done

    feh -D 0.1 -Fx --zoom fill --cycle-once *.jpg

    trap - INT			# enable Ctrl-c
    seconds=10
    printf "Loop will restart in %d seconds. Press Ctrl-C now to quit." $seconds
    sleep $seconds
done
