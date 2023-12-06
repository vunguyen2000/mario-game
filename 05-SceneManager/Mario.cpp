﻿#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Box.h"
#include "BrickQuestion.h"
#include "Effect.h"
#include "FlowerAttack.h"
#include "FireFlower.h"
#include "Koopas.h"
#include "FlowerAttack.h"
#include "GoombaPara.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_FOX;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CBox*>(coObjects->at(i)))
		{
			if(e->ny > 0 || e->nx !=0)
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
		{
			if(untouchable == 0)
			coEvents.push_back(e);
			else if(e->ny != 0)
			{
				coEvents.push_back(e);
			}
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

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

	if (state != MARIO_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}
	if (GetState() == MARIO_STATE_DIE)
	{
		Reset();
	}

	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	
	// No collision occured, proceed normally
	if (coEvents.size() == 0 )
	{
		x += dx;
		y += dy;
		checkFree = true;
	}
	else
	{
		checkFree = false;
		float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		//// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		// Collision logic with other objects
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny < 0)
			{
				checkjumping = 0;
			}
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->ny < 0 )
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FOX)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
			}
			if (dynamic_cast<CBrickQuestion*>(e->obj))
			{
				if (e->ny > 0)
				{
					CBrickQuestion* brickQuestion = dynamic_cast<CBrickQuestion*>(e->obj);
					int ids = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (brickQuestion->GetBefore())
					{
						brickQuestion->SetUp(true);
						brickQuestion->SetBefore(false);
						brickQuestion->SetAfter(true);
						if (brickQuestion->GetStatus() == BRICK_MUSHROOM_GREEN) {
							if (level == MARIO_LEVEL_SMALL)
							{
								brickQuestion->SetStatus(BRICK_QUESTION_STATUS_MUSHROOM);
							}
						}
						brickQuestion->SetState(BRICK_QUESTION_STATE_AFTER);
					}

				}
			}
			if (dynamic_cast<CEffect*>(e->obj))
			{
				CEffect* effect = dynamic_cast<CEffect*>(e->obj);
				if (e->ny < 0)
				{
					if (effect->GetState() != EFFECT_P_STATE_AFTER)
					{
						effect->y += 20;
						effect->SetState(EFFECT_P_STATE_AFTER);
					}
				}
			}
			if (dynamic_cast<CFlowerAttack*>(e->obj))
			{
				CFlowerAttack* flower = dynamic_cast<CFlowerAttack*>(e->obj);
				
							if (level == MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_FOX)
							{
								level = MARIO_LEVEL_BIG;
								StartUntouchable();
							}
							else
							{
								SetState(MARIO_STATE_DIE);
							}
			}
			if (dynamic_cast<CFireFlower*>(e->obj))
			{
				CFireFlower* fire = dynamic_cast<CFireFlower*>(e->obj);
				fire->SetState(FIRE_FLOWER_STATE_HIDE);
				if (level == MARIO_LEVEL_BIG)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_FOX)
				{
					level = MARIO_LEVEL_BIG;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (holdKoopas == true && koopas->GetState() == KOOPAS_STATE_DIE && holdKoopasCol == false)
				{
					holdKoopasCol = true;
					koopas->SetState(KOOPAS_STATE_HOLD);
				}
				else if (e->ny < 0)
				{
					if ( koopas->GetState() != KOOPAS_STATE_DIE)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
					{
						koopas->vx = 0;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0 && koopas->GetState() != KOOPAS_STATE_HOLD)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
						{
								if (level == MARIO_LEVEL_BIG)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else if (level == MARIO_LEVEL_FOX)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else
								{
									SetState(MARIO_STATE_DIE);
								}
						}
						else
						{
							if (koopas->vx != 0)
							{
									if (level == MARIO_LEVEL_BIG)
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable();
									}
									else if (level == MARIO_LEVEL_FOX)
									{
										level = MARIO_LEVEL_BIG;
										StartUntouchable();
									}
									else
									{
										SetState(MARIO_STATE_DIE);
									}
							}
							else
							{
								koopas->SetState(KOOPAS_STATE_THROW);
								if (nx > 0)
								{
									koopas->vx = -KOOPAS_RUN_SPEED;
								}

								else
								{
									koopas->vx = +KOOPAS_RUN_SPEED;
								}

							}
						}
					}
				}
			}
			if (dynamic_cast<CGoombaPara*>(e->obj))
			{
				CGoombaPara* goomba = dynamic_cast<CGoombaPara*>(e->obj);
					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							{
								if (goomba->GetLevel() == GOOMBA_LEVEL_WALKING)
								{
									goomba->y -= 5;
									goomba->SetState(GOOMBA_STATE_DIE);
								}
							}
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level == MARIO_LEVEL_FOX)
								{
									level = MARIO_LEVEL_BIG;
									StartUntouchable();
								}
								else
									if (level == MARIO_LEVEL_BIG)
									{
										level = MARIO_LEVEL_SMALL;
										StartUntouchable();
									}
									else
									{
										SetState(MARIO_STATE_DIE);
									}
							}
						}
					}
			}

			else
			{
				if (ny != 0 && state!= MARIO_STATE_DIE) vy = 0;
			}
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
	else{
		if (level == MARIO_LEVEL_BIG) {

			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (nx > 0)
				{
					if (vx < MARIO_GEARING_MAX && checkidle != true)
					{
						ani = MARIO_ANI_BIG_GEARING_RIGHT;
					}
					else
					{
						if (vx > MARIO_GEARING_MAX)
							checkidle = false;
						if (state == MARIO_STATE_WALKING_RIGHT_FAST)
							ani = MARIO_ANI_BIG_RUN_RIGHT;
						else
							ani = MARIO_ANI_BIG_WALKING_RIGHT;
					}

				}
				else
				{
					if (vx < MARIO_GEARING_MAX)
					{
						ani = MARIO_ANI_BIG_GEARING_LEFT;
					}
					else
						if (state == MARIO_STATE_WALKING_LEFT_FAST)
							ani = MARIO_ANI_BIG_RUN_LEFT;
						else
							ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
			}
			else
			{
				if (nx < 0)
				{
					if (vx > -MARIO_GEARING_MAX && checkidle != true)
					{
						ani = MARIO_ANI_BIG_GEARING_LEFT;
					}
					else
					{
						if (vx < -MARIO_GEARING_MAX)
							checkidle = false;
						if (state == MARIO_STATE_WALKING_LEFT_FAST)
							ani = MARIO_ANI_BIG_RUN_LEFT;
						else
							ani = MARIO_ANI_BIG_WALKING_LEFT;
					}

				}
				else
				{
					if (vx > -MARIO_GEARING_MAX)
					{
						ani = MARIO_ANI_BIG_GEARING_RIGHT;
					}
					else
						if (state == MARIO_STATE_WALKING_RIGHT_FAST)
							ani = MARIO_ANI_BIG_RUN_RIGHT;
						else
							ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
			}
			if (sit == true)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_SIT_RIGHT;
				else
					ani = MARIO_ANI_BIG_SIT_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			{
				if (vx == 0)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
				else if (vx > 0)
				{
					if (nx > 0)
					{
						if (vx < MARIO_GEARING_MAX && checkidle != true)
						{
							ani = MARIO_ANI_SMALL_GEARING_RIGHT;
						}
						else
						{
							if (vx > MARIO_GEARING_MAX)
								checkidle = false;
							if (state == MARIO_STATE_WALKING_RIGHT_FAST)
								ani = MARIO_ANI_SMALL_RUN_RIGHT;
							else
								ani = MARIO_ANI_SMALL_WALKING_RIGHT;
						}

					}
					else
					{
						if (vx < MARIO_GEARING_MAX)
						{
							ani = MARIO_ANI_SMALL_GEARING_LEFT;
						}
						else
							if (state == MARIO_STATE_WALKING_LEFT_FAST)
								ani = MARIO_ANI_SMALL_RUN_LEFT;
							else
								ani = MARIO_ANI_SMALL_WALKING_LEFT;
					}
				}
				else
				{
					if (nx < 0)
					{
						if (vx > -MARIO_GEARING_MAX && checkidle != true)
						{
							ani = MARIO_ANI_SMALL_GEARING_LEFT;
						}
						else {
							if (vx < -MARIO_GEARING_MAX)
								checkidle = false;
							if (state == MARIO_STATE_WALKING_LEFT_FAST)
								ani = MARIO_ANI_SMALL_RUN_LEFT;
							else
								ani = MARIO_ANI_SMALL_WALKING_LEFT;
						}

					}
					else
					{
						if (vx > -MARIO_GEARING_MAX)
						{
							ani = MARIO_ANI_SMALL_GEARING_RIGHT;
						}
						else
							if (state == MARIO_STATE_WALKING_RIGHT_FAST)
								ani = MARIO_ANI_SMALL_RUN_RIGHT;
							else
								ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					}
				}

				if (checkjumping == 1)
				{
					if (nx < 0)
						ani = MARIO_ANI_SMALL_FLY_LEFT;
					else ani = MARIO_ANI_SMALL_FLY_RIGHT;
				}
				if (holdKoopas == true && holdKoopasCol == true)
				{
					if (vx == 0)
					{
						if (nx > 0)
							ani = MARIO_ANI_SMALL_HOLDKOOPAS_RIGHT;
						else
							ani = MARIO_ANI_SMALL_HOLDKOOPAS_LEFT;
					}
					else
					{
						if (vx > 0)
							ani = MARIO_ANI_SMALL_HOLDKOOPAS_WALK_RIGHT;
						else
							ani = MARIO_ANI_SMALL_HOLDKOOPAS_WALK_LEFT;
					}


				}
			}


		}
		else if (level == MARIO_LEVEL_FOX)
		{
				if (vx == 0)
				{
					if (nx > 0) ani = MARIO_ANI_FOX_IDLE_RIGHT;
					else ani = MARIO_ANI_FOX_IDLE_LEFT;
				}
				else if (vx > 0)
				{
					if (nx > 0)
					{
						if (vx < MARIO_GEARING_MAX && checkidle != true)
						{
							ani = MARIO_ANI_FOX_GEARING_RIGHT;
						}
						else
						{
							if (vx > MARIO_GEARING_MAX)
								checkidle = false;
							if (state == MARIO_STATE_WALKING_RIGHT_FAST)
								ani = MARIO_ANI_FOX_RUN_RIGHT;
							else
								ani = MARIO_ANI_FOX_WALKING_RIGHT;
						}
					}
					else
					{
						if (vx < MARIO_GEARING_MAX)
						{
							ani = MARIO_ANI_FOX_GEARING_LEFT;
						}
						else
							if (state == MARIO_STATE_WALKING_LEFT_FAST)
								ani = MARIO_ANI_FOX_RUN_LEFT;
							else
								ani = MARIO_ANI_FOX_WALKING_LEFT;
					}
				}
				else
				{
					if (nx < 0)
					{
						if (vx > -0.051f && checkidle != true)
						{
							ani = MARIO_ANI_FOX_GEARING_LEFT;
						}
						else
						{
							if (vx < -0.051f)
								checkidle = false;
							if (state == MARIO_STATE_WALKING_LEFT_FAST)
								ani = MARIO_ANI_FOX_RUN_LEFT;
							else
								ani = MARIO_ANI_FOX_WALKING_LEFT;
						}

					}
					else
					{
						if (vx > -0.051f)
						{
							ani = MARIO_ANI_FOX_GEARING_RIGHT;
						}
						else
							if (state == MARIO_STATE_WALKING_RIGHT_FAST)
								ani = MARIO_ANI_FOX_RUN_RIGHT;
							else
								ani = MARIO_ANI_FOX_WALKING_RIGHT;
					}
				}
				if (checkjumping == 1)
				{
					if (nx < 0)
						ani = MARIO_ANI_FOX_JUMP_LEFT;
					else ani = MARIO_ANI_FOX_JUMP_RIGHT;
				}

				else if (sit == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_FOX_SIT_RIGHT;
					else
						ani = MARIO_ANI_FOX_SIT_LEFT;
				}
			}
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);
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
	case MARIO_STATE_DIE:
		timeReset = GetTickCount();
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	case MARIO_STATE_WALKING_RIGHT_FAST:
		if (vx < MARIO_WALKING_RUN_MAX)
		{
			if (vx < -LANDING_LIMIT)//giới hạn tốc độ để mario lấy đà mượt hơn
				vx = -LANDING_LIMIT_SPEED;
			else
			{
				vx += MARIO_WALKING_SPEED_PLUS;
			}
		}
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT_FAST:
		if (vx > -MARIO_WALKING_RUN_MAX)
		{
			if (vx > LANDING_LIMIT)
				vx = LANDING_LIMIT_SPEED;
			else
			{
				vx -= MARIO_WALKING_SPEED_PLUS;
			}
		}
		nx = -1;
		break;
	}

}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FOX)
	{
		right = x + MARIO_FOX_BBOX_WIDTH;
		bottom = y + MARIO_FOX_BBOX_HEIGHT;
	}
	if (sit == true)
	{
		if (level != MARIO_LEVEL_SMALL)
			bottom = y + MARIO_SIT_BBOX_HEIGHT;
	}
	if (state == MARIO_STATE_DIE ) {
		left = top = right = bottom = 0;
	}
}

void CMario::Reset()	
{
	if(GetTickCount()-timeReset>1000){
		SetState(MARIO_STATE_IDLE);
		SetLevel(MARIO_LEVEL_SMALL);
		SetPosition(start_x, start_y);
		SetSpeed(0, 0);
	}

}
