ARDUINO_C=arduino-cli
ARDUINO_DIR=$(HOME)/Projects/Arduino/Arch-IoT-Project
ARDUINO_PORT=/dev/ttyACM0

attach:
	$(ARDUINO_C) board attach -p $(ARDUINO_PORT)

compile:
	$(ARDUINO_C) compile

upload:
	$(ARDUINO_C) upload

run: attach compile upload

monitor:
	$(ARDUINO_C) monitor -p $(ARDUINO_PORT)
