#include "FlowerAttack.h"
#include "Box.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScene.h"
#include <algorithm>
#include "FireFlower.h"


void CFlowerAttack::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	switch (status)
	{
	case FLOWER_RED:
		l = x;
		t = y;
		r = x + FLOWER_RED_BBOX_WIDTH;
		b = y + FLOWER_RED_BBOX_HEIGHT;
		break;
	case FLOWER_GREEN:
		l = x;
		t = y;
		r = x + FLOWER_GREEN_BBOX_WIDTH;
		b = y + FLOWER_GREEN_BBOX_HEIGHT;
		break;
	}
}

void CFlowerAttack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGame* game = CGame::GetInstance();

	if (abs(mario->x - x) < game->GetScreenWidth() / 2 && isTimeAgain != 0)
	{
		Up = true;
	}
	else
	{
		Up = false;
	}

	coEvents.clear();
	if (Up)
	{
		if (isTime == 0)
		{
			Appear();
			isTimeAgain = GetTickCount();
		}
		if (GetTickCount() - isTime <= 3000)
		{
			vy = -0.02f;
			if (y <= 71)
			{
				vy = 0;
				shooting = true;
				Attack(coObjects);
			}
		}
		else
		{
			if (y <= 71)
		isTimeAgain = 0;
			isTime = 0;
			Up = false;
			shooting = false;
			shooted = false;
		}
	}
	else
	{
		if (isTime == 0)
		{
			Appear();
		}
		if (GetTickCount() - isTime <= 3000)
		{
			vy = 0.02f;
			if (y >= 117)
			{
				vy = 0;
			}
		}
		else
		{
			isTimeAgain = 1;
			Up = true;
			isTime = 0;
		}
	}
	if (coEvents.size() == 0)
	{
		y += dy;
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CFlowerAttack::Render()
{
	int ani = -1;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->x <= x)
	{
		if (mario->y >= y)
		{
			if (vy == 0)
			{
				ani = FLOWER_RED_ANI_LEFT_DOWN;
			}
			else
			{
				ani = FLOWER_RED_ANI_LEFT_UP;
			}

		}
		else
		{
			if (vy == 0)
			{
				ani = FLOWER_RED_ANI_LEFT_UP;
			}
			else
			{
				ani = FLOWER_RED_ANI_LEFT_UP;
			}
		}

	}
	else
	{
		if (mario->y >= y)
		{
			if (vy == 0)
			{
				ani = FLOWER_RED_ANI_RIGHT_DOWN;
			}
			else
			{
				ani = FLOWER_RED_ANI_LEFT_UP;
			}

		}
		else
		{
			if (vy == 0)
			{
				ani = FLOWER_RED_ANI_RIGHT_UP;
			}
			else
			{
				ani = FLOWER_RED_ANI_RIGHT_UP;
			}
		}
	}
	animation_set->at(ani)->Render(x, y);
}

void CFlowerAttack::Attack(vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (abs(mario->x - x) >= game->GetScreenWidth() / 2 || shooted == true) return;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CFireFlower*>(obj))
		{
			CFireFlower* fire = dynamic_cast<CFireFlower*>(obj);
			if (fire->GetAppear()) continue;
			if (shooting)
			{
				shooting = false;
				shooted = true;
				fire->y = y + 3;
				if (mario->x <= x)
				{
					fire->x = x - 1;
					nx = -1;
				}
				else
				{
					fire->x = x + FLOWER_RED_BBOX_WIDTH + 2;
					this->nx = 1;
				}
				if (mario->y <= y)
				{
					this->SetShootY(-1);
				}
				else
				{
					this->SetShootY(1);
				}
				if (abs(mario->x - x) <= FIRE_FLOWER_X_DEFAULT)
				{
					fire->vx = FIRE_FLOWER_SPEED * nx;
					fire->vy = FIRE_FLOWER_SPEED * shootY;
				}
				else
				{
					fire->vx = FIRE_FLOWER_SPEED * this->nx;
					fire->vy = FIRE_FLOWER_SPEED / 2 * shootY;
				}
				fire->SetState(FIRE_FLOWER_STATE_ATTACK);
				return;
			}
		}
	}
}