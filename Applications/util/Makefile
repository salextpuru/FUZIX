CC = sdcc
ASM = sdasz80
AR = sdar
LINKER = sdcc

CODELOC= 0xC000

#CC_OPT = -mz80 -c --opt-code-size --std-c89 --max-allocs-per-node 2000000 -I../../Library/include
#CC_OPT = -mz80 --std-c99 -c --opt-code-size --max-allocs-per-node 20000 -I../../Library/include 
CC_OPT = -mz80 --std-c99 -c --opt-code-size -I../../Library/include 
# --oldralloc
ASM_OPT = -l -o -s
LINKER_OPT = -mz80 --nostdlib --no-std-crt0 --code-loc $(CODELOC) --data-loc  0

.SUFFIXES: .c .rel


SRCS  = init.c \
	ssh.c \
	basename.c \
	bd.c \
	cal.c \
	cat.c \
	chgrp.c \
	chmod.c \
	chown.c \
	cp.c \
	cut.c \
	date.c \
	dd.c \
	df.c \
	dirname.c \
	du.c \
	echo.c \
	ed.c \
	false.c \
	fsck.c \
	grep.c \
	id.c \
	kill.c \
	ll.c \
	ln.c \
	ls.c \
	mkdir.c \
	mknod.c \
	more.c \
	mount.c \
	mv.c \
	od.c \
	patchcpm.c \
	printenv.c \
	prtroot.c \
	pwd.c \
	rm.c \
	rmdir.c \
	sleep.c \
	su.c \
	sync.c \
	touch.c \
	tr.c \
	true.c \
	umount.c \
	uniq.c \
	uud.c \
	uue.c \
	wc.c \
	which.c \
	whoami.c

#	mkfs.c \
#	passwd.c \
#	ps.c \
#	termcap.c \
#	tget.c \

OBJS = $(SRCS:.c=.rel)

LIBS = ../../Library/libs/syslib.lib

APPS = $(OBJS:.rel=)

all: $(APPS)
	mkdir -p _install
	cp -f $(APPS) _install/

$(OBJS): $(SRCS)

.c.rel:
	$(CC) $(CC_OPT) $(@:.rel=.c)

%: %.rel
	$(LINKER) $(LINKER_OPT) ../../Library/libs/crt0.rel $<  $(LIBS) -o $@.ihex
	hex2bin  $@.ihex
	mv -f $@.bin $@

clean:
	rm -f $(OBJS) $(APPS) core *~ *.asm *.lst *.sym *.map *.noi *.lk *.ihex *.bin
	rm -rf _install

rmbak:
	rm -f *~ core

