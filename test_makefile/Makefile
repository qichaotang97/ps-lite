ifdef config
config = ../make/config.mk
endif

include $(config)
include ../make/ps.mk

ifndef CXX
CXX = g++
endif

ifndef DEPS_PATH
DEPS_PATH = $(shell pwd)/../deps
endif


ifndef PROTOC
PROTOC = ${DEPS_PATH}/bin/protoc
endif

DMLC_USE_GLOG = 1

ifeq ($(DMLC_USE_GLOG), 1)
ADD_CFLAGS += -DDMLC_USE_GLOG=1
endif

#ADD_CFLAGS += -DDMLC_USE_GLOG=1
INCPATH = -I./src -I./include -I$(DEPS_PATH)/include
CFLAGS = -std=c++11 -msse2 -fPIC -O3 -ggdb -Wall -finline-functions $(INCPATH) $(ADD_CFLAGS)

debug:
	@echo $(ADD_CFLAGS)
	@echo $(CFLAGS)
	@echo $(DMLC_USE_GLOG)
	@echo $(CXX)

include ../tests/test.mk

include ../make/deps.mk

deps: gflags glog protobuf zmq lz4 cityhash
