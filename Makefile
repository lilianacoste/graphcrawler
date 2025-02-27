

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./rapidjson/include
LDFLAGS = -L/usr/lib64 -lcurl  # Ensure correct path

TARGET = graph_crawler
SRC = graph_crawler.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)  # Correctly include LDFLAGS

clean:
	rm -f $(TARGET)

