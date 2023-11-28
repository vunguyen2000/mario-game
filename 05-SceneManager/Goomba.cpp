#include "Goomba.h"
#include <algorithm>
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
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

void CGoomba::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++) {
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CGoomba*>(coObjects->at(i)))
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

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects){

	CGameObject::Update(dt);

	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE_DOWN && state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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
	
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (x < 0)
	{
		x = GOOMBA_START_X;
		vx = -vx;
	}
}


void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);
}

void CGoomba::SetState(int state)
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
