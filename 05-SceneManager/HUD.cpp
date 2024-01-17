#include "Scence.h"
#include "PlayScene.h"
#include <string>
#include "HUD.h"
#include "Game.h"
#include "Mario.h"

CHUD::CHUD()
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)game->GetCurrentScene())->GetPlayer();

	CSprites* sprites = CSprites::GetInstance();

	backgroundBlack = sprites->Get(HUD_BACKGROUND_SPRITE);
	hudBoard = sprites->Get(HUD_BOARD_SPRITE);
	marioLife = mario->GetLife();
}


void CHUD::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}

void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	SetPosition(game->GetCamPosX(), game->GetCamPosY() + 260);
	CMario* mario = ((CPlayScene*)game->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE)
	{
		marioLife = mario->GetLife();
		string stringLife = to_string(marioLife);
		while (stringLife.length() < LIFE_LENGTH)
		{
			stringLife = "0" + stringLife;
		}
		life = StringToSprite(stringLife);
		return;

	}
	marioLife = mario->GetLife();
	string stringLife = to_string(marioLife);
	while (stringLife.length() < LIFE_LENGTH)
	{
		stringLife = "0" + stringLife;
	}
	life = StringToSprite(stringLife);
}

void CHUD::Render()
{
	backgroundBlack->Draw(x, y - 84);
	hudBoard->Draw(x + HUDBOARD_X, y - HUDBOARD_Y);
	for (int i = 0; i < life.size(); i++)
	{
		life[i]->Draw(x + HUD_CHAR_WIDTH_L + HUD_CHAR_WIDTH * i, y - HUD_CHAR_HEIGHT_LTS);
	}
}

vector<LPSPRITE> CHUD::StringToSprite(string str)
{
	vector<LPSPRITE> spritesVector;
	LPSPRITE sprite;
	char temp;
	for (int i = 0; i < str.size(); i++)
	{
		temp = (char)str[i];
		sprite = getHUD(temp);
		if (sprite != NULL)
			spritesVector.push_back(sprite);
	}
	return spritesVector;
}

LPSPRITE CHUD::getHUD(char a)
{
	CSprites* number = CSprites::GetInstance();
	switch (a)
	{
	case '0':
		return number->Get(HUD_0_SPRITE);
		break;
	case '1':
		return number->Get(HUD_1_SPRITE);
		break;
	case '2':
		return number->Get(HUD_2_SPRITE);
		break;
	case '3':
		return number->Get(HUD_3_SPRITE);
		break;
	case '4':
		return number->Get(HUD_4_SPRITE);
		break;
	case '5':
		return number->Get(HUD_5_SPRITE);
		break;
	case '6':
		return number->Get(HUD_6_SPRITE);
		break;
	case '7':
		return number->Get(HUD_7_SPRITE);
		break;
	case '8':
		return number->Get(HUD_8_SPRITE);
		break;
	case '9':
		return number->Get(HUD_9_SPRITE);
		break;
	default:
		return NULL;
		break;
	}
}

