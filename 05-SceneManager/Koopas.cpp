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
}

void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

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
	
}

void CKoopas::Render()
{

	animation_set->at(0)->Render(x, y);

}

void CKoopas::SetState(int state)
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	CGameObject::SetState(state);
}