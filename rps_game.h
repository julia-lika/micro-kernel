#ifndef RPS_GAME_H
#define RPS_GAME_H

// Cores no mesmo esquema do Termo
#define COLOR_DEFAULT 0x07   // cinza claro
#define COLOR_WIN     0x2F   // verde vivo
#define COLOR_LOSE    0x4F   // vermelho
#define COLOR_DRAW    0x1F   // azul/ciano
#define COLOR_FAINT   0x87   // cinza escuro

typedef enum {
    RPS_ROCK = 0,
    RPS_PAPER = 1,
    RPS_SCISSORS = 2,
    RPS_NONE = 3
} RPSChoice;

typedef enum {
    RPS_ONGOING,
    RPS_ROUND_OVER
} RPSState;

// Externs do kernel já existentes (iguais aos usados no termo_game)
extern void clear_screen(void);
extern void kprint(const char *str);
extern void kprint_char(char c, int row, int col, char color);
extern void set_cursor(int row, int col);

// API espelhando o termo_game.*
void rps_init_game(void);
void rps_process_input(char key_char);
void rps_update_screen(void);
void rps_reset_game(void);

#endif
