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
}

void CNoCollision::SetState(int state)
{
	CGameObject::SetState(state);
}

