#pragma once
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Scence.h"
#include "Goomba.h"
#include "Map.h"


class CPlayScene: public CScene
{
protected: 

	Map* map;
	CMario* player;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsW;

	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_MAP(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	void Clear();

};

typedef CPlayScene* LPPLAYSCENE;

