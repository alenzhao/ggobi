ifdef ADMIN
 include Install/GNUmakefile.admin
endif

include local.config

ggobi:

CC = gcc
#CC = cc

# This defaults to $(CC) and is reset to CXX by any optional 
# segment that needs to use C++, e.g  USE_MYSQL 
LD=$(CXX)
LD=$(CC)

CFLAGS= -g2 -ansi -Wall -fpic -DHAVE_CONFIG_H
#CFLAGS= -g -ansi -DHAVE_CONFIG_H  # when using Irix cc
CXXFLAGS=$(CFLAGS)

ifdef TEST_KEYS
 CFLAGS+= -DTEST_KEYS=1
endif

ifndef DOXYGEN
  DOXYGEN=doxygen
endif

# used to comment out sections of code for incompletely
# implemented or buggy functionality
ifdef BINARY_IO_IMPLEMENTED
  CFLAGS+= -DBINARY_IO_IMPLEMENTED=1
endif
ifdef INFERENCE_IMPLEMENTED
  CFLAGS+= -DINFERENCE_IMPLEMENTED=1
endif
ifdef SMOOTH_IMPLEMENTED
  CFLAGS+= -DSMOOTH_IMPLEMENTED=1
endif
ifdef EDIT_EDGES_IMPLEMENTED
  CFLAGS+= -DEDIT_EDGES_IMPLEMENTED=1
endif
ifdef PRINTING_IMPLEMENTED
  CFLAGS+= -DPRINTING_IMPLEMENTED=1
endif
ifdef TS_EXTENSIONS_IMPLEMENTED
  CFLAGS+= -DTS_EXTENSIONS_IMPLEMENTED=1
endif
ifdef TOUR_PP_IMPLEMENTED
  CFLAGS+= -TOUR_PP_IMPLEMENTED=1
endif
ifdef TOUR_ADV_IMPLEMENTED
  CFLAGS+= -TOUR_ADV_IMPLEMENTED=1
endif
ifdef ROTATION_IMPLEMENTED
  CFLAGS+= -DROTATION_IMPLEMENTED=1
endif
ifdef DATAD_ADDED_SIGNAL_IMPLEMENTED
  CFLAGS+= -DDATAD_ADDED_SIGNAL_IMPLEMENTED
endif
ifdef COLORSCHEMES_IMPLEMENTED
  CFLAGS+= -DCOLORSCHEMES_IMPLEMENTED
endif

SHLIB_LDFLAGS= -shared
SHARED_LD_FLAGS= -shared
LDFLAGS=

SRC=array.c ash1d.c \
 brush_api.c brush_bins.c brush.c brush_init.c brush_link.c brush_ui.c \
 color.c color_ui.c cpanel.c \
 datad.c display.c display_tree.c display_ui.c \
 edges.c exclusion.c exclusion_ui.c \
 ggobi-API.c ggobi.c identify.c identify_ui.c \
 impute.c impute_ui.c io.c jitter.c jitter_ui.c \
 limits.c lineedit.c lineedit_ui.c \
 main_ui.c make_ggobi.c menus.c missing.c \
 movepts.c movepts_ui.c noop-checkbutton.c \
 p1d.c p1d_ui.c parcoords.c parcoords_ui.c pipeline.c \
 ppcorr_ui.c \
 read_array.c read_data.c record_id.c rotate_ui.c \
 scale_api.c scale_click.c scale_drag.c scale_ui.c \
 scatmat.c scatmat_ui.c scatterplot.c scatterplot_ui.c \
 smooth_ui.c sphere.c sphere_ui.c splash.c \
 splot.c sp_plot.c subset.c subset_ui.c svd.c \
 texture.c timeplot.c time_ui.c \
 tour1d.c tour1d_pp.c tour1d_pp_ui.c tour1d_ui.c tour_pp.c\
 tour2d.c tour2d_ui.c tour2d_pp.c tour2d_pp_ui.c \
 tour.c tourcorr.c tourcorr_ui.c \
 transform.c transform_ui.c \
 utils.c utils_gdk.c utils_ui.c \
 varchange.c varcircles.c varpanel_ui.c vartable.c vartable_ui.c \
 vector.c wvis.c wvis_ui.c win32_draw.c \
 writedata.c writedata_ui.c write_svg.c \
 xyplot.c xyplot_ui.c \
\
 gtkextruler.c gtkexthruler.c gtkextvruler.c \
 mt19937-1.c cokus.c \
 fileio.c plugin.c print.c \
 xlines.c

OB=array.o ash1d.o \
 brush_api.o brush_bins.o brush.o brush_init.o brush_link.o brush_ui.o \
 color.o color_ui.o cpanel.o \
 datad.o display.o display_tree.o display_ui.o \
 edges.o exclusion.o exclusion_ui.o \
 ggobi-API.o ggobi.o identify.o identify_ui.o \
 impute.o impute_ui.o io.o jitter.o jitter_ui.o \
 limits.o lineedit.o lineedit_ui.o \
 main_ui.o make_ggobi.o menus.o missing.o \
 movepts.o movepts_ui.o noop-checkbutton.o \
 p1d.o p1d_ui.o parcoords.o parcoords_ui.o pipeline.o \
 ppcorr_ui.o \
 read_array.o read_data.o record_id.o rotate_ui.o \
 scale_api.o scale_click.o scale_drag.o scale_ui.o \
 scatmat.o scatmat_ui.o scatterplot.o scatterplot_ui.o \
 smooth_ui.o sphere.o sphere_ui.o splash.o \
 splot.o sp_plot.o subset.o subset_ui.o svd.o \
 texture.o timeplot.o time_ui.o \
 tour1d.o tour1d_pp.o tour1d_pp_ui.o tour1d_ui.o tour_pp.o\
 tour2d.o tour2d_ui.o tour2d_pp.o tour2d_pp_ui.o \
 tour.o tourcorr.o tourcorr_ui.o \
 transform.o transform_ui.o \
 utils.o utils_gdk.o utils_ui.o \
 varchange.o varcircles.o varpanel_ui.o vartable.o vartable_ui.o \
 vector.o wvis.o wvis_ui.o win32_draw.o \
 writedata.o writedata_ui.o write_svg.o \
 xyplot.o xyplot_ui.o \
\
 gtkextruler.o gtkexthruler.o gtkextvruler.o \
 fileio.o plugin.o print.o \
 xlines.o 


ifdef USE_XML
 XML_SRC= read_xml.c write_xml.c  read_init.c write_state.c read_color.c
 XML_OB= read_xml.o write_xml.o read_init.o write_state.o read_color.o

# XML_FLAGS+= -DSUPPORT_PLUGINS=1 -DSUPPORT_INIT_FILES=1
 CFLAGS+= $(XML_INC_DIRS:%=-I%) -DUSE_XML=$(USE_XML) $(XML_FLAGS) -DSUPPORT_PLUGINS=1 -DSUPPORT_INIT_FILES=1

 SRC+=$(XML_SRC)
 OB+= $(XML_OB)

ifdef DL_RESOLVE_FLAG
 DL_RESOLVE_PATH+=$(XML_LIB_DIRS:%=$(DL_RESOLVE_FLAG) %)
endif
ifeq ($(USE_XML),2)
 XML_LIB_NO=2
endif
 XML_LIBS=$(XML_LIB_DIRS:%=-L%) -lxml$(XML_LIB_NO) -lz 

main_ui.o: write_xml.h
read_xml.o: read_xml.h
endif

OB+=mt19937-1.o cokus.o  

ifdef USE_DBMS
 SRC+= dbms.c dbms_ui.c
 OB+= dbms.o dbms_ui.o
endif

ggobi: $(OB) $(EXTRA_OB)
	$(LD) $(OB) $(EXTRA_OB) $(LDFLAGS) -o ggobi $(XML_LIBS) $(MYSQL_LIBS)  $(EXTRA_LIBS) `gtk-config --cflags --libs`  $(DL_RESOLVE_PATH)


pure: ggobi.o $(OB)
	purify -cache-dir=/tmp  -always-use-cache-dir=yes \
	$(LD) $(OB) $(LDFLAGS) -o ggobi $(XML_LIBS) $(MYSQL_LIBS) `gtk-config --cflags --libs`  $(DL_RESOLVE_PATH)


%.sched: %.c
	$(CC) -dS -c $(CFLAGS) -I. `gtk-config --cflags` $*.c

ggobi.sched: $(OB)
	$(CC) -S $(OB) $(LDFLAGS)  $(XML_LIB_DIRS:%=-L%) $(XML_LIBS) `gtk-config --cflags --libs`

mt19937-1.o: mt19937-1.c
	$(CC) $(CFLAGS) `gtk-config --cflags` -c $<

efence: $(OB)
	MALLOC_CHECK_=2
	$(CC) $(OB) $(LDFLAGS) $(EFENCE_LIBS) $(XML_LIB_DIRS:%=-L%) $(XML_LIBS) `gtk-config --cflags --libs`

lib: libggobi.so
libggobi.so: $(OB)
	$(CC) -g $(SHARED_LD_FLAGS) -o $@ $(OB) $(XML_LIB_DIRS:%=-L%) $(XML_LIBS) `gtk-config --libs` $(DL_RESOLVE_PATH)


ggobi-API.o ggobi.o: config.h

config.h: developerConfigure
	./developerConfigure

developerConfigure: developerConfigure.in
	autoconf $< > $@
	chmod +x $@


clean: 
	rm -f *.o ggobi libggobi.so

.c.o:
	$(CC) -c -I. $(CFLAGS) `gtk-config --cflags`  $*.c

# A version that compiles all C code (except for mt19937-1.c) as 
# C++.
#	$(CXX) -c $(CXXFLAGS) -I. `gtk-config --cflags` $*.c


ifdef DEPENDS_FLAG
depends: $(SRC)
	$(CC) $(DEPENDS_FLAG) $(CFLAGS) -I. `gtk-config --cflags` $(SRC) > $@	

include depends

endif


# If USE_XML, may need to add XML_LIB_DIRS and XML_INC_DIRS
local.config:
	@echo "# Whether to enable support for reading XML data" > $@
	@echo "# USE_XML=1" >> $@
	@echo "# XML_INC_DIRS=" >> $@
	@echo "# XML_LIB_DIRS=" >> $@
	@echo "# Location of dmalloc" >> $@
	@echo "# DM=1" >> $@


ifdef USE_XML
xmlConvert: xmlConvert.o libggobi.so
	$(CC) -g -o $@ xmlConvert.o $(XML_LIBS) $(XML_LIB_DIRS:%=-L%) -L. -lggobi $(DL_RESOLVE_PATH) -lgtk

make_ggobi.o: read_xml.h
endif

dbms_ui.o: dbms_ui.c dbms_ui.h dbms.h
dbms.c: dbms.h  ggobi.h


ifdef USE_PROPERTIES
dbms_ui.o: dbms_ui.c dbms_ui.h
	$(CXX) `gtk-config --cflags` $(CFLAGS) -c $<
endif


# Emacs's tags for navigating through the source.
TAGS etags: $(SRC)
	etags $(SRC)

# The version for vi
tags:
	tags $(SRC)


datad.o read_xml.o: datad.c datad.h


../bin/ggobi: ggobi
	cp ggobi $@


print.o: print.c print.h

%.o: %.d

%.d: %.c
	$(CC) -M $(CFLAGS) -I. `gtk-config --cflags` $< > $@

# Where is the output?
apiDoc: Install/apiDocConfig
	$(DOXYGEN) Install/apiDocConfig

# DO NOT DELETE
