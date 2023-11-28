#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	bool check_rec = false;
	bool check_brickbroken = false;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CCoin*>(c->obj))
		{
			//nx = 0;
			ny = 0;
		}

	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
	
}



void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
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
		if (ny != 0) vy = 0;
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];
		if (e->ny < 0)
		{
			checkjumping = 0;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	float cx, cy;
	GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;

	if (x <= (game->GetScreenWidth() / 2)) cx = 0;
	if (y >= -46)
		cy = -10;


	else cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
	if (x < camX_update)
	{
		x = camX_update;
	}
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	{
		if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
		{
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		}
		else {
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
		if (checkjumping == 1)
		{
			if (nx < 0)
				ani = MARIO_ANI_SMALL_FLY_LEFT;
			else ani = MARIO_ANI_SMALL_FLY_RIGHT;
		}

		int alpha = 255;
		if (untouchable) alpha = 128;
		animation_set->at(ani)->Render(x, y, alpha);
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
				vx = MARIO_WALKING_SPEED_PLUS1;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
				vx = -MARIO_WALKING_SPEED_PLUS1;
		nx = -1;
		break;
	case MARIO_STATE_IDLE:
		checkidle = true;
		vx = 0;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		checkjumping = 1;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_JUMP_HIGH:
		checkjumping = 1;
		vy = -MARIO_JUMP_SPEED_Y_HIGH;
		break;
	}

}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}

void CMario::SetLevel(int l)
{

}

