
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<list>
#include<vector>
#include<map>
#include<fstream>
#include "PersonajeJugador.h"
#include "NPC.h"
#include <stdlib.h>     /* srand, rand */

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character;
SDL_Rect rect_background,rect_character;

class Tile
{
public:
    int fuente_x;
    int fuente_y;
    int destino_x;
    int destino_y;
    Tile(int fuente_x, int fuente_y, int destino_x, int destino_y)
    {
        this->fuente_x=fuente_x;
        this->fuente_y=fuente_y;
        this->destino_x=destino_x;
        this->destino_y=destino_y;
    }
};
vector<Tile*> cargarTiles2(string nombre_archivo, int inicio_y, int max_y, int inicio_x, int max_x)
{
    vector<Tile*> tiles;
    ifstream archivo(nombre_archivo.c_str());
    int num;
    for(int destino_y=inicio_y; destino_y < max_y; destino_y++)
    {
        for(int destino_x = inicio_x; destino_x < max_x; destino_x++)
        {
            int fuente_x;
            int fuente_y;
            archivo>>fuente_x;
            archivo>>fuente_y;
            tiles.push_back(new Tile(fuente_x, fuente_y, destino_x, destino_y));
        }
    }
    archivo.close();
    return tiles;
}
vector<Tile*> cargarTiles(string nombre_archivo)
{
    vector<Tile*> tiles;
    ifstream archivo(nombre_archivo.c_str());
    for(int destino_y=0; destino_y<18; destino_y++)
    {
        for(int destino_x=0; destino_x<18; destino_x++)
        {
            int fuente_x;
            int fuente_y;
            archivo>>fuente_x;
            archivo>>fuente_y;
            tiles.push_back(new Tile(fuente_x,fuente_y,destino_x,destino_y));
        }
    }
    archivo.close();
    return tiles;
}

void dibujarTile(SDL_Texture *tileset_texture, SDL_Renderer* renderer,
        int fuente_x, int fuente_y, int destino_x, int destino_y)
{
    SDL_Rect fuente,destino;
    fuente.x = 32*fuente_x;
    fuente.y = 32*fuente_y;
    fuente.w = 32;
    fuente.h = 32;

    destino.x = 32*destino_x;
    destino.y = 32*destino_y;
    destino.w = 32;
    destino.h = 32;


    SDL_RenderCopy(renderer,tileset_texture,&fuente,&destino);
}
void dibujarTile2(SDL_Texture *tileset_texture, SDL_Renderer* renderer,
        int fuente_x, int fuente_y, int destino_x, int destino_y)
{
    SDL_Rect fuente,destino;
    fuente.x = 32*fuente_x;
    fuente.y = 32*fuente_y;
    fuente.w = 32;
    fuente.h = 32;

    destino.x = 32*destino_x;
    destino.y = 32*destino_y;
    destino.w = 32;
    destino.h = 32;


    SDL_RenderCopy(renderer,tileset_texture,&fuente,&destino);

}
int main( int argc, char* args[] )
{
    cargarTiles("assets/tile.map");
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 500/*WIDTH*/, 250/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0; rect_background.y = 0; rect_background.w = w; rect_background.h = h;

    character = IMG_LoadTexture(renderer, "assets/personaje/down.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = 0;
    rect_character.y = 100;
    rect_character.w = w;
    rect_character.h = h;


    list<Personaje*>personajes;
    personajes.push_back(new PersonajeJugador (0,0,renderer,&personajes));
    personajes.push_back(new NPC (50,50,renderer,&personajes));
    personajes.push_back(new NPC (100,200,renderer,&personajes));
    personajes.push_back(new NPC (100,300,renderer,&personajes));
    personajes.push_back(new NPC (150,50,renderer,&personajes));
    personajes.push_back(new NPC (50,150,renderer,&personajes));

    SDL_Texture* tileset_texture = IMG_LoadTexture(renderer, "assets/tileset.png");
    vector<Tile*>tiles=cargarTiles2("assets/tile.map",0,8,0,16);
    vector<Tile*>tiles2=cargarTiles2("assets/tile2.txt",0,3,3,6);
     vector<Tile*>tiles3=cargarTiles2("assets/tile3.map",0,8,0,16);
     vector<Tile*>tiles4=cargarTiles2("assets/tile5.txt",0,3,13,16);
     vector<Tile*>tiles5=cargarTiles2("assets/tile4.txt",0,3,13-3,16-3);
        vector<Tile*>tiles6=cargarTiles2("assets/tile6.txt",0,3,9,11);
        vector<Tile*>tiles7=cargarTiles2("assets/tile7.txt",4,6,0,16);

    //Main Loop
    int frame=0;
    while(true)
    {
        frame++;
        if(frame%100==0)
            personajes.push_back(new NPC (100,rand() % 100,renderer,&personajes));
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState( &x, &y );
                cout<<x<<","<<y<<endl;
            }
        }

        for(list<Personaje*>::iterator i = personajes.begin();
                i!=personajes.end();
                i++)
            (*i)->logic((Uint8*)SDL_GetKeyboardState( NULL ));

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        for(int i=0;i<tiles.size();i++)
        {
            dibujarTile(tileset_texture,renderer,
                    tiles[i]->fuente_x,tiles[i]->fuente_y,
                    tiles[i]->destino_x,tiles[i]->destino_y);
        }
         for(int i = 0; i <tiles3.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles3[i]->fuente_x, tiles3[i]->fuente_y, tiles3[i]->destino_x, tiles3[i]->destino_y);
        }
         for(int i = 0; i <tiles2.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles2[i]->fuente_x, tiles2[i]->fuente_y, tiles2[i]->destino_x, tiles2[i]->destino_y);
        }

         for(int i = 0; i <tiles4.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles4[i]->fuente_x, tiles4[i]->fuente_y, tiles4[i]->destino_x, tiles4[i]->destino_y);
        }
         for(int i = 0; i <tiles5.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles5[i]->fuente_x, tiles5[i]->fuente_y, tiles5[i]->destino_x, tiles5[i]->destino_y);
        }
         for(int i = 0; i <tiles6.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles6[i]->fuente_x, tiles6[i]->fuente_y, tiles6[i]->destino_x, tiles6[i]->destino_y);
        }
          for(int i = 0; i <tiles7.size(); i++)
        {
            dibujarTile(tileset_texture, renderer, tiles7[i]->fuente_x, tiles7[i]->fuente_y, tiles7[i]->destino_x, tiles7[i]->destino_y);
        }

      //  dibujarTile(tileset_texture,renderer,
        //            0,6,
          //          5,4);

        for(list<Personaje*>::iterator i = personajes.begin();
                i!=personajes.end();
                i++)
            (*i)->render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16.66);
    }

	return 0;
}
