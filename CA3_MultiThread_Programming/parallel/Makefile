CC = g++ -std=c++11
BUILD_DIR = ./Build
SRC_DIR = ./Src
INCLUDE_DIR = ./Includes

all : ImageFilters.out

ImageFilters.out : ${BUILD_DIR}/Allocate.o ${BUILD_DIR}/Average.o ${BUILD_DIR}/Cross.o ${BUILD_DIR}/Delete.o ${BUILD_DIR}/Sepia.o ${BUILD_DIR}/Smooth.o ${BUILD_DIR}/Image.o ${BUILD_DIR}/readImg.o
	${CC} ${BUILD_DIR}/Allocate.o ${BUILD_DIR}/Average.o ${BUILD_DIR}/Cross.o ${BUILD_DIR}/Delete.o ${BUILD_DIR}/Sepia.o ${BUILD_DIR}/Smooth.o ${BUILD_DIR}/Image.o ${BUILD_DIR}/readImg.o -o ImageFilters.out

${BUILD_DIR}/Image.o : ${SRC_DIR}/Image.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Image.cpp -o ${BUILD_DIR}/Image.o

${BUILD_DIR}/readImg.o : $(SRC_DIR)/readImg.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/readImg.cpp -o ${BUILD_DIR}/readImg.o

${BUILD_DIR}/Allocate.o : $(SRC_DIR)/Allocate.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Allocate.cpp -o ${BUILD_DIR}/Allocate.o

${BUILD_DIR}/Average.o : $(SRC_DIR)/Average.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Average.cpp -o ${BUILD_DIR}/Average.o

${BUILD_DIR}/Cross.o : $(SRC_DIR)/Cross.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Cross.cpp -o ${BUILD_DIR}/Cross.o

${BUILD_DIR}/Delete.o : $(SRC_DIR)/Delete.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Delete.cpp -o ${BUILD_DIR}/Delete.o

${BUILD_DIR}/Sepia.o : $(SRC_DIR)/Sepia.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Sepia.cpp -o ${BUILD_DIR}/Sepia.o

${BUILD_DIR}/Smooth.o : $(SRC_DIR)/Smooth.cpp ${INCLUDE_DIR}/Image.hpp
	${CC} -c -I ${INCLUDE_DIR} ${SRC_DIR}/Smooth.cpp -o ${BUILD_DIR}/Smooth.o

clean :
	rm *.out ${BUILD_DIR}/*