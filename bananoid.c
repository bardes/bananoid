#include <stdio.h>
#include <allegro5/allegro.h>

#include "raquete.h"

int main()
{
    
    ALLEGRO_DISPLAY *tela = NULL;           /*Ponteiro para tela do jogo*/
    ALLEGRO_EVENT_QUEUE *eventos = NULL;    /*Ponteiro pra fila de eventos*/
    
    BN_RAQUETE r;
    r.x = 300;
    r.y = 650;
    r.alt = 16;
    r.larg = 128;
    
    /*Inicializando e verificando erro ao executar biblioteca grafica*/ 
    if(!al_init())
    {
        printf("Erro ao inicializar a biblioteca grafica!!\n");
        return 1;        
    }
    
    /*Criando a tela e armazenando na variavel tela*/
    tela = al_create_display(800, 600);
        
    /*Testando se a tela foi aberta*/
    if(tela == NULL)
    {
        printf("Erro na criacao da tela!!\n");
        return 2;
    }
    
    /*Criando uma fila de eventos e armazenando no ponteiro eventos*/
    eventos = al_create_event_queue();
    
    /*Testando se a fila foi criada*/
    if(eventos == NULL)
    {
        printf("Erro na criacao da lista de eventos!!\n");
       
        /*Destruindo a tela pra devolver a memoria*/
        al_destroy_display(tela);
        return 3;
    }
    
    /*Registrar a fonte de eventos da tela*/
    al_register_event_source(eventos, al_get_display_event_source(tela));
    
    al_set_target_bitmap(tela);
    
    while (1)
    {
        ALLEGRO_EVENT evt;
        ALLEGRO_TIMEOUT espera;
        al_init_timeout(&espera, 0.06);
 
        char get_event = al_wait_for_event_until(eventos, &evt, &espera);
        
        if(get_event && evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
        {
            break;
        }
        
        /* Desenhando tuti */
        al_clear_to_color(al_map_rgb(0,0,0));
        bn_desenhaRaquete(&r);
        al_flip_display();        
    }
    
    /*Liberando memoria antes de fechar o programa*/
    al_destroy_display(tela);
    al_destroy_event_queue(eventos);
    return 0;
}
