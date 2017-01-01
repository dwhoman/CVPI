.NOTPARALLEL:

# test endianness
.ONESHELL:
endianness.channel.%:
	INPUT_DIR=test_images/endianness.channel/input
	OUTPUT_DIR=test_images/endianness.channel/output
	printf "%s/input.rgba,CVPI_COLOR_SPACE,1,1,R\n" $$INPUT_DIR > files
	printf "%s/$(COLOR).rgba,CVPI_COLOR_SPACE,1,1,W\n" $$OUTPUT_DIR >> files
	./cvpi_tests test_endianness_channel_$(COLOR) files `wc -l files | awk '{print $$1}'`
	TOTAL_NUM=$$(($$TOTAL_NUM + 1))
	if [ $$(cmp -s $$(INPUT_DIR)/$(COLOR).rgba $$(OUTPUT_DIR)/$(COLOR).rgba) ]
	then
	FAIL_NUM=$$(($$FAIL_NUM + 1))
	printf "$@ FAILED\n"
	else
	PASS_NUM=$$(($$PASS_NUM + 1))
	printf "$@ PASSED\n"
	fi
endianness.channel.red:
COLOR=red

endianness.channel.green:
COLOR=green

endianness.channel.blue:
COLOR=blue

endianness.channel.alpha:
COLOR=alpha

yuyv2yuva:
	INPUT_DIR=test_images/yuyv2yuva/input
	OUTPUT_DIR=test_images/yuyv2yuva/output
	printf "%s/image.yuyv,YUYV,1280,960,R\n" $$(INPUT_DIR) > files
	printf "%s/image.yuva,CVPI_COLOR_SPACE,1280,960,W\n" $$(OUTPUT_DIR) >> files
	./cvpi_tests test_cvpi_yuyv2yuva files `wc -l files | awk '{print $$1}'`
	TOTAL_NUM=$$(($$TOTAL_NUM + 1))
	if [ $$(cmp -s $$(INPUT_DIR)/image.yuva $$(OUTPUT_DIR)/image.yuva) ]
	then
	FAIL_NUM=$$(($$FAIL_NUM + 1))
	printf "$@ FAILED\n"
	else
	PASS_NUM=$$(($$PASS_NUM + 1))
	printf "$@ PASSED\n"
	fi
