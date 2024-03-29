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
#include "FireFlower.h"
#include "Koopas.h"
#include "GoombaPara.h"
#include "KoopasPara.h"
#include "BrickBroken.h"
#include "Drain.h"
#include "Card.h"
#include "HUD.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	mergeObject = false;
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
#define OBJECT_TYPE_KOOPASPARA	14
#define OBJECT_TYPE_FLOWER_GREEN	15
#define OBJECT_TYPE_FLOWER_NORMAL	16
#define OBJECT_TYPE_BRICK_BROKEN	25
#define OBJECT_TYPE_BRICK_QUESTION_EFFECT	26
#define OBJECT_TYPE_BRICK_QUESTION_SPECIAL_GREEN	27
#define OBJECT_TYPE_BRICK_QUESTION_SPECIAL	12
#define OBJECT_TYPE_BRICK_QUESTION_LEAF	39
//#define OBJECT_TYPE_BOX_START	40
//#define OBJECT_TYPE_BOX_END	41
#define OBJECT_TYPE_BRICK_MUSHROOM_GREEN	12
#define MAX_SCENE_LINE 1024
#define OBJECT_TYPE_CARD	28
#define OBJECT_TYPE_HUD	7


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

bool CPlayScene::IsInUseArea(float Ox, float Oy)
{
	float CamX, CamY;

	CamX = CGame::GetInstance()->GetCamPosX();

	CamY = CGame::GetInstance()->GetCamPosY();
	if (CamY < 0) CamY = 0;
	if (((CamX < Ox + 50) && (Ox < CamX + 380)) && ((CamY < Oy) && (Oy < CamY + 250)))
		return true;
	return false;
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
		int frame_time = atoi(tokens[i + 1].c_str());
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

	// Check start merge
	if (line[0] == '!')
	{
		// false -> true and true -> false
		if (mergeObject == false)
		{
			mergeObject = true;				//Start merge
			CGameObject* obj = new CBoxs();
			objects.push_back(obj);
		}
		else {
			mergeObject = false;			// End merge
			for (size_t i = 0; i < objects.size(); i++)
			{
				if (dynamic_cast<CBoxs*>(objects[i]))
				{
					CBoxs* boxs = dynamic_cast<CBoxs *>(objects[i]);
					if (boxs->getMergeComplete() == false)
					{
						boxs->setMergeComplete(true);
						vector<CBox*> listBox = boxs->getListBox();
						float xBoxs, yBoxs;
						listBox[0]->GetPosition(xBoxs, yBoxs);
						boxs->SetPosition(xBoxs, yBoxs);
						boxs->setWidth(BOX_BBOX_WIDTH);
						boxs->setHeight(BOX_BBOX_HEIGHT);


						for (size_t i = 1; i < listBox.size(); i++)
						{
							float xItem, yItem;
							listBox[i]->GetPosition(xItem, yItem);

							// Set width
							if (xItem >= xBoxs)
							{
								boxs->setWidth(xItem - xBoxs + BOX_BBOX_WIDTH);
							}
							else
							{
								double width = boxs->getWidth();
								boxs->setWidth(xBoxs - xItem + width);
								xBoxs = xItem;
							}

							// Set height
							if (yItem < yBoxs)
							{
								double height = boxs->getHeight();
								boxs->setHeight(yBoxs - yItem + height);
								yBoxs = yItem;
							}
							else
							{
								boxs->setHeight(yItem - yBoxs + BOX_BBOX_HEIGHT);
							}
						}

						boxs->SetPosition(xBoxs, yBoxs);
						//objects.push_back(boxs);
					}
				}

			}
		}
	}

	// MERGE BOX
	if (mergeObject)
	{
		if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		double x = atof(tokens[1].c_str());
		double y = atof(tokens[2].c_str());

		int ani_set_id = atoi(tokens[3].c_str());

		CAnimationSets* animation_sets = CAnimationSets::GetInstance();

		CBox* obj = new CBox();

		// General object setup
		obj->SetPosition(x, y);

		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBoxs*>(objects[i]))
			{
				CBoxs* boxs = dynamic_cast<CBoxs*>(objects[i]);
				if (boxs->getMergeComplete() == false)
				{
					boxs->PushBox(obj);
				}
			}
		}
	}
	else {

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
		//case OBJECT_TYPE_BOX_END: obj = new CBox(BOX_STATUS_END); break;
		case OBJECT_TYPE_BRICK_QUESTION: obj = new CBrickQuestion(BRICK_QUESTION_STATUS_COIN); break;
		//case OBJECT_TYPE_BOX_START: obj = new CBox(BOX_STATUS_START); break;
		case OBJECT_TYPE_BRICK_QUESTION_LEAF: obj = new CBrickQuestion(BRICK_QUESTION_STATUS_LEAF); break;
		case OBJECT_TYPE_BRICK_MUSHROOM_GREEN:obj = new CBrickQuestion(BRICK_MUSHROOM_GREEN); break;
		case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlowerAttack(FLOWER_RED); break;
		case OBJECT_TYPE_FLOWER_FIRE:	  obj = new CFireFlower(); break;
		case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
		case OBJECT_TYPE_GOOMBAPARA:	  obj = new CGoombaPara(); break;
		case OBJECT_TYPE_KOOPASPARA:	  obj = new CKoopasPara(); break;
			break;
		case OBJECT_TYPE_COIN: obj = new CCoin(COIN); break;
		case OBJECT_TYPE_BRICK_QUESTION_SPECIAL_GREEN:	  obj = new CBrickQuestion(BRICK_QUESTION_STATUS_MUSHROOM_GREEN); break;
		case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlowerAttack(FLOWER_GREEN); break;
		case OBJECT_TYPE_FLOWER_NORMAL:	  obj = new CFlowerAttack(FLOWER_NORMAL); break;
		case OBJECT_TYPE_BRICK_BROKEN:	  obj = new CBrickBroken(); break;
		case OBJECT_TYPE_BRICK_QUESTION_EFFECT:	  obj = new CBrickQuestion(BRICK_QUESTION_STATUS_EFFECT); break;
		case OBJECT_TYPE_DRAIN: obj = new CDrain(); break;
		case OBJECT_TYPE_CARD:	  obj = new CCard(); break;
		case OBJECT_TYPE_HUD: obj = new CHUD(); break;
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

	if (player->x < 0) player->x = 0; //Ch?ng l�i qa tr�i


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
		if (mario->checkEnd == false && mario->GetState() != MARIO_STATE_DIE) {
			switch (KeyCode)
			{
			case DIK_S:
				if (mario->checkjumping == 1 && mario->GetState() != MARIO_STATE_DIE)
				{
					if (mario->GetSpeed() == 7 && mario->GetLevel() == MARIO_LEVEL_FOX)
					{
						mario->flyCan = true;
						mario->SetState(MARIO_STATE_FLY);
						mario->timeFly = GetTickCount();
					}
					else
					{
						if (mario->GetLevel() == MARIO_LEVEL_FOX) {
							mario->landingCheck = true;
						}
						else
						mario->SetState(MARIO_STATE_JUMP);
					}
				}
				else
				{
					if (mario->GetLevel() == MARIO_LEVEL_FOX)
					{
						mario->landingCheck = true;
					}
				}
				break;
			case DIK_DOWN:
				if (mario->flagDownDrain == true) {
					mario->SetState(MARIO_STATE_DRAIN_1);
				}else
				mario->sit = true;
				break;
			case DIK_A:
				if (mario->GetLevel() == MARIO_LEVEL_FOX && mario->sit != true)
				{
					mario->attackCheck = GetTickCount();
					mario->attack = true;
				}
				mario->holdKoopas = true;
				break;
			case DIK_1:
				mario->SetPosition(540, 80);
				break;
			case DIK_2:
				mario->SetPosition(1200, 80);
				break;
			case DIK_3:
				mario->SetPosition(1410, -140);
				break;
			case DIK_4:
				mario->SetPosition(1950, 80);
				break;
			case DIK_5:
				mario->SetPosition(2265, -200);
				break;
			case DIK_Z:
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					mario->y -= 12;
					mario->SetLevel(MARIO_LEVEL_BIG);
				}
				else {
					mario->y -= 3;
					mario->SetLevel(MARIO_LEVEL_FOX);
				}
				break;
			}

		}

	}

}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();


	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario != NULL)
	{
		if (mario->checkEnd == false && mario->GetState() != MARIO_STATE_DIE) {
			switch (KeyCode)
			{
			case DIK_DOWN:
				mario->sit = false;
				if (mario->GetLevel() != MARIO_LEVEL_SMALL)
					mario->y -= MARIO_SIT_BBOX_HEIGHT;
				break;
			case DIK_A:
				mario->holdKoopas = false;
				mario->holdKoopasCol = false;
				mario->startRun = 0;
				mario->stopRun = GetTickCount();
				mario->attack = false;
				break;
			case DIK_S:
				if (mario->flyCan == true)
				{
					mario->SetState(MARIO_STATE_LANDING);
					mario->landingCheck = true;
				}
				mario->flyCan = false;
				mario->timeFly = 0;
				mario->levelFly = 0;
				mario->landingCheck = false;
				break;
			default:
				break;
			}

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
		if (mario->GetState() != MARIO_STATE_DRAIN_1 && mario->GetState() != MARIO_STATE_DRAIN_2)
		{
			if (game->IsKeyDown(DIK_RIGHT))
			{
				if (game->IsKeyDown(DIK_A))
				{
					mario->SetState(MARIO_STATE_WALKING_RIGHT_FAST);
					if (mario->vx != 0)
					{
						if (mario->startRun == 0)
						{
							mario->startRun = GetTickCount();
							mario->stopRun = 0;
						}
					}
				}
				else
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
				if (game->IsKeyDown(DIK_A))
				{
					mario->SetState(MARIO_STATE_WALKING_LEFT_FAST);
					if (mario->vx != 0)
					{
						if (mario->startRun == 0)
						{
							mario->startRun = GetTickCount();
							mario->stopRun = 0;
						}
					}
				}
				else
					mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
			else
				mario->SetState(MARIO_STATE_IDLE);
			if (game->IsKeyDown(DIK_S) && mario->checkEnd == false) {
				if (mario->flyCan == false) {
					if (GetTickCount() - mario->timeLimitJump < 200 && mario->checkjumping == 0) {
						mario->SetState(MARIO_STATE_JUMP_HIGH);
					}
					else
					{
						mario->checkjumping = 1;
						mario->timeLimitJump = GetTickCount();
					}
				}
			}

		}
		if (game->IsKeyDown(DIK_UP) && mario->flagUpDrain == true) {
			if (game->IsKeyDown(DIK_S)) {
				mario->SetState(MARIO_STATE_DRAIN_2);
			}
		}
	}

}

