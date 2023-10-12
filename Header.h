#pragma once

#include <string>
#include <iostream>
#include <filesystem>

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

/*-------------------- MAIN HELPER FUNCTIONS --------------------*/
void DrawTiles(vector<Sprite>&, int, RenderWindow&);

/*-------------------- SETTING TEXTURES AND SPRITES FUNCTIONS --------------------*/
bool SetTextures(vector<Texture>&, int);
void SetSprites(vector<vector<Sprite>>&, int, int, float, vector<Texture>&);
void SetSelectableSprites(vector<Sprite>&, int, float, Vector2u, vector<Texture>&);

/*-------------------- CLICKING FUNCTIONS --------------------*/
int DiscoverClickArea(Vector2f, Vector2f, Vector2f, Vector2f);
void ClickSprite(vector<vector<Sprite>>&, int, float, Vector2f, Texture&);
int SelectSprite(float, Vector2f);


