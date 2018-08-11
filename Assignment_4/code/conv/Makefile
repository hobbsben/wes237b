CXX?=g++
CXXFLAGS=-O0

EXEC=fp

SRCDIR=src
OBJDIR=obj
INCLUDEDIR=include

OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

LIBS=-lc $(OPENCV_LIBS)
INCLUDES=-I$(INCLUDEDIR) $(OPENCV_INCLUDE_PATH)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

all: $(SRCS) $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(EXEC)
	rm -f $(OBJDIR)/*.o

