#ifndef RAQUETE_H
#define RAQUETE_H

#include <allegro5/allegro_color.h>

/* Struct para controlar a raquete */
struct bn_raquete_t
{
    float x, y;         /* Centro da raquete. */
    float larg, alt;    /* Largura e altura da raquete. */
    ALLEGRO_COLOR cor;  /* Cor da raquete. */
};
typedef struct bn_raquete_t BN_RAQUETE;

/* Desenha a raquete no "target" atual */
void bn_desenhaRaquete(const BN_RAQUETE *raq);

#endif
