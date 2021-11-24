CC = g++ -std=c++17 -g
BUILD_DIR = .
SRC_DIR = ./src
INCLUDE_DIR = ./include

all :
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Map_Process.cpp -o ${BUILD_DIR}/Map_Proc
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Reduce_Process.cpp -o ${BUILD_DIR}/Red_Proc
	${CC} -I ${INCLUDE_DIR} ${SRC_DIR}/Common_Tools.cpp ${SRC_DIR}/Main_Process.cpp -o Main_Proc

clean:
	rm *.out *.o *.txt