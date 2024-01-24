#pragma once
#include "GameObject.h"

#define BOX_BBOX_WIDTH  16
#define BOX_BBOX_HEIGHT  16
#define BOX_STATUS_START  1
#define BOX_STATUS_END  2
#define BOX_STATUS_NORMAL  3

#define OBJECT_TYPE_BOXS	6

class CBox : public CGameObject
{
public:
	float status;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	CBox(float status);
	CBox() {};
	int GetStatus()
	{
		return status;
	}
};

class CBoxs : public CGameObject
{
	boolean mergeComplete;
	float width;
	float height;

	vector<CBox*> listBox;
public:

	CBoxs() { mergeComplete = false; width = 0; height = 0; }
	boolean getMergeComplete() { return mergeComplete; }
	vector<CBox*> getListBox() { return listBox; }
	void setWidth(float width) { this->width = width; }
	void setHeight(float height) { this->height = height; }
	double getWidth() { return this->width; }
	double getHeight() { return this->height; }
	void setMergeComplete(boolean mergeComplete) { this->mergeComplete = mergeComplete; }

	void PushBox(CBox* box);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetTypeObject() {
		return OBJECT_TYPE_BOXS;
	}

};


