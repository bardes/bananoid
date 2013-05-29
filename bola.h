#ifndef BOLA_H
#define BOLA_H

#include <allegro5/allegro.h>

/* Struct representando uma bola */
struct bn_bola_t
{
    float x, y;         /* Posição do centro da bola */
    float vx, vy;       /* Velocidade da bola (px/s) */
    float raio;         /* Raio da bola */
    ALLEGRO_COLOR cor;  /* Cor da bola */
    char efeito;        /* Usado pra desnehar a bola com efeitos especiais */
};
typedef struct bn_bola_t BN_BOLA;

void bn_desenhaBola(const BN_BOLA *b);

#endif