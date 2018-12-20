
INCLUDES := -I  ../../../opencv-3.4.1/install/include/opencv \
    -I ../../../opencv-3.4.1/install/include

LIBS := -L ../../../opencv-3.4.1/install/lib -lopencv_highgui  -lopencv_core -lopencv_imgproc\
	-lopencv_imgcodecs -lopencv_calib3d  -lopencv_ml

OBJS := non_linear_svms.o

SRC := $(patsubst %.o, %.cpp, $(OBJS)) 

OUTFILE := non_linear_svms.out

$(OUTFILE):$(OBJS)
	g++ -o $(OUTFILE) $< $(LIBS)

$(OBJS):$(SRC)
	g++ $(INCLUDES) -c $< -o $@

clean:
	rm -rf ./*.o*

