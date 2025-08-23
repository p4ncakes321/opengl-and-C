BUILD_DIR := build

.PHONY: build run clean

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && cmake --build .

run: build
	./$(BUILD_DIR)/built/game

clean:
	rm -rf $(BUILD_DIR)

