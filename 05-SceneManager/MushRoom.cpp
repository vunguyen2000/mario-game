#include "MushRoom.h"
#include "Mario.h"
#include "PlayScene.h"
#include <algorithm>
#include "Brick.h"
#include "BrickQuestion.h"

void CMushRoom::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CMushRoom::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CMario*>(coObjects->at(i)) || dynamic_cast<CBrick*>(coObjects->at(i)) ||dynamic_cast<CBrickQuestion*>(coObjects->at(i)))
		{
			if (e->t > 0 && e->t <= 1.0f && e->nx==0)
				coEvents.push_back(e);
			else
				delete e;
		}
	
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

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
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{

				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					mario->SetLevel(MARIO_LEVEL_BIG);
					mario->y -= MARIO_BIG_BBOX_HEIGHT;
				}
				isDisAppear = true;
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
