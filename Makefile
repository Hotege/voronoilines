
ECHO := echo
RM := rm
CMAKE := cmake
MAKE := make
CXX := g++

CXXFLAGS := -s -Os -static
WIN32FLAGS := 

BUILDS := .builds
CDT_SUB := $(shell pwd)/CDT
VL_OBJ_DIR := $(BUILDS)/voronoilines

OBJS := \
$(VL_OBJ_DIR)/geometry.o \
$(VL_OBJ_DIR)/randomize.o \
$(VL_OBJ_DIR)/poisson.o \
$(VL_OBJ_DIR)/voronoi.o \
$(VL_OBJ_DIR)/game.o
TESTS_EXE := \
$(VL_OBJ_DIR)/test-poisson.exe \
$(VL_OBJ_DIR)/test-voronoi.exe \
$(VL_OBJ_DIR)/test-game-map.exe \
$(VL_OBJ_DIR)/test-game.exe
EXES := \
$(VL_OBJ_DIR)/voronoilines-win32.exe

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

make_voronoi_lines: $(OBJS) $(TESTS_EXE) $(EXES)

$(VL_OBJ_DIR)/voronoilines-win32.exe: $(VL_OBJ_DIR)/voronoilines-win32.o
	@$(CXX) $(CXXFLAGS) $(WIN32FLAGS) -o $(VL_OBJ_DIR)/voronoilines-win32.exe \
	$(VL_OBJ_DIR)/voronoilines-win32.o \
	-lgdi32

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
	$(VL_OBJ_DIR)/geometry.o \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/test-game-map.exe: $(VL_OBJ_DIR)/test-game-map.o
	@$(CXX) $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-game-map.exe \
	$(VL_OBJ_DIR)/test-game-map.o \
	$(VL_OBJ_DIR)/poisson.o \
	$(VL_OBJ_DIR)/randomize.o \
	$(VL_OBJ_DIR)/voronoi.o \
	$(VL_OBJ_DIR)/geometry.o \
	$(VL_OBJ_DIR)/game.o \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/test-game.exe: $(VL_OBJ_DIR)/test-game.o
	@$(CXX) $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-game.exe \
	$(VL_OBJ_DIR)/test-game.o \
	$(VL_OBJ_DIR)/poisson.o \
	$(VL_OBJ_DIR)/randomize.o \
	$(VL_OBJ_DIR)/voronoi.o \
	$(VL_OBJ_DIR)/geometry.o \
	$(VL_OBJ_DIR)/game.o \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/geometry.o: src/geometry.cpp src/geometry.h
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/geometry.o \
	src/geometry.cpp

$(VL_OBJ_DIR)/randomize.o: src/randomize.cpp src/randomize.h
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/randomize.o \
	src/randomize.cpp

$(VL_OBJ_DIR)/poisson.o: src/poisson.cpp src/poisson.h \
$(VL_OBJ_DIR)/randomize.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/poisson.o \
	src/poisson.cpp

$(VL_OBJ_DIR)/voronoi.o: src/voronoi.cpp src/voronoi.h \
$(VL_OBJ_DIR)/geometry.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/voronoi.o \
	src/voronoi.cpp \
	-I $(BUILDS)/CDT/install/include

$(VL_OBJ_DIR)/game.o: src/game.cpp src/game.h \
$(VL_OBJ_DIR)/voronoi.o $(VL_OBJ_DIR)/randomize.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/game.o \
	src/game.cpp

$(VL_OBJ_DIR)/test-poisson.o: test/test-poisson.cpp $(VL_OBJ_DIR)/poisson.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-poisson.o \
	test/test-poisson.cpp \
	-lgdiplus -lgdi32

$(VL_OBJ_DIR)/test-voronoi.o: test/test-voronoi.cpp \
$(VL_OBJ_DIR)/poisson.o $(VL_OBJ_DIR)/voronoi.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-voronoi.o \
	test/test-voronoi.cpp

$(VL_OBJ_DIR)/test-game-map.o: test/test-game-map.cpp \
$(VL_OBJ_DIR)/poisson.o $(VL_OBJ_DIR)/voronoi.o $(VL_OBJ_DIR)/game.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-game-map.o \
	test/test-game-map.cpp

$(VL_OBJ_DIR)/test-game.o: test/test-game.cpp $(VL_OBJ_DIR)/game.o
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/test-game.o \
	test/test-game.cpp

$(VL_OBJ_DIR)/voronoilines-win32.o: src/main-win32.cpp
	@$(CXX) -c $(CXXFLAGS) -o $(VL_OBJ_DIR)/voronoilines-win32.o \
	src/main-win32.cpp

clean:
	@$(RM) -rf $(BUILDS)
