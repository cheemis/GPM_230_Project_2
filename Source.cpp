#include "Header.h"

int main()
{
	//Rendering variables
	RenderWindow window(VideoMode(512, 512), "Sprites!");

	//Texture variables
	int numTextures = 8;
	int numSettingsTextures = 4;

	//Sprite variables
	float selectableScale = 1;
	int currentSpriteIndex = 0;
	bool eyeDropperSelected = false;

	//Tilemap variables
	int numCol = 16;
	int numRow = 16;
	float scale = .4225;

	//Clikcing variables
	bool clicked = false;

	//Texture/sprites variables
	vector<Texture> textures(numTextures + numSettingsTextures + 1 /*1 is for the background*/);
	vector<Sprite> selectableSprites(numTextures);
	vector<Sprite> settingsSprites(numSettingsTextures);
	vector<vector<mySprite>> sprites(numCol, vector<mySprite>(numRow));
	Sprite background;

	//Set textures/sprites
	if (!SetTextures(textures, numTextures + numSettingsTextures + 1)) return 0;
	SetSelectableSprites(selectableSprites, Vector2i(0, numTextures), selectableScale, window.getSize(), true, textures);
	SetSelectableSprites(settingsSprites, Vector2i(numTextures, numTextures + numSettingsTextures), selectableScale, window.getSize(), false, textures);
	SetSprites(sprites, numCol, numRow, scale, selectableSprites);

	//set the background image
	background.setTexture(textures[textures.size() - 1]);

	//main loop
	while (window.isOpen())
	{
		//check key presses
		bool mousePressed = Mouse::isButtonPressed(Mouse::Left);

		//check closing window
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();

		//check if clicking somewhere
		if (!clicked && mousePressed)
		{
			Vector2f tilemapArea = Vector2f(numCol * 64 * scale,numRow * 64 * scale);
			Vector2f selectingTilesArea = Vector2f(window.getSize().x - (64 * selectableScale), numTextures * 64 * selectableScale);
			Vector2f settingsTilesArea = Vector2f(numSettingsTextures * 64 * selectableScale, window.getSize().y - (64 * selectableScale));
			Vector2f currentMousePos = Vector2f(Mouse::getPosition(window));

			int clickedArea = DiscoverClickArea(tilemapArea, selectingTilesArea, settingsTilesArea, currentMousePos, Vector2f(window.getSize()));

			//determine what area was clicked
			switch (clickedArea)
			{
			case 1: //Clicked doesnt need to be set, painting
				clicked = ClickSprite(&eyeDropperSelected, sprites, numCol, 64 * scale ,currentMousePos, textures, &currentSpriteIndex);
				cout << "clicked tile map: " << clicked << endl;
				break;

			case 2://Clicked does need to be set, only select one sprite at a time
				currentSpriteIndex = SelectSprite(64 * selectableScale, currentMousePos.y);
				cout << "sprite clicked index: " << currentSpriteIndex << endl;
				clicked = true;
				break;

			case 3://Clicked does need to be set, only select one setting at a time
				//currentSpriteIndex = SelectSprite(64 * selectableScale, currentMousePos.x);
				cout << "setting clicked index: " << SelectSprite(64 * selectableScale, currentMousePos.x) << endl;
				ClickSettings(SelectSprite(64 * selectableScale, currentMousePos.x),
							  &currentSpriteIndex,
							  &eyeDropperSelected,
							  sprites,
							  numCol,
							  numRow,
							  textures,
							  numTextures);
				clicked = true;
				break;

			default://Clicked does need to be set, nothing only needs to happen once
				cout << "didn't click anything of importance" << endl;
				clicked = true;
				//do nothing
			}
		}
		//stop clicking
		else if (clicked && !mousePressed)
		{
			clicked = false;
		}
		
		//draw background
		window.draw(background);

		//draw tiles
		for (int i = 0; i < numCol; i++)
		{
			for (int j = 0; j < numRow; j++)
			{
				window.draw(sprites[i][j].sprite);
			}
		}

		//draw selectable tiles & settings
		DrawTiles(selectableSprites, numTextures, window);
		DrawTiles(settingsSprites, numSettingsTextures, window);

		window.display();
	}
	return 0;
}



/*-------------------- MAIN HELPER FUNCTIONS --------------------*/

void DrawTiles(vector<Sprite>& sprites, int numSprites, RenderWindow& window)
{
	for (int i = 0; i < numSprites; i++)
	{
		window.draw(sprites[i]);
	}
}



/*-------------------- SETTING TEXTURES AND SPRITES FUNCTIONS --------------------*/

//This function gets the textures from the assets folder and puts them into an array
bool SetTextures(vector<Texture>& textures, int numTextures)
{
	string path = "Assets/";
	int i = 0;

	//is there a better way to do this?
	for (const auto& entry : fs::directory_iterator(path))
	{
		//base return case - either no files or got all files needed
		if (i >= numTextures)
		{
			if(i > 0) return true;
			return false;
		}


		//if the image is NOT found in the file system
		if (!textures[i].loadFromFile(entry.path().string()))
		{
			cout << "couldn't find image" << '\n';
			return false;
		}
		textures[i].setSmooth(true);
		cout << "adding " << entry.path() << endl;
		i++;
	}
	cout << "all images successfully added!" << endl;
	return true;
}



//this function sets all the sprites with their textures when the scene starts
void SetSprites(vector<vector<mySprite>>& sprites, int numCols, int numRows, float scale, vector<Sprite>& selectableSprites)
{
	int count = 0;

	for(int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			sprites[i][j].TextureIndex = 0;
			sprites[i][j].sprite.setTexture(*selectableSprites[0].getTexture());//set everything to dirt
			sprites[i][j].sprite.setScale(scale, scale);

			sprites[i][j].sprite.setPosition(64 * scale * i, 64 * scale * j);
			
			count++;
		}
	}
	cout << count << " sprites set!" << endl;
}

void SetSelectableSprites(vector<Sprite>& sprites, Vector2i SpriteRange, float scale, Vector2u screenSize, bool screenLeft, vector<Texture>& textures)
{
	int current = 0; //makes sure always starts at front of passed array

	for (int i = SpriteRange.x; i < SpriteRange.y; i++)
	{
		sprites[current].setTexture(textures[i]);
		sprites[current].setScale(scale, scale);
		if (screenLeft) sprites[current].setPosition(screenSize.x - 64 * scale, 64 * scale * current);
		else sprites[current].setPosition(64 * scale * current, screenSize.y - 64 * scale);

		current++;
	}
	cout << current << " sprites set!" << endl;
}

void SetSettings(vector<Sprite>& settingsSprites, int numSprites, float scale, Vector2u screenSize, vector<Texture>& textures)
{
	string path = "Assets/";
}



/*-------------------- CLICKING FUNCTIONS --------------------*/

//This functions finds out where on the screen the player clicked
int DiscoverClickArea(Vector2f tilemapArea, Vector2f selectingTilesArea, Vector2f settingsTilesArea, Vector2f currentMousePos, Vector2f windowSize)
{
	//see if it overlaps with the tilemap
	//(bound by top left)
	if (currentMousePos.x > 0 &&
		currentMousePos.y > 0 &&
		currentMousePos.x < tilemapArea.x &&
		currentMousePos.y < tilemapArea.y)
	{
		return 1;
	}

	//if not, see if overlaps with the selecting tiles
	//(Bound by top right)
	if (currentMousePos.x < windowSize.x &&
		currentMousePos.y > 0 &&
		currentMousePos.x > selectingTilesArea.x &&
		currentMousePos.y < selectingTilesArea.y)
	{
		return 2;
	}

	//if not, see if overlaps with the settings tiles
	//(Bound by top right)
	if (currentMousePos.x > 0 &&
		currentMousePos.y < windowSize.y &&
		currentMousePos.x < settingsTilesArea.x &&
		currentMousePos.y > settingsTilesArea.y)
	{
		return 3;
	}

	return 0;
	
}

//this function finds out which tile the player clicked
bool ClickSprite(bool* eyeDropperSelected, vector<vector<mySprite>>& sprites, int numSprites, float spriteSize, Vector2f mousePos, vector<Texture>& textures, int* newTextureIndex)
{
	mySprite* closestSprite = &sprites[mousePos.x / (spriteSize)][mousePos.y / (spriteSize)];

	cout << "eye dropper selected: " << *eyeDropperSelected << endl;

	//eye dropper tool
	if (*eyeDropperSelected)
	{
		*newTextureIndex = closestSprite->TextureIndex;
		*eyeDropperSelected = false;
		return true;
	}
	//regular drawing
	else
	{
		closestSprite->TextureIndex = *newTextureIndex;
		closestSprite->sprite.setTexture(textures[*newTextureIndex]);
		return false;
	}

	
}

int SelectSprite(float spriteSize, float mousePos)
{
	cout << mousePos << " / " << spriteSize << " = " << mousePos / spriteSize << endl;
	return mousePos / spriteSize;
}


/*-------------------- SETTINGS FUNCTIONS --------------------*/
void ClickSettings(int settingsIndex, int* currentSpriteIndex, bool *eyeDropperSelected, vector<vector<mySprite>>& sprites, int numCol, int numRow, vector<Texture>& textures, int numTextures)
{
	switch (settingsIndex)
	{
	case 0: //set the eraser sprite
		*currentSpriteIndex = 0;
		break;

	case 1: //save the tilemap
		SaveMap(sprites, numCol, numRow, textures, numTextures);
		break;

	case 2: //load the tilemap
		LoadMap(sprites, numCol, numRow, textures, numTextures);
		break;
	case 3: // use eye dropper tool
		*eyeDropperSelected = true;
		break;

	default: //do nothing
		break;
	}
}


void SaveMap(vector<vector<mySprite>>& sprites, int numCol, int numRow, vector<Texture>& textures, int numTextures)
{
	string saveFileName = "Save.txt";
	ofstream saveFile(saveFileName, ofstream::out | ofstream::trunc);

	for (int i = 0; i < numCol; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			saveFile << sprites[i][j].TextureIndex;
		}
		saveFile << endl;
	}
	cout << "saved map!" << endl;
}

bool LoadMap(vector<vector<mySprite>>& sprites, int numCol, int numRow, vector<Texture>& textures, int numTextures)
{
	string saveFileName = "Save.txt";
	string outputString = "";
	ifstream saveFile(saveFileName);

	//make sure doesnt load out of bounds
	int col = 0;
	
	cout << "loading file!" << endl;

	// Use a while loop together with the getline() function to read the file line by line
	while (getline(saveFile, outputString))
	{
		cout << "output: " << outputString << " : " << outputString.length() << endl;

		//too many columns - bad save file
		if (col > numCol)
		{
			cout << "local col was greater than max columns: " << col << " > " << numCol << endl;
			return false;
		}
		//row size isn't the same - bad save file
		if (outputString.length() != numRow)
		{
			cout << "string lengths werent the same: " << outputString.length() << " != " << numRow << endl;
			return false;
		}

		for (int row = 0; row < outputString.length(); row++)
		{
			int newRowIndex = (int)(outputString[row] - '0');

			//saved texture out of bounds -- bad save file
			if (newRowIndex < 0 || newRowIndex > numTextures)
			{
				cout << "texture ID was out of range: " << newRowIndex << endl;
				return false;
			}
			sprites[col][row].TextureIndex = newRowIndex;
			sprites[col][row].sprite.setTexture(textures[newRowIndex]);

		}
		col++;
	}
	saveFile.close();
	return true;
}

