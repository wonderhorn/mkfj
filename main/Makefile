# http://boysenberrypi.hatenadiary.jp/entry/2014/03/15/113703

OLD_SHELL := $(SHELL)
SHELL = $(warning [Making: $@]   [Dependencies: $^]   [Changed: $?])$(OLD_SHELL)

CC      = g++
CFLAGS  = -g -MMD -MP -std=c++14 -lm `sdl2-config --cflags --libs`
LDFLAGS = `sdl2-config --cflags --libs`
LIBS    = 
INCLUDE = -I./include
DEFINE = -DGFW_MAC
CXXFLAGS = $(INCLUDE) $(LIBS) $(CFLAGS) $(DEFINE)
SRC_DIR = ./src
OBJ_DIR = ./build
SOURCES = $(shell find * -name *.cpp)
#SOURCES = $(shell ls $(SRC_DIR)/*.cpp) 
ALL_OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
OBJS = $(filter-out src/windows/% src/directx/% src/framework/win32/% src/network/% src/android/% %_, $(ALL_OBJS))
#OBJS = $(filter-out src/%win32/%, $(ALL_OBJS))
$(warning OBJS = $(OBJS))
#TARGET  = $(basename $(pwd))
TARGET  = main
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	#$(warning CC = $(CC))
	#$(warning CFLAGS = $(CFLAGS))
	#$(warning ICLUDE = $(INCLUDE))
	echo "command: $(CC) -o $@ $(OBJS) $(LDFLAGS) "
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
$(OBJ_DIR)/%.o: %.cpp
	@if [ ! -d $(OBJ_DIR) ]; \
		then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
	fi
	$(warning CC = $(CC))
	$(warning CFLAGS = $(CFLAGS))
	$(warning ICLUDE = $(INCLUDE))
	echo "command: $(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< "
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

clean:
	 $(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean
