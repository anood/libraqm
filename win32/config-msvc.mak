# NMake Makefile portion for enabling features for Windows builds


# GLib is required for all utility programs and tests
RAQM_GLIB_LIBS = glib-2.0.lib

# Freetype is needed for building FreeType support and hb-view
FREETYPE_LIB = freetype.lib
HARFBUZZ_LIB = harfbuzz.lib
FRIBIDI_LIB = fribidi.lib


# Please do not change anything beneath this line unless maintaining the NMake Makefiles
# Bare minimum features and sources built into HarfBuzz on Windows
RAQM_DEFINES =
RAQM_CFLAGS = /DHAVE_CONFIG_H
RAQM_SOURCES =	\
        $(RAQM_BASE_sources)	\

RAQM_HEADERS =	\
        $(RAQM_INCLUDE_headers)



# We build the Raqm DLL/LIB at least
RAQM_LIBS = $(CFG)\$(PLAT)\raqm.lib

# Note: All the utility and test programs require GLib support to be present!
RAQM_UTILS =
RAQM_UTILS_DEP_LIBS = $(RAQM_GLIB_LIBS)
RAQM_TESTS =
RAQM_TESTS_DEP_LIBS = $(RAQM_GLIB_LIBS)

# Use libtool-style DLL names, if desired
!if "$(LIBTOOL_DLL_NAME)" == "1"
RAQM_DLL_FILENAME = $(CFG)\$(PLAT)\libraqm-0

RAQM_UTILS_DEP_LIBS = $(RAQM_UTILS_DEP_LIBS) $(FREETYPE_LIB) $(HARFBUZZ_LIB) $(FRIBIDI_LIB)
!endif

# Enable freetype if desired
!if "$(FREETYPE)" == "1"
RAQM_DEFINES = $(RAQM_DEFINES) /DHAVE_FREETYPE=1
RAQM_SOURCES = $(RAQM_SOURCES)
RAQM_HEADERS = $(RAQM_HEADERS)
RAQM_DEP_LIBS =  $(FREETYPE_LIB) $(HARFBUZZ_LIB) $(FRIBIDI_LIB)
!endif


# Enable GLib if desired
!if "$(GLIB)" == "1"
RAQM_DEFINES = $(RAQM_DEFINES) /DHAVE_GLIB=1
RAQM_CFLAGS =	\
	$(RAQM_CFLAGS)					\
	/FImsvc_recommended_pragmas.h	\
	/I$(PREFIX)\include\glib-2.0	\
	/I$(PREFIX)\lib\glib-2.0\include

RAQM_SOURCES = $(RAQM_SOURCES)
RAQM_HEADERS = $(RAQM_HEADERS)
RAQM_DEP_LIBS =  $(RAQM_GLIB_LIBS)

RAQM_UTILS = \
        $(RAQM_UTILS)

RAQM_TESTS = \
        $(RAQM_TESTS)

!else
# If there is no GLib support, use the built-in UCDN
# and define some of the macros in GLib's msvc_recommended_pragmas.h
# to reduce some unneeded build-time warnings
RAQM_DEFINES = $(RAQM_DEFINES) /DHAVE_UCDN=1
RAQM_CFLAGS =	\
	$(RAQM_CFLAGS)					\
	/wd4244							\
	/D_CRT_SECURE_NO_WARNINGS		\
	/D_CRT_NONSTDC_NO_WARNINGS

RAQM_SOURCES = $(RAQM_SOURCES) $(LIBHB_UCDN_sources) $(HB_UCDN_sources)
!endif


RAQM_LIB_CFLAGS = $(RAQM_CFLAGS) /DHB_EXTERN="__declspec (dllexport) extern"
