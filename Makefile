
# Copyright (c) 2017 The Gupta Empire - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
#
# Written by Aryan Gupta <me@theguptaempire.net>
# 
# ==============================================================================
# @author 			Aryan Gupta
# @project 			Tic Tac Toe
# @file 			Makefile
# @date				2017-03-12 (YYYY-MM-DD)
# @version			1.0.0
# @brief 		This is the Makefile for The Tic Tac Toe Project
# ==============================================================================
.DEFAULT_GOAL := all
# ==========================  CONST MACROS  ====================================
CC = "C:\Compiler\MinGW-w64\mingw32\bin\g++.exe"
7Z = "C:\Program Files (Portable)\7-Zip\7z.exe"
RES = "C:\Compiler\MinGW-w64\mingw32\bin\windres.exe"
OBJDIR = .\obj
BINDIR = .\bin
DATDIR = .\dat

DEBUG = -g -DDEBUG=true

# ==============================  MACROS  ======================================
CFLAGS = $(DEBUG) -Wall -std=c++17 -c
LFLAGS = $(DEBUG) -Wall
OBJ = $(OBJDIR)\PrimeFinder.o $(OBJDIR)\res.o $(OBJDIR)\MT_bN.o $(OBJDIR)\MT_bS.o $(OBJDIR)\MT_NP.o $(OBJDIR)\MT_SN.o $(OBJDIR)\ST.o

# ============================ RECEPIES ========================================

$(OBJDIR)\PrimeFinder.o: .\PrimeFinder.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)
	
$(OBJDIR)\MT_bN.o: .\MT_bN.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)
	
$(OBJDIR)\MT_bS.o: .\MT_bS.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)
	
$(OBJDIR)\MT_NP.o: .\MT_NP.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)

$(OBJDIR)\MT_SN.o: .\MT_SN.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)
	
$(OBJDIR)\ST.o: .\ST.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS)

$(OBJDIR)\%.o: .\%.cpp
	$(CC) .\$^ -o .\$@ $(CFLAGS) 

$(OBJDIR)\res.o: .\res.rc .\info.h
	$(RES) .\res.rc  .\$@
	
# Link	
$(BINDIR)\main.exe: $(OBJ)
	$(CC) .\$^ -o .\$@ $(LFLAGS)

# ============================= PHONY RECEPIES =================================
.PHONY: all
all: clean $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $(BINDIR)\main.exe

.PHONY: link
link:
	$(CC) .\$^ $(LFLAGS) $(LSDLL) $(LSDLIL) $(LSDLTL) -o $(BINDIR)\main.exe	
	
.PHONY: clean
clean:
	del $(OBJDIR)\*.o
	del $(BINDIR)\*.exe
	del $(DATDIR)\*.dat

.PHONY: archive
archive:
	$(7Z) a -tzip .\arc\"%DATE:~-4%%DATE:~4,2%%DATE:~7,2%".zip * -xr!obj -xr!bin -xr!arc