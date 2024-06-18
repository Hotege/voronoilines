
ECHO := echo
RM := rm
CMAKE := cmake
MAKE := make
CXX := g++

CXXFLAGS := -s -Os -static

BUILDS := .builds
CDT_SUB := $(shell pwd)/CDT
VL_OBJ_DIR := $(BUILDS)/voronoilines

OBJS := $(VL_OBJ_DIR)/randomize.o \
$(VL_OBJ_DIR)/poisson.o \
$(VL_OBJ_DIR)/voronoi.o
TESTS := $(VL_OBJ_DIR)/test-poisson.o \
$(VL_OBJ_DIR)/test-voronoi.cpp
TESTS_EXE := $(VL_OBJ_DIR)/test-poisson.exe \
$(VL_OBJ_DIR)/test-voronoi.exe

.PHONY: all
all: make_CDT make_voronoi_lines

prepare:
	@mkdir -p $(BUILDS)
	@mkdir -p $(BUILDS)/CDT
	@mkdir -p $(BUILDS)/voronoilines

check_CDT:
	@$(ECHO) "Checking CDT"
	@if [ -d $(CDT_SUB) ]; then \
		$(ECHO) "- CDT exists"; \
	else \
		$(ECHO) "- CDT does not exist"; \
		$(ECHO) "- Please update this submodule"; \
		exit 1; \
	fi

check_CMake:
	@$(ECHO) "Checking CMake"
	@$(CMAKE) --version >/dev/null 2>&1
	@$(ECHO) -n "- CMake: "
	@$(CMAKE) --version | awk 'NR==1{print $$3}'

check_env: check_CDT check_CMake

make_CDT: prepare check_env
	@$(ECHO) "Making CDT"
	@pushd $(BUILDS)/CDT >/dev/null; \
	$(CMAKE) $(CDT_SUB)/CDT -G "Unix Makefiles" \
	-DCMAKE_INSTALL_PREFIX=install ; \
	make install; \
	popd >/dev/null

make_voronoi_lines: $(OBJS) $(TESTS_EXE)

$(VL_OBJ_DIR)/test-poisson.exe: $(VL_OBJ_DIR)/test-poisson.o
	@$(CXX) $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-poisson.exe \
	$(VL_OBJ_DIR)/test-poisson.o \
	$(VL_OBJ_DIR)/randomize.o \
	$(VL_OBJ_DIR)/poisson.o \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/test-voronoi.exe: $(VL_OBJ_DIR)/test-voronoi.o
	@$(CXX) $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-voronoi.exe \
	$(VL_OBJ_DIR)/test-voronoi.o \
	$(VL_OBJ_DIR)/poisson.o \
	$(VL_OBJ_DIR)/randomize.o \
	$(VL_OBJ_DIR)/voronoi.o \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/randomize.o: src/randomize.cpp
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/randomize.o \
	src/randomize.cpp

$(VL_OBJ_DIR)/poisson.o: src/poisson.cpp $(VL_OBJ_DIR)/randomize.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/poisson.o \
	src/poisson.cpp

$(VL_OBJ_DIR)/voronoi.o: src/voronoi.cpp
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/voronoi.o \
	src/voronoi.cpp \
	-I $(BUILDS)/CDT/install/include

$(VL_OBJ_DIR)/test-poisson.o: test/test-poisson.cpp $(VL_OBJ_DIR)/poisson.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-poisson.o \
	test/test-poisson.cpp \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/test-voronoi.o: test/test-voronoi.cpp \
$(VL_OBJ_DIR)/poisson.o $(VL_OBJ_DIR)/voronoi.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-voronoi.o \
	test/test-voronoi.cpp

clean:
	@$(RM) -rf $(BUILDS)
