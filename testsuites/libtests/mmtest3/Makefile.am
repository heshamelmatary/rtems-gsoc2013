rtems_tests_PROGRAMS = mmtest3
mmtest3_SOURCES = init.c apptask1.c system.h ../../support/src/locked_print.c

dist_rtems_tests_DATA = mmtest3.scn

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

AM_CPPFLAGS += -I$(top_srcdir)/../support/include
AM_CPPFLAGS += -DSMPTEST 

LINK_OBJS = $(mmtest3_OBJECTS) $(mmtest3_LDADD)
LINK_LIBS = $(mmtest3_LDLIBS)

mmtest3$(EXEEXT): $(mmtest3_OBJECTS) $(mmtest3_DEPENDENCIES)
	  @rm -f mmtest3$(EXEEXT)
	  $(make-exe)

include $(top_srcdir)/../automake/local.am
