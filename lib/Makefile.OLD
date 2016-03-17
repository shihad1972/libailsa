CPPFLAGS += -I../include/

lib_LIBRARIES = libailsalib.a

lib libs: $(lib_LIBRARIES)

$(lib_LIBRARIES): libailsalib.a(data.o) libailsalib.a(generic.o)

libailsalib.a(data.o): data.o
	$(AR) $(ARFLAGS) $@ $<

libailsalib.a(generic.o): generic.o
	$(AR) $(ARFLAGS) $@ $<

.PHONY:	clean

clean:
	$(RM) *.a *.o
