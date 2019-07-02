#
#  Makefile
#  RayTracingInAWeekend
#
#  Created by Abhijit Bhelande on 2019-07-01.
#  Copyright (c) 2019 Abhijit does code.
#

PLATFORM := $(shell uname -s)

# CC
ifeq ($(PLATFORM),Darwin)
  CC := clang++ -arch x86_64
else
  CC := g++
endif

# Folders
SRCDIR := RayTracingInAWeekend
BUILDDIR := build
TARGETDIR := bin
INSTALLBINDIR := bin
TARGET := bin/RayTracingInAWeekend

# Normal CPP files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# HPP files
SRCEXT2 := hpp
SOURCES2 := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT2))
OBJECTS2 := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES2:.$(SRCEXT2)=.o))

CFLAGS := -c
ifeq ($(UNAME_S),Linux)
  CFLAGS += -std=gnu++11 -O2
else
  CFLAGS += -std=c++11 -stdlib=libc++ -O2
endif

LIB := -L /usr/local/lib
INC := -I /usr/local/include

$(TARGET): $(OBJECTS) $(OBJECTS2)
	mkdir -p $(TARGETDIR)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT2)
	mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	@echo " $(RM) -r $(INSTALLBINDIR) $(TARGET)"; $(RM) -r $(INSTALLBINDIR) $(TARGET)

install:
	@echo " Installing...";
	@echo " cp $(TARGET) $(INSTALLBINDIR)"; cp $(TARGET) $(INSTALLBINDIR)

.PHONY: clean