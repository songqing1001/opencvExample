
INCLUDES := -I  ../../opencv-3.4.1/install/include/opencv \
    -I ../../opencv-3.4.1/install/include

LIBS := -L ../../opencv-3.4.1/install/lib -lopencv_highgui  -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_calib3d -lopencv_flann \
    -lopencv_shape -lopencv_features2d \
    -lopencv_ml 

OBJS := correspond.o

SRC := $(patsubst %.o, %.cpp, $(OBJS)) 

OUTFILE := correspond.out

$(OUTFILE):$(OBJS)
	g++ -o $(OUTFILE) -g $< $(LIBS)

$(OBJS):$(SRC)
	g++ $(INCLUDES) -g -c $< -o $@

clean:
	rm -f ./*.o*

