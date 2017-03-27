
SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = imagio.exe



### Common settings

WARNINGS			  = -Wall -Wshadow -Wpointer-arith \
						-Wwrite-strings -Wno-unknown-pragmas -Werror
CEXTRA                = $(WARNINGS) -Wbad-function-cast
CXXEXTRA              = -std=c++11 $(WARNINGS) -ggdb -g3
RCEXTRA               =
DEFINES               =
INCLUDE_PATH          = -isystem/usr/local/include -isystemimagio/libs/gl3w -isystem/usr/include/eigen3 \
						-isystem../../SKV/dist/include
DLL_PATH              =
DLL_IMPORTS           =
LIBRARY_PATH          = -L../../SKV/dist/lib/Ubuntu14.04_x86_64_gcc/
LIBRARIES             = -lglut -lGL -lGLEW -lGLU -lglfw3 -lX11 -ldl \
						-lXxf86vm -lXrandr -lpthread -lXi -lm -lrt \
						-lXcursor -lXinerama -lboost_filesystem -lboost_system \
						-lskv


### imagio.exe sources and settings

imagio_exe_MODULE     = imagio.exe
imagio_exe_C_SRCS     = imagio/libs/gl3w/GL/gl3w.c
imagio_exe_CXX_SRCS   = imagio/imagio.cpp \
							imagio/imgui_opengl3/imgui_impl_glfw_gl3.cpp \
							imagio/main.cpp \
							imagio/imgui/imgui_draw.cpp \
							imagio/imgui/imgui.cpp \
							imagio/imgui/imgui_demo.cpp \
							imagio/wimgui/window.cpp \
							imagio/wimgui/dock.cpp \
							imagio/wimgui/workspace.cpp \
							imagio/3dpaint/3dpaint.cpp \
							imagio/3dpaint/paint_window.cpp  \
							imagio/point_cloud_window.cpp
imagio_exe_RC_SRCS    = # imagio/imagio.rc
imagio_exe_LDFLAGS    = 
imagio_exe_ARFLAGS    =
imagio_exe_DLL_PATH   =
imagio_exe_DLLS       =
imagio_exe_LIBRARY_PATH =
imagio_exe_LIBRARIES  = 

imagio_exe_OBJS       = $(imagio_exe_C_SRCS:.c=.o) \
			$(imagio_exe_CXX_SRCS:.cpp=.o) \
			$(imagio_exe_RC_SRCS:.rc=.res)



### Global source lists

C_SRCS                = $(imagio_exe_C_SRCS)
CXX_SRCS              = $(imagio_exe_CXX_SRCS)
RC_SRCS               = $(imagio_exe_RC_SRCS)


### Tools

CC = gcc
CXX = g++
RC = wrc
AR = ar


### Generic targets

all: $(SUBDIRS) $(DLLS:%=%.so) $(LIBS) $(EXES)

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

# Implicit rules

.SUFFIXES: .cpp .cxx .rc .res
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)

.c.o:
	$(CC) -c $(CFLAGS) $(CEXTRA) $(DEFINCL) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CXXEXTRA) $(DEFINCL) -o $@ $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(LIBS) $(EXES) $(EXES:%=%.so)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

### Target specific build rules
DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH) $(DLL_IMPORTS:%=-l%)

$(imagio_exe_MODULE): $(imagio_exe_OBJS)
	$(CXX) $(imagio_exe_LDFLAGS) -o $@ $(imagio_exe_OBJS) $(imagio_exe_LIBRARY_PATH) $(imagio_exe_DLL_PATH) $(DEFLIB) $(imagio_exe_DLLS:%=-l%) $(imagio_exe_LIBRARIES:%=-l%)

