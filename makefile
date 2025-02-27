


# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I./rapidjson/include
LDFLAGS = -L/opt/homebrew/Cellar/curl/8.12.1/lib -lcurl

# Target executable
TARGET = graph_crawler

# Source files
SRC = graph_crawler.cpp

# Build rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)  # <-- Add LDFLAGS here

# Clean rule (Mac-compatible)
clean:
	rm -f $(TARGET)


