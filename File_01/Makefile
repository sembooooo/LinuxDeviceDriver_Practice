
obj-m += chardriver.o

KDIR =/lib/modules/$(shell uname -r)/build 

all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	rm -f *.0 *.ko
