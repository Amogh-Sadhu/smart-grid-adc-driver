# This must point to your build directory (where .config lives)
#KDIR := /home/amogh/smart-grid-battery-gateway/builds/beagleBone/tmp/work/beaglebone_yocto-poky-linux-gnueabi/linux-yocto/6.6.21+git/linux-beaglebone_yocto-standard-build

#for Office VM
KDIR := /home/amogh/beagleBone/tmp/work/beaglebone_yocto-poky-linux-gnueabi/linux-yocto/6.6.21+git/linux-beaglebone_yocto-standard-build
obj-m += smart_grid_adc.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean