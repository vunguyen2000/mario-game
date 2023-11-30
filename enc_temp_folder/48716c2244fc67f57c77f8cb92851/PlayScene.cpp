#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "PlayScene.h"
#include "Box.h"
#include "BrickQuestion.h"
#include "FlowerAttack.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP				7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_NOCOLLISION	4
#define OBJECT_TYPE_BOX	5
#define OBJECT_TYPE_DRAIN	6
#define OBJECT_TYPE_COIN	8
#define OBJECT_TYPE_BRICK_QUESTION	9
#define OBJECT_TYPE_FLOWER_RED	10
#define OBJECT_TYPE_FLOWER_FIRE	11
#define OBJECT_TYPE_GOOMBAPARA	13
#define OBJECT_TYPE_KOOPAPARA	14
#define OBJECT_TYPE_FLOWER_GREEN	15
#define OBJECT_TYPE_FLOWER_NORMAL	16
#define OBJECT_TYPE_BRICK_BROKEN	25
#define OBJECT_TYPE_BRICK_QUESTION_EFFECT	26
#define OBJECT_TYPE_BRICK_QUESTION_SPECIAL_GREEN	27
#define OBJECT_TYPE_BRICK_QUESTION_SPECIAL	12
#define OBJECT_TYPE_BRICK_QUESTION_LEAF	39
#define OBJECT_TYPE_BOX_START	40
#define OBJECT_TYPE_BOX_END	41
#define OBJECT_TYPE_BRICK_MUSHROOM_GREEN	12

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	CGameObject* objW = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_NOCOLLISION: obj = new CNoCollision(); break;
	case OBJECT_TYPE_BOX: obj = new CBox(BOX_STATUS_NORMAL); break;
	case OBJECT_TYPE_BOX_END: obj = new CBox(BOX_STATUS_END); break;
	case OBJECT_TYPE_BRICK_QUESTION: obj = new CBrickQuestion(BRICK_QUESTION_STATUS_COIN); break;
	case OBJECT_TYPE_BOX_START: obj = new CBox(BOX_STATUS_START); break;
	case OBJECT_TYPE_BRICK_QUESTION_LEAF: obj = new CBrickQuestion(BRICK_QUESTION_STATUS_LEAF); break;
	case OBJECT_TYPE_BRICK_MUSHROOM_GREEN:obj = new CBrickQuestion(BRICK_MUSHROOM_GREEN); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlowerAttack(FLOWER_RED); break;

		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin(x, y, obj->GetState());
		if (id == 4)
			obj->SetRenderLayer(atoi(tokens[4].c_str()));
		obj->SetisOriginObj(true);
		objects.push_back(obj);
	}
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGame* game = CGame::GetInstance();

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{

		LPGAMEOBJECT obj = objects[i];
		if (obj->isDisAppear)
		{
			objects.erase(objects.begin() + i);
			delete obj;
		}
		else
		{
			if (player != NULL)
			{
					if (obj->x <= player->x + game->GetScreenWidth())
					{
						if (obj->y >= player->y - game->GetScreenHeight() && obj->y <= player->y + game->GetScreenHeight())
							objects[i]->Update(dt, &coObjects);
					}
			}
			else
				objects[i]->Update(dt, &coObjects);
		}

	}

	//CGame::GetInstance()->SetCamPos((int)0, (int)220);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	if (player->x < 0) player->x = 0; //Ch?ng lùi qa trái


}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();
	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();


	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario != NULL)
	{
			switch (KeyCode)
			{
			case DIK_S:
				if (mario->checkjumping == 0)
				{
					if (game->IsKeyDown(DIK_LSHIFT))
					{
						mario->SetState(MARIO_STATE_JUMP_HIGH);
					}
					else
					{
						mario->SetState(MARIO_STATE_JUMP);
					}
				}
				break;
			case DIK_DOWN:
				mario->sit = true;
				break;
			}
	}

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();


	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario != NULL)
	{
			switch (KeyCode)
			{
			case DIK_DOWN:
				mario->sit = false;
				if (mario->GetLevel() != MARIO_LEVEL_SMALL)
					mario->y -= MARIO_SIT_BBOX_HEIGHT;
				break;
			default:
				break;
			}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario != NULL)
	{
		if (mario->GetState() == MARIO_STATE_DIE) return;

			if (game->IsKeyDown(DIK_RIGHT))
			{
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
					mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
			else
				mario->SetState(MARIO_STATE_IDLE);
	}


}

