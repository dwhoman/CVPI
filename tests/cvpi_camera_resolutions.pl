#!/usr/bin/perl
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
