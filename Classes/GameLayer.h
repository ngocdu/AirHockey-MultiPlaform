#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <Box2D/Box2D.h>
#include "Ball.h"

#define PTM_RATIO 32.0

using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;

class GameLayer : public cocos2d::CCLayer
{
private:
    CCSize size;
    float w, h;
    
    // Game Objects
    Ball *_humanPlayer;
    Ball *_aiPlayer;
    Ball *_puck;
    
    // Physics Object
    b2Body *_groundBody;
    
    // Mouse Joint
    b2MouseJoint *_mouseJoint;
    
public:
    CC_SYNTHESIZE(b2World*, _world, World);
    GameLayer();
    ~GameLayer();
    
    // Init all Box2D Physics here
    void initPhysics();
    void createEdge(float x1, float y1, float x2, float y2, int groupIndex);
    
    // Update
    void update(float dt);
    
    // Touch Event Handle
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
//    
//    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
//    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
//    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
    // Main Scene
    static CCScene* scene();
    
    b2Vec2 ptm(CCPoint point);
    b2Vec2 ptm2(float x, float y);
    
    CREATE_FUNC(GameLayer);
};

#endif // __GAME_LAYER_H__
