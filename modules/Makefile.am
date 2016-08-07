## Process this file with automake to produce Makefile.in

libgimpbase = $(top_builddir)/libgimpbase/libgimpbase-$(GIMP_API_VERSION).la
libgimpcolor = $(top_builddir)/libgimpcolor/libgimpcolor-$(GIMP_API_VERSION).la
libgimpconfig = $(top_builddir)/libgimpconfig/libgimpconfig-$(GIMP_API_VERSION).la
libgimpmodule = $(top_builddir)/libgimpmodule/libgimpmodule-$(GIMP_API_VERSION).la
libgimpwidgets = $(top_builddir)/libgimpwidgets/libgimpwidgets-$(GIMP_API_VERSION).la

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

libdir = $(gimpplugindir)/modules

AM_CPPFLAGS = \
        -I$(top_srcdir)	\
	$(GEGL_CFLAGS)	\
        $(GTK_CFLAGS)	\
        -I$(includedir)

if PLATFORM_WIN32
else
controller_midi_module = libcontroller-midi.la
endif

if HAVE_LINUX_INPUT
controller_linux_input_module = libcontroller-linux-input.la
endif

if HAVE_DX_DINPUT
controller_dx_dinput_module = libcontroller-dx-dinput.la
endif

lib_LTLIBRARIES = \
	libdisplay-filter-lcms.la		\
	$(controller_midi_module)		\
	$(controller_linux_input_module)	\
	$(controller_dx_dinput_module)


modules_libadd = $(libgimpmodule) $(libgimpwidgets) $(GTK_LIBS)

color_selector_libadd = $(libgimpconfig) $(libgimpcolor) $(modules_libadd) $(BABL_LIBS)
display_filter_libadd = $(libgimpbase) $(libgimpconfig) $(libgimpcolor) $(modules_libadd) $(GEGL_LIBS)
controller_libadd = $(modules_libadd)

libdisplay_filter_lcms_la_SOURCES = display-filter-lcms.c
libdisplay_filter_lcms_la_LDFLAGS = -avoid-version -module $(no_undefined)
libdisplay_filter_lcms_la_LIBADD = $(display_filter_libadd)

libcontroller_linux_input_la_SOURCES = \
	gimpinputdevicestore-gudev.c	\
	gimpinputdevicestore.h		\
	controller-linux-input.c
libcontroller_linux_input_la_CFLAGS = $(GUDEV_CFLAGS)
libcontroller_linux_input_la_LDFLAGS = -avoid-version -module $(no_undefined)
libcontroller_linux_input_la_LIBADD = \
	$(controller_libadd) $(GUDEV_LIBS)

libcontroller_dx_dinput_la_SOURCES = \
	gimpinputdevicestore-dx.c	\
	gimpinputdevicestore.h		\
	controller-dx-dinput.c
# Use -Wl to avoid libtool lossage
libcontroller_dx_dinput_la_LDFLAGS = -avoid-version -module $(no_undefined) -Wl,-ldinput8 -Wl,-ldxguid
libcontroller_dx_dinput_la_LIBADD = \
	$(controller_libadd) -lrpcrt4

libcontroller_midi_la_SOURCES = controller-midi.c
libcontroller_midi_la_CFLAGS = $(ALSA_CFLAGS)
libcontroller_midi_la_LDFLAGS = -avoid-version -module $(no_undefined)
libcontroller_midi_la_LIBADD = $(controller_libadd) $(ALSA_LIBS)
