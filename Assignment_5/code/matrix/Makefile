TARGET=mm

CUDA_CHECK_ERROR?=1

CXX?=g++
CXXFLAGS=-O3

NVCC?=nvcc
NVCCFLAGS=#-gencode arch=compute_62,code=sm_62
		  #-gencode arch=compute_50,code=sm_50 \
          #-gencode arch=compute_52,code=sm_52 \
          #-gencode arch=compute_53,code=sm_53 \
		  #-gencode arch=compute_60,code=sm_60 \
		  #-gencode arch=compute_61,code=sm_61 \
		  #-gencode arch=compute_62,code=sm_62


ifeq ($(CUDA_CHECK_ERROR), 1)
	CXXFLAGS+= -DCUDA_ERROR_CHECK
	NVCCFLAGS+= -DCUDA_ERROR_CHECK
endif


INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart
CUDA_INCLUDE=-I/usr/local/cuda/include

LIBS=-lc $(CUDA_LIBS) $(OPENCV_LIBS)
INCLUDES=-I$(INCLUDEDIR) $(CUDA_INCLUDE) $(OPENCV_INCLUDE_PATH)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

SRCS_CUDA=$(wildcard $(SRCDIR)/*.cu)
OBJS_CUDA=$(patsubst $(SRCDIR)/%.cu, $(OBJDIR)/%.o, $(SRCS_CUDA))

all: $(TARGET)

$(TARGET): $(OBJS) $(OBJS_CUDA)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(OBJS_CUDA) -o $(TARGET) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cu
	$(NVCC) $(NVCCFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJDIR)/*.o
