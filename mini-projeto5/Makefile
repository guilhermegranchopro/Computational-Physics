EIGEN_INC := $(shell pkg-config eigen3 --cflags)
ROOT_INC := -I$(shell root-config --cflags)
ROOT_LIBS := $(shell root-config --libs)

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

C_FLAGS := -std=c++17

TARGET: rPendulum.exe

%.exe: main/%.cpp $(OBJ)
	@echo "compiling $< --> $@"
	@g++ $(C_FLAGS) $^ -o $@ -I src $(EIGEN_INC) $(ROOT_INC) $(ROOT_LIBS)

%.o: %.cpp
	@echo "compiling $< --> $@"
	@g++ $(C_FLAGS) -c $< -o $@ -I src $(EIGEN_INC) $(ROOT_INC)

dump:
	@echo $(EIGEN_INC)
	@echo $(ROOT_INC)
	@echo $(ROOT_LIBS)
	@echo $(SRC)

clean:
	@rm -fv $(wildcard main/*~)
	@rm -fv $(wildcard src/*~)
	@rm -fv $(wildcard *.exe)
	@rm -fv $(OBJ)