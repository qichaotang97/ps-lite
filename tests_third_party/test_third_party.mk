TEST_THIRD_PARTY_SRC = $(wildcard tests_third_party/test_*.cc)
TEST_THIRD_PARTY = $(patsubst tests_third_party/test_%.cc, tests_third_party/test_%, $(TEST_THIRD_PARTY_SRC))

# -ltcmalloc_and_profiler
LDFLAGS = -Wl,-rpath,$(DEPS_PATH)/lib $(PS_LDFLAGS_SO) -pthread
tests_third_party/% : tests_third_party/%.cc build/libps.a
	$(CXX) -std=c++0x $(CFLAGS) -MM -MT tests_third_party/$* $< >tests_third_party/$*.d
	$(CXX) -std=c++0x $(CFLAGS) -o $@ $(filter %.cc %.a, $^) $(LDFLAGS)

-include tests_third_party/*.d
