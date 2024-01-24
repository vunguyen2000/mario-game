#include "BrickBroken.h"
#include "PlayScene.h"

CBrickBroken::CBrickBroken()
{
	SetState(BRICK_BROKEN_STATE_SHOW);
}

void CBrickBroken::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount() - timeout > 5000)
	{
		if (timeout != -1 && state != BRICK_BROKEN_STATE_HIDE)
		{
			SetState(BRICK_BROKEN_STATE_SHOW);
			timeout = -1;
		}
	}
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
	if (state == BRICK_BROKEN_STATE_SHOW)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		if (state == BRICK_BROKEN_STATE_COIN)
			animation_set->at(1)->Render(x, y);
	}
}

void CBrickBroken::SetState(int state)
{
	CGameObject::SetState(state);
}

void CBrickBroken::BrokenAnimation()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	CNoCollision* col1 = new CNoCollision();
	CNoCollision* col2 = new CNoCollision();
	col1->SetState(STATE_ANI_BRICKBROKEN);
	col2->SetState(STATE_ANI_BRICKBROKEN);
	col1->SetPosition(x + 4, y + 4);
	col2->SetPosition(x + 4, y + 4);
	col1->vx = -0.1f;
	col1->vy = -0.1f;
	col2->vx = 0.1f;
	col2->vy = -0.1f;
	LPANIMATION_SET ani_set = animation_sets->Get(5012);
	col1->SetAnimationSet(ani_set);
	col2->SetAnimationSet(ani_set);
	((CPlayScene*)scene)->addObject(col1);
	((CPlayScene*)scene)->addObject(col2);
}