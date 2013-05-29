#ifndef RAQUETE_H
#define RAQUETE_H

#include <allegro5/allegro_color.h>

#include "bola.h"

/* Struct para representar a raquete */
struct bn_raquete_t
{
    float x, y;         /* Centro da raquete. */
    float larg, alt;    /* Largura e altura da raquete. */
    float vel;          /* Velocidade da raquete em px/s */
    ALLEGRO_COLOR cor;  /* Cor da raquete. */
};
typedef struct bn_raquete_t BN_RAQUETE;

/* Desenha a raquete no "target" atual */
void bn_desenhaRaquete(const BN_RAQUETE *raq);

char bn_colidindo(const BN_RAQUETE* r, const BN_BOLA* b);

#endif
