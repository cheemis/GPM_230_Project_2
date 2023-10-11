#include "Header.h"


int main()
{
	RenderWindow window(VideoMode(512, 512), "Sprites!");
	CircleShape shape(100.f);
	int index = 0;
	bool clicked = false;
	int numCols = 4;

	//set texture/Sprites
	Texture textures[8];
	vector<vector<Sprite>> sprites(numCols, vector<Sprite>(numCols));

	if(!SetTextures(textures, 8)) return 0;
	SetSprites(sprites, numCols, textures);

	shape.setFillColor(Color::Green);
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
			Vector2i temp = Mouse::getPosition(window);
			Vector2f currentMousePos = Vector2f(temp);

			clicked = true;

			cout << "current mouse position: (" << currentMousePos.x << ", " << currentMousePos.y << ")" << endl;

			ClickSprite(sprites, numCols, currentMousePos, textures, index);
			index = (index + 1) % 8;
		}
		else if (clicked && !mousePressed)
		{
			clicked = false;
		}
		
		for (int i = 0; i < numCols; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				window.draw(sprites[i][j]);
			}
		}
		window.display();
	}
	return 0;
}

bool SetTextures(Texture textures[], int numTextures)
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

void SetSprites(vector<vector<Sprite>>& sprites, int numSprites, Texture textures[])
{
	int index = 1; //for testing, remove later and set all textures to texture 1
	float scale = 1;
	int count = 0;

	for(int i = 0; i < numSprites; i++)
	{
		for (int j = 0; j < numSprites; j++)
		{
			sprites[i][j].setTexture(textures[index]);
			sprites[i][j].setScale(scale, scale);

			sprites[i][j].setPosition(64 * scale * i, 64 * scale * j);
			//cout << "sprite " << count << ": (" << sprites[i][j].getPosition().x << ", " << sprites[i][j].getPosition().y << ")" << endl;

			index = (index + i) % 8;
			count++;
		}
		index = (index + i) % 8;
	}
	cout << count << " sprites set!" << endl;
}

void ClickSprite(vector<vector<Sprite>>& sprites, int numSprites, Vector2f mousePos, Texture textures[], int newTexture)
{
	float spriteSize = sprites[0][0].getScale().x * sprites[0][0].getTexture()->getSize().x;
	Sprite *closestSprite = &sprites[mousePos.x / (spriteSize)][mousePos.y / (spriteSize)];
	closestSprite->setTexture(textures[newTexture]);
}

float CheckDistance(Vector2f spritePos, Vector2f mousePos, int spriteSize)
{
	double newX = pow(spritePos.x - mousePos.x + spriteSize/2, 2);
	double newY = pow(spritePos.y - mousePos.y + spriteSize/2, 2);

	return sqrt(newX + newY);
}
