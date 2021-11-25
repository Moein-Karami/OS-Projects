CC = g++ -std=c++17
BUILD_DIR = ./Build
SRC_DIR = ./src
INCLUDE_DIR = ./include

all :
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Map_Process.cpp -o ${BUILD_DIR}/Map_Proc.out
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Reduce_Process.cpp -o ${BUILD_DIR}/Red_Proc.out
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Main_Process.cpp -o ${BUILD_DIR}/Main_Proc.out

clean:
	rm ./Pipes/* ./Build/*