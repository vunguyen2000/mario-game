#pragma once
#include "GameObject.h"

#define BOX_BBOX_WIDTH  16
#define BOX_BBOX_HEIGHT  16
#define BOX_STATUS_START  1
#define BOX_STATUS_END  2
#define BOX_STATUS_NORMAL  3

class CBox : public CGameObject
{
public:
	float status;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	CBox(float status);
	int GetStatus()
	{
		return status;
	}
};

