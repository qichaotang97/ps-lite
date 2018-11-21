# Install dependencies

URL1=https://raw.githubusercontent.com/mli/deps/master/build
URL2=https://github.com/google/protobuf/releases/download/v3.5.1
ifndef WGET
WGET = wget
endif

# protobuf
PROTOBUF = ${DEPS_PATH}/include/google/protobuf/message.h
${PROTOBUF}:
	$(eval FILE=third_party/protobuf-cpp-3.5.1.tar.gz)
	$(eval DIR=protobuf-3.5.1)
	rm -rf $(DIR)
	tar --no-same-owner -zxf $(FILE)
	cd $(DIR) && export CFLAGS=-fPIC && export CXXFLAGS=-fPIC && ./configure -prefix=$(DEPS_PATH) && $(MAKE) && $(MAKE) install
	rm -rf $(DIR)
protobuf: | ${DEPS_PATH}/include/google/protobuf/message.h

# zmq
ZMQ = ${DEPS_PATH}/include/zmq.h

${ZMQ}:
	$(eval FILE=third_party/zeromq-4.1.4.tar.gz)
	$(eval DIR=zeromq-4.1.4)
	rm -rf $(DIR)
	tar --no-same-owner -zxf $(FILE)
	cd $(DIR) && export CFLAGS=-fPIC && export CXXFLAGS=-fPIC && ./configure -prefix=$(DEPS_PATH) --with-libsodium=no --with-libgssapi_krb5=no && $(MAKE) && $(MAKE) install
	rm -rf $(DIR)
zmq: | ${DEPS_PATH}/include/zmq.h

# lz4
LZ4 = ${DEPS_PATH}/include/lz4.h
${LZ4}:
	$(eval FILE=third_party/lz4-r129.tar.gz)
	$(eval DIR=lz4-r129)
	rm -rf $(DIR)
	tar --no-same-owner -zxf $(FILE)
	cd $(DIR) && $(MAKE) && PREFIX=$(DEPS_PATH) $(MAKE) install
	rm -rf $(DIR)
lz4: | ${DEPS_PATH}/include/lz4.h

# cityhash
CITYHASH = ${DEPS_PATH}/include/city.h
${CITYHASH}:
	$(eval FILE=third_party/cityhash-1.1.1.tar.gz)
	$(eval DIR=cityhash-1.1.1)
	rm -rf $(DIR)
	tar --no-same-owner -zxf $(FILE)
	cd $(DIR) && ./configure -prefix=$(DEPS_PATH) --enable-sse4.2 && $(MAKE) CXXFLAGS="-g -O3 -msse4.2" && $(MAKE) install
	rm -rf $(DIR)
cityhash: | ${DEPS_PATH}/include/city.h

# gflags
${DEPS_PATH}/include/google/gflags.h:
	$(eval FILE=third_party/gflags-2.0-no-svn-files.tar.gz)
	$(eval DIR=gflags-2.0)
	rm -rf $(DIR)
	tar -zxf $(FILE)
	cd $(DIR) && ./configure -prefix=$(DEPS_PATH) && $(MAKE) && $(MAKE) install
	rm -rf $(DIR)
gflags: | ${DEPS_PATH}/include/google/gflags.h

# # glog
${DEPS_PATH}/include/glog/logging.h: | ${DEPS_PATH}/include/google/gflags.h
	$(eval FILE=third_party/v0.3.4.tar.gz)
	$(eval DIR=glog-0.3.4)
	rm -rf $(DIR)
	tar -zxf $(FILE)
	cd $(DIR) && ./configure -prefix=$(DEPS_PATH) --with-gflags=$(DEPS_PATH) && $(MAKE) && $(MAKE) install
	rm -rf $(DIR)
glog: | ${DEPS_PATH}/include/glog/logging.h
