SRCDIR                = .
SUBDIRS               =
DLLS                  =
LIBS                  =
EXES                  = imagio.exe



### Common settings

WARNINGS			  = -Wall -Wshadow -Wpointer-arith \
						-Wwrite-strings -Wno-unknown-pragmas -Werror
CEXTRA                = $(WARNINGS) -Wbad-function-cast
CPPEXTRA              = -std=c++11 $(WARNINGS) -ggdb -g3
CPPEXTRA_IMGUI        = -Wno-class-memaccess -Wno-misleading-indentation
RCEXTRA               =
DEFINES               =
INCLUDE_PATH          = -isystem/usr/local/include -isystemimagio/libs/glfw/include \
                        -isystemimagio/libs/gl3w -isystem/usr/include/eigen3 \
                        -isystemimagio/wimgui -isystemimagio/libs/glm/dist/include \
						-isystemimagio
DLL_PATH              =
DLL_IMPORTS           =
LIBRARY_PATH          = 
LIBRARIES             = -lglut -lGL -lGLEW -lGLU -lglfw -lX11 -ldl \
						-lXxf86vm -lXrandr -lpthread -lXi -lm -lrt \
						-lXcursor -lXinerama


### imagio.exe sources and settings

imagio_exe_MODULE     = imagio.exe
imagio_exe_C_SRCS     = imagio/libs/gl3w/GL/gl3w.c
imagio_exe_CPP_SRCS   = imagio/imagio.cpp \
							imagio/imgui_opengl3/imgui_impl_glfw.cpp \
							imagio/imgui_opengl3/imgui_impl_opengl3.cpp \
							imagio/main.cpp \
							imagio/imgui/imgui_draw.cpp \
							imagio/imgui/imgui.cpp \
							imagio/imgui/imgui_widgets.cpp \
							imagio/imgui/imgui_demo.cpp \
							imagio/wimgui/window.cpp \
							imagio/wimgui/dock.cpp \
							imagio/wimgui/workspace.cpp \
							imagio/viewers/3d/painter3d.cpp \
							imagio/viewers/3d/pointcloud_painter.cpp \
							imagio/viewers/3d/meshes/mesh_painter.cpp \
							imagio/viewers/3d/view3d.cpp \
							imagio/point_cloud/point_cloud_window.cpp
imagio_exe_RC_SRCS    = # imagio/imagio.rc
imagio_exe_LDFLAGS    = 
imagio_exe_ARFLAGS    =
imagio_exe_DLL_PATH   =
imagio_exe_DLLS       =
imagio_exe_LIBRARY_PATH = $(LIBRARY_PATH)
imagio_exe_LIBRARIES  = 

imagio_exe_OBJS       = $(imagio_exe_C_SRCS:.c=.o) $(imagio_exe_CPP_SRCS:.cpp=.o)


### Global source lists

C_SRCS                = $(imagio_exe_C_SRCS)
CPP_SRCS              = $(imagio_exe_CPP_SRCS)
RC_SRCS               = $(imagio_exe_RC_SRCS)


### Tools

CC = gcc
CXX = g++
RC = wrc
AR = ar
DEFINCL = $(INCLUDE_PATH) $(DEFINES) $(OPTIONS)


### Generic targets

all: $(SUBDIRS) $(DLLS:%=%.so) $(LIBS) $(EXES)

DEPFILES := $(patsubst %.cpp,%.d,$(CPP_SRCS))
-include $(DEPFILES)
%.o: %.cpp Makefile
	@$(CXX) $(CXXFLAGS) $(CPPEXTRA) $(DEFINCL) -MMD -MP -c $< -o $@

### Build rules

.PHONY: all clean dummy

$(SUBDIRS): dummy
	@cd $@ && $(MAKE)

imgui%.cpp.o: imgui%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(CPPEXTRA) $(CPPEXTRA_IMGUI) $(DEFINCL) $<

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(CPPEXTRA) $(DEFINCL) $<

%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $(CEXTRA) $(DEFINCL) $<

.rc.res:
	$(RC) $(RCFLAGS) $(RCEXTRA) $(DEFINCL) -fo$@ $<

# Rules for cleaning

CLEAN_FILES     = y.tab.c y.tab.h lex.yy.c core *.orig *.rej \
                  \\\#*\\\# *~ *% .\\\#*

clean:: $(SUBDIRS:%=%/__clean__) $(EXTRASUBDIRS:%=%/__clean__)
	$(RM) ./.depend $(CLEAN_FILES) $(RC_SRCS:.rc=.res) $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)
	$(RM) $(DLLS:%=%.so) $(LIBS) $(EXES) $(EXES:%=%.so)

$(SUBDIRS:%=%/__clean__): dummy
	cd `dirname $@` && $(MAKE) clean

$(EXTRASUBDIRS:%=%/__clean__): dummy
	-cd `dirname $@` && $(RM) $(CLEAN_FILES)

DEFLIB = $(LIBRARY_PATH) $(LIBRARIES) $(DLL_PATH) $(DLL_IMPORTS:%=-l%)

$(imagio_exe_MODULE): $(imagio_exe_OBJS)
	$(CXX) $(imagio_exe_LDFLAGS) -o $@ $(imagio_exe_OBJS) $(imagio_exe_LIBRARY_PATH) $(imagio_exe_DLL_PATH) $(DEFLIB) $(imagio_exe_DLLS:%=-l%) $(imagio_exe_LIBRARIES:%=-l%)

