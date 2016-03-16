#
# Makefile
#
RM = rm -f
LIBDIR = lib
SUBDIRS = lib
lib_LIBRARIES = libailsalib.a

all: $(lib_LIBRARIES)

lib libs: $(lib_LIBRARIES)

$(lib_LIBRARIES):
	$(MAKE) -C $(LIBDIR) $@

.PHONY: clean

clean:
	for dir in $(SUBDIRS); do \
	  $(MAKE) -C $$dir clean; \
	done
