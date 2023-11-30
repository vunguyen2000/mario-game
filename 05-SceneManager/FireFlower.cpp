#include "FireFlower.h"
#include "Mario.h"
#include "Playscene.h"
#include <algorithm>

void CFireFlower::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CMario*>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}


void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (!((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->IsInUseArea(x-20, y-20))
	{
		SetState(FIRE_FLOWER_STATE_HIDE);
	}
}

void CFireFlower::Render()
{
	int ani = -1;

	if (checkAppear)
	{
		ani = FIRE_FLOWER_ANI_ATTACK;
	}
	else return;
	animation_set->at(ani)->Render(x, y);
}

void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FIRE_FLOWER_STATE_ATTACK:
		checkAppear = true;
		break;
	case FIRE_FLOWER_STATE_HIDE:
		checkAppear = false;
		vx = 0;
		vy = 0;
		SetPosition(-100, -100);
		break;
	}
}

void CFireFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIRE_FLOWER_BBOX_WIDTH;
	b = y + FIRE_FLOWER_BBOX_HEIGHT;
}