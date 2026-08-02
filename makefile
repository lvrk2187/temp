SRC = $(wildcard *.c)
trash := $(wildcard */)

temp:
	gcc $(SRC) && ./a.out 

clean:
	@echo "cleaning temporary files"
	@$(foreach dir,$(trash),rm -rf $(dir))
	
