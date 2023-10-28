.PHONY: all run
CC := clang
CXX := clang++
LD := clang++
# CC := gcc-13
# CXX := g++-13
# LD := g++-13
AR := ar
RANLIB :=
CFLAGS := -g -O0 -Wall -MP -MMD -DGL_SILENCE_DEPRECATION
CXXFLAGS := $(CFLAGS) -std=c++20
SRCDIR := ./src
OBJDIR := ./obj
BINDIR := ./bin
INCLUDE := -I/opt/homebrew/include/ -I/usr/include/ -I/usr/local/include/ -I./src/ -I./src/vendors/
LIBS := -lm
DEFINES := 
TARGET := ./bin/App
FRAMEWORKS  := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
OPENGLLIB := -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -L/opt/homebrew/Cellar/glfw/3.3.8/lib
LDFLAGS     := -L/usr/lib -L/usr/local/lib $(OPENGLLIB) -lGLEW.2.2 -lglfw.3.3

SRC := Main.cpp
SRC := $(SRC) vendors/imgui/imgui.cpp vendors/imgui/imgui_demo.cpp vendors/imgui/imgui_draw.cpp vendors/imgui/imgui_impl_glfw.cpp 
SRC := $(SRC) vendors/imgui/imgui_tables.cpp vendors/imgui/imgui_widgets.cpp vendors/imgui/imgui_impl_opengl3.cpp
SRC := $(SRC) Renderer.cpp Shader.cpp logGL.cpp

OBJS  := $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
DEPS  := $(addprefix $(OBJDIR)/, $(SRC:.cpp=.d))

all: $(TARGET)

run:
	$(TARGET)

$(TARGET): $(OBJS)
	@echo '$$'LDFLAGS is $(LDFLAGS)
	mkdir -p $(@D)
	$(LD) $^ -o $@ $(LDFLAGS) $(FRAMEWORKS)

$(OBJDIR)/%.d:

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	# @[ -d $(OBJDIR) ]
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

debug : $(TARGET)
	@echo "debug $(TARGET)"
	lldb $(TARGET)

clean :
	rm -rf $(BINDIR) $(OBJDIR)

-include $(DEPS)
