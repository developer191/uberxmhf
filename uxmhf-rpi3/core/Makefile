#
# uxmhf-rpi3 core Makefile
# 
# author: amit vasudevan (amitvasudevan@acm.org)
#
#

.PHONY: all
all: core.bin


entry.o : entry.s
	$(TOOLPREFIX)gcc $(CFLAGS) entry.s -o entry.o


miniuart.o : ../common/miniuart.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../common/miniuart.c -o miniuart.o

strlen.o : ../common/strlen.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../common/strlen.c -o strlen.o

subr_prf.o : ../common/subr_prf.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../common/subr_prf.c -o subr_prf.o


debug.o : ../common/debug.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../common/debug.c -o debug.o

atags.o : ../common/atags.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../common/atags.c -o atags.o

arm8-32.o : ../common/arm8-32.s
	$(TOOLPREFIX)gcc $(CFLAGS) -D__ASSEMBLY__ ../common/arm8-32.s -o arm8-32.o


hypvtablestubs.o : hypvtablestubs.s
	$(TOOLPREFIX)gcc $(CFLAGS) hypvtablestubs.s -o hypvtablestubs.o

hypvtable.o : hypvtable.c
	$(TOOLPREFIX)gcc $(CFLAGS) hypvtable.c -o hypvtable.o

mmu.o : mmu.c
	$(TOOLPREFIX)gcc $(CFLAGS) mmu.c -o mmu.o

hyppgtbl.o : hyppgtbl.c
	$(TOOLPREFIX)gcc $(CFLAGS) hyppgtbl.c -o hyppgtbl.o


s2pgtbl.o : s2pgtbl.c
	$(TOOLPREFIX)gcc $(CFLAGS) s2pgtbl.c -o s2pgtbl.o

uapi_s2pgtbl.o : uapi_s2pgtbl.c
	$(TOOLPREFIX)gcc $(CFLAGS) uapi_s2pgtbl.c -o uapi_s2pgtbl.o

dmaprot.o : dmaprot.c
	$(TOOLPREFIX)gcc $(CFLAGS) dmaprot.c -o dmaprot.o

intprot.o : intprot.c
	$(TOOLPREFIX)gcc $(CFLAGS) intprot.c -o intprot.o

secboot.o : secboot.c
	$(TOOLPREFIX)gcc $(CFLAGS) secboot.c -o secboot.o


bcm2837.o : bcm2837.c
	$(TOOLPREFIX)gcc $(CFLAGS) bcm2837.c -o bcm2837.o


ctxtrace.o : ../uapps/uapp-ctxtrace/ctxtrace.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-ctxtrace/ctxtrace.c -o ctxtrace.o

hyptimer.o : hyptimer.c
	$(TOOLPREFIX)gcc $(CFLAGS) hyptimer.c -o hyptimer.o

uapp-uhcalltest.o : ../uapps/uapp-uhcalltest/uapp-uhcalltest.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-uhcalltest/uapp-uhcalltest.c -o uapp-uhcalltest.o

uapp-utpmtest.o : ../uapps/uapp-utpmtest/uapp-utpmtest.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-utpmtest/uapp-utpmtest.c -o uapp-utpmtest.o

uapp-pa5encfs.o : ../uapps/uapp-pa5encfs/uapp-pa5encfs.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-pa5encfs/uapp-pa5encfs.c -o uapp-pa5encfs.o


uapp-watchdogsup.o : ../uapps/uapp-watchdog/uapp-watchdogsup.s
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-watchdog/uapp-watchdogsup.s -o uapp-watchdogsup.o

uapp-watchdog.o : ../uapps/uapp-watchdog/uapp-watchdog.c
	$(TOOLPREFIX)gcc $(CFLAGS) ../uapps/uapp-watchdog/uapp-watchdog.c -o uapp-watchdog.o


ghcall.o : ghcall.c
	$(TOOLPREFIX)gcc $(CFLAGS) ghcall.c -o ghcall.o


main.o : main.c
	$(TOOLPREFIX)gcc $(CFLAGS) main.c -o main.o

core.bin: entry.o miniuart.o strlen.o subr_prf.o debug.o atags.o arm8-32.o hypvtablestubs.o hypvtable.o mmu.o hyppgtbl.o s2pgtbl.o uapi_s2pgtbl.o dmaprot.o intprot.o secboot.o bcm2837.o hyptimer.o ctxtrace.o uapp-uhcalltest.o uapp-utpmtest.o uapp-pa5encfs.o uapp-watchdogsup.o uapp-watchdog.o ghcall.o main.o
	cp -f core.lscript core.lscript.c
	$(TOOLPREFIX)gcc -E -P $(CFLAGS_DEFS) core.lscript.c -o core.ls 
	$(TOOLPREFIX)ld entry.o miniuart.o strlen.o subr_prf.o debug.o atags.o arm8-32.o hypvtablestubs.o hypvtable.o mmu.o hyppgtbl.o s2pgtbl.o uapi_s2pgtbl.o dmaprot.o intprot.o secboot.o bcm2837.o hyptimer.o ctxtrace.o uapp-uhcalltest.o uapp-utpmtest.o uapp-pa5encfs.o uapp-watchdogsup.o uapp-watchdog.o ghcall.o main.o -T core.ls -o core.elf $(LIBS) $(CCLIB) 
	$(TOOLPREFIX)objcopy core.elf -O binary core.bin
	
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f core.lscript.c