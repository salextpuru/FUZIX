CC=gcc
CCOPTS=-O2 -g -Wall -Wno-char-subscripts -Wno-deprecated-declarations
TARGETS=mkfs.fuzix fsck.fuzix ucp.fuzix
UTILS=util.o devio.o xfs1.o xfs1a.o xfs1b.o xfs2.o

all:	$(TARGETS)

clean:
	rm -f $(TARGETS) *.o *~

mkfs.fuzix:	mkfs.o util.o
	$(CC) $(CCOPTS) -o $@ $< util.o

fsck.fuzix:	fsck.o util.o
	$(CC) $(CCOPTS) -o $@ $< util.o

ucp.fuzix:	ucp.o $(UTILS)
	$(CC) $(CCOPTS) -o $@ $< $(UTILS)

%.o:	%.c
	$(CC) $(CCOPTS) -c -o $@ $<
