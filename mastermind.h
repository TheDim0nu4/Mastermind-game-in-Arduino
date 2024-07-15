#ifndef MASTERMIND_H
#define MASTERMIND_H

char *generate_code(bool repeat, int length);
void get_score(char *secret, char *guess, int *peg_a, int *peg_b);
void turn_off_leds();
void render_leds(const int peg_a, const int peg_b);
void play_game(char *secret);
void set();


#endif // MASTERMIND_H
