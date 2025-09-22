#ifndef RPS_GAME_H
#define RPS_GAME_H

// Cores
#define COLOR_DEFAULT 0x07   // cinza claro
#define COLOR_WIN     0x2F   // verde vivo = ganhou
#define COLOR_LOSE    0x4F   // vermelho = perdeu
#define COLOR_DRAW    0x1F   // azul/ciano = empatou
#define COLOR_FAINT   0x87   // cinza escuro

typedef enum {
    RPS_ROCK = 0,
    RPS_PAPER = 1,
    RPS_SCISSORS = 2,
    RPS_NONE = 3
} RPSChoice; // Facilita a lógica (switch, comparação) sem ficar usando chars 'r', 'p', 's'.

typedef enum {
    RPS_ONGOING,
    RPS_ROUND_OVER
} RPSState;

// Externs do kernel já existentes (iguais aos usados no pedra papel ou tesoura)
extern void clear_screen(void);
extern void kprint(const char *str);
extern void kprint_char(char c, int row, int col, char color);


// Essas são as quatro funções que o kernel pode chamar:
// rps_init_game → inicializa o jogo (zera placar, limpa tela, etc).
// rps_process_input → recebe cada tecla digitada e atualiza o estado do jogo.
// rps_update_screen → redesenha a interface com o estado atual.
// rps_reset_game → começa uma nova rodada, mantendo o placar.

void rps_init_game(void);
void rps_process_input(char key_char);
void rps_update_screen(void);
void rps_reset_game(void);

#endif
