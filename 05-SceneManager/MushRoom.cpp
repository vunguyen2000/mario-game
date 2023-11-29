#include "MushRoom.h"
#include "Mario.h"
#include "PlayScene.h"

void CMushRoom::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MUSHROOM_BBOX_WIDTH;
	b = y + MUSHROOM_BBOX_HEIGHT;
}
void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();
	CGameObject::Update(dt, coObjects);

	vy += MUSHROOM_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
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

		vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
