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

}
void CBrickBroken::Render()
{
	{
		animation_set->at(0)->Render(x, y);
	}
}