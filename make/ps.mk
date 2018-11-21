#---------------------------------------------------------------------------------------
#  parameter server configuration script
#
#  include ps.mk after the variables are set
#
#----------------------------------------------------------------------------------------

ifeq ($(USE_KEY32), 1)
ADD_CFLAGS += -DUSE_KEY32=1
endif

ifeq ($(DMLC_USE_GLOG), 1)
ADD_CFLAGS += -DDMLC_USE_GLOG=1
endif

PS_LDFLAGS_SO = -L$(DEPS_PATH)/lib -lprotobuf-lite -lzmq -lglog -lgflags -lcityhash -llz4
PS_LDFLAGS_A = $(addprefix $(DEPS_PATH)/lib/, libprotobuf-lite.a libzmq.a libglog.a libgflags.a libcityhash.a liblz4.a)
