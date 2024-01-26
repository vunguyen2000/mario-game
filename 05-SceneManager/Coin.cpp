#include "Coin.h"
#include "BrickQuestion.h"
#include <algorithm>
#include "Koopas.h"
#include "Box.h"


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

		if (dynamic_cast<CKoopas*>(coObjects->at(i)))
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

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != COIN_STATE_IDLE)
	{
		vy += 0.003f * dt;
		CalcPotentialCollisions(coObjects, coEvents);

	}


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (!show) return;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		// Collision logic with other objects
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrickQuestion*>(e->obj))
			{
				state = COIN_STATE_HIDE;
			}
			if (dynamic_cast<CBoxs*>(e->obj))
			{
				state = COIN_STATE_HIDE;
			}
		}
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
	this->state = state;
	switch (this->state)
	{
	case COIN_STATE_IDLE:
		vx = vy = 0;
		break;
	case COIN_STATE_UP:
		vx = 0;
		vy = -0.5f;
	default:
		break;
	}
}