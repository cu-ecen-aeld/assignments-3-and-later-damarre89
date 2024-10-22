ifndef CROSS_COMPILE
override CROSS_COMPILE=
endif
CFLAGS = -g -Wall  

write:
	$(CROSS_COMPILE)gcc $(CFLAGS) -o writer writer.c

clean:
	rm -f writer

all: write