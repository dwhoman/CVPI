#!/usr/bin/perl

# This file is part of CVPI.
#
#  Copyright 2017 Devin Homan
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

# takes output from v4l2-ctl --list-formats-ext -d "/dev/video?" and
# creates a list of resolutions supported by the camera

use strict;
my $camera = @ARGV[0];
my $camera_list_formats_ext = qx/v4l2-ctl --list-formats-ext -d $camera/;
my @camera_formats = (grep /Type\s*:\s*Video\s*Capture/, (grep /Index/, (split /(?=\s*Index\s*:\s*\d+\s*)/, $camera_list_formats_ext)));

foreach my $listing (@camera_formats) {
    if(index($listing, "YUYV") != -1) {
	foreach my $line (split /\n/, $listing) {
	    if(index($line, "Size") != -1) {
		$_ = $line;
		s/Size:\s+Discrete\s+(.*)/$1/;
		print ($_);
	    }
	}
    }
}
