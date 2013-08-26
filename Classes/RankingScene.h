//
//  RankingScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RankingScene__
#define __AirHockey__RankingScene__
#include "AppMacros.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "GameManager.h"
#include "Difficulty.h"
#include "RewardScene.h"
#include "curl/curl.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

class Player : public CCObject {
private:
    CC_SYNTHESIZE(string, _name, Name);
    CC_SYNTHESIZE(int, _point, Point);
    CC_SYNTHESIZE(int, _reward, Reward);
    
public:
    Player(string name , int mark);
    ~Player();
};


class RankingScene : public CCLayer {
private:
    CCSize size;
    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    int time;
    bool musicPlayed;
    bool introEnd;
    
    string dataBuf;
    
    CCSprite *bgm_off;
    CCMenu *rewardMenu;
    CCArray *players;
public:
    virtual bool init();
    static CCScene* scene();
    
    void play(CCObject* pSender);
    void bgm(CCObject* pSender);
    void reward(CCObject* pSender);
    
    void playIntro();
    void playBGM();
    void upBestScore();
    void getRanking();
    void displayRanking();
    void postUrl(string url);
    
    CREATE_FUNC(RankingScene);
    void convertName(char *str_name);
    void convertName2(char *str_name);
    string scoreFormat(string score);
};
#endif /* defined(__AirHockey__RankingScene__) */