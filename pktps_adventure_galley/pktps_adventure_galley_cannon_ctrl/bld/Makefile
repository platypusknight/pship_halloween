############################################################################################
##
############################################################################################
BASE_DIR=$(shell cd ../../../; pwd)
COMMON_DIR=$(shell cd ../../../../pkt_common; pwd)
PRJ_DIR=${BASE_DIR}/pktps_adventure_galley/pktps_adventure_galley_cannon_ctrl
BLD_DIR=${PRJ_DIR}/bld

DEVICE=atmega328p
AD_DEV=m328

PRJ=pktps_adventure_galley_cannon_ctrl

include ${PRJ_DIR}/bld/pktps_adventure_galley_cannon_ctrl_src_list.mk
SRC_ONLY=$(notdir $(SRC_LIST))

# Object files (map src/xyz/foo.cpp → build/foo.o)
OBJ_LIST:=$(addprefix $(BLD_DIR)/, $(SRC_ONLY:.cpp=.o))

INC_ARGS:=$(addprefix -I, $(INC_DIRS))

AVRDUDE_DIR=/usr/
AVRDUDE=${AVRDUDE_DIR}/bin/avrdude

CXX = avr-g++
CXXFLAGS = -DF_CPU=16000000UL
CXXFLAGS += -Os
CXXFLAGS += -g
CXXFLAGS += -mmcu=$(DEVICE)
CXXFLAGS += -Wall
CXXFLAGS += $(INC_ARGS)

all: $(PRJ).hex

# Let make find each .cpp in whichever repo it lives in
vpath %.cpp $(sort $(dir $(SRC_LIST)))

$(BLD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link it all
$(PRJ).elf: $(OBJ_LIST)
	avr-g++ -g -mmcu=$(DEVICE) -Wall -Os -o $@ $^

$(PRJ).hex: $(PRJ).elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

# Upload the built program (hex file) to Atmega328P using USBasp
upload: $(PRJ).hex
	$(AVRDUDE)  -C/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$(PRJ).hex:i

# Remove build directory with all built files
clean:
	rm -rf $(BLD_DIR)/*.hex $(BLD_DIR)/*.elf $(BLD_DIR)/*.o

