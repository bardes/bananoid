#include "bola.h"

#include <allegro5/allegro_primitives.h>


void bn_desenhaBola(const BN_BOLA *b)
{
    if(b->efeito == 'c')
    {
        al_draw_circle(b->x, b->y, b->raio, b->cor, 2);
    }
    else if(b->efeito == 't')
    {
        al_draw_filled_circle(b->x, b->y, b->raio, al_map_rgba(255, 255, 255, 128));
    }
    else
    {
        al_draw_filled_circle(b->x, b->y, b->raio, b->cor);
    }
}