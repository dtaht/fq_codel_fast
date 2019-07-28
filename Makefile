obj-m := sch_fq_codel_fast.o
KERNEL_VERSION := $(shell uname -r)
IDIR := /lib/modules/$(KERNEL_VERSION)/kernel/net/sched/
KDIR := /lib/modules/$(KERNEL_VERSION)/build
PWD := $(shell pwd)
VERSION := $(shell git rev-parse HEAD 2>/dev/null)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules $(if $(VERSION),LDFLAGS_MODULE="--build-id=0x$(VERSION)" CFLAGS_MODULE="-DCAKE_VERSION=\\\"$(VERSION)\\\"")

install:
	install -v -m 644 sch_fq_codel_fast.ko $(IDIR)
	depmod "$(KERNEL_VERSION)"
	[ "$(KERNEL_VERSION)" != `uname -r` ] || modprobe sch_fq_codel_fast

clean:
	rm -rf Module.markers modules.order Module.symvers sch_fq_codel_fast.ko sch_fq_codel_fast.mod.c sch_fq_codel_fast.mod.o sch_fq_codel_fast.o
