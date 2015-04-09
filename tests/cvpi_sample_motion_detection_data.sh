#!/bin/bash

# This file is part of CVPI.
#
# Copyright (C) 2015
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
