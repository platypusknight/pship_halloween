############################################################################################
##
############################################################################################

include ${COMMON_DIR}/avr_mods/pktcmn_uart/bld/pktcmn_uart_src_list.mk

PRJ_DIR=${BASE_DIR}/pktps_adventure_galley/pktps_adventure_galley_cannon_ctrl
SRC_LIST+=${PRJ_DIR}/src/pktps_adventure_galley_cannon_ctrl_main.cpp

SRC_DIRS+=$(BASE_DIR)/pktps_adventure_galley/pktps_adventure_galley_cannon_ctrl/src
INC_DIRS+=${BASE_DIR}/pktps_adventure_galley/pktps_adventure_galley_cannon_ctrl/inc

