#include "GameObject.h"

#define HUD_0_SPRITE	90112
#define HUD_1_SPRITE	90113
#define HUD_2_SPRITE	90114
#define HUD_3_SPRITE	90115
#define HUD_4_SPRITE	90116
#define HUD_5_SPRITE	90117
#define HUD_6_SPRITE	90118
#define HUD_7_SPRITE	90119
#define HUD_8_SPRITE	90120
#define HUD_9_SPRITE	90121

#define HUD_BOARD_SPRITE	90100
#define HUD_ITEM_SPRITE	90101
#define HUD_BACKGROUND_SPRITE	90148

#define HUDBOARD_X	55
#define HUDBOARD_Y	81

#define LIFE_LENGTH	2
#define COIN_LENGTH	3
#define SCORE_LENGTH	7
#define HUD_CHAR_WIDTH	8
#define HUD_CHAR_HEIGHT_LTS	66
#define HUD_CHAR_HEIGHT_CSS	74
#define HUD_CHAR_WIDTH_S	107
#define HUD_CHAR_WIDTH_L	84
#define HUD_CHAR_WIDTH_T	179
#define HUD_CHAR_WIDTH_C	187

class CHUD : public CGameObject
{
	LPSPRITE backgroundBlack;	//Nền đen
	vector<LPSPRITE> life;	//Mạng
	vector<LPSPRITE> coin;	//Tiền
	vector<LPSPRITE> score;	//Điểm
	LPSPRITE hudBoard;	//Khung HUD trái
	vector<LPSPRITE> time;	//Thời gian chơi
	int marioTime = 0;	//Thời gian chơi
	int marioLife = 4;	//Mạng của mario
	int marioScore = 0;	//Điểm
	int marioCoin = 0;	//Tiền
	int tempTime = 0;

public:
	CHUD();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void resetHUD();
	vector<LPSPRITE> StringToSprite(string str);
	LPSPRITE getHUD(char temp);
};