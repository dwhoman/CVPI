#!/bin/bash

# This file is part of CVPI.
#
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

# get frame rate data 

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
