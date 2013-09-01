
rtems_tests_PROGRAMS = psxtmman01
psxtmman01_SOURCES = init.c ../../tmtests/include/timesys.h \
    ../../support/src/tmtests_empty_function.c \
    ../../support/src/tmtests_support.c

dist_rtems_tests_DATA = psxtmman01.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

OPERATION_COUNT = @OPERATION_COUNT@
AM_CPPFLAGS += -I$(top_srcdir)/../tmtests/include
AM_CPPFLAGS += -DOPERATION_COUNT=$(OPERATION_COUNT)
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(psxtmman01_OBJECTS)
LINK_LIBS = $(psxtmman01_LDLIBS)

psxtmman01$(EXEEXT): $(psxtmman01_OBJECTS) $(psxtmman01_DEPENDENCIES)
	@rm -f psxtmman01$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
