#!/usr/bin/env python

import random
import os

output_directory = "cvpi_image_mean_test_files";

DIM_MOD = 2048
FILES = 100

for i in range(0,FILES):
    width = random.randint(0,DIM_MOD)
    height = random.randint(0,DIM_MOD)

    command = "dd if=/dev/urandom of=./cvpi_image_mean_test_files/" + \
        str(i) + "_" + str(width) + "_" + str(height) + " bs=4 count=" + str(width*height)
    os.system(command)
