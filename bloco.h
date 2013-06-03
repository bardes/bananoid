#ifndef BLOCO_H
#define BLOCO_H

#include "bola.h"

struct bn_bloco_t
{
    float x, y;         /* Posição do bloco */
    float larg, alt;    /* Largura e altura do bloco */
    char tipo;          /* Tipo do bloco */
};
typedef struct bn_bloco_t BN_BLOCO;

/* Desenha o bloco passado como arqumento... */
void bn_desenhaBloco(const BN_BLOCO *b);

/* Detecta colisão entre uma bola e um bloco */
char bn_colideBloco(const BN_BLOCO *bloco, const BN_BOLA *bola);
#endif