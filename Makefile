CFLAGS ?= -O2

TEST_SRC= $(wildcard tests/*.c)
TEST_BIN= $(TEST_SRC:.c=)

TEST_SXX = $(wildcard tests/*.cpp)
TEST_BXX = $(TEST_SXX:.cpp=)

all:	libdatex.so

libdatex.so: datex.c
	$(CC)  $(CFLAGS) $(INCLUDE) $(LIB) -fPIC -I./ -o $@ $^ -shared -lrt

clean:
	rm -f *\.so $(TEST_BIN) $(TEST_BXX)

tests/%: tests/%.c
	$(CC)  $(CFLAGS) $(INCLUDE) $(LIBS) -I./ -Itests/ -L./ -o $@ $^ -ldatex -lrt

tests/%: tests/%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE) $(LIBS) -I./ -Itests/ -L./ -o $@ $^ -ldatex -lrt

tests: libdatex.so $(TEST_BIN) $(TEST_SRC) $(TEST_BXX) $(TEST_SXX)
