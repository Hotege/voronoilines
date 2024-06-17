
CMAKE := cmake
MAKE := make

BUILDS := .builds
CDT_SUB := $(shell pwd)/CDT

.PHONY: all
all: make_CDT

prepare:
	@mkdir -p $(BUILDS)

check_CDT:
	@echo "Checking CDT"
	@if [ -d $(CDT_SUB) ]; then \
		echo "- CDT exists"; \
	else \
		echo "- CDT does not exist"; \
		echo "- Please update this submodule"; \
		exit 1; \
	fi

check_CMake:
	@echo "Checking CMake"
	@$(CMAKE) --version >/dev/null 2>&1
	@echo -n "- CMake: "
	@$(CMAKE) --version | awk 'NR==1{print $$3}'

check_env: check_CDT check_CMake

make_CDT: prepare check_env
	@echo "Making CDT"
	@mkdir -p $(BUILDS)/CDT;
	@pushd $(BUILDS)/CDT >/dev/null; \
	$(CMAKE) $(CDT_SUB)/CDT -G "Unix Makefiles" \
	-DCMAKE_INSTALL_PREFIX=install ; \
	make install; \
	popd >/dev/null

clean:
	rm -rf $(BUILDS)
