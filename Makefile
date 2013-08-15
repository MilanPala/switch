BIN=switch
CCFLAGS=-g -Wall -pedantic -lpcap -lpthread -lnet
CC=g++

all: $(BIN)

$(BIN): macaddress.o frame.o iface.o ifaces.o camtablerecord.o runswitch.o switch.o camtable.o
	${CC} -o $(BIN) macaddress.o frame.o iface.o ifaces.o camtablerecord.o camtable.o switch.o runswitch.o ${CCFLAGS}

runswitch.o: RunSwitch.cpp Switch.cpp Ifaces.cpp
	${CC} -c RunSwitch.cpp -o runswitch.o ${CCFLAGS}

switch.o: Switch.cpp Switch.hpp
	${CC} -c Switch.cpp -o switch.o ${CCFLAGS}

camtable.o: CamTable.hpp CamTable.cpp
	${CC} -c CamTable.cpp -o camtable.o ${CCFLAGS}

camtablerecord.o: CamTableRecord.cpp CamTableRecord.hpp
	${CC} -c CamTableRecord.cpp -o camtablerecord.o ${CCFLAGS}

iface.o: Iface.cpp Iface.hpp
	${CC} -c Iface.cpp -o iface.o ${CCFLAGS}

ifaces.o: Ifaces.cpp Ifaces.hpp
	${CC} -c Ifaces.cpp -o ifaces.o ${CCFLAGS}

frame.o: Frame.cpp Frame.hpp
	${CC} -c Frame.cpp -o frame.o ${CCFLAGS}

macaddress.o: MacAddress.cpp MacAddress.hpp
	${CC} -c MacAddress.cpp -o macaddress.o ${CCFLAGS}

pack:
	tar -czf xpalam00 *.cpp *.hpp Makefile readme.txt

unpack:
	tar -xf xpalam00

load:
	scp xpalam00 milan@192.168.56.101:/home/milan/switch/xpalam00

clean:
	rm -f *.o *~ ${BIN} *.output
