CC = g++
CFLAGS = -I.
TARGET = test
LIBS = -lEposCmd

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(TARGET).cpp $(CFLAGS) $(LIBS)

clean:
	$(RM) $(TARGET)
