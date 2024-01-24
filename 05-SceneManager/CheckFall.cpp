#include "Goomba.h"
#include <algorithm>
#include "Brick.h"
#include "Box.h"
#include "Brick.h"
#include "Koopas.h"
#include "Mario.h"
#include "GoombaPara.h"
#include "FireFlower.h"
#include "CheckFall.h"
CCheckFall::CCheckFall()
{
	SetState(GOOMBA_STATE_WALKING);

}
void CCheckFall::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CCheckFall*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CFireFlower*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CKoopas*>(coObjects->at(i)))
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
			if (koopas->GetState() == KOOPAS_STATE_WALKING)
				continue;
		}
		if (dynamic_cast<CMario*>(coObjects->at(i)))
		{
			CMario* mario = dynamic_cast<CMario*>(e->obj);
			if (mario->untouchable == 1)
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}

void CCheckFall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
	if (state == GOOMBA_STATE_DIE)
	{
		right = left = bottom = top = 0;
	}
}

void CCheckFall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	CGameObject::Update(dt);

	vy += GOOMBA_GRAVITY * dt;

	//Event va ch?m
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE_DOWN && state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (koopas->GetState() == KOOPAS_STATE_HOLD) {
					SetState(GOOMBA_STATE_DIE);
					koopas->SetState(KOOPAS_STATE_HIDE);
				}
				else if (koopas->GetState() == KOOPAS_STATE_THROW) {
					SetState(GOOMBA_STATE_DIE);
				}
			}
			else if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CCheckFall::Render()
{

	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);

}

void CCheckFall::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = -0.2f;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;

	}
}
