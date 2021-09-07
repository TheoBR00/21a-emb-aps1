#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include "notes.h"
#include "songs.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// LED1 - Left Led
#define LED1_PIO				PIOA
#define LED1_PIO_ID				ID_PIOA
#define LED1_PIO_IDX			0
#define LED1_PIO_IDX_MASK		(1 << LED1_PIO_IDX)

// BOTAO 1 (START)
#define BUT1_PIO				PIOD
#define BUT1_PIO_ID				ID_PIOD
#define BUT1_PIO_IDX			28
#define BUT1_PIO_IDX_MASK		(1u << BUT1_PIO_IDX)

// BOTAO 2 (SELECAO)
#define BUT2_PIO				PIOC
#define BUT2_PIO_ID				ID_PIOC
#define BUT2_PIO_IDX			31
#define BUT2_PIO_IDX_MASK		(1u << BUT2_PIO_IDX)

// BOTAO 3
#define BUT3_PIO				PIOA
#define BUT3_PIO_ID				ID_PIOA
#define BUT3_PIO_IDX			19
#define BUT3_PIO_IDX_MASK		(1u << BUT3_PIO_IDX)

// BUZZER
#define BUZZER_PIO				PIOA
#define BUZZER_PIO_ID			ID_PIOA
#define BUZZER_PIO_IDX			6
#define BUZZER_PIO_IDX_MASK		(1u << BUZZER_PIO_IDX)

// flags 
volatile char flag1 = 0;
volatile char started = 0;
volatile char flag3;

 
// calbacks 
void but1_callback() {
	flag1++;
	if(flag1 == 2){
		flag1 = 0;
	}
}

void but2_callback() {
	started = !started;
}

// funcoes

// pisca led
void pisca_led(int n, int t){
	for (int i=0; i<n; i++){
		pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
		delay_ms(t);
		pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
		delay_ms(t);
	}
}

void play_buzzer(int freq) {
	pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
	pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
	delay_us((1e6)/freq);
	pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
	pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
	delay_us((1e6)/freq);
}


// freq = 1/T
// time = s
// t = time * freq (segundos)
void tone(int freq, int time){
	int tempo_musica = time*freq/1E3;
	for (int i = 0; i < tempo_musica; i++){
		play_buzzer(freq);
	}
}

void init(void) {
	sysclk_init();
	WDT->WDT_MR = WDT_MR_WDDIS;
	board_init();
	delay_init();
	gfx_mono_ssd1306_init();
	
	// Configura led
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
	
	// buttons
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but1_callback);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but2_callback);
	
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_EnableIRQ(BUT2_PIO_ID);
	
	NVIC_SetPriority(BUT1_PIO_ID, 4); // Prioridade 4
	NVIC_SetPriority(BUT2_PIO_ID, 4); // Prioridade 4

	// buzzer
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	
	//Inicializa Buzzer como sa�da
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0, 0, 0);
	started = 0;
	flag1 = 0;
}

int tempo = 80;
int tempoM = 250;
int divider = 0, noteDuration = 0;

int notes = sizeof(melodyMario.notes) / sizeof(melodyMario.notes[0][0]) / 2;

int thisNote = 0;

int main (void) {
	init();
	
	songs songsOptions[] = {melodyMario, melody};
	
	int wholenote = (60000 * 4) / melodyMario.tempo;
	
	gfx_mono_ssd1306_init();	
	while(1) {
		if (started) {
			
			//int thisNote = 0;
			if(flag1 == 0){
				gfx_mono_draw_string("             ", 0, 5, &sysfont);
				delay_ms(10);
				gfx_mono_draw_string("Mario Bros.", 0, 5, &sysfont);
				while((thisNote < notes * 2 ) & started & (flag1 == 0)){
					
					divider = melodyMario.notes[thisNote][1];
					
					int noteDuration = divider > 0 ? wholenote/divider : 1.5*wholenote/abs(divider);
					
					tone(melodyMario.notes[thisNote][0], noteDuration*0.9);
					delay_ms(noteDuration*0.2);
					thisNote++;
				}
			}
			if(flag1 == 1){
				
				gfx_mono_draw_string("The Godfather", 0, 5, &sysfont);
				thisNote = 0;
				while((thisNote < notes * 2 ) & started & (flag1 == 1)){
					
					divider = melody.notes[thisNote][1];
					
					int noteDuration = divider > 0 ? wholenote/divider : 1.5*wholenote/abs(divider);
					
					tone(melody.notes[thisNote][0], noteDuration*0.9);
					delay_ms(noteDuration*0.2);
					thisNote++;
				}
			}
			
				
				
			}
			//for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
				// calculates the duration of each note
			//	divider = melodyMario[thisNote + 1];
			//	if (divider > 0) {
					// regular note, just proceed
			//		noteDuration = (wholenote) / divider;
			//		} else if (divider < 0) {
					// dotted notes are represented with negative durations!!
			//		noteDuration = (wholenote) / abs(divider);
			//		noteDuration *= 1.5; // increases the duration in half for dotted notes
			//	}
			//	tone(melodyMario[thisNote], noteDuration*0.9);
			//	delay_ms(noteDuration*0.2);
			//}
		//}
		//pisca_led(1, 200);
	}
}