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
volatile int selected = 0;
volatile char started = 0;
volatile char change_music = 0;

// prototypes
void but1_callback();
void but2_callback();
void play_buzzer(int freq);
void tone(int freq, int time);
void init(void);
int main (void);

// calbacks 
void but1_callback() {
	selected++;
	change_music = 1;
	if(selected == 3){
		selected = 0;
	}
}

void but2_callback() {
	started = !started;
}

// funcoes
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
	
	//Inicializa Buzzer como saída
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0, 0, 0);
	started = 0;
	selected = 0;
}

int main (void) {
	init();
	gfx_mono_ssd1306_init();
	songs song_choosed;
	
	songs songsOptions[] = {melodyMario, melodyGodFather, melodyTetris};
	song_choosed = songsOptions[selected];	
	int wholenote = (60000 * 4)/song_choosed.tempo;
	int i = 0;
	while(1) {
		if (started) {
			if(selected == 0){
				song_choosed = melodyMario;
				gfx_mono_draw_string("             ", 0, 3, &sysfont);
				gfx_mono_draw_string(song_choosed.name, 0, 3, &sysfont);
			} 
			if(selected == 1) {
				song_choosed = melodyGodFather;
				gfx_mono_draw_string(song_choosed.name, 0, 3, &sysfont);
			} if(selected == 2) {
				song_choosed = melodyTetris;
				gfx_mono_draw_string("                ", 0, 3, &sysfont);
				gfx_mono_draw_string(song_choosed.name, 0, 3, &sysfont);
			}
			
			note nota_atual = song_choosed.notes[i];
			int freq = nota_atual.freq;
			int divider = nota_atual.duration;
			int noteDuration = divider > 0 ? wholenote/divider : 1.5*wholenote/abs(divider);
			tone(freq, noteDuration);
			delay_ms(10);
			if (change_music) {
				i = 0;
				change_music = 0;
			}
			gfx_mono_draw_string("t/t/t/t/t", 0, 15, &sysfont);
			i++;
		}
	}
}