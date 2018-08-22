CXX?=g++
CXXFLAGS=-O3
NVCC?=nvcc
NVCCFLAGS=-O3

EXEC=lab5

INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart
CUDA_INCLUDE=-I/usr/local/cuda/include

LIBS=-lc $(OPENCV_LIBS) $(CUDA_LIBS)
INCLUDES=-I$(INCLUDEDIR) $(OPENCV_INCLUDE_PATH) $(CUDA_INCLUDE)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

SRCS_CUDA=$(wildcard $(SRCDIR)/*.cu)
OBJS_CUDA=$(patsubst $(SRCDIR)/%.cu, $(OBJDIR)/%.o, $(SRCS_CUDA))

all: $(EXEC)

$(EXEC): $(OBJS) $(OBJS_CUDA)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(OBJS_CUDA) -o $(EXEC) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cu
	$(NVCC) $(NVCCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(EXEC)
	rm -f $(OBJDIR)/*.o
