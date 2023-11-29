#include "Effect.h"

CEffect::CEffect()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(5013);
	SetAnimationSet(ani_set);
}

void CEffect::Render()
{
	if (state == EFFECT_P_STATE_BEFORE)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		animation_set->at(1)->Render(x, y);
	}
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == EFFECT_P_STATE_BEFORE)
	{
		l = x;
		t = y;
		r = x + EFFECT_P_BBOX_WIDTH;
		b = y + EFFECT_P_BBOX_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + EFFECT_P_AFTER_BBOX_WIDTH;
		b = y + EFFECT_P_AFTER_BBOX_HEIGHT;
	}
	
}

void CEffect::SetState(int state)
{
	CGameObject::SetState(state);
}



