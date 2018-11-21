ifdef config
config = make/config.mk
endif

include $(config)
include make/ps.mk

ifndef CXX
CXX = g++
endif

ifndef DEPS_PATH
DEPS_PATH = $(shell pwd)/deps
endif


ifndef PROTOC
PROTOC = ${DEPS_PATH}/bin/protoc
endif

# use glog log
DMLC_USE_GLOG = 1

ifeq ($(DMLC_USE_GLOG), 1)
ADD_CFLAGS += -DDMLC_USE_GLOG=1
endif

INCPATH = -I./src -I./include -I$(DEPS_PATH)/include
CFLAGS = -std=c++11 -msse2 -fPIC -O3 -ggdb -Wall -finline-functions $(INCPATH) $(ADD_CFLAGS)

all: deps ps test

clean:
	rm -rf build $(TEST) tests/*.d tests/*.dSYM
	find src -name "*.pb.[ch]*" -delete

lint:
	python tests/lint.py ps all include/ps src

ps: build/libps.a

OBJS = $(addprefix build/, customer.o postoffice.o van.o meta.pb.o)
build/libps.a: $(OBJS)
	ar crv $@ $(filter %.o, $?)

build/%.o: src/%.cc ${ZMQ} src/meta.pb.h
	@mkdir -p $(@D)
	$(CXX) $(INCPATH) -std=c++0x -MM -MT build/$*.o $< >build/$*.d
	$(CXX) $(CFLAGS) -c $< -o $@

src/%.pb.cc src/%.pb.h : src/%.proto ${PROTOBUF}
	$(PROTOC) --cpp_out=./src --proto_path=./src $<

-include build/*.d
-include build/*/*.d

include tests/test.mk
test: $(TEST)

# test third party
include tests_third_party/test_third_party.mk
test_third_party: $(TEST_THIRD_PARTY)

include make/deps.mk

deps: gflags glog protobuf zmq lz4 cityhash
