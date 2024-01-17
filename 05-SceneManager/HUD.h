#include "GameObject.h"


class CHUD : public CGameObject
{

public:
	CHUD();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};