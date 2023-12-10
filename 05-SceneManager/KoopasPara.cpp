#include "KoopasPara.h"
#include <algorithm>
#include "PlayScene.h"
#include "Game.h"
CKoopasPara::CKoopasPara()
{
}

void CKoopasPara::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}

void CKoopasPara::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}

void CKoopasPara::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CKoopasPara::Render()
{

	animation_set->at(0)->Render(x, y);

}
void CKoopasPara::SetState(int state)
{

}