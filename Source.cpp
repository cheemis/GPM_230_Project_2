#include "Header.h"

int main()
{
	//Rendering variables
	RenderWindow window(VideoMode(512, 512), "Sprites!");

	//Texture variables
	int numTextures = 8;

	//Sprite variables
	float selectableScale = 1;
	int currentSpriteIndex = 0;

	//Tilemap variables
	int numCols = 16;
	int numRows = 16;
	float scale = .425;

	//Clikcing variables
	bool clicked = false;

	//Texture/sprites variables
	vector<Texture> textures(numTextures);
	vector<Sprite> selectableSprites(numTextures);
	vector<vector<mySprite>> sprites(numCols, vector<mySprite>(numRows));
	


	//Set textures/sprites
	if (!SetTextures(textures, numTextures)) return 0;
	SetSelectableSprites(selectableSprites, numTextures, selectableScale, window.getSize(), textures);
	SetSprites(sprites, numCols, numRows, scale, textures);

	while (window.isOpen())
	{
		bool mousePressed = Mouse::isButtonPressed(Mouse::Left);
		bool savePressed = Keyboard::isKeyPressed(Keyboard::F);
		bool loadPressed = Keyboard::isKeyPressed(Keyboard::H);

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();

		if (savePressed)
		{
			SaveMap(sprites, numCols, numRows, textures, numTextures);
		}

		if (loadPressed)
		{
			if (!LoadMap(sprites, numCols, numRows, textures, numTextures))
			{
				
				return 0;
			}
			else
			{
				cout << "load successful!" << endl;
			}
		}

		if (!clicked && mousePressed)
		{
			Vector2f tilemapArea = Vector2f(numCols * 64 * scale,numRows * 64 * scale);
			Vector2f selectingTilesArea = Vector2f(window.getSize().x - (64 * selectableScale),numTextures * 64 * selectableScale);
			Vector2f currentMousePos = Vector2f(Mouse::getPosition(window));

			int clickedArea = DiscoverClickArea(tilemapArea, selectingTilesArea, currentMousePos, Vector2f(window.getSize()));

			switch (clickedArea)
			{
			case 1: //Clicked doesnt need to be set, painting
				ClickSprite(sprites, numCols, 64 * scale ,currentMousePos, textures, currentSpriteIndex);
				cout << "clicked tile map" << endl;
				break;

			case 2://Clicked does need to be set, only select one sprite at a time
				currentSpriteIndex = SelectSprite(64 * selectableScale, currentMousePos);
				cout << "sprite clicked index: " << currentSpriteIndex << endl;
				clicked = true;
				break;

			default://Clicked does need to be set, nothing only needs to happen once
				cout << "didn't click anything of importance" << endl;
				clicked = true;
				//do nothing
			}
		}
		else if (clicked && !mousePressed)
		{
			clicked = false;
		}
		

		for (int i = 0; i < numCols; i++)
		{
			for (int j = 0; j < numRows; j++)
			{
				window.draw(sprites[i][j].sprite);
				//cout << "drawing " << sprites[i][j].getTexture() << " - (" << sprites[i][j].getPosition().x << ", " << sprites[i][j].getPosition().y << ")" << endl;
			}
		}

		DrawTiles(selectableSprites, numTextures, window);

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
void SetSprites(vector<vector<mySprite>>& sprites, int numCols, int numRows, float scale, vector<Texture>& textures)
{
	int count = 0;

	for(int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			sprites[i][j].TextureIndex = 0;
			sprites[i][j].sprite.setTexture(textures[0]);//set everything to dirt
			sprites[i][j].sprite.setScale(scale, scale);

			sprites[i][j].sprite.setPosition(64 * scale * i, 64 * scale * j);
			
			count++;
		}
	}
	cout << count << " sprites set!" << endl;
}

void SetSelectableSprites(vector<Sprite>& sprites, int numSprites, float scale, Vector2u screenSize, vector<Texture>& textures)
{
	int count = 0;

	for (int i = 0; i < numSprites; i++)
	{
		sprites[i].setTexture(textures[i]);
		sprites[i].setScale(scale, scale);
		sprites[i].setPosition(screenSize.x - 64 * scale, 64 * scale * i);

		cout << "setting " << sprites[i].getTexture() << " at (" << sprites[i].getPosition().x << ", " << sprites[i].getPosition().y << ")" << endl;
	}
	//cout << count << " sprites set!" << endl;
}



/*-------------------- CLICKING FUNCTIONS --------------------*/

//This functions finds out where on the screen the player clicked
int DiscoverClickArea(Vector2f tilemapArea, Vector2f selectingTilesArea, Vector2f currentMousePos, Vector2f windowSize)
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

	return 0;
	
}

//this function finds out which tile the player clicked
void ClickSprite(vector<vector<mySprite>>& sprites, int numSprites, float spriteSize, Vector2f mousePos, vector<Texture>& textures, int newTextureIndex)
{
	mySprite *closestSprite = &sprites[mousePos.x / (spriteSize)][mousePos.y / (spriteSize)];
	closestSprite->TextureIndex = newTextureIndex;
	closestSprite->sprite.setTexture(textures[newTextureIndex]);
}

int SelectSprite(float spriteSize, Vector2f mousePos)
{
	cout << mousePos.y << " / " << spriteSize << " = " << mousePos.y / spriteSize << endl;
	return mousePos.y / spriteSize;
}



/*-------------------- SAVING FUNCTIONS --------------------*/
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

