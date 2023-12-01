#include "Koopas.h"


CKoopas::CKoopas()
{
}

void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

}


void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
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

}