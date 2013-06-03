#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "bola.h"
#include "raquete.h"
#include "bloco.h"

#define LINHAS 4
#define MAX_COL 100

int main()
{
    /* Dados sobre a tela */
    const int lTela = 800;              /* Largura da tela */
    const int aTela = 600;              /* Altura da tela */
    const int margemHud = 48;           /* Margem para desenhar o hud no topo da tela */
    ALLEGRO_DISPLAY *tela = NULL;       /* Ponteiro para tela do jogo */
    ALLEGRO_EVENT_QUEUE *eventos = NULL;/* Ponteiro pra fila de eventos */
    ALLEGRO_FONT *fonte = NULL;

    srandom(time(NULL));
    
   /* Array contendo a situação atual de cada tecla (1 = perss, 0 = solto) */
    char teclado[ALLEGRO_KEY_MAX];
    int i;
    for(i = 0; i < ALLEGRO_KEY_MAX; ++i)
        teclado[i] = 0;
    
    /* Inicializando e verificando erro ao executar biblioteca grafica*/ 
    if(!al_init())
    {
        printf("Erro ao inicializar a biblioteca grafica!!\n");
        return 1;        
    }

    /* Inicializando o plugin de desenhos primitivos */
    if(!al_init_primitives_addon())
    {
        printf("Erro ao inicializar plugin de desenhos primitivos!\n");
        return 2;
    }

    /* Inicializando o plugin de texto */
    al_init_font_addon();
    al_init_ttf_addon();

    /* Inicializando teclado */
    if(!al_install_keyboard())
    {
        printf("Erro ao inicializar o teclado!\n");
        return 5;
    }

    /* Inicializando o mouse */
    if(!al_install_mouse())
    {
        printf("Erro ao inicializar o mouse!\n");
        return 6;
    }
    
    /* Criando a tela e armazenando na variavel tela */
    tela = al_create_display(lTela, aTela);
        
    /* Testando se a tela foi aberta */
    if(tela == NULL)
    {
        printf("Erro na criacao da tela!!\n");
        return 3;
    }
    
    /* Criando uma fila de eventos e armazenando no ponteiro eventos */
    eventos = al_create_event_queue();
    
    /* Testando se a fila foi criada */
    if(eventos == NULL)
    {
        printf("Erro na criacao da lista de eventos!!\n");
       
        /* Destruindo a tela pra devolver a memoria */
        al_destroy_display(tela);
        return 4;
    }

    /* Carregando a fonte */
    fonte = al_load_font("fonte.ttf", margemHud - 12, 0);
    
    /* Registrando fontes de eventos... */
    al_register_event_source(eventos, al_get_display_event_source(tela));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_mouse_event_source());
    
    /* Colocando o alvo dos desenhos na tela */
    al_set_target_bitmap(al_get_backbuffer(tela));

    /* Criando a raquete */
    BN_RAQUETE raq;
    raq.alt = 6.0f;    /* Altura da rquete */
    raq.larg = 48.0f;   /* Largura da raquete */
    raq.vel = 350.0f;   /* 100 pixels por segundo */
    raq.cor = al_map_rgb_f(1.0f, 0.0f, 0.0f);   /* Cor inicial da rquete */
    raq.x = lTela / 2;                  /* A raquete começa centralizada */
    raq.y = aTela - raq.alt - 8;  /* 8 pixels a cima do fundo */

    /* Criando a bola */
    BN_BOLA bola;
    bola.raio = 8.0f;
    bola.x = lTela / 2;
    bola.y = aTela / 2;
    bola.vx = 300.0f;
    bola.vy = -250.0f;
    bola.efeito = 0;
    bola.cor = al_map_rgb_f(0.0f, 1.0f, 1.0f);

    /* Variáveis usadas no loop principal */
    const double ifps = 1/60.0f; /* Duração de um frame (em segundos) */
    char mouse = 0;              /* Flag indicando se é para usar mouse ou teclado */
    char sair = 0;               /* Flag para indicar saída */
    char pause = 1;              /* Flag para pausar o jogo */
    char vidas = 3;              /* Vidas disponíveis */
    int pontos = 0;              /* Pontos obtidos pelo jogador */
    char faf = 0;                /* Avançar o jogo a um frame por segundo */
    char autoPlay = 0;           /* Controla a barra para nunca errar */
    float off = 0;               /* offset do autoPlay */
    char hud[256];

    /* Criando um bloco temporário */
    BN_BLOCO b;
    b.larg = 15.0f;
    b.alt = 6.0f;
    b.tipo = 1;
    b.y = margemHud + b.alt + 120;

    /* Criando um array de blocos */
    BN_BLOCO blocos[LINHAS][MAX_COL];
    int nColBlocos, nLinBlocos;
    const int espH = 8; /* Espaçamento horizontal */
    const int espV = 8; /* Espaçamento horizontal */
    for(nLinBlocos = 0; nLinBlocos < LINHAS; ++nLinBlocos)
    {
        nColBlocos = 0;
        b.x = 0;
        while((b.x = b.x + 2 * b.larg + espH) + b.larg < lTela && nColBlocos < MAX_COL)
        {
            /* copia o bloco */
            blocos[nLinBlocos][nColBlocos] = b;

            /* Chance de 1/30 de transformar num bloco especial... */
            blocos[nLinBlocos][nColBlocos].tipo = random() % 30 ? b.tipo : 6;

            nColBlocos++;
        }
        b.y += 2 * b.alt + espV;
        b.tipo++;
    }
    
    /* Loop principal */
    while(!sair)
    {
        double tComeco, espera;     /* Usados para regular o fps */
        tComeco = al_get_time();    /* Registra o tempo no início do frame */
        ALLEGRO_EVENT evt;          /* Evento usado para ler a fila */
        
        /* Lendo todos os eventos que aconteceram desde o último frame */
        while(al_get_next_event(eventos, &evt))
        {
            if(evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = 1;
            }
            else if(evt.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                /* Registrando que a tecla foi apertada */
                teclado[evt.keyboard.keycode] = 1;

                switch(evt.keyboard.keycode)
                {
                    
                    case ALLEGRO_KEY_M:
                    mouse = !mouse;
                    mouse ? al_hide_mouse_cursor(tela) : al_show_mouse_cursor(tela);
                    break;

                    case ALLEGRO_KEY_ESCAPE: sair = 1; break;
                    case ALLEGRO_KEY_SPACE: pause = !pause; break;
                    case ALLEGRO_KEY_ENTER: faf = !faf; break;
                    case ALLEGRO_KEY_A: autoPlay = !autoPlay; break;
                    case ALLEGRO_KEY_K: off -= 5; break;
                    case ALLEGRO_KEY_L: off += 5; break;
                }
            }
            else if(evt.type == ALLEGRO_EVENT_KEY_UP)
            {
                /* Registrando que a tecla foi solta */
                teclado[evt.keyboard.keycode] = 0;
            }
            else if(evt.type == ALLEGRO_EVENT_MOUSE_AXES && mouse && !pause)
            {
                /* Caso esteja usando o mouse, a raquete já é posicionada... */
                raq.x = evt.mouse.x;
            }
        }

        if(teclado[ALLEGRO_KEY_UP])
            raq.vel *= 1.05;
        if(teclado[ALLEGRO_KEY_DOWN])
            raq.vel /= 1.05;
        if(teclado[ALLEGRO_KEY_W])
        {
            bola.vx *= 1.05;
            bola.vy *= 1.05;
        }
        if(teclado[ALLEGRO_KEY_S])
        {
            bola.vx /= 1.05;
            bola.vy /= 1.05;
        }
        
        /* Movendo a raquete (caso esteja usando o teclado)... */
        if(teclado[ALLEGRO_KEY_LEFT] && !mouse && !pause)
            raq.x -= raq.vel * ifps;
        if(teclado[ALLEGRO_KEY_RIGHT] && !mouse && !pause)
            raq.x += raq.vel * ifps;

        /* Movendo a bola */
        if(!pause)
        {
            bola.x += bola.vx * ifps;
            bola.y += bola.vy * ifps;
        }

        if(autoPlay)
            raq.x = bola.x + off;
        
        /* Detectando colisões com as laterais e impedindo que a raquete "fuja" */
        if(raq.x - raq.larg < 0)
            raq.x = raq.larg;
        else if(raq.x + raq.larg > lTela)
            raq.x = lTela - raq.larg;

        /* Detectando e refletindo colisões da bola com as laterais */
        if(bola.x + bola.raio >= lTela)
        {
            bola.x -= 2 * (bola.x + bola.raio - lTela);
            bola.vx *= -1;
            pontos += 5;
        }
        else if(bola.x - bola.raio < 0)
        {
            bola.x += 2 * -(bola.x - bola.raio);
            bola.vx *= -1;
            pontos += 5;
        }

        /* Detectando colisões entre a raquete e a bola */
        switch(bn_colideRaquete(&raq, &bola))
        {
            /* Bateu na lataral */
            case 1: bola.vx *= -1;
            bola.x += bola.vx * ifps; break;

                /* Bateu no topo */
            case 2: bola.vy *= -1;
            bola.vx += 250 * (bola.x - raq.x) / raq.larg;
            bola.y += bola.vy * ifps; break;

            /* Bateu no canto */
            case 3: bola.vx *= -1; bola.x += bola.vx * ifps;
                    bola.vy *= -1; bola.y += bola.vy * ifps; break;
        }
        
        /* Detectando e refletindo colisões com o topo */
        if(bola.y - bola.raio < margemHud)
        {
            bola.y += 2 * -(bola.y - margemHud - bola.raio);
            bola.vy *= -1;
            pontos += 10;
        }
        else if(bola.y + bola.raio > aTela) /* Bateu no fundo */
        {
            /* TODO tocar som */
            
            /* Pausa dramática! */
            al_rest(1);
            
            /* Perde uma vida e checa game over */
            if(--vidas < 0)
            {
                sair = 1;
                al_clear_to_color(al_map_rgb_f(0, 0, 0));
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 32, 32, 0, "GAME OVER!");
                al_flip_display();
                while(1)
                {
                    al_wait_for_event(eventos, &evt);
                    if(evt.type == ALLEGRO_EVENT_KEY_DOWN || evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                        break;
                    al_rest(0.05);
                }
                
            }

            /* Perde também 40% dos pontos */
            pontos *= 0.6;

            /* Pausa e reposiciona a bola... */
            pause = 1;
            bola.x = lTela / 2;
            bola.y = aTela / 2;
            bola.vy = -250.0f;
            bola.vx = bola.vx > 0 ? 300 : -300;
        }

        /* Colisão com os blocos */
        int i, j;
        char flipX = 0, flipY = 0;
        for(i = 0; i < nLinBlocos; i++)
        {
            for(j = 0; j < nColBlocos; j++)
            {
                char colidiu;
                if(blocos[i][j].tipo && (colidiu = bn_colideBloco(&blocos[i][j], &bola)))
                {
                    /* Atribuindo pontuação e bônus */
                    switch(blocos[i][j].tipo)
                    {
                        /* Blocos comuns */
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5: pontos += ((0.2 * (LINHAS - i)) * 100); break;

                        /* Bloco especial */
                        case 6: pontos += 0.2 * (LINHAS - i) * 500; vidas++; break;
                        
                        default: break;
                    }
                    
                    /* Desativando o bloco */
                    blocos[i][j].tipo = 0;

                    /* Vendo aonde colidiu */
                    switch(colidiu)
                    {
                        case 1: flipX = 1; break;
                        case 2: flipY = 1; break;
                        case 3: flipX = flipY = 1; break;
                    }
                }
            }
        }
        if(flipX)
            bola.vx *= -1;
        if(flipY)
            bola.vy *= -1;

        /* Desenhando tudo na tela */
        al_clear_to_color(al_map_rgb_f(0, 0, 0));
        al_draw_filled_rectangle(0, margemHud - 2, lTela, margemHud, al_map_rgb_f(0.8f, 0.8f, 0.8f));
        bn_desenhaBola(&bola);

        /* Desenhando os blocos */
        for(i = 0; i < nLinBlocos; i++)
            for(j = 0; j < nColBlocos; j++)
                bn_desenhaBloco(&blocos[i][j]);
            
        bn_desenhaRaquete(&raq);
        snprintf(hud, 256, "VIDAS: %d     PONTOS: %05d", vidas, pontos);
        al_draw_text(fonte, al_map_rgb(255, 255, 255), 5, 5, 0, hud);
        al_flip_display();
        
        /* Regulando o fps */
        espera = ifps - (al_get_time() - tComeco);

        if(faf)
            al_rest(1);
        
        if(espera > 0)
            al_rest(espera);
    }
    
    /* Liberando memoria antes de fechar o programa */
    al_destroy_display(tela);
    al_destroy_event_queue(eventos);
    al_destroy_font(fonte);
    return 0;
}