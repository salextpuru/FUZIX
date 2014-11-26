# Top level makefile for library

tools/syscall: tools/syscall.c ../Kernel/include/syscall_name.h
	$(CC) $(CFLAGS) -I../Kernel/include tools/syscall.c -o tools/syscall
	$(CC) $(CFLAGS) -I../Kernel/include tools/binman.c -o tools/binman
	$(CC) $(CFLAGS) -I../Kernel/include tools/fcc.c -o tools/fcc
	$(CC) $(CFLAGS) -I../Kernel/include tools/fsize.c -o tools/fsize

clean:
	rm -f tools/syscall tools/binman tools/fcc tools/fsize


