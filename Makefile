ifneq ($(KERNELRELEASE),)
obj-m := eepromm_i2c.o
else
KDIR := /home/IMD/linux
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif