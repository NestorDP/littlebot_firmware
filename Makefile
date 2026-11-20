# Copyright (c) 2022-2025 Nestor D. Pereira Neto

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

# Include the nanopb provided Makefile rules
include nanopb/extra/nanopb.mk

TOOLCHAIN = arm-none-eabi-
CC        = $(TOOLCHAIN)gcc
CXX       = $(TOOLCHAIN)g++
AS        = $(TOOLCHAIN)as
LD        = $(TOOLCHAIN)ld
OBJCOPY   = $(TOOLCHAIN)objcopy
AR        = $(TOOLCHAIN)ar
SIZE      = $(TOOLCHAIN)size

# GCC flags
#--------------------
CPUFLAG = -mthumb -mcpu=cortex-m4 -mlittle-endian
FPUFLAG = -mfpu=fpv4-sp-d16 -mfloat-abi=hard

WFLAGS  = -Wall 
WFLAGS += -Wextra
WFLAGS += -Wpedantic
WFLAGS += -Wconversion
WFLAGS += -Wsign-conversion
WFLAGS += -Wduplicated-cond
WFLAGS += -Wduplicated-branches
WFLAGS += -Wlogical-op
WFLAGS += -Wnull-dereference
WFLAGS += -Wjump-misses-init
WFLAGS += -Wdouble-promotion
WFLAGS += -Wshadow
WFLAGS += -Wformat=2

CFLAGS  = $(CPUFLAG) 
CFLAGS += $(FPUFLAG)
CFLAGS += -std=c11 
CFLAGS += -MD
CFLAGS += -pedantic
CFLAGS += -nostdlib
CFLAGS += -ffunction-sections 
CFLAGS += -fdata-sections 
CFLAGS += -specs=nosys.specs
CFLAGS += -DPART_TM4C123GH6PM

DEBFLAG  = -g
DEBFLAG += -DDEBUG

# Directories variables 
#---------------------
PORT_TARGET 		 = GCC/ARM_CM4F/
OBJ_DIR     		 = obj/
SRC_DIR     		 = src/
DRIVERS_DIR 		 = $(SRC_DIR)littlebot_drivers/
NANO_DIR   			 = $(NANOPB_DIR)/
FREERTOS_SRC_DIR     = FreeRTOS/Source/
FREERTOS_MEMMANG_DIR = $(FREERTOS_SRC_DIR)portable/MemMang/
FREERTOS_PORT_DIR    = $(FREERTOS_SRC_DIR)portable/$(PORT_TARGET)
DRIVERLIB_UTILS_DIR  = $(TIVAWARE_DIR)/utils/

# Generator and options for protobuf message
#---------------------
$(shell ./nanopb/generator/protoc --nanopb_out=. $(SRC_DIR)*.proto)

# Source files variables
#---------------------
FREERTOS_PORT_SOURCE = $(shell ls $(FREERTOS_PORT_DIR)*.c)
DRIVERS_SOURCES      = $(shell ls $(DRIVERS_DIR)*.c)
SRC_SOURCES          = $(shell ls $(SRC_DIR)*.c)
NANO_SOURCES         = $(NANOPB_CORE)

# Object files variables: Include all .o utils files that you have using in project
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

DRIVERLIB_UTILS_OBJS = uartstdio.o

FREERTOS_PORT_OBJS = $(patsubst $(FREERTOS_PORT_DIR)%,$(OBJ_DIR)%,$(FREERTOS_PORT_SOURCE:.c=.o))
DRIVERS_OBJS       = $(patsubst $(DRIVERS_DIR)%,$(OBJ_DIR)%,$(DRIVERS_SOURCES:.c=.o))
SRC_OBJS           = $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SOURCES:.c=.o))
NANO_OBJS          = $(patsubst $(NANO_DIR)%,$(OBJ_DIR)%,$(NANO_SOURCES:.c=.o))

OBJS   = $(addprefix $(OBJ_DIR), $(FREERTOS_OBJS))    
OBJS  += $(addprefix $(OBJ_DIR), $(FREERTOS_MEMMANG_OBJS))
OBJS  += $(addprefix $(OBJ_DIR), $(DRIVERLIB_UTILS_OBJS)) 
OBJS  += $(FREERTOS_PORT_OBJS) 
OBJS  += $(DRIVERS_OBJS)
OBJS  += $(SRC_OBJS)
OBJS  += $(NANO_OBJS)

# Get the location of libgcc.a, libc.a and libm.a from the GCC front-end.
#---------------------
LIBGCC := ${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC   := ${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM   := ${shell ${CC} ${CFLAGS} -print-file-name=libm.a}
LIBNOSYS := ${shell ${CC} ${CFLAGS} -print-file-name=libnosys.a}

# Linker flags.
#---------------------
# LDFLAGS  = -specs=nosys.specs
# LDFLAGS += -specs=nano.specs

# Include paths to be passed to $(CC) where necessary
#---------------------
INC_DIR       = include/
INC_FREERTOS  = $(FREERTOS_SRC_DIR)include/
INC_TIVAWARE  = $(TIVAWARE_DIR)/
INC_FLAGS     = -I $(INC_FREERTOS) -I $(SRC_DIR) -I $(FREERTOS_PORT_DIR) -I $(INC_DIR) \
			    -I $(INC_TIVAWARE) -I $(NANOPB_DIR) -I .

# Dependency on HW specific settings
#---------------------
DEP_FRTOS_CONFIG = $(SRC_DIR)FreeRTOSConfig.h

# Definition of the linker script and final targets
#---------------------
LINKER_SCRIPT = tm4c123gh6pm.lds
ELF_IMAGE     = image.elf
TARGET_IMAGE  = image.bin

# Make rules:
#---------------------
.PHONY :  all
all : $(TARGET_IMAGE) size

.PHONY :  rebuild
rebuild : clean all

$(TARGET_IMAGE) : $(OBJ_DIR) $(ELF_IMAGE)
	$(OBJCOPY) -O binary $(word 2,$^) $@

$(OBJ_DIR) :
	mkdir -p $@

# Linker
#---------------------
$(ELF_IMAGE) : $(OBJS) $(LINKER_SCRIPT)
	$(LD) -L $(OBJ_DIR) -L $(TIVAWARE_DIR)/driverlib/gcc -T $(LINKER_SCRIPT) $(OBJS) \
    --gc-sections -o $@ -ldriver '$(LIBC)' '$(LIBGCC)' '$(LIBM)' '$(LIBNOSYS)'

.PHONY : debug
debug : _debug_flags all

.PHONY : debug_rebuild
debug_rebuild : _debug_flags rebuild

_debug_flags :
	$(eval CFLAGS += $(DEBFLAG))

# Rules for all object files
#---------------------
# FreeRTOS core
$(OBJ_DIR)%.o:  $(FREERTOS_SRC_DIR)%.c $(DEP_FRTOS_CONFIG)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# HW specific part, in FreeRTOS/Source/portable/$(PORT_TARGETET)
$(OBJ_DIR)port.o : $(FREERTOS_PORT_DIR)port.c $(DEP_FRTOS_CONFIG)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Rules for all MemMang implementations are provided
$(OBJ_DIR)%.o : $(FREERTOS_MEMMANG_DIR)%.c $(DEP_FRTOS_CONFIG)
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Tivaware driverlib utils
$(OBJ_DIR)%.o : $(DRIVERLIB_UTILS_DIR)%.c
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# littlebot drivers
$(OBJ_DIR)%.o : $(DRIVERS_DIR)%.c
	$(CC) -c $(CFLAGS) $(WFLAGS) $(INC_FLAGS) $< -o $@

# nanopb core
$(OBJ_DIR)%.o : $(NANO_DIR)%.c
	$(CC) -c $(CFLAGS) $(INC_FLAGS) $< -o $@

# Main Code
$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(DEP_FRTOS_CONFIG)
	$(CC) -c $(CFLAGS) $(WFLAGS) $(INC_FLAGS) $< -o $@

# Size code:
#---------------------
.PHONY : size
size : 
	$(SIZE) $(ELF_IMAGE)

# Cleanup directives:
#---------------------
.PHONY : clean_obj
clean_obj :
	$(RM) -r $(OBJ_DIR)

.PHONY : clean_intermediate
clean_intermediate : clean_obj
	$(RM) *.elf
	$(RM) *.img
	$(RM) ${SRC_DIR}*.pb.c
	$(RM) ${SRC_DIR}*.pb.h
	
.PHONY : clean
clean : clean_intermediate
	$(RM) *.bin

# Debug target for file type detection
#--------------------
.PHONY : debug-filetype
debug-filetype :
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "SRC_SOURCES: $(SRC_SOURCES)"

# Short help instructions:
#---------------------
.PHONY : help
help :
	@echo "LittleBot Firmware Makefile"
	@echo "==========================="
	@echo ""
	@echo "Targets:"
	@echo "  all       	     - Build the firmware and create the target image '$(TARGET_IMAGE)'"
	@echo "  rebuild   	     - Rebuild everything from scratch"
	@echo "  debug     	     - Build with debug symbols"
	@echo "  debug_rebuild      - Rebuild with debug symbols"
	@echo "  size     	     - Show binary size info for '$(ELF_IMAGE)'"
	@echo "  clean_obj 	     - Remove object files"
	@echo "  clean_intermediate - Remove intermediate files (keep image)"
	@echo "  clean     	     - Remove all generated files (including '$(TARGET_IMAGE)')"
	@echo "  docs      	     - Generate Doxygen documentation"
	@echo "  docs-view 	     - Generate and open Doxygen documentation"
	@echo "  docs-clean	     - Clean generated documentation"
	@echo "  help     	     - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make all           # Build firmware"
	@echo "  make debug         # Build with debug symbols"
	@echo "  make docs          # Generate documentation"
	@echo
# Documentation targets
#---------------------
.PHONY : docs
docs:
	@echo "Generating documentation with Doxygen..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen docs/Doxyfile; \
		mkdir -p docs/html/docs/images; \
		cp docs/images/* docs/html/docs/images/ 2>/dev/null || true; \
		echo "Documentation generated in docs/html/"; \
		echo "Open docs/html/index.html in your browser"; \
	else \
		echo "Error: Doxygen not installed. Install with:"; \
		echo "  Ubuntu/Debian: sudo apt install doxygen"; \
	fi

.PHONY : docs-clean
docs-clean:
	@echo "Cleaning documentation..."
	@rm -rf docs/html docs/latex docs/man docs/rtf docs/xml
	@echo "Documentation cleaned."

.PHONY : docs-view
docs-view: docs
	@if command -v xdg-open >/dev/null 2>&1; then \
		xdg-open docs/html/index.html; \
	else \
		echo "Open docs/html/index.html in your browser"; \
	fi
#------------------------------------------------------------------------------
# End of Makefile