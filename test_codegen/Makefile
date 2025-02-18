

PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
override PROJ_DIR := $(abspath $(PROJ_DIR))
$(info PROJ_DIR: $(PROJ_DIR))

target:
	./build.sh target $(MAKEFLAGS)
.PHONY: target

clean:
	./build.sh clean $(MAKEFLAGS)
.PHONY: clean



distclean:
	rm -rf $(PROJ_DIR)/build
	rm -rf $(PROJ_DIR)/src/gen

.PHONY: distclean
