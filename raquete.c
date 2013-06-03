#include "raquete.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include <math.h>

void bn_desenhaRaquete(const BN_RAQUETE *raq)
{
    /* Desenhado o retângulo da raquete */
    al_draw_filled_rounded_rectangle(raq->x - raq->larg, raq->y - raq->alt,
                                        raq->x + raq->larg, raq->y + raq->alt,
                                        2, 2, raq->cor);
    
    /* TODO Desenhar "efeitos especiais". Ex: sombra, campo de força etc.. */
}

char bn_colideRaquete(const BN_RAQUETE *r, const BN_BOLA *b)
{
    float dx, dy;
    dx = fabs(r->x - b->x); /* Distância X */
    dy = fabs(r->y - b->y); /* Distância Y */

    /* Testando se a bola está longe de mais para estar colidindo */
    if(dx > r->larg + b->raio || dy > r->alt + b->raio)
        return 0;

    /* Testando se está tão perto que com certeza está colidindo */
    if(dx <= r->larg + b->raio && dy <= r->alt)
        return 1;
    if(dy <= r->alt + b->raio && dx <= r->larg)
        return 2;

    /* Testando colisão com os cantos */
    return (pow(dx - r->larg, 2) + pow(dy - r->alt, 2)) <= pow(b->raio, 2) ? 3 : 0;
}