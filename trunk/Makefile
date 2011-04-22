CC=g++
CFLAGS=-O2 $(shell wx-config --cxxflags --unicode=yes) -D__GUI__
LDFLAGS=$(shell wx-config --libs --unicode=yes) -D__GUI__ 
SOURCES=main.cpp change.cpp process.cpp tibiapid.cpp gui.cpp history.cpp languages.cpp setup.cpp network.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=change

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS)$(OBJECTS) -o $@
	mv *.o ./Objects

.cpp.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf ./Objects/*.o $(EXECUTABLE)

install:
	mv $(EXECUTABLE) /usr/local/bin
	cp -R ./Languages /home/miziak/.linux-ip-changer

uninstall:
	rm -rf /usr/local/bin/$(EXECUTABLE)
