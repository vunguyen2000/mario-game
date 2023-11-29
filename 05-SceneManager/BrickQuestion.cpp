#include "BrickQuestion.h"
#include <algorithm>
#include "Effect.h"
CBrickQuestion::CBrickQuestion(int status)
{
	this->status = status;
	state = -1;
}


void CBrickQuestion::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CCoin*>(coObjects->at(i)))
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

void CBrickQuestion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_QUESTION_BBOX_WIDTH;
	b = y + BRICK_QUESTION_BBOX_HEIGHT;

}

void CBrickQuestion::Render()
{
	int ani = -1;

	if (status == BRICK_QUESTION_STATUS_EFFECT)
	{
		if (status_before)
		{
			ani = 2;
		}
		else
			ani = 3;
	}
	else
	{
		if (status_before)
		{
			ani = BRICK_QUESTION_ANI_BEFORE;
		}
		else
			ani = BRICK_QUESTION_ANI_AFTER;
	}
	animation_set->at(ani)->Render(x, y);

}

void CBrickQuestion::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CalcPotentialCollisions(coObjects, coEvents);

	if (status_after)//n?u g?ch k còn tr?ng thái ch?m h?i
	{
		
			if (check)
			{
				if (timeAni > BRICK_QUESTION_COUNT_TIME)
				{
					timeAni = 0;
					check = false;
				}
				else
				{
					y -= BRICK_QUESTION_COUNT_Y;
					timeAni++;
				}
			}
			else
			{
				if (timeAni <= BRICK_QUESTION_COUNT_TIME)
				{
					y += BRICK_QUESTION_COUNT_Y;
					timeAni++;
				}
				else
				{
					status_before = false;
				}
			}
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


}

void CBrickQuestion::SetState(int state)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
	if (state == BRICK_QUESTION_STATE_AFTER)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		switch (status)
		{
		case BRICK_QUESTION_STATUS_EFFECT:
		{
			CEffect* effect = new CEffect();
			effect->SetPosition(x, y - 16);
			((CPlayScene*)scene)->addObject(effect);
			break;
		}
		default:
			break;
		}
	}
}