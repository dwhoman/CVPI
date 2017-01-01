#!/bin/bash

# This file is part of CVPI.
#
# Copyright (C) 2015 Devin Homan
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
cd ..

rate_data=$(./cvpi_sample_motion_detection $frames $width $height)

rate1=$(echo $rate_data | sed 's/,.*//')
rate2=$(echo $rate_data | sed 's/[^,]*,//')

printf "Running at %f to %f fps\n" $rate2 $rate1;

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