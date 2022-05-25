#******************************************************************************
#
# Makefile - Rules for building the main example.
#
# Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
# Texas Instruments (TI) is supplying this software for use solely and
# exclusively on TI's microcontroller products. The software is owned by
# TI and/or its suppliers, and is protected under applicable copyright
# laws. You may not combine this software with "viral" open-source
# software in order to form a larger program.
# 
# THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
# NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
# NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
# DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 2.2.0.295 of the EK-TM4C123GXL Firmware Package.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM

#
# The base directory for TivaWare.
#
ROOT= ${TIVAWARE_PATH}

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=${TIVAWARE_PATH}
CFLAGS+= -I include/

OBJDIR = ${COMPILER}/

APP_SRC 		= app/src/
APP_SOURCES	= $(shell ls $(APP_SRC)*.c)
APP_OBJS		= $(patsubst $(APP_SRC)%,$(OBJDIR)%,$(APP_SOURCES:.c=.o))

ROOT_SOURCES= $(shell ls *.c)
ROOT_OBJS		= $(patsubst %,$(OBJDIR)%,$(ROOT_SOURCES:.c=.o))

#
# The default rule, which causes the main example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/main.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the main example.
#
${COMPILER}/main.axf: ${COMPILER}/main.o
${COMPILER}/main.axf: ${COMPILER}/bluetooth.o
${COMPILER}/main.axf: ${COMPILER}/encoder.o
${COMPILER}/main.axf: ${COMPILER}/motor.o
${COMPILER}/main.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/main.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/main.axf: main.ld
SCATTERgcc_main=main.ld
ENTRY_main=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C123_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif


help :
	@echo
	@echo ---------------------------------------------------------
	@echo - ROOT_OBJS $(ROOT_OBJS)
	@echo
	@echo - APP_OBJS \'$(APP_OBJS)\'.
	@echo
	@echo - libgm \'$(LIBM)\'.
	@echo
	@echo ---------------------------------------------------------