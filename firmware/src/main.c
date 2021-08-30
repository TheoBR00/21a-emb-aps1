#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"


/************************************************************************/
/* defines                                                              */
/************************************************************************/

// BOTAO 1 (START)
#define BUT1_PIO				PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX			28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)

// BOTAO 2 (SELECAO)
#define BUT2_PIO				PIOC
#define BUT2_PIO_ID			ID_PIOC
#define BUT2_PIO_IDX			31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)

// BOTAO 3
#define BUT3_PIO				PIOA
#define BUT3_PIO_ID			ID_PIOA
#define BUT3_PIO_IDX			19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

// BUZZER
#define BUZZER_PIO				PIOA
#define BUZZER_PIO_ID			ID_PIOA
#define BUZZER_PIO_IDX			6
#define BUZZER_PIO_IDX_MASK (1u << BUZZER_PIO_IDX)

void set_buzzer(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_SODR = ul_mask; //PIO_SODR -> Set Output Data Register
}

void clear_buzzer(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_CODR = ul_mask; //PIO_CODR -> Clear Output Data Register
}

uint32_t  get_startstop(Pio *p_pio, const pio_type_t ul_type,
const uint32_t ul_mask)
{
	uint32_t ul_reg;

	if ((ul_type == PIO_OUTPUT_0)) {
		ul_reg = p_pio->PIO_ODSR; // ve se a saida é um ou zero
		
		} else {
		ul_reg = p_pio->PIO_PDSR; // ve se a entrada é um ou zero
	}

	if ((ul_reg & ul_mask) == 0) {
		return 0;
		} else {
		return 1;
	}
}

int get_selecao(Pio *p_pio, const pio_type_t ul_type,
const uint32_t ul_mask)
{
	uint32_t ul_reg;

	if ((ul_type == PIO_OUTPUT_0)) {
		ul_reg = p_pio->PIO_ODSR; // ve se a saida é um ou zero
		
		} else {
		ul_reg = p_pio->PIO_PDSR; // ve se a entrada é um ou zero
	}

	if ((ul_reg & ul_mask) == 0) {
		return 0;
		} else {
		return 1;
	}
	
}

void buzzer_test(int freq) {
	
}


void init(void){
	// Initialize the board clock
	sysclk_init();

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// Ativa o PIO na qual o BUZZER foi conectado
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	
	// Ativa o PIO do botão como input
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	//Inicializa Buzzer como saída
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0, 0, 0);
}


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();

  // Init OLED
	gfx_mono_ssd1306_init();
  
  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
  gfx_mono_draw_string("mundo", 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
	for (;;){
		pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
		delay_ms(200);
		pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
		delay_ms(200);
	}
	}
}