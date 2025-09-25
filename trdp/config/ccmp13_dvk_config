#//
#// Digi ConnectCore MP13 (i.MX93) TRDP config for Yocto Linux
#//
#// Updated for Digi MP13 by Vivek Pal - 2025
#//

# Set this to your actual SDK installation directory before building
TCPATH = /home/bacancy/yocto-work/toolchain/sysroots/x86_64-deysdk-linux/usr/bin/arm-dey-linux-gnueabi/

ARCH = ccmp13-dvk
TARGET_VOS = posix
TARGET_OS = LINUX
TCPREFIX = arm-dey-linux-gnueabi-
TCPOSTFIX =
DOXYPATH = /usr/local/bin/

# adding path of source File 
ADD_SRC += uart_eth_trdp_gw/core/src
ADD_SRC += uart_eth_trdp_gw/ethernet_trdp
ADD_SRC += uart_eth_trdp_gw/uart_trdp
ADD_SRC += uart_eth_trdp_gw/uart_eth_gw


# adding path of include file 
ADD_INC += uart_eth_trdp_gw/core/inc
ADD_INC += uart_eth_trdp_gw/ethernet_trdp
ADD_INC += uart_eth_trdp_gw/uart_trdp
ADD_INC += uart_eth_trdp_gw/uart_eth_gw

BAC_PRO_INCPATH += -I uart_eth_trdp_gw/core/inc
BAC_PRO_INCPATH += -I uart_eth_trdp_gw/ethernet_trdp
BAC_PRO_INCPATH += -I uart_eth_trdp_gw/uart_trdp
BAC_PRO_INCPATH += -I uart_eth_trdp_gw/uart_eth_gw


# flag for build the example  
# EXAMPLES_BUILD = FLASE for not build the Example 
EXAMPLES_BUILD = FALSE
BACANCY_PROJECT = TRUE
# Compiler flags
#//CFLAGS += --sysroot=$(YOCTO_DIR)/sysroots/$(ARCH)
#//CFLAGS += --sysroot=/home/bacancy/yocto-work/toolchain/sysroots/x86_64-deysdk-linux
CFLAGS += --sysroot=/home/bacancy/yocto-work/toolchain/sysroots/cortexa7t2hf-neon-vfpv4-dey-linux-gnueabi
CFLAGS += -Wall -Wno-unknown-pragmas -Wno-format -fstrength-reduce -fno-builtin -fsigned-char
CFLAGS += -pthread -fPIC -D_GNU_SOURCE -DPOSIX
CFLAGS += -mfpu=neon -mfloat-abi=hard
CXXFLAGS += -mfpu=neon -mfloat-abi=hard

# Linker flags
LDFLAGS += -lrt

# Linting directives (if used)
LINT_SYSINCLUDE_DIRECTIVES = -i ./src/vos/posix -wlib 0 -DL_ENDIAN

