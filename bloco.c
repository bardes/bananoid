#include "bloco.h"

#include <math.h>

#include <allegro5/allegro_primitives.h>

void bn_desenhaBloco(const BN_BLOCO* b)
{
    switch(b->tipo)
    {
        case 1: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(0, 0, .5, 1));
        break;
        
        case 2: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(0, 0, .6, 1));
        break;
        
        case 3: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(0, 0, .7, 1));
        break;
        
        case 4: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(0, 0, .8, 1));
        break;

        case 5: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(0, 0, .9, 1));
        break;

        case 6: al_draw_filled_rounded_rectangle(b->x - b->larg, b->y - b->alt,
                                                    b->x + b->larg, b->y + b->alt,
                                                    2, 2, al_map_rgba_f(1, 0, 0, 1));
        al_draw_filled_circle(b->x, b->y, b->alt - 2, al_map_rgb_f(1, .7, .1));
        break;
        default: break;
    }
}

char bn_colideBloco(const BN_BLOCO* bloco, const BN_BOLA* bola)
{
    float dx, dy;
    dx = fabs(bloco->x - bola->x); /* Distância X */
    dy = fabs(bloco->y - bola->y); /* Distância Y */
    
    /* Testando se a bola está longe de mais para estar colidindo */
    if(dx > bloco->larg + bola->raio || dy > bloco->alt + bola->raio)
        return 0;
    
    /* Testando se está tão perto que com certeza está colidindo */
    if(dx <= bloco->larg + bola->raio && dy <= bloco->alt)
        return 1;
    if(dy <= bloco->alt + bola->raio && dx <= bloco->larg)
        return 2;
    
    /* Testando colisão com os cantos */
    return (pow(dx - bloco->larg, 2) + pow(dy - bloco->alt, 2)) <= pow(bola->raio, 2) ? 3 : 0;
}
