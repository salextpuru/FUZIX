#include <kernel.h>
#include <kdata.h>
#include <printf.h>
#include <timer.h>
#include <devsd.h>
#include <stdbool.h>

#define NUM_DEV_SD 31

static unsigned char zsd_init()__naked;
static unsigned char zsd_cmp()__naked;
static unsigned char zsd_rdblk(unsigned char count, unsigned long nsect, void* buf)__naked;
static unsigned char zsd_wrblk(unsigned char count, unsigned long nsect, void* buf)__naked;

unsigned char sd_blockdev_count=0;

// offset block for reading (first sector of UZI image)
unsigned long part_offset;

// 
unsigned long part_size;

int sd_open(uint8_t minor, uint16_t flag){
	flag;

	if(minor < sd_blockdev_count){
        	return 0;
	} else {
		udata.u_error = EIO;
		return -1;
	}
}


int sd_close(uint8_t minor){
	minor;
	return 0;
}

int sd_read(uint8_t minor, uint8_t rawflag, uint8_t flag){
	unsigned long sec;
	irqflags_t irq;
	int r=0;
	minor; flag;
	
	if (rawflag != 0)
		return 0;
	
	if(minor >= sd_blockdev_count){
        	udata.u_error = EIO;
		return -1;
	}
	
	sec = part_offset + (unsigned long)(udata.u_buf->bf_blk);
	
	irq = di();
	if(!zsd_rdblk(1, sec, udata.u_buf->bf_data)){
		r=1;
	}
	
	irqrestore(irq);
	return (r);
}

int sd_write(uint8_t minor, uint8_t rawflag, uint8_t flag){
	unsigned long sec;
	irqflags_t irq;
	int r=0;
	minor; flag;
	
	if (rawflag != 0)
		return 0;
	
	if(minor >= sd_blockdev_count){
        	udata.u_error = EIO;
		return -1;
	}
	
	sec = part_offset + (unsigned long)(udata.u_buf->bf_blk);
	
	irq = di();
	if(!zsd_wrblk(1, sec, udata.u_buf->bf_data)){
		r=1;
	}
	
	irqrestore(irq);
	return (r);

}

//				  start    part2    sect
#define MBR_PART2_START (sector+(0x01BE + 0x0010 + 0x008))
int sd_init(){
	irqflags_t irq;
	int r=0;
	unsigned char count=0;
	
	char* sector = (unsigned char*)tmpbuf();
	
	irq = di();
	if(!(zsd_init() || zsd_cmp())){
		count++;
		//
		if(zsd_rdblk(1, 0, sector)){
			r=-1;
		}
		else{
			memcpy(&part_offset, MBR_PART2_START, sizeof(long));
		}
	}
	sd_blockdev_count=count;
	
	irqrestore(irq);
	
	brelse((bufptr)sector);
	
	kprintf("Z-Controller SD-card %u parts",sd_blockdev_count);
	
	return (r);
}


static unsigned char zsd_init()__naked{
__asm;
	call ZSD_SHAD_STORE
	call ZSD_INIT
	call ZSD_SHAD_RESTORE
	ld l,a
	ret
__endasm;
}

static unsigned char zsd_cmp()__naked{
__asm;
	call ZSD_SHAD_STORE
	call ZSD_CMP
	call ZSD_SHAD_RESTORE
	ld l,a
	ret
__endasm;
}



static unsigned char zsd_rdblk(unsigned char count, unsigned long nsect, void* buf)__naked{
	count; nsect; buf;
__asm;
	call ZSD_SHAD_STORE
	ld  iy,#0
	add iy,sp
	;// count
	ld  a,2(iy)
	;// nsect
	ld e,3(iy)
	ld d,4(iy)
	ld c,5(iy)
	ld b,6(iy)
	; // buf
	ld l,7(iy)
	ld h,8(iy)
	;
	call ZSD_RDMULTI
	call ZSD_SHAD_RESTORE
	ld l,a
	ret
__endasm;
}

static unsigned char zsd_wrblk(unsigned char count, unsigned long nsect, void* buf)__naked{
	count; nsect; buf;
__asm;
	call ZSD_SHAD_STORE
	ld  iy,#0
	add iy,sp
	;// count
	ld  a,2(iy)
	;// nsect
	ld e,3(iy)
	ld d,4(iy)
	ld c,5(iy)
	ld b,6(iy)
	; // buf
	ld l,7(iy)
	ld h,8(iy)
	;
	call ZSD_WRMULTI
	call ZSD_SHAD_RESTORE
	ld l,a
	ret
__endasm;
}
