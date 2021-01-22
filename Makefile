
.PHONY: all clean
all: 
	make -C stub/ && make -C loader/
clean:
	make -C stub/ clean
	make -C loader/ clean
