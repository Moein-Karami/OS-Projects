CC = g++ -std=c++11
BUILD_DIR = ./Build
SRC_DIR = ./Src
INCLUDE_DIR = ./Includes

all : ImageFilters.out

ImageFilters.out : ${BUILD_DIR}/Image.o ${BUILD_DIR}/readImg.o
	${CC} ${BUILD_DIR}/Image.o ${BUILD_DIR}/readImg.o -o ImageFilters.out

${BUILD_DIR}/Image.o : ${SRC_DIR}/Image.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Image.cpp -o ${BUILD_DIR}/Image.o

${BUILD_DIR}/readImg.o : $(SRC_DIR)/readImg.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/readImg.cpp -o ${BUILD_DIR}/readImg.o

clean :
	rm *.out
	rm ${BUILD_DIR}/*
