//
//  RankingScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RankingScene__
#define __AirHockey__RankingScene__
#include "AppMacros.h"
#include "cocos-ext.h"
#include "CCTableView.h"
#include "CCScrollView.h"
#include "CCTableViewCell.h"
#include "SimpleAudioEngine.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "GameManager.h"
#include "Difficulty.h"
#include "RewardScene.h"

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


class RankingScene : public CCLayer, public CCTableViewDelegate, CCTableViewDataSource {
private:
    CCSize size;
    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    
    bool musicPlayed;
    bool introEnd;
    
    
    CCSprite *bgm_off;
    CCMenu *rewardMenu;
    CCTableView *tableView;
    CCArray *players;
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    void play(CCObject* pSender);
    void bgm(CCObject* pSender);
    void reward(CCObject* pSender);
    
    void playIntro();
    void playBGM();
    
    void update(float dt);
    
    CREATE_FUNC(RankingScene);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    void convertName(char *str_name);
};
#endif /* defined(__AirHockey__RankingScene__) */