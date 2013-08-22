//
//  GameLayer.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __FRAMGIA__ 2013年. All rights reserved.
//
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "AppMacros.h"
#include "Ball.h"
#include "MyContactListener.h"
#include "SimpleAudioEngine.h"
#include "GameManager.h"
#include "HttpClient.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "RankingScene.h"
#include "GetPresent.h"

#define GOALSIZE_RATIO 3.8

USING_NS_CC;
using namespace cocos2d::extension;
using namespace CocosDenshion;
using namespace std;

class GameLayer : public CCLayer {
public:
    ~GameLayer();
    GameLayer();
    
    CC_SYNTHESIZE(b2World *, _world, World);
    
    static CCScene* scene();
    
    void initPhysics();
    void createEdge(float x1, float y1, float x2, float y2, int groupIndex);
    
    void onStart();
    void playIntro();
    void playBGM();
    
    virtual void draw();
    void update(float dt);
    
    void getStateInfo();
    void handleProcess();
    void defenseCenter();
    void defenseLeft();
    void defenseRight();
    void attack();
    
    void onQuitClick();
    void onRestartClick();
    void onContinueClick();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void newTurn();
    void gameReset();
    void scoreCounter(int player);
    
    void Timer();
    
    void upScore(int score);
    void getTopRankingList();
    void checkScore(int score);
    
    void checkHighScore();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    void endGame();
    void addEffect(CCPoint point);
    void convertName(char *str_name);
    
    b2Vec2 ptm(CCPoint point);
    b2Vec2 ptm2(float x, float y);
private:
    CCLayer *_controlLayer;
    CCSize size;
    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    float GOAL_RADIUS;
    
    
    b2Body *_groundBody;

    CCSprite *_pauseButton;
    CCSprite *_endLayerBg;
    CCSprite *_continueButton;
    CCSprite *_restartButton;
    CCSprite *_quitButton;
    CCSprite *human;
    CCSprite *ai;
    
    Ball *_player1;
    Ball *_player2;
    Ball *_puck;

    b2MouseJoint *_mouseJoint;
    
    int _score1;
    int _score2;
    int lastHit;
    int _minutes, _seconds;
    int point;
    int pointCal;
    int pointUnit;
    // Game Level Easy = 1, Medium = 2, Hard = 3
    int _level;
    
    bool _playing;
    bool _isPauseClicked;
    bool _isEnd;
    
    float centerX, centerY, centerZ;
    float x, y, px, py;
    float vx, vy, vpx, vpy;
    float pr;
    float ew, eh;
    
    string dataBuf;
    
    CCLabelTTF *_scoreLabel1;
    CCLabelTTF *_scoreLabel2;
    CCLabelTTF *_timer;
    CCLabelTTF *_resultLabel;
    CCLabelTTF *_scoreLabel;
    
    MyContactListener *_contactListener;
};

#endif
