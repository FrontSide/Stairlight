# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = $(HOME)/arduino-1.8.5
ARDMK_DIR = /usr/share/arduino
AVR_TOOLS_DIR = $(ARDUINO_DIR)/hardware/tools/avr

BOARD_TAG    = uno
BOARDS_TXT   = $(ARDUINO_DIR)/hardware/arduino/avr/boards.txt
ISP_PROG     = usbtiny
AVRDUDE_OPTS = -v
MONITOR_PORT = /dev/ttyUSB0
FORCE_MONITOR_PORT = true
#MONITOR_BAUDRATE = 115200

ARDUINO_MK_PRESENT := $(call ls Arduino.mk)

ifeq ($(ARDUINO_MK_PRESENT),)
        $(info install Arduino.mk)
        $(shell cp $(ARDMK_DIR)/Arduino.mk .)
endif

include ./Arduino.mk

FAST_LED_LIB = https://github.com/FastLED/FastLED/archive/3.1.7.tar.gz
SHELL := /bin/bash

.PHONY: deps
deps:
	pushd $(ARDUINO_DIR)/libraries && \
		wget $(FAST_LED_LIB) -O FastLED.tar.gz && \
		tar -xf FastLED.tar.gz && \
		rm -f FastLED.tar.gz && \
		mv FastLED* FastLED && \
		popd

.PHONY: deploy
deploy: ispload

.PHONY: test
test:
	rm -rf exec_test || true
	mkdir exec_test
	cp $(shell ls *.cpp *.h | grep -vP "(^main|^strip)") ./exec_test
	cp ./test/* ./exec_test
	cd exec_test && g++ ./*cpp -DTEST -otest
	@echo -----------------------------------------------------------
	./exec_test/test
	
