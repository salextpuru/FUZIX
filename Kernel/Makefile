# Makefile must be redable

# List of possible targets
TARGET_LIST = platform-nc100	platform-micropack platform-pcw8256 
TARGET_LIST+= platform-socz80	platform-trs80 platform-dragon
TARGET_LIST+= platform-z80pack	platform-z80pack-lite 
TARGET_LIST+= platform-zx128	platform-zxpentevo

CPU_LIST=z80 6502

# TARGET must be defined as external
ifndef TARGET
export TARGET= zxpentevo
endif

ifndef CPU
export CPU = z80
endif

export VERSION = "0.1"
export SUBVERSION = "ac1"

UNAME_S := $(shell uname -s)

ifeq ($(CPU),z80)
export CROSS_AS=sdasz80
export CROSS_LD=sdldz80
export CROSS_CC=sdcc

export CROSS_CCOPTS=-c --std-sdcc99 --no-std-crt0 -mz80 -I$(ROOT_DIR)/cpu-z80 -I$(ROOT_DIR)/platform-$(TARGET) -I$(ROOT_DIR)/include --max-allocs-per-node 100000 --opt-code-size --Werror --stack-auto --constseg CONST

export CROSS_CC_SEG2=--codeseg CODE2
export CROSS_CC_SEGDISC=--codeseg DISCARD
export CROSS_CC_FONT=--constseg FONT
export CROSS_CC_VIDEO=--codeseg VIDEO
export ASOPTS=-plosff
export BINEXT = .rel

#
#	Adjust this as needed for your platform (or contribute a script
#	to look in the usual places !)
#
ifeq ($(UNAME_S),Darwin)
export LIBZ80=/usr/local/share/sdcc/lib/z80
else
export LIBZ80=/usr/share/sdcc/lib/z80
endif

else ifeq ($(CPU),6502)
export CROSS_AS=ca65
export CROSS_LD=cl65
export CROSS_CC=cl65
export CROSS_CCOPTS=-c -Or -t c128 -I$(ROOT_DIR)/cpu-6502 -I$(ROOT_DIR)/platform-$(TARGET) -I$(ROOT_DIR)/include
export BINEXT = .o
else
export CROSS_AS=m6809-unknown-as
export CROSS_LD=lwlink
export CROSS_CC = m6809-unknown-gcc
#export CROSS_CCOPTS=-Wall -O2 -I$(ROOT_DIR)/cpu-6809 -I$(ROOT_DIR)/platform-$(TARGET) -I$(ROOT_DIR)/include
export CROSS_CCOPTS=-c -Wall -O2 -msoft-reg-count=4 -mfar-stack-param -I$(ROOT_DIR)/cpu-6809 -I$(ROOT_DIR)/platform-$(TARGET) -I$(ROOT_DIR)/include
export CROSS_CC_SEG1=-mcode-section=.text -mfar-code-page=1
export CROSS_CC_SEG2=-mcode-section=.text2 -mfar-code-page=2
export CROSS_CC_SEGDISC=-mcode-section=.discard -mfar-code-page=3
export CROSS_CC_VIDEO=-mcode-section=.video -mdata-section=.video -mfar-code-page=4
export ASOPTS=
export BINEXT = .o
endif

ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))


#
#	We put these into two code segments so that the caller can plan to
#	bank them with a ROM banking tool. We pull out const data because on
#	such a system you want to the constants unbanked otherwise badness
#	happens on stuff like printf("Hello world\n"); when you bank switch
#	the code.
#
#	This is also useful if you've got an annoying box where the kernel can't
#	be a linear chunk eg if you need to put the kernel in two non adjacent 16K
#	chunks or compile it around a hole for videomemory or similar
#
#	VT and FONT are handled specially because they can be mapped more
#	freely and also because you often want to overlay part of the kernel
#	with the framebuffer when doing video output
#

CDSRCS = start.c
C1SRCS =  version.c filesys.c devio.c kdata.c
C1SRCS += inode.c tty.c
#
#	Keep all of the syscalls in C2SRCS
#
C2SRCS =  syscall_proc.c syscall_fs.c
C2SRCS += syscall_fs2.c syscall_other.c syscall_exec.c process.c
C2SRCS += simple.c single.c bank16k.c bank32k.c bankfixed.c
C2SRCS += devsys.c mm.c swap.c usermem.c timer.c
CVIDEOSRCS = vt.c
CFONTSRCS = font4x6.c font8x8.c


ASRCS =  lowlevel-$(CPU).s usermem_std-$(CPU).s


CDOBJS = $(CDSRCS:.c=$(BINEXT))
C1OBJS = $(C1SRCS:.c=$(BINEXT))
C2OBJS = $(C2SRCS:.c=$(BINEXT))
CFONTOBJS = $(CFONTSRCS:.c=$(BINEXT))
CVIDEOOBJS = $(CVIDEOSRCS:.c=$(BINEXT))
AOBJS = $(ASRCS:.s=$(BINEXT))

CSRCS = $(CDSRCS) $(C1SRCS) $(C2SRCS) $(CFONTSRCS) $(CVIDEOSRCS)
COBJS = $(CDOBJS) $(C1OBJS) $(C2OBJS) $(CFONTOBJS) $(CVIDEOOBJS)

OBJS  = $(COBJS) $(AOBJS)


JUNK = $(CSRCS:.c=.lst) $(CSRCS:.c=.s) $(CSRCS:.c=.asm) $(CSRCS:.c=.sym) $(ASRCS:.s=.lst) $(ASRCS:.s=.sym) $(CSRCS:.c=.rst) $(CSRCS:.c=.o)  $(ASRCS:.s=.rst)

all:	fuzix.bin

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .c .s .rel

target:
	-rm platform
	ln -sf platform-$(TARGET) platform
	$(MAKE) -C platform-$(TARGET)

$(C1OBJS): %$(BINEXT): %.c
	$(CROSS_CC) $(CROSS_CCOPTS) $(CROSS_CC_SEG1) $<

$(CDOBJS): %$(BINEXT): %.c
	$(CROSS_CC) $(CROSS_CCOPTS) $(CROSS_CC_SEGDISC) $<

$(C2OBJS): %$(BINEXT): %.c
	$(CROSS_CC) $(CROSS_CCOPTS) $(CROSS_CC_SEG2) $<

$(CFONTOBJS): %$(BINEXT): %.c
	$(CROSS_CC) $(CROSS_CCOPTS) $(CROSS_CC_FONT) $<

$(CVIDEOOBJS): %$(BINEXT): %.c
	$(CROSS_CC) $(CROSS_CCOPTS) $(CROSS_CC_VIDEO) $<

$(AOBJS): %$(BINEXT): %.s
	$(CROSS_AS) $(ASOPTS) -o $*$(BINEXT) $<

version.c: makeversion
	./makeversion $(VERSION) $(SUBVERSION) $(TARGET)

tools/analysemap: tools/analysemap.c

tools/memhogs: tools/analysemap
	cp tools/analysemap tools/memhogs

tools/binman: tools/binman.c

tools/filesizes: tools/filesizes.c

tools/make4x6: tools/make4x6.c

font4x6.c: tools/make4x6
	tools/make4x6 >font4x6.c

ifeq ($(CPU), z80)
LIBZ80+=$(addprefix -k , $(shell ./tools/sdcc-path z80 lib) )
uzi.ihx: target $(OBJS) platform-$(TARGET)/uzi.lnk
	$(CROSS_LD) -n -k $(LIBZ80) -f platform-$(TARGET)/uzi.lnk

fuzix.bin: uzi.ihx tools/analysemap tools/memhogs tools/binman
	tools/analysemap <uzi.map
	-cp hogs.txt hogs.txt.old
	tools/memhogs <uzi.map |sort -nr >hogs.txt
	head -5 hogs.txt
	makebin -s 65536 -p uzi.ihx >uzi.tmp
	tools/binman uzi.tmp uzi.map fuzix.bin
	$(MAKE) -C platform-$(TARGET) image
else
fuzix.bin: target $(OBJS)
	$(MAKE) -C platform-$(TARGET) image
endif

clean:
	rm -f $(OBJS) $(JUNK) uzi.cdb uzi.com uzi.tmp platform fuzix.bin uzi.map uzi.noi uzi.ihx core *~ include/*~ version.c tools/make4x6 tools/analysemap tools/memhogs tools/binman hogs.txt hogs.txt.old tools/*~
	+make -C platform-$(TARGET) clean

clean-all: clean
	$(foreach target,$(TARGET_LIST), make -C $(target) clean;)
