
ECHO := echo
RM := rm
CMAKE := cmake
MAKE := make

BUILDS := .builds
CDT_SUB := $(shell pwd)/CDT

.PHONY: all
all: make_CDT make_voronoi_lines

prepare:
	@mkdir -p $(BUILDS)

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
	@mkdir -p $(BUILDS)/CDT;
	@pushd $(BUILDS)/CDT >/dev/null; \
	$(CMAKE) $(CDT_SUB)/CDT -G "Unix Makefiles" \
	-DCMAKE_INSTALL_PREFIX=install ; \
	make install; \
	popd >/dev/null

make_voronoi_lines:

clean:
	@$(RM) -rf $(BUILDS)
