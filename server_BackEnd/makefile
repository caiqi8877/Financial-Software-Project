RESULT = server

LIB = util.a

LIB_OBJ = date.o io.o cal.o clock.o yieldCurve.o ratings.o protocol.o history.o

MAIN_OBJ = server.o

CXX = g++

CXXFLAGS = -g

all : $(RESULT)

$(RESULT) : $(MAIN_OBJ) $(LIB)
	$(CXX) -g $(MAIN_OBJ) $(LIB) -o $(RESULT)

$(LIB) : $(LIB_OBJ)
	ar rucsv $(LIB) $(LIB_OBJ)

date.o : date.h
history.o : history.h
io.o : io.h date.h
cal.o : cal.h date.h io.h
clock.o : clock.h
yieldCurve.o : yieldCurve.h io.h date.h
ratings.o : ratings.h
server.o : $(LIB_OBJ:.o=.h) socket.h

clean: 
	-rm -f *.o *.a testdriver server