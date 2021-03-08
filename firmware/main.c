#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

void init (void) {
	board_init();
	sysclk_init();
	delay_init();
	gfx_mono_ssd1306_init();
}

int main (void)
{

  init();
  /* Insert application code here, after the board has been initialized. */

  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
  gfx_mono_draw_string("mundo", 50,16, &sysfont);

	while(1) {

	}
}
