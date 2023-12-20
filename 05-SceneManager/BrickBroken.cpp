#include "BrickBroken.h"
#include "PlayScene.h"

CBrickBroken::CBrickBroken()
{
	SetState(BRICK_BROKEN_STATE_SHOW);
}

void CBrickBroken::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CBrickBroken::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == BRICK_BROKEN_STATE_SHOW)
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
	else if (state == BRICK_BROKEN_STATE_COIN)
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}
void CBrickBroken::Render()
{
	{
		animation_set->at(0)->Render(x, y);
	}
}