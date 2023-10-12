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
	vector<vector<Sprite>> sprites(numCols, vector<Sprite>(numRows));
	


	//Set textures/sprites
	if(!SetTextures(textures, numTextures)) return 0;
	SetSelectableSprites(selectableSprites, numTextures, selectableScale, window.getSize(), textures);
	SetSprites(sprites, numCols, numRows, scale, textures);

	while (window.isOpen())
	{
		bool mousePressed = Mouse::isButtonPressed(Mouse::Left);
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();

		if (!clicked && mousePressed)
		{
			Vector2f tilemapArea = Vector2f(numCols * 64 * scale,numRows * 64 * scale);

			Vector2f selectingTilesArea = Vector2f(window.getSize().x - (64 * selectableScale),numTextures * 64 * selectableScale);
			
			Vector2f currentMousePos = Vector2f(Mouse::getPosition(window));

			int clickedArea = DiscoverClickArea(tilemapArea, selectingTilesArea, currentMousePos, Vector2f(window.getSize()));

			/* debugging click locations to terminal
			cout << "---------------------------" << endl;
			cout << "tilemapArea: (" << tilemapArea.x << ", " << tilemapArea.y <<")" << endl;
			cout << "selectingTilesArea: (" << selectingTilesArea.x << ", " << selectingTilesArea.y <<")" << endl;
			cout << "currentMousePos: (" << currentMousePos.x << ", " << currentMousePos.y <<")" << endl;
			*/

			switch (clickedArea)
			{
			case 1: //Clicked doesnt need to be set, painting
				ClickSprite(sprites, numCols, 64 * scale ,currentMousePos, textures[currentSpriteIndex]);
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

			/*
			cout << "current mouse position: (" << currentMousePos.x << ", " << currentMousePos.y << ")" << endl;

			ClickSprite(sprites, numCols, currentMousePos, textures, index);
			index = (index + 1) % 8;
			*/
		}
		else if (clicked && !mousePressed)
		{
			clicked = false;
		}
		

		for (int i = 0; i < numCols; i++)
		{
			for (int j = 0; j < numRows; j++)
			{
				window.draw(sprites[i][j]);
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
		//base return case
		if (i >= numTextures) return false;

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
void SetSprites(vector<vector<Sprite>>& sprites, int numCols, int numRows, float scale, vector<Texture>& textures)
{
	int count = 0;

	for(int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			sprites[i][j].setTexture(textures[0]);//set everything to dirt
			sprites[i][j].setScale(scale, scale);

			sprites[i][j].setPosition(64 * scale * i, 64 * scale * j);
			
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
void ClickSprite(vector<vector<Sprite>>& sprites, int numSprites, float spriteSize, Vector2f mousePos, Texture& newTexture)
{
	Sprite *closestSprite = &sprites[mousePos.x / (spriteSize)][mousePos.y / (spriteSize)];
	closestSprite->setTexture(newTexture);
}

int SelectSprite(float spriteSize, Vector2f mousePos)
{
	cout << mousePos.y << " / " << spriteSize << " = " << mousePos.y / spriteSize << endl;
	return mousePos.y / spriteSize;
}