FLAGS = -Wall -Werror -std=c++11

src = $(wildcard *.cpp)
objects = $(patsubst %.cpp, %.o, $(src))

INCLUDE = -ImysqlLib/include
LIBS = -LmysqlLib/lib -lmysql


ifeq ($(OS), Windows_NT)
	RM = del
	TARGET = main.exe
else
	RM = rm -f
	TARGET = main
endif


$(TARGET): $(objects)
	g++  $(objects) $(LIBS) $(FLAGS) -o $@


$(objects):%.o : %.cpp 
	g++ -c $(FLAGS) $(INCLUDE)  $<  -o $@

	
	
.PHONY:clean
clean:
	$(RM) $(objects)
	$(RM) $(TARGET)
	