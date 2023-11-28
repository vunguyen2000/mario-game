#include "Goomba.h"
#include <algorithm>
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);

}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
}

void CGoomba::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)

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
}
