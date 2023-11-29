#include "Coin.h"
#include <algorithm>


#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 15


#define COIN_STATE_IDLE		0
#define COIN_STATE_UP		1
#define COIN_STATE_HIDE		2

#define COIN			100


CCoin::CCoin(int temp)
{
	this->status = temp;
	if (status == COIN)
	{
		show = true;
	}
	else
		show = false;
	SetState(COIN_STATE_IDLE);
}

void CCoin::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CCoin*>(coObjects->at(i)))
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

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == COIN_STATE_HIDE)
	{
		l = t = r = b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}

}
void CCoin::Render()
{
	{
		if (state == COIN_STATE_HIDE)
			return;
		animation_set->at(0)->Render(x, y);
	}
}

void CCoin::SetState(int state)
{

}