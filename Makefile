isCXX := g++
CXXFLAGS := -std=c++17 -Iinclude -pthread -Wall -Wextra
SRCS := src/main_crow.cpp src/job_portal.cpp src/Trie.cpp src/candidate.cpp
TARGET := job_portal_server

all: $(TARGET)

.PHONY: prepare-web
prepare-web:
	@echo "Preparing web assets..."
	@if [ -d web ] ; then echo "web directory present"; else mkdir -p web; fi
	# Optional: copy web/index.html to project root for compatibility
	@if [ -f web/index.html ] ; then cp web/index.html index.html || true; fi

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)
