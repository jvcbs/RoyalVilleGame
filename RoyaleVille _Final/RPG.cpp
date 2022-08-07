#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//Structs
typedef struct personagem
{
    int vida;
    int dano;
    int critico;
//1=barbaro 2=mago
    int classe;

} Personagem;
typedef struct inimigo
{
    int vida;
    int dano;
    int defesa;
} Inimigo;

//Ponteiros
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *pimagem = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_TIMEOUT timeout;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *sample = NULL;
FILE *dialogo_1,*rei_mal,*dialogo_barbaro_1,*dialogo_mago_1,*mapa,*escreve_vida,*carrega_vida,*dialogo_bruxa;
ALLEGRO_EVENT ev;
Personagem *heroi;
Inimigo *vilao;
//Constantes
const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;
const int Buffer_som = 4;
const int Sample_som = 1024;
//Funcoes
//x,y,z é a posiçao da imagem na tela
void Carrega_Imagem(char nome[50],int x,int y,int z);
//   tam flag  nome r g b     posiçao hor  pos vert
void Vida_Personagem(int nvilao,Inimigo *vilao, Personagem *heroi);
int **Aloca_Matriz(int m,int n);
void Preenche_Matriz(int **matriz,char *nome,int m, int n);
void Carrega_Texto(int t, int f,char *texto,int r,int g,int b,int pos_hor, int pos_vert);
void Carrega_Som(char nome[50]);
void Carrega_Sample(char nome[50]);
void Inicializa();
void Destroi();
void abrearquivo();
int testa_critico();
int testa_defesa();
void ataque(int nvilao);

int main(void)

{
    Inicializa();
    abrearquivo();
//Variaveis
    ALLEGRO_EVENT evento;
    int tem_eventos,aux_rei=0,fif=0,magic=170,ajuda=0,aux_bruxa=0,morteboss=0,aux_perna=0,i=0,som=0,critico=0,defesa=0,aux_classe_barbaro=0,aux_classe_mago=0,posx=0,posy=0,posmapax=0,posmapay=0,aux=0,auxupa=0;
    char linha[45],sprintfc[30], lixo;
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    al_set_window_title(janela,"Royale Ville");
    fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos,al_get_keyboard_event_source());
    int **mapa_1;

    srand(time(NULL));

    //Alocaçoes
    mapa_1 = Aloca_Matriz(13,17);

    vilao=(Inimigo*)malloc(3*sizeof(Inimigo));
    if(vilao==NULL)
    {
        return -1;
    }

    heroi=(Personagem*)malloc(sizeof(Personagem));
    Preenche_Matriz(mapa_1,"matriz_mapa.txt",13,17);

    //Inicio
    Carrega_Imagem("fundo.jpg",0,0,0);
    Carrega_Imagem("royaleville.png",415, 165, 0);
    Carrega_Texto(40,0,"Pressione SPACE para continuar",255,255,255,(LARGURA_TELA/2)-400,550);
    al_flip_display();
    Carrega_Sample("sample_inicio.wav");
    while(1)
    {
//While inicial
        al_wait_for_event(fila_eventos,&ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            goto fecha;
        }
        //While do dialogo
        while (1)
        {
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {

            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                {
                    al_destroy_sample(sample);
                    al_clear_to_color(al_map_rgb(0,0,0));
                    Carrega_Imagem("trono.jpg",0,0,0);
                    Carrega_Imagem("rei_dialogo.png",800,100,0);
                    Carrega_Imagem("dialogo.png",0,0,0);
                    al_flip_display();
                    if(som==0)
                    {
                        Carrega_Som("audio_1.ogg");
                        som++;
                    }

                    if (fscanf(dialogo_1,"%[^\n]",linha) != EOF)
                    {
                        fscanf(dialogo_1,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,540);
                        fscanf(dialogo_1,"%[^\n]",linha);
                        fscanf(dialogo_1,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,600);
                        al_flip_display();
                        if(ferror(dialogo_1))
                        {
                            printf("Erro ao ler o aarquivo");
                            clearerr(dialogo_1);
                        }

                    }
                    if (fscanf(dialogo_1,"%[^\n]",linha) == EOF)
                    {
                        fclose(dialogo_1);
                        break;
                    }


                }
            }
            //fecha while dos dialogos
        }
        //while das classes
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {

            }

            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                {
                    Carrega_Sample("barbaro_sample.ogg");
                    Carrega_Imagem("fundo_barbaro.jpg",0,0,0);
                    Carrega_Imagem("dialogo.png",0,0,0);
                    Carrega_Imagem("classe.png",100,525,0);
                    Carrega_Imagem("seta.png",50,580,0);
                    Carrega_Imagem("fundo_classe.png",800,100,0);
                    Carrega_Imagem("vida_barbaro.png",895,130,0);
                    Carrega_Imagem("coracao.png",850,150,0);
                    Carrega_Imagem("espada.png",850,250,0);
                    Carrega_Imagem("dado.png",850,350,0);
                    Carrega_Imagem("7.png",900,360,0);
                    heroi->classe=1;
                    al_flip_display();

                }
                //While das setas
                while(1)
                {
                    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    {
                        goto fecha;
                    }
                    al_wait_for_event(fila_eventos,&ev);
                    if(ev.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT)
                        {
                            Carrega_Imagem("fundo_barbaro.jpg",0,0,0);
                            Carrega_Imagem("dialogo.png",0,0,0);
                            Carrega_Imagem("classe.png",100,525,0);
                            Carrega_Imagem("seta.png",50,580,0);
                            Carrega_Sample("barbaro_sample.ogg");
                            Carrega_Imagem("fundo_classe.png",800,100,0);
                            Carrega_Imagem("vida_barbaro.png",895,130,0);
                            Carrega_Imagem("coracao.png",850,150,0);
                            Carrega_Imagem("espada.png",850,250,0);
                            Carrega_Imagem("dado.png",850,350,0);
                            Carrega_Imagem("7.png",900,360,0);
                            al_flip_display();
                            heroi->classe=1;
                        }

                        if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT)
                        {
                            Carrega_Imagem("fundo_mago.jpg",0,0,0);
                            Carrega_Imagem("dialogo.png",0,0,0);
                            Carrega_Imagem("seta.png",450,580,0);
                            Carrega_Imagem("classe.png",100,525,0);
                            Carrega_Sample("showtime_mago.ogg");
                            Carrega_Imagem("fundo_classe.png",800,100,0);
                            Carrega_Imagem("vida_mago.png",905,120,0);
                            Carrega_Imagem("coracao.png",850,150,0);
                            Carrega_Imagem("espada.png",850,250,0);
                            Carrega_Imagem("dado.png",850,350,0);
                            Carrega_Imagem("7.png",900,360,0);
                            al_flip_display();
                            heroi->classe=2;
                        }
                        fflush(stdin);
                        if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
                        {
                            fflush(stdin);
                            if(heroi->classe==1)
                            {
                                heroi->vida=1200;
                                heroi->dano=130;
                                heroi->critico=7;
                                goto inicio;
                            }
                            if(heroi->classe==2)
                            {
                                heroi->vida=1000;
                                heroi->dano=150;
                                heroi->critico=7;
                                goto inicio;
                            }
                        }
                    }
                    //Fecha o while das setas
                }
            }
            //Fecha o while das classes
        }
inicio:
        //while pos classe
        while(1)
        {
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }

            if(heroi->classe==1)
            {

                Carrega_Imagem("trono.jpg",0,0,0);
                Carrega_Imagem("rei_rindo.png",800,100,0);
                Carrega_Imagem("dialogo.png",0,0,0);
                if(aux_classe_barbaro==0)
                {
                    Carrega_Imagem("posclasse_barbaro.png",50,525,0);
                    aux_classe_barbaro++;
                    al_flip_display();
                }
                if(ev.type == ALLEGRO_EVENT_KEY_UP )
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        if (fscanf(dialogo_barbaro_1,"%[^\n]",linha) == EOF)
                        {
                            fclose(dialogo_barbaro_1);
                            goto inicio_mapa;
                        }
                        if (fscanf(dialogo_barbaro_1,"%[^\n]",linha) != EOF)
                        {
                            fscanf(dialogo_barbaro_1,"%c",&lixo);
                            Carrega_Texto(40,0,linha,224,219,226,50,540);
                            fscanf(dialogo_barbaro_1,"%[^\n]",linha);
                            fscanf(dialogo_barbaro_1,"%c",&lixo);
                            Carrega_Texto(40,0,linha,224,219,226,50,600);
                            al_flip_display();
                        }


                    }
                }
                al_wait_for_event(fila_eventos,&ev);
            }

            if(heroi->classe==2)
            {
                Carrega_Imagem("trono.jpg",0,0,0);
                Carrega_Imagem("rei_rindo.png",800,100,0);
                Carrega_Imagem("dialogo.png",0,0,0);
                if(aux_classe_mago==0)
                {
                    Carrega_Imagem("posclasse_mago.png",30,525,0);
                    aux_classe_mago++;
                    al_flip_display();

                }
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        if (fscanf(dialogo_mago_1,"%[^\n]",linha) == EOF)
                        {
                            fclose(dialogo_mago_1);
                            goto inicio_mapa;
                        }

                        if (fscanf(dialogo_mago_1,"%[^\n]",linha) != EOF)
                        {
                            fscanf(dialogo_mago_1,"%c",&lixo);
                            Carrega_Texto(40,0,linha,224,219,226,50,540);
                            fscanf(dialogo_mago_1,"%[^\n]",linha);
                            fscanf(dialogo_mago_1,"%c",&lixo);
                            Carrega_Texto(40,0,linha,224,219,226,50,600);
                            al_flip_display();
                        }


                    }
                }
                al_wait_for_event(fila_eventos,&ev);
            }
            //Fecha while pos classe
        }
inicio_mapa:

        Carrega_Imagem("mapa_teste.jpg",0,0,0);
        if(heroi->classe==1)
        {
            Carrega_Imagem("barbaro_frente_1.png",220,550,0);
        }
        if(heroi->classe==2)
        {
            Carrega_Imagem("Mago_frente_1.png",220,550,0);

        }
        al_flip_display();
        posx=220;
        posy=550;
        posmapax=10;
        posmapay=3;
        //while dos mapas
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);


            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(heroi->classe==1)
            {
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    goto fecha;
                }
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_UP)
                    {
                        posmapax=posmapax-1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posy=posy-55;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_tras_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_tras_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapax=posmapax+1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;

                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN)
                    {
                        posmapax=posmapax+1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posy=posy+55;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_frente_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_frente_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapax=posmapax-1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;

                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }

                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT)
                    {
                        posmapay=posmapay-1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posx=posx-76;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_esquerda_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_esquerda_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapay=posmapay+1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;
                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT)
                    {

                        posmapay=posmapay+1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posx=posx+76;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_direita_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("barbaro_direita_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapay=posmapay-1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;
                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                }
            }
            if(heroi->classe==2)
            {
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    goto fecha;
                }
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_UP)
                    {
                        posmapax=posmapax-1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posy=posy-55;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_tras_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_tras_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapax=posmapax+1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;

                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_DOWN)
                    {
                        posmapax=posmapax+1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posy=posy+55;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_frente_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_frente_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;

                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapax=posmapax-1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;

                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }

                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_LEFT)
                    {
                        posmapay=posmapay-1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posx=posx-76;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_esquerda_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_esquerda_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapay=posmapay+1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;
                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                    if(ev.keyboard.keycode==ALLEGRO_KEY_RIGHT)
                    {

                        posmapay=posmapay+1;
                        if(mapa_1[posmapax][posmapay]==0)
                        {
                            Carrega_Imagem("mapa_teste.jpg",0,0,0);
                            posx=posx+76;
                            if((aux_perna%2) == 0)
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_direita_1.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                            else
                            {
                                Carrega_Imagem("mapa_teste.jpg",0,0,0);
                                Carrega_Imagem("mago_direita_2.png",posx,posy,0);
                                al_flip_display();
                                aux_perna++;
                            }
                        }
                        else if(mapa_1[posmapax][posmapay]==1)
                        {
                            posmapay=posmapay-1;
                        }
                        else if(mapa_1[posmapax][posmapay]==2)
                        {
                            goto selva;
                        }
                        else if(mapa_1[posmapax][posmapay]==3)
                        {
                            goto gelo;
                        }
                    }
                }
            }
            //Fecha while do mapa
        }

selva:
        Carrega_Sample("batalha.ogg");
        al_destroy_audio_stream(musica);
        Carrega_Som("audio_1.ogg");
        Carrega_Imagem("fundo_selva.jpg",0,0,0);
        Carrega_Imagem("combate.png",0,0,0);
        Carrega_Imagem("ataque.png",120,550,0);
        Carrega_Imagem("seta.png",50,560,0);
        Carrega_Imagem("boss_selva_parado.png",900,300,0);
        if(heroi->classe==1)
        {
            Carrega_Imagem("barbaro_grande.png",100,300,0);
        }
        if(heroi->classe==2)
        {
            Carrega_Imagem("mago_grande.png",100,300,0);
        }
        al_flip_display();

        vilao[1].vida = 1500;
        vilao[1].dano = 85;
        Vida_Personagem(1,vilao,heroi);

//whiile da batalha
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(heroi->classe==1)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(1);
                        //animacao do barbaro
                        for(i=0; i<4; i++)
                        {
                            Carrega_Imagem("fundo_selva.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_selva_parado.png",900,300,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro1.png",900,300,0);
                            Vida_Personagem(1,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_selva.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_selva_parado.png",900,300,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro2.png",900,300,0);
                            Vida_Personagem(1,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);

                        }
                        //animacao do jungle
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_parado.png",900,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_ataque1.png",800,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_ataque2.png",800,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_parado.png",900,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[1].dano;
                    Vida_Personagem(1,vilao,heroi);
                    al_flip_display();


                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[1].vida<=0)
                    {
                        mapa_1[8][13] = 1;
                        mapa_1[9][13] = 1;
                        goto upanivel;
                    }

                }
            }

            if(heroi->classe==2)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(1);
                        for(i=0; i<3; i++)
                        {
                            Carrega_Imagem("fundo_selva.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_selva_parado.png",900,300,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago1.png",940,330,0);
                            Vida_Personagem(1,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_selva.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_selva_parado.png",900,300,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago2.png",940,330,0);
                            Vida_Personagem(1,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);
                        }
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_parado.png",900,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_ataque1.png",800,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_ataque2.png",800,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_selva.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_selva_parado.png",900,300,0);
                        Vida_Personagem(1,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[1].dano;
                    Vida_Personagem(1,vilao,heroi);
                    al_flip_display();

                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[1].vida<=0)
                    {
                        mapa_1[8][13] = 1;
                        mapa_1[9][13] = 1;
                        goto upanivel;
                    }

                }
            }
        }
//ARENA GELO
gelo:
        Carrega_Sample("batalha.ogg");
        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
        Carrega_Imagem("combate.png",0,0,0);
        Carrega_Imagem("ataque.png",120,550,0);
        Carrega_Imagem("seta.png",50,560,0);
        Carrega_Imagem("boss_gelo_parado.png",900,240,0);
        if(heroi->classe==1)
        {
            Carrega_Imagem("barbaro_grande.png",100,300,0);
        }
        if(heroi->classe==2)
        {
            Carrega_Imagem("mago_grande.png",100,300,0);
        }
        al_flip_display();

        vilao[0].vida = 2000;
        vilao[0].dano = 60;
        Vida_Personagem(0,vilao,heroi);

//while da batalha
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(heroi->classe==1)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(0);
                        for(i=0; i<4; i++)
                        {
                            Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro1.png",900,300,0);
                            Vida_Personagem(0,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro2.png",900,300,0);
                            Vida_Personagem(0,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);
                        }
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_ataque1.png",800,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_ataque2.png",800,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[0].dano;
                    Vida_Personagem(0,vilao,heroi);
                    al_flip_display();

                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[0].vida<=0)
                    {
                        mapa_1[2][7] = 1;
                        mapa_1[2][8] = 1;
                        goto upanivel;
                    }
                }
            }

            if(heroi->classe==2)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(0);
                        for(i=0; i<3; i++)
                        {
                            Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago1.png",930,350,0);
                            Vida_Personagem(0,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago2.png",930,350,0);
                            Vida_Personagem(0,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);
                        }
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_ataque1.png",800,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_ataque2.png",800,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_gelo.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("boss_gelo_parado.png",900,240,0);
                        Vida_Personagem(0,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[0].dano;
                    Vida_Personagem(0,vilao,heroi);
                    al_flip_display();

                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[0].vida<=0)
                    {
                        mapa_1[2][7] = 1;
                        mapa_1[2][8] = 1;
                        goto upanivel;
                    }

                }
            }
        }

upanivel:
        ajuda=0;
        aux_bruxa=0;
        dialogo_bruxa = fopen("texto_bruxa.txt", "r");
        morteboss++;
        if(heroi->classe==1)
            heroi->vida=1200;
        if(heroi->classe==2)
            heroi->vida=1000;
        Carrega_Imagem("fundo_bruxa.jpg",0,0,0);
        Carrega_Imagem("bruxa.png",800,50,0);
        Carrega_Imagem("dialogo.png",0,0,0);
        if(aux_bruxa==0)
        {
            Carrega_Imagem("texto_bruxa1.png",35,535,0);
            aux_bruxa++;
            al_flip_display();
        }
        //bruxa inicial (((( o mesmo dialogo sera falado para as 2 upadas de nivel
        while(1)
        {
            aux=0;
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    Carrega_Imagem("dialogo.png",0,0,0);
                    al_flip_display();
                    if (fscanf(dialogo_bruxa,"%[^\n]",linha) == EOF)
                    {
                        fclose(dialogo_bruxa);
                        goto label1;
                    }

                    if (fscanf(dialogo_bruxa,"%[^\n]",linha) != EOF)
                    {
                        fscanf(dialogo_bruxa,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,540);
                        fscanf(dialogo_bruxa,"%[^\n]",linha);
                        fscanf(dialogo_bruxa,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,600);
                        al_flip_display();
                        Carrega_Imagem("dialogo.png",0,0,0);
                    }

                }
            }
            //fecha while da bruxa
        }

        //while da escolha da classe
label1:
        Carrega_Imagem("fundo_bruxa.jpg",0,0,0);
        Carrega_Imagem("bruxa.png",800,50,0);
        Carrega_Imagem("dialogo.png",0,0,0);
        Carrega_Imagem("seta.png",75,580,0);
        Carrega_Imagem("critico.png",250,200,0);
        Carrega_Imagem("dano.png",600,200,0);
        Carrega_Imagem("seta.png",190,250,0);
        Carrega_Imagem("bruxa_escolha.png",150,575,0);
        al_flip_display();
        auxupa=0;
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {


                if(ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    Carrega_Imagem("fundo_bruxa.jpg",0,0,0);
                    Carrega_Imagem("bruxa.png",800,50,0);
                    Carrega_Imagem("dialogo.png",0,0,0);
                    Carrega_Imagem("seta.png",75,580,0);
                    Carrega_Imagem("critico.png",250,200,0);
                    Carrega_Imagem("dano.png",600,200,0);
                    Carrega_Imagem("seta.png",190,250,0);
                    Carrega_Imagem("bruxa_escolha.png",150,575,0);
                    al_flip_display();
                    auxupa=0;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    Carrega_Imagem("fundo_bruxa.jpg",0,0,0);
                    Carrega_Imagem("bruxa.png",800,50,0);
                    Carrega_Imagem("dialogo.png",0,0,0);
                    Carrega_Imagem("seta.png",450,580,0);
                    Carrega_Imagem("critico.png",250,200,0);
                    Carrega_Imagem("dano.png",600,200,0);
                    Carrega_Imagem("seta.png",540,250,0);
                    Carrega_Imagem("bruxa_escolha.png",150,575,0);

                    al_flip_display();
                    auxupa=1;
                }

                if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
                {
                    if(auxupa==0)
                    {
                        heroi->critico=heroi->critico+20;
                        if(morteboss==1)
                            goto inicio_mapa;
                        else
                            goto dialogo_final;

                    }
                    if(auxupa==1)
                    {
                        heroi->dano=heroi->dano+50;
                        //Carrega lado direito e atribui
                        if(morteboss==1)
                            goto inicio_mapa;
                        else
                            goto dialogo_final;

                    }
                }
            }
        }
dialogo_final:

        Carrega_Imagem("trono.jpg",0,0,0);
        Carrega_Imagem("rei_dialogo.png",800,100,0);
        Carrega_Imagem("dialogo.png",0,0,0);
        if(aux_rei==0)
        {
            Carrega_Imagem("texto_fim.png",50,535,0);
            aux_rei++;
            al_flip_display();
        }
        al_flip_display();
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                {
                    if(fif<6)
                    {
                        Carrega_Imagem("rei_dialogo.png",800,100,0);
                        Carrega_Imagem("dialogo.png",0,0,0);
                        al_flip_display();
                        fif++;
                    }
                    if(fif>=6)
                    {
                        Carrega_Imagem("trono.jpg",0,0,0);
                        Carrega_Imagem("rei_fala_mal.png",750,200,0);
                        Carrega_Imagem("dialogo.png",0,0,0);
                        al_flip_display();
                    }


                    al_flip_display();

                    if (fscanf(rei_mal,"%[^\n]",linha) == EOF)
                    {
                        fclose(rei_mal);
                        break;
                    }

                    if (fscanf(rei_mal,"%[^\n]",linha) != EOF)
                    {
                        Carrega_Imagem("dialogo.png",0,0,0);
                        fscanf(rei_mal,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,540);
                        fscanf(rei_mal,"%[^\n]",linha);
                        fscanf(rei_mal,"%c",&lixo);
                        Carrega_Texto(40,0,linha,224,219,226,50,600);
                        al_flip_display();
                    }

                }
            }
        }

batalha_final:
        Carrega_Sample("batalha.ogg");
        al_destroy_audio_stream(musica);
        Carrega_Som("audio_1.ogg");
        Carrega_Imagem("fundo_rei.jpg",0,0,0);
        Carrega_Imagem("combate.png",0,0,0);
        Carrega_Imagem("ataque.png",120,550,0);
        Carrega_Imagem("seta.png",50,560,0);
        Carrega_Imagem("rei_parado.png",900,300,0);
        if(heroi->classe==1)
        {
            Carrega_Imagem("barbaro_grande.png",100,300,0);
        }
        if(heroi->classe==2)
        {
            Carrega_Imagem("mago_grande.png",100,300,0);
        }
        al_flip_display();

        vilao[2].vida = 2400;
        vilao[2].dano = 80;
        Vida_Personagem(2,vilao,heroi);

//whiile da batalha
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);

            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {

                goto fecha;
            }
            if(heroi->classe==1)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(2);
                        for(i=0; i<4; i++)
                        {
                            Carrega_Imagem("fundo_rei.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("rei_parado.png",900,300,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro1.png",900,300,0);
                            Vida_Personagem(2,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_rei.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("rei_parado.png",900,300,0);
                            Carrega_Imagem("barbaro_grande.png",200,300,0);
                            Carrega_Imagem("ataque_barbaro2.png",900,300,0);
                            Vida_Personagem(2,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);
                        }
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",900,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",800,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",800,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("barbaro_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",900,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[2].dano;
                    Vida_Personagem(2,vilao,heroi);
                    al_flip_display();

                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[2].vida<=0)
                    {
                        goto fim;
                    }
                }
            }

            if(heroi->classe==2)
            {
                if(ev.type == ALLEGRO_EVENT_KEY_UP)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE)
                    {
                        ataque(2);
                        for(i=0; i<3; i++)
                        {
                            Carrega_Imagem("fundo_rei.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("rei_parado.png",900,300,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago1.png",900,300,0);
                            Vida_Personagem(2,vilao,heroi);
                            al_flip_display;
                            al_rest(0.05);
                            Carrega_Imagem("fundo_rei.jpg",0,0,0);
                            Carrega_Imagem("combate.png",0,0,0);
                            Carrega_Imagem("ataque.png",120,550,0);
                            Carrega_Imagem("seta.png",50,560,0);
                            Carrega_Imagem("rei_parado.png",900,300,0);
                            Carrega_Imagem("mago_grande.png",200,300,0);
                            Carrega_Imagem("ataque_mago2.png",900,300,0);
                            Vida_Personagem(2,vilao,heroi);
                            al_flip_display();
                            al_rest(0.05);
                        }
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",900,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",800,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",800,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                        al_rest(0.2);
                        Carrega_Imagem("fundo_rei.jpg",0,0,0);
                        Carrega_Imagem("combate.png",0,0,0);
                        Carrega_Imagem("ataque.png",120,550,0);
                        Carrega_Imagem("seta.png",50,560,0);
                        Carrega_Imagem("mago_grande.png",100,300,0);
                        Carrega_Imagem("rei_parado.png",900,300,0);
                        Vida_Personagem(2,vilao,heroi);
                        al_flip_display();
                    }
                    heroi->vida=heroi->vida-vilao[2].dano;
                    Vida_Personagem(2,vilao,heroi);
                    al_flip_display();



                    if(heroi->vida <= 0)
                    {
                        Carrega_Imagem("game_over.jpg",0,0,0);
                        al_flip_display();
                        al_rest(5);
                        goto fecha;
                    }
                    if(vilao[2].vida<=0)
                    {
                        goto fim;
                    }

                }
            }
        }

fim:

        Carrega_Imagem("vitoria.jpg",0,0,0);
        Carrega_Sample("rei_chorando.ogg");
        al_flip_display();
        al_rest(1);
        while(1)
        {
            al_wait_for_event(fila_eventos,&ev);
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                goto fecha;
            }
            if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE || ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
                {
                    goto fecha;
                }
            }
        }
//Fecha o while prinicpal
    }

//PARA FECHAR
fecha:
    Destroi();
    free(vilao);
    return 0;
    exit(1);


}

void Carrega_Imagem(char nome[50],int x,int y,int z)
{
    al_destroy_bitmap(pimagem);
    pimagem = al_load_bitmap(nome);
    if (!pimagem)
    {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
    }
    al_draw_bitmap(pimagem,x,y,z);
}
void Carrega_Texto(int t, int f,char *texto,int r,int g,int b,int pos_hor, int pos_vert)
{
    al_destroy_font(fonte);
    // Carregando o arquivo de fonte
    fonte = al_load_font("BOLTB.TTF", t, f);
    if (!fonte)
    {
        al_destroy_display(janela);
        fprintf(stderr, "Falha ao carregar fonte.\n");
    }
// Exemplo de impressão de valores variáveis
    al_draw_textf(fonte, al_map_rgb(r, g, b),pos_hor,pos_vert, ALLEGRO_ALIGN_LEFT, "%s", texto);

}
void Carrega_Som(char nome[50])
{
    musica = al_load_audio_stream(nome, Buffer_som, Sample_som);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);
}
void Carrega_Sample(char nome[50])
{
    sample = al_load_sample(nome);
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void Inicializa()
{
//addons
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(1);

}
void Destroi()
{
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    al_destroy_bitmap(pimagem);
    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_font(fonte);
}
void abrearquivo()
{
    dialogo_barbaro_1=fopen("texto_barbaro_1.txt","r");
    dialogo_1=fopen("texto_inicial.txt","r");
    dialogo_mago_1=fopen("texto_mago_1.txt","r");
    dialogo_bruxa = fopen("texto_bruxa.txt", "r");
    rei_mal = fopen("rei_mal.txt","r");
}
int **Aloca_Matriz(int m,int n)
{
    int **matriz,i,j;
    matriz=(int**)malloc(m*(sizeof(int*)));
    for(i=0; i<m; i++)
    {
        matriz[i]=(int*)calloc(sizeof(int), n*(sizeof(int)));
    }
    return matriz;
}
void Preenche_Matriz(int **matriz,char nome[50],int m, int n)
{
    int i,j,t,s=0;
    char lixo;
    mapa = fopen(nome,"r");
    for(i=0; i<m; i++)
        for(j=0; j<n; j++)
        {
            fscanf(mapa,"%d",&t);
            matriz[i][j]=t;
            fscanf(mapa,"%c",&lixo);
            if(lixo==EOF)
            {
                fclose(mapa);
                break;
            }
            else
            {


                lixo='\0';
                continue;

            }
        }

}
void ataque(int nvilao)
{
    al_flip_display();
    int testac=0, testad=0,i;
    //0 nao 1 sim
    testac=testa_critico();
    testad=testa_defesa();
    if(testac==0&&testad==0)
    {
        vilao[nvilao].vida = vilao[nvilao].vida - heroi->dano;
    }
    else if(testac==1&&testad==0)
    {
        vilao[nvilao].vida=vilao[nvilao].vida- (2*(heroi->dano));
    }
    else if(testac==1&&testad==1)
    {
        vilao[nvilao].vida = vilao[nvilao].vida - heroi->dano;
    }





}
int testa_critico()
{
    int critico=rand()% 100-0;
    if(critico<=heroi->critico)
    {
        return 1;
    }
    else return 0;

}

int testa_defesa()
{
    int defesa=rand()%100-0;
    if(defesa<=10)
    {
        return 1;
    }
    else return 0;

}

void Vida_Personagem(int nvilao,Inimigo *vilao, Personagem *heroi)
{

    char vida_heroi[10],vida_vilao[10],lixo;
    escreve_vida = fopen("vida.txt","w");
    fprintf(escreve_vida,"%d\n",heroi->vida);
    fprintf(escreve_vida,"%d",vilao[nvilao].vida);

    fclose(escreve_vida);
    carrega_vida = fopen("vida.txt","r");
    fscanf(carrega_vida,"%s%c%s",vida_heroi,&lixo,vida_vilao);
    Carrega_Imagem("fundo_ataque.png",1030,540,0);
    Carrega_Texto(30,0,vida_heroi,219,215,216,1060,550);
    Carrega_Texto(30,0,vida_vilao,219,215,216,1060,630);
    al_flip_display();
    fclose(carrega_vida);
}
