CC	:= g++

LCFLAGS = -I/usr/local/include/tesseract/ -I/usr/local/include/leptonica/
LCFLAGS += -I/usr/include/opencv/
LIBPATH	:= -L/usr/local/lib 

LIBS	:=  -lz  -lm  -ltesseract -llept `pkg-config --libs opencv`

CFLAGS	:= -Wall -g -O2  
	
%.o : %.cpp
	$(CC) $(CFLAGS) $(LCFLAGS) -c $< -o $@ 

TARGET = OCR
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))


.PHONY:	all
all:  $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lpthread $(LIBPATH) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET) *.o 
