#include "rps_game.h"

// ------------------------
// Estado do jogo
// ------------------------
static RPSState state = RPS_ONGOING;
static RPSChoice player_choice = RPS_NONE;
static RPSChoice cpu_choice = RPS_NONE;
static int score_player = 0;
static int score_cpu = 0;
static unsigned int rng_seed = 2463534242u; // LCG simples

// ------------------------
// Helpers do “render”
// ------------------------
static void print_at(int row, int col, const char *s, char color) {
    for (int i = 0; s[i] != '\0'; ++i) {
        kprint_char(s[i], row, col + i, color);
    }
}

static const char* choice_to_str(RPSChoice c) {
    switch (c) {
        case RPS_ROCK:     return "PEDRA   ";
        case RPS_PAPER:    return "PAPEL   ";
        case RPS_SCISSORS: return "TESOURA ";
        default:           return "--------";
    }
}

static int decide(RPSChoice p, RPSChoice a) {
    // retorna:  1 = win, 0 = draw, -1 = lose
    if (p == a) return 0;
    if ((p == RPS_ROCK     && a == RPS_SCISSORS) ||
        (p == RPS_PAPER    && a == RPS_ROCK)     ||
        (p == RPS_SCISSORS && a == RPS_PAPER)) {
        return 1;
    }
    return -1;
}

// Linear Congruential Generator
static unsigned int lcg(unsigned int x) {
    return x * 1664525u + 1013904223u;
}

static RPSChoice cpu_random_choice(void) {
    rng_seed = lcg(rng_seed);
    return (RPSChoice)(rng_seed % 3);
}

static void mix_seed(char key_char) {
    rng_seed ^= ((unsigned int)key_char << 17) ^ ((unsigned int)key_char << 5);
}

// ------------------------
// API
// ------------------------
void rps_init_game(void) {
    clear_screen();
    state = RPS_ONGOING;
    player_choice = RPS_NONE;
    cpu_choice = RPS_NONE;
    score_player = 0;
    score_cpu = 0;
    rng_seed ^= 0x9E3779B9u; // embaralha um pouco
} // É chamada no kmain()

void rps_reset_game(void) {
    state = RPS_ONGOING;
    player_choice = RPS_NONE;
    cpu_choice = RPS_NONE;
} // Update no placar

void rps_update_screen(void) {
    // Cabeçalho
    print_at(0, 0,  "===== PEDRA  PAPEL  TESOURA =====", COLOR_DEFAULT);
    print_at(1, 0,  "Controles: [r]=Pedra  [p]=Papel  [s]=Tesoura", COLOR_DEFAULT);
    print_at(2, 0,  "          [space]=Nova rodada  [q]=Zerar placar", COLOR_DEFAULT);

    // Placar
    char placar[32];
    // formatação manual
    // "Voce: XX  CPU: YY"
    int i = 0;
    const char *pfx = "Voce: ";
    while (pfx[i]) { placar[i] = pfx[i]; i++; }
    int sp = score_player;
    placar[i++] = (char)('0' + (sp/10)%10);
    placar[i++] = (char)('0' + (sp%10));
    placar[i++] = ' ';
    placar[i++] = ' ';
    placar[i++] = 'C';
    placar[i++] = 'P';
    placar[i++] = 'U';
    placar[i++] = ':';
    placar[i++] = ' ';
    int sc = score_cpu;
    placar[i++] = (char)('0' + (sc/10)%10);
    placar[i++] = (char)('0' + (sc%10));
    placar[i] = '\0';
    print_at(4, 0, placar, COLOR_DEFAULT);

    // Escolhas
    print_at(6, 0,  "Sua escolha: ", COLOR_DEFAULT);
    print_at(6, 14, choice_to_str(player_choice), COLOR_DEFAULT);
    print_at(7, 0,  "CPU:         ", COLOR_DEFAULT);
    print_at(7, 14, choice_to_str(cpu_choice), COLOR_DEFAULT);

    // Resultado da rodada
    if (player_choice != RPS_NONE && cpu_choice != RPS_NONE) {
        int res = decide(player_choice, cpu_choice);
        if (res > 0)
            print_at(10, 0, "Voce VENCEU a rodada!", COLOR_WIN);
        else if (res < 0)
            print_at(10, 0, "Voce PERDEU a rodada!", COLOR_LOSE);
        else
            print_at(10, 0, "Voce EMPATOU com CPU!", COLOR_DRAW);
    } else {
        // dica inicial em outra linha (ex: linha 10)
        print_at(9, 0, "Pressione r/p/s para jogar.", COLOR_FAINT);
    }
}

void rps_process_input(char key_char) {
    // Cada tecla mexe no seed para “aleatorizar” um pouco
    mix_seed(key_char);

    if (key_char == 'q') {
        // Zera placar e reseta escolhas
        score_player = 0;
        score_cpu = 0;
        rps_reset_game();
        return;
    }
    if (key_char == ' ') {
        rps_reset_game();
        return;
    }

    // Só aceita jogada quando a rodada está “aberta”
    if (state == RPS_ONGOING) {
        if (key_char == 'r' || key_char == 'p' || key_char == 's') {
            // escolha do jogador
            if (key_char == 'r') player_choice = RPS_ROCK;
            if (key_char == 'p') player_choice = RPS_PAPER;
            if (key_char == 's') player_choice = RPS_SCISSORS;

            // escolha da CPU e decisão
            cpu_choice = cpu_random_choice();
            int res = decide(player_choice, cpu_choice);
            if (res > 0) score_player++;
            else if (res < 0) score_cpu++;

            state = RPS_ROUND_OVER;
        }
    } else {
        // rodada encerrada: “ ” abre outra; qualquer outra tecla ignora
        if (key_char == ' ') {
            rps_reset_game();
        }
    }
}
