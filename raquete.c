#include "raquete.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

void bn_desenhaRaquete(const BN_RAQUETE *raq)
{
    /* Desenhado o retângulo da raquete */
    al_draw_filled_rectangle(raq->x - (raq->larg / 2), raq->y - (raq->alt / 2),
                             raq->x + (raq->larg / 2), raq->y + (raq->alt / 2),
                             raq->cor);
    /* TODO Desenhar "efeitos especiais". Ex: sombra, campo de força etc.. */
}
