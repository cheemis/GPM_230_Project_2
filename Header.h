#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace std;
using namespace sf;
namespace fs = std::filesystem;

struct mySprite
{
	Sprite sprite;
	int TextureIndex;
};



/*-------------------- MAIN HELPER FUNCTIONS --------------------*/
void DrawTiles(vector<Sprite>&, int, RenderWindow&);

/*-------------------- SETTING TEXTURES AND SPRITES FUNCTIONS --------------------*/
bool SetTextures(vector<Texture>&, int);
void SetSprites(vector<vector<mySprite>>&, int, int, float, vector<Texture>&);
void SetSelectableSprites(vector<Sprite>&, int, float, Vector2u, vector<Texture>&);

/*-------------------- CLICKING FUNCTIONS --------------------*/
int DiscoverClickArea(Vector2f, Vector2f, Vector2f, Vector2f);
void ClickSprite(vector<vector<mySprite>>&, int, float, Vector2f, vector<Texture>&, int);
int SelectSprite(float, Vector2f);

/*-------------------- CLICKING FUNCTIONS --------------------*/
void SaveMap(vector<vector<mySprite>>&, int, int, vector<Texture>&, int);
bool LoadMap(vector<vector<mySprite>>&, int, int, vector<Texture>&, int);