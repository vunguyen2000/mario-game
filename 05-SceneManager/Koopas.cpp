#include "Koopas.h"
#include "Box.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include <algorithm>
#include "FireFlower.h"
#include "BrickQuestion.h"
#include "Coin.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CBox*>(coObjects->at(i)))
		{
			if (e->nx != 0)
				continue;
		}
		if (dynamic_cast<CGoomba*>(coObjects->at(i)))
		{
			if (GetState() != KOOPAS_STATE_DIE)
				continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);


}


void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	if (state == KOOPAS_STATE_HIDE)
	{
		left = top = right = bottom = 0;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	CGameObject::Update(dt);
	vy += dt * KOOPAS_GRAVITY;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	float tempy = y + dy;
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		if (nx < 0)
			y += min_ty * dy + ny * 0.4f;

		if (ny != 0) {
			vy = 0;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBox*>(e->obj))
			{
				CBox* box = dynamic_cast<CBox*>(e->obj);
				if (e->nx != 0)
				{
					x += dx;
				}
				else {
					if (state != KOOPAS_STATE_DIE)
					{
						if (box->GetStatus() == BOX_STATUS_START) {
							if (x < box->x) {
								x = box->x;
								vx = -vx;
							}
							else {
								x += dx;
							}
						}
						else if (box->GetStatus() == BOX_STATUS_END) {
							if (x > box->x) {
								x = box->x;
								vx = -vx;
							}
							else {
								x += dx;
							}
						}
					}
				}
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (nx != 0)
				{
					vx = -vx;
				}
			}
			if (!dynamic_cast<CMario*>(e->obj) && nx == 0)
			{
				tempbacky = y;
				back = true;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (x < 0)
	{
		x = 5;
		vx = -vx;
	}
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		if (vx != 0)
		{
			ani = KOOPAS_ANI_TURN;
		}
		else
		{
			ani = KOOPAS_ANI_DIE;
		}
	if (vx != 0)
	{

		ani = KOOPAS_ANI_TURN;
	}
	else
	{
		ani = KOOPAS_ANI_DIE;
	}
	}
	else if (vx > 0)
	{
			ani = 5;
			ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	else if (vx < 0)
	{
			ani = KOOPAS_ANI_WALKING_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

}

void CKoopas::SetState(int state)
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_HOLD:
		vx = 0;
		vy = 0;
		break;
	}
	DebugOut(L"[ERROR] Vo %d\n");
}