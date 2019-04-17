
ldscar: ldscar.o serial.o unix.o list_ports_linux.o
	$(CXX) $(LDFLAGS) -lmosquitto ldscar.o serial.o unix.o list_ports_linux.o -o ldscar

ldscar.o: ldscar.cpp
	$(CXX) $(CFLAGS) -c ldscar.cpp

serial.o: serial.cc
	$(CXX) $(CFLAGS) -c serial.cc

list_ports_linux.o: list_ports_linux.cc
	$(CXX) $(CFLAGS) -c list_ports_linux.cc

unix.o: unix.cc
	$(CXX) $(CFLAGS) -c unix.cc

clean:
	rm *.o ldscar
