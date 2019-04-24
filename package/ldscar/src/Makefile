objects = ldscar.o serial.o unix.o list_ports_linux.o mqtt_handler.o test_cases.o

ldscar: $(objects)
	$(CXX) $(LDFLAGS) -lmosquitto -lpthread $(objects) -o ldscar

ldscar.o: ldscar.cpp
	$(CXX) $(CFLAGS) -c ldscar.cpp

mqtt_handler.o: mqtt_handler.cpp
	$(CXX) $(CFLAGS) -c mqtt_handler.cpp

test_cases.o : test_cases.cpp
	$(CXX) $(CFLAGS) -c test_cases.cpp

serial.o: serial.cc
	$(CXX) $(CFLAGS) -c serial.cc

list_ports_linux.o: list_ports_linux.cc
	$(CXX) $(CFLAGS) -c list_ports_linux.cc

unix.o: unix.cc
	$(CXX) $(CFLAGS) -c unix.cc

clean:
	rm *.o ldscar
