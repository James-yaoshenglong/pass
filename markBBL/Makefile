.PHONY: target clean

OBJ_DIR = ./build

target: $(OBJ_DIR)
	cd $(OBJ_DIR) && cmake .. && make && cp libMarkBBL.so ../../lib

$(OBJ_DIR): 
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)