#include "NoCollision.h"

void CNoCollision::Render()
{
	animation_set->at(0)->Render(x, y);
}
void CNoCollision::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}

void CNoCollision::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_ANI_BRICKBROKEN)
	{
		CGameObject::Update(dt);
		vy += 0.002f * dt;
		x += dx;
		y += dy;
	}
}

void CNoCollision::SetState(int state)
{
	CGameObject::SetState(state);
}

