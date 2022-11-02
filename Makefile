# Copyright (c) 2022 Nestor D. Pereira Neto

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# This makefile was written based on the 
# https://github.com/jkovacic/FreeRTOS-GCC-tm4c123glx/blob/master/Makefile





TOOLCHAIN = arm-none-eabi-
CC        = $(TOOLCHAIN)gcc
CXX       = $(TOOLCHAIN)g++
AS        = $(TOOLCHAIN)as
LD        = $(TOOLCHAIN)ld
OBJCOPY   = $(TOOLCHAIN)objcopy
AR        = $(TOOLCHAIN)ar

# GCC flags
#--------------------
CPUFLAG  = -mthumb -mcpu=cortex-m4
WFLAG    = -Wall 
WFLAG   +=-Wextra
FPUFLAG +=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS   = $(CPUFLAG) 
CFLAGS  += $(WFLAG) 
CFLAGS  += $(FPUFLAG)
CFLAGS  += -std=c99 
CFLAGS  += -ffunction-sections 
CFLAGS  += -fdata-sections 
CFLAGS  += -DPART_TM4C123GH6PM

DEB_FLAG = -g -DDEBUG

# Directories variables 
#---------------------
PORT_TARGET = GCC/ARM_CM4F/
OBJ_DIR     = obj/
DRIVERS_DIR = drivers/
SRC_DIR     = src/

FREERTOS_SRC_DIR     = FreeRTOS/Source/
FREERTOS_MEMMANG_DIR = $(FREERTOS_SRC_DIR)portable/MemMang/
FREERTOS_PORT_DIR    = $(FREERTOS_SRC_DIR)portable/$(PORT_TARGET)

# Object files
#---------------------
FREERTOS_OBJS  = queue.o list.o tasks.o
FREERTOS_OBJS += timers.o
FREERTOS_OBJS += croutine.o
FREERTOS_OBJS += event_groups.o
FREERTOS_OBJS += stream_buffer.o

#FREERTOS_MEMMANG_OBJS = heap_1.o
FREERTOS_MEMMANG_OBJS = heap_2.o
#FREERTOS_MEMMANG_OBJS = heap_3.o
#FREERTOS_MEMMANG_OBJS = heap_4.o
#FREERTOS_MEMMANG_OBJS = heap_5.o

FREERTOS_PORT_OBJS  = port.o

FREERTOS_PORT_SOURCE= $(shell ls $(FREERTOS_PORT_DIR)*.c)
DRIVERS_SOURCES     = $(shell ls $(DRIVERS_DIR)*.c)
SRC_SOURCES         = $(shell ls $(SRC_DIR)*.c)

FREERTOS_PORT_OBJS  = $(patsubst $(FREERTOS_PORT_DIR)%,$(OBJ_DIR)%,$(FREERTOS_PORT_SOURCE:.c=.o))
DRIVERS_OBJS        = $(patsubst $(DRIVERS_DIR)%,$(OBJ_DIR)%,$(DRIVERS_SOURCES:.c=.o))
SRC_OBJS            = $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SOURCES:.c=.o))

OBJS  = $(addprefix $(OBJ_DIR), $(FREERTOS_OBJS))    
OBJS  += $(addprefix $(OBJ_DIR), $(FREERTOS_MEMMANG_OBJS))
OBJS  += $(FREERTOS_PORT_OBJS)
OBJS  += $(DRIVERS_OBJS)
OBJS  += $(SRC_OBJS)

# Get the location of libgcc.a, libc.a and libm.a from the GCC front-end.
#---------------------
LIBGCC := ${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC   := ${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM   := ${shell ${CC} ${CFLAGS} -print-file-name=libm.a}

# Include paths to be passed to $(CC) where necessary
#---------------------
INC_DIR       = include/
INC_FREERTOS  = $(FREERTOS_SRC_DIR)include/
INC_TIVAWARE  = $(TIVAWARE_DIR)/
INC_FLAGS     = -I $(INC_FREERTOS) -I $(SRC_DIR) -I $(FREERTOS_PORT_DIR) -I $(INC_DIR) -I $(INC_TIVAWARE)

# Dependency on HW specific settings
#---------------------
DEP_BSP          = $(INC_DIR)drivers/bsp.h
DEP_FRTOS_CONFIG = $(SRC_DIR)FreeRTOSConfig.h
DEP_SETTINGS     = $(DEP_FRTOS_CONFIG)

# Definition of the linker script and final targets
#---------------------
LINKER_SCRIPT = $(addprefix , tm4c123gh6pm.lds)
ELF_IMAGE     = image.elf
TARGET_IMAGE  = image.bin

# Make rules:
#---------------------
print-%  : ; @echo $* = $($*)

all : $(TARGET_IMAGE)

rebuild : clean all

$(TARGET_IMAGE) : $(OBJ_DIR) $(ELF_IMAGE)
	$(OBJCOPY) -O binary $(word 2,$^) $@

$(OBJ_DIR) :
	mkdir -p $@

# Linker
$(ELF_IMAGE) : $(OBJS) $(LINKER_SCRIPT)
	$(LD) -L $(OBJ_DIR) -L$(TIVAWARE_DIR)/driverlib/gcc -T $(LINKER_SCRIPT) $(OBJS) -o $@ -ldriver '$(LIBGCC)' '$(LIBC)' '$(LIBM)'

debug : _debug_flags all

debug_rebuild : _debug_flags rebuild

_debug_flags :
	$(eval CFLAGS += $(DEB_FLAG))

# FreeRTOS core
$(OBJ_DIR)%.o:  $(FREERTOS_SRC_DIR)%.c $(DEP_FRTOS_CONFIG) $(DEP_SETTINGS)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# HW specific part, in FreeRTOS/Source/portable/$(PORT_TARGETET)
$(OBJ_DIR)port.o : $(FREERTOS_PORT_DIR)port.c $(DEP_FRTOS_CONFIG)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Rules for all MemMang implementations are provided
$(OBJ_DIR)%.o : $(FREERTOS_MEMMANG_DIR)%.c $(DEP_FRTOS_CONFIG)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Drivers
$(OBJ_DIR)%.o : $(DRIVERS_DIR)%.c
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Main Code
$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(DEP_SETTINGS)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Cleanup directives:
#---------------------
clean_obj :
	$(RM) -r $(OBJ_DIR)

clean_intermediate : clean_obj
	$(RM) *.elf
	$(RM) *.img
	
clean : clean_intermediate
	$(RM) *.bin

# Short help instructions:
#---------------------
help :
	@echo
	@echo Valid targets:
	@echo - all: builds missing dependencies and creates the target image \'$(TARGET_IMAGE)\'.
	@echo - rebuild: rebuilds all dependencies and creates the target image \'$(TARGET_IMAGE)\'.
	@echo - debug: same as \'all\', also includes debugging symbols to \'$(ELF_IMAGE)\'.
	@echo - debug_rebuild: same as \'rebuild\', also includes debugging symbols to \'$(ELF_IMAGE)\'.
	@echo - clean_obj: deletes all object files, only keeps \'$(ELF_IMAGE)\' and \'$(TARGET_IMAGE)\'.
	@echo - clean_intermediate: deletes all intermediate binaries, only keeps the target image \'$(TARGET_IMAGE)\'.
	@echo - clean: deletes all intermediate binaries, incl. the target image \'$(TARGET_IMAGE)\'.
	@echo - help: displays these help instructions.
	@echo


.PHONY :  all rebuild clean clean_intermediate clean_obj debug debug_rebuild _debug_flags help
