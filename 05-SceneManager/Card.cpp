#include "Card.h"
void CCard::Render()
{
	switch (state)
	{
	case CARD_STATE_MUSHROOM:
	{
		animation_set->at(CARD_ANI_MUSHROOM)->Render(x, y);
		break;
	}
	case CARD_STATE_STAR:
	{
		animation_set->at(CARD_ANI_STAR)->Render(x, y);
		break;
	}
	case CARD_STATE_TREE:
	{
		animation_set->at(CARD_ANI_TREE)->Render(x, y);
		break;
	}
	default:
		break;
	}
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != CARD_STATE_HIDE)
	{
		l = x;
		t = y;
		r = x + CARD_BBOX_WIDTH;
		b = y + CARD_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != CARD_STATE_HIDE)
	{
		if (GetTickCount() - timeChange >= 1000)
		{
			if (state < 3)
			{
				state++;
			}
			else
			{
				state = CARD_STATE_MUSHROOM;
			}
			timeChange = GetTickCount();
		}
	}
}

