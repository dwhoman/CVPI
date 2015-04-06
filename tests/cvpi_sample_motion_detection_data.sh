#!/bin/bash

# get frame rate data 

cd ./cvpi_sample_motion_detect_output
if [[ 'cvpi_sample_motion_detect_output' != ${PWD##*/} ]]
then
    echo 'Invalid locale.'
    exit
fi
rm *.yuv 2> /dev/null
rm *.bmp 2> /dev/null
rm *.jpg 2> /dev/null
cd ..

width=640
height=480
frames=50
for dimensions in '160x120' '176x144' '320x176' '320x240' '352x288' '432x240' '544x288' '640x360'
do
    width=$(echo $dimensions | sed 's/x.*//')
    height=$(echo $dimensions | sed 's/[^x]*x//')
    pixels=$(($width*$height))

    rate_data=$(./cvpi_sample_motion_detection $frames $width $height)
    
    printf "%s,%d,%d,%s\n" $dimensions $frames, $pixels, $rate_data

done
