#pragma once
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Textures.h"
#include "Scence.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "NoCollision.h"
#include "Coin.h"
#include "Cell.h"


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
	void addObject(CGameObject* a) { objects.push_back(a); };
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }
	void Clear();

};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


