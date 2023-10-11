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

bool SetTextures(Texture[], int);
void SetSprites(vector<vector<Sprite>>&, int, Texture[]);

void ClickSprite(vector<vector<Sprite>>&, int, Vector2f, Texture[], int);
float CheckDistance(Vector2f, Vector2f, int);

