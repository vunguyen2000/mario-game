#include <algorithm>
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
#include "KoopasPara.h"
#include "BrickBroken.h"
#include "Drain.h"
#include "Card.h"
#include "MushRoom.h"
#include "KoopasPara.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
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
			if (e->ny > 0 || e->nx != 0)
				continue;
		}
		if (dynamic_cast<CBoxs*>(coObjects->at(i)))
		{
			if (e->ny > 0 || e->nx != 0)
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
		{
			if (untouchable == 0)
				coEvents.push_back(e);
			else if (e->ny != 0 || dynamic_cast<CBrickQuestion*>(coObjects->at(i)))
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
		if (dynamic_cast<CBrickBroken*>(c->obj))
		{
			check_brickbroken = true;
			if (check_rec == true)
			{
				ny = c->ny;
				break;
			}
			CBrickBroken* question = dynamic_cast<CBrickBroken*>(c->obj);
			if (question->GetState() == BRICK_BROKEN_STATE_COIN)
			{
				if (c->ny != 0)
					ny = 0;
			}
		}
		if (dynamic_cast<CMushRoom*>(c->obj))
		{
			nx = 0;
		}
		if (dynamic_cast<CKoopasPara*>(c->obj))
		{
			nx = 0;
			ny = 0;
		}

	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

}



void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (flyCan == false && landingCheck == false && state != MARIO_STATE_DRAIN_1 && state != MARIO_STATE_DRAIN_2)
		vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE && state != MARIO_STATE_DRAIN_1 && state != MARIO_STATE_DRAIN_2) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	if (GetState() == MARIO_STATE_DIE)
	{
		Reset();
	}

	if (checkEnd == true)
	{
		nx = 1;
		vx = MARIO_WALKING_SPEED_ENDMAP;
		dx = vx * dt;
	}


	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (state == MARIO_STATE_DRAIN_1)
		{
			vy = MARIO_TIME_DRAIN_1_SPEED;
			dy = vy * dt;
		}
		else
		{
			if (state == MARIO_STATE_DRAIN_2)
			{
				vy = -MARIO_TIME_DRAIN_2_SPEED;
				dy = vy * dt;
			}
		}

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
				landingCheck = false;
			}
			if (dynamic_cast<CGoombaPara*>(e->obj))
			{
				CGoombaPara* goomba = dynamic_cast<CGoombaPara*>(e->obj);

				if (level == MARIO_LEVEL_FOX && attack == true)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE_DOWN);
					}
				}
				else
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (goomba->GetLevel() == GOOMBA_LEVEL_JUMP)
							{
								goomba->y -= 5;
								goomba->SetLevel(GOOMBA_LEVEL_WALKING);
							}
							else
							{
								if (goomba->GetLevel() == GOOMBA_LEVEL_WALKING)
								{
									marioScore += MARIO_SCORE;
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
				if (ny != 0 && state != MARIO_STATE_DIE) vy = 0;
			}
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (level == MARIO_LEVEL_FOX && attack == true)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE_DOWN);
					}
				}
				else
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							marioScore += MARIO_SCORE;
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
					checkjumping = 1;
					CBrickQuestion* brickQuestion = dynamic_cast<CBrickQuestion*>(e->obj);
					if (brickQuestion->GetBefore())
					{
						brickQuestion->SetUp(true);
						brickQuestion->SetBefore(false);
						brickQuestion->SetAfter(true);
						if (brickQuestion->GetStatus() == BRICK_QUESTION_STATUS_SPECIAL)
						{
							if (level == MARIO_LEVEL_SMALL)
							{
								brickQuestion->SetStatus(BRICK_QUESTION_STATUS_MUSHROOM);
							}
							else if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FOX)
							{
								brickQuestion->SetStatus(BRICK_QUESTION_STATUS_LEAF);
							}
						}
						brickQuestion->SetState(BRICK_QUESTION_STATE_AFTER);
						marioCoin++;
						marioScore += MARIO_SCORE;
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
						effect->y += 12;
						effect->SetState(EFFECT_P_STATE_AFTER);
						for (UINT i = 0; i < coObjects->size(); i++)
						{
							if (dynamic_cast<CBrickBroken*>(coObjects->at(i)))
							{
								CBrickBroken* brick = dynamic_cast<CBrickBroken*>(coObjects->at(i));
								if (brick->GetState() != BRICK_BROKEN_STATE_HIDE)
								{
									brick->timeout = GetTickCount();
									brick->SetState(BRICK_BROKEN_STATE_COIN);
								}
							}
						}
					}
				}
			}
			if (dynamic_cast<CFlowerAttack*>(e->obj))
			{
				CFlowerAttack* flower = dynamic_cast<CFlowerAttack*>(e->obj);
				if (level == MARIO_LEVEL_FOX && attack == true)
				{
					flower->isDisAppear = true;
				}
				else {
					if (untouchable == 0) {
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
				else if (level == MARIO_LEVEL_FOX && attack == true)
				{
					if (koopas->GetState() != KOOPAS_STATE_HIDE)
					{
						koopas->SetState(KOOPAS_STATE_HIDE);
					}
				}
				else if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_DIE)
					{
						if (koopas->GetState() != KOOPAS_STATE_THROW)
							marioScore += MARIO_SCORE;
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
					{
						koopas->SetState(KOOPAS_STATE_THROW);
						if (this->nx > 0)
						{
							koopas->vx = KOOPAS_RUN_SPEED;
							koopas->x = 16;
						}

						else
						{
							koopas->vx = -KOOPAS_RUN_SPEED;
							koopas->x -= 16;
						}
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
			if (dynamic_cast<CKoopasPara*>(e->obj))
			{
				CKoopasPara* koopas = dynamic_cast<CKoopasPara*>(e->obj);
				if (level == MARIO_LEVEL_FOX && attack == true)
				{
					if (koopas->GetState() != KOOPAS_STATE_HIDE)
					{
						koopas->SetState(KOOPAS_STATE_HIDE);
					}
				}
				else
					if (holdKoopas == true && koopas->GetState() == KOOPAS_STATE_DIE && holdKoopasCol == false)
					{
						holdKoopasCol = true;
						koopas->SetState(KOOPAS_STATE_HOLD);
					}
					else
						if (e->ny < 0)
						{
							if (koopas->GetState() != KOOPAS_STATE_THROW)
							{
								if (koopas->GetState() != KOOPAS_STATE_DIE)
								{
									if (koopas->GetState() == KOOPAS_STATE_JUMP)
									{
										koopas->SetState(KOOPAS_STATE_WALKING);
									}
									else if (koopas->GetState() == KOOPAS_STATE_WALKING)
									{
										marioScore += MARIO_SCORE;
										koopas->SetState(KOOPAS_STATE_DIE);
									}
								}
								else {
									koopas->SetState(KOOPAS_STATE_THROW);
									if (this->nx > 0)
									{
										koopas->vx = KOOPAS_RUN_SPEED;
										koopas->x += 16;
									}

									else
									{
										koopas->vx = -KOOPAS_RUN_SPEED;
										koopas->x -= 16;
									}
								}
							}
							else
							{
								koopas->SetState(KOOPAS_STATE_DIE);
								koopas->vx = 0;
							}
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							vx = vx * 1.2;
						}
						else if (e->nx != 0)
						{
							if (untouchable == 0)
							{
								if (koopas->GetState() != KOOPAS_STATE_DIE)
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
								else
								{
									if (koopas->vx != 0)
									{
										if (level == MARIO_LEVEL_FOX)
										{
											level = MARIO_LEVEL_BIG;
											StartUntouchable();
										}
										else
										{
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
			if (dynamic_cast<CCoin*>(e->obj))
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				coin->SetState(COIN_STATE_HIDE);
				marioCoin++;
			}
			if (dynamic_cast<CBrickBroken*>(e->obj))
			{
				CBrickBroken* brick = dynamic_cast<CBrickBroken*>(e->obj);
				if (brick->GetState() == BRICK_BROKEN_STATE_COIN)
				{
					brick->SetState(BRICK_BROKEN_STATE_HIDE);
					marioCoin++;
				}
				else
				{
					if (level == MARIO_LEVEL_FOX)
					{
						if (attack == true)
						{
							if (e->nx != 0)
							{
								/*	if (brick->y >= (y + MARIO_FOX_BBOX_HEIGHT / 3) && brick->y <= (y + MARIO_FOX_BBOX_HEIGHT))
									{*/
								if (brick->GetState() == BRICK_BROKEN_STATE_SHOW)
								{
									brick->BrokenAnimation();
									brick->SetState(BRICK_BROKEN_STATE_HIDE);
								}
								//}
							}
						}
					}
					if (level != MARIO_LEVEL_SMALL)
					{
						if (e->ny > 0)
						{
							if (brick->GetState() == BRICK_BROKEN_STATE_SHOW)
							{
								brick->BrokenAnimation();
								brick->SetState(BRICK_BROKEN_STATE_HIDE);
							}
						}
					}
				}
			}
			if (dynamic_cast<CDrain*>(e->obj))
			{
				if (e->ny < 0)
				{
					SetState(MARIO_STATE_DRAIN_1);
				}
				else
				{
					SetState(MARIO_STATE_DRAIN_2);
				}
			}
			if (dynamic_cast<CMushRoom*>(e->obj))
			{
				CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
				if (level == MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_BIG;
					y -= MARIO_BIG_BBOX_HEIGHT;
				}
				if (mushroom->green)
				{
					marioLife++;
				}
				e->obj->isDisAppear = true;
			}
			if (dynamic_cast<CCard*>(e->obj))
			{
				CCard* CCCard = dynamic_cast<CCard*>(e->obj);
				card = CCCard->GetState();
				CCCard->SetState(CARD_STATE_HIDE);
				checkEnd = true;
				LPSCENE scene = CGame::GetInstance()->GetCurrentScene();
				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				CNoCollision* content = new CNoCollision();
				content->SetPosition(2580, -20);
				LPANIMATION_SET content_ani = animation_sets->Get(5030);
				content->SetAnimationSet(content_ani);
				((CPlayScene*)scene)->addObject(content);
				switch (card)
				{
				case 1:
				{
					CNoCollision* item = new CNoCollision();
					item->SetPosition(2753, 24);
					LPANIMATION_SET item_ani = animation_sets->Get(5031);
					item->SetAnimationSet(item_ani);
					((CPlayScene*)scene)->addObject(item);
					break;
				}
				case 2:
				{
					CNoCollision* item = new CNoCollision();
					item->SetPosition(2753, 24);
					LPANIMATION_SET item_ani = animation_sets->Get(5032);
					item->SetAnimationSet(item_ani);
					((CPlayScene*)scene)->addObject(item);
					break;
				}
				case 3:
				{
					CNoCollision* item = new CNoCollision();
					item->SetPosition(2753, 24);
					LPANIMATION_SET item_ani = animation_sets->Get(5033);
					item->SetAnimationSet(item_ani);
					((CPlayScene*)scene)->addObject(item);
					break;
				}
				default:
					break;
				}
			}
		}
	}

	if (GetTickCount() - attackCheck > MARIO_TIME_ATTACK)
	{
		if (attackCheck != 0)
		{
			attack = false;
			attackCheck = 0;
		}
	}

	if (landingCheck == true)
	{
		SetState(MARIO_STATE_LANDING);
	}

	if (timeFly != 0 && GetTickCount() - timeFly > TIME_FLY)
	{
		flyCan = false;
		landingCheck = true;
		SetState(MARIO_STATE_LANDING);
		timeFly = 0;
		levelFly = 0;
	}

	if (startRun != 0)
	{
		if (holdKoopasCol != true)
			if (GetTickCount() - startRun > MARIO_FLY_LEVEL_SPEED)
			{
				if (levelFly < MARIO_FLY_LEVEL)
				{
					levelFly++;
				}
				startRun = 0;
			}
	}
	if (stopRun != 0 || landingCheck == true)
	{
		if (GetTickCount() - stopRun > MARIO_FLY_LEVEL_SPEED)
		{
			if (levelFly >= 0)
			{
				levelFly--;
			}
			stopRun = GetTickCount();
		}
	}
	if (state == MARIO_STATE_DRAIN_1)
	{
		if (GetTickCount() - timeDrain >= MARIO_TIME_DRAIN)
		{
			if (timeDrain != 0)
			{
				x = MARIO_TIME_DRAIN_1_X;
				y = MARIO_TIME_DRAIN_1_Y;
				timeDrain = 0;
			}
		}
		if (y >= MARIO_TIME_DRAIN_1_Y_DEFAULT)
		{
			SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}
	if (state == MARIO_STATE_DRAIN_2)
	{
		if (GetTickCount() - timeDrain >= MARIO_TIME_DRAIN)
		{
			if (timeDrain != 0)
			{
				x = MARIO_TIME_DRAIN_2_X;
				y = MARIO_TIME_DRAIN_2_Y;
				timeDrain = 0;
			}
		}
		if (y <= MARIO_TIME_DRAIN_2_Y_DEFAULT)
		{
			SetState(MARIO_STATE_WALKING_RIGHT);
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

	if (y > 250)
	{
		cx = 2030;
		cy = 266;
	}
	if (x > 2625)
	{
		cx = 2445;
		cy = -10;
	}

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
	else {
		if (level == MARIO_LEVEL_BIG) {
			if (GetState() == MARIO_STATE_DRAIN_1 || GetState() == MARIO_STATE_DRAIN_2)
			{
				ani = MARIO_ANI_BIG_DRAIN;
			}
			else
			{
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
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (GetState() == MARIO_STATE_DRAIN_1 || GetState() == MARIO_STATE_DRAIN_2)
			{
				ani = MARIO_ANI_SMALL_DRAIN;
			}
			else
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
			if (GetState() == MARIO_STATE_DRAIN_1 || GetState() == MARIO_STATE_DRAIN_2)
			{
				ani = MARIO_ANI_FOX_DRAIN;
			}
			else
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
							if (vx >= MARIO_GEARING_MAX)
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
				if (holdKoopas == true && holdKoopasCol == true)
				{
					if (vx == 0)
					{
						if (nx > 0)
							ani = MARIO_ANI_FOX_HOLDKOOPAS_RIGHT;
						else
							ani = MARIO_ANI_FOX_HOLDKOOPAS_LEFT;
					}
					else
					{
						if (vx > 0)
							ani = MARIO_ANI_FOX_HOLDKOOPAS_WALK_RIGHT;
						else
							ani = MARIO_ANI_FOX_HOLDKOOPAS_WALK_LEFT;
					}
				}
				else if (attack == true)
				{
					ani = MARIO_ANI_FOX_ATTACK;
				}
				else if (flyCan == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_FOX_FLY_RIGHT;
					else
						ani = MARIO_ANI_FOX_FLY_LEFT;
				}
				else if (landingCheck == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_FOX_LANDING_RIGHT;
					else
						ani = MARIO_ANI_FOX_LANDING_LEFT;
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
	case MARIO_STATE_JUMP_HIGH:
		vy = -MARIO_JUMP_SPEED_Y_HIGH;
		break;
	case MARIO_STATE_DIE:
		marioLife--;
		timeReset = GetTickCount();
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	case MARIO_STATE_FLY:
		vy = -MARIO_WALKING_FLY;
		checkjumping = 1;
		break;
	case MARIO_STATE_LANDING:
		vy = MARIO_WALKING_FLY;
		checkjumping = 1;
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
	case MARIO_STATE_DRAIN_1:
		timeDrain = GetTickCount();
		vx = 0;
		break;
	case MARIO_STATE_DRAIN_2:
		timeDrain = GetTickCount();
		vx = 0;
		break;
	}

}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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
	if (state == MARIO_STATE_DIE) {
		left = top = right = bottom = 0;
	}
}

void CMario::Reset()
{
	if (GetTickCount() - timeReset > 1000) {
		SetState(MARIO_STATE_IDLE);
		SetLevel(MARIO_LEVEL_SMALL);
		SetPosition(start_x, start_y);
		SetSpeed(0, 0);
	}

}
