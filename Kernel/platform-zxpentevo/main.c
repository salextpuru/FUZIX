#include <kernel.h>
#include <timer.h>
#include <kdata.h>
#include <printf.h>
#include <devtty.h>

uint8_t *ramtop = PROGTOP;


void pagemap_init(void)
{
  // 0 1 2 5 - used by kernel
  
  // Add free pages
  /*
  pagemap_add(3);
  pagemap_add(4);
  pagemap_add(6);
  pagemap_add(7);
  */
  unsigned char i;
  for(i=128; i<192; i++){
	 pagemap_add(i);
  }
}

/* The uarea is already synched to the stash which is written with the
   process */
uint8_t *swapout_prepare_uarea(ptptr p)
{
  p;
  return NULL;
}

/* The switchin code will move the uarea into the process itself, we just
   need to fix up the u_page pointer */
uint8_t *swapin_prepare_uarea(ptptr p)
{
  p;
  return NULL;
}

/* On idle we spin checking for the terminals. Gives us more responsiveness
   for the polled ports */
void platform_idle(void)
{
  /* We don't want an idle poll and IRQ driven tty poll at the same moment */
  irqflags_t irq = di();
  tty_pollirq(); 
  irqrestore(irq);
}

void platform_interrupt(void)
{
 tty_pollirq();
 timer_interrupt();
}

/* Nothing to do for the map of init */
void map_init(void)
{
}
