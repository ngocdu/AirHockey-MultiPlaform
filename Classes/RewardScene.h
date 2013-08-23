//
//  RewardScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RewardScene__
#define __AirHockey__RewardScene__

#include "AppMacros.h"
#include "cocos-ext.h"
#include "CCTableView.h"
#include "CCScrollView.h"
#include "CCTableViewCell.h"
#include "SimpleAudioEngine.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "Difficulty.h"
#include "GameManager.h"
#include "RankingScene.h"
#include "curl/curl.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace CocosDenshion;

class Player1 : public CCObject {
private:
    CC_SYNTHESIZE(string, _name, Name);
    CC_SYNTHESIZE(int, _point, Point);
    CC_SYNTHESIZE(int, _reward, Reward);
    CC_SYNTHESIZE(string, _time, Time);
    CC_SYNTHESIZE(string, _email, Email);
public:
    Player1(string name , int mark, string email, string time, int reward);
    ~Player1();
};


class RewardScene : public CCLayer, public CCTableViewDelegate, CCTableViewDataSource {
private:
    CCSize size;
    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    CCMenu * rewardMenu;
    
    CCArray *players;
    int celltouch;
    string dataBuf;
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    void getRanking();
    void displayRanking();
    string scoreFormat(string score);
    
    void back(CCObject* pSender);
    void clickBtSendEmail(CCObject* pSender);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    void convertName(char *str_name);
    void convertName2(char *str_name);
    void convertTime(char *str_time);
    void convertTime2(char *str_time);
    CREATE_FUNC(RewardScene); 
};
#endif /* defined(__AirHockey__RewardScene__) */