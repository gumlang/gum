# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  libgrt_config = debug
  gum_config = debug
endif
ifeq ($(config),release)
  libgrt_config = release
  gum_config = release
endif

PROJECTS := libgrt gum

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

libgrt:
ifneq (,$(libgrt_config))
	@echo "==== Building libgrt ($(libgrt_config)) ===="
	@${MAKE} --no-print-directory -C . -f libgrt.make config=$(libgrt_config)
endif

gum: libgrt
ifneq (,$(gum_config))
	@echo "==== Building gum ($(gum_config)) ===="
	@${MAKE} --no-print-directory -C . -f gum.make config=$(gum_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f libgrt.make clean
	@${MAKE} --no-print-directory -C . -f gum.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   libgrt"
	@echo "   gum"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"