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
void SetSprites(vector<vector<mySprite>>&, int, int, float, vector<Sprite>&);
void SetSelectableSprites(vector<Sprite>&, Vector2i, float, Vector2u, bool, vector<Texture>&);

/*-------------------- CLICKING FUNCTIONS --------------------*/
int DiscoverClickArea(Vector2f, Vector2f, Vector2f, Vector2f, Vector2f);
bool ClickSprite(bool*, vector<vector<mySprite>>&, int, float, Vector2f, vector<Texture>&, int*);
int SelectSprite(float, float);

/*-------------------- SETTINGS FUNCTIONS --------------------*/
void ClickSettings(int, int*, bool*, vector<vector<mySprite>>&, int, int, vector<Texture>&, int);
void SaveMap(vector<vector<mySprite>>&, int, int, vector<Texture>&, int);
bool LoadMap(vector<vector<mySprite>>&, int, int, vector<Texture>&, int);