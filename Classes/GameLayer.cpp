#include "GameLayer.h"

#pragma mark CONSTRUCTOR
GameLayer::GameLayer() {
//    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->setAccelerometerEnabled(true);
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    CCSprite* pSprite = CCSprite::create("Court3.png");
    pSprite->setPosition(ccp(w/2, h/2));
    this->addChild(pSprite, 0);
    
    this->initPhysics();
    this->scheduleUpdate();
}

#pragma mark DESTRUCTOR
GameLayer::~GameLayer() {
    delete _world;
    _world = NULL;
}

#pragma mark INIT PHYSICS
void GameLayer::initPhysics() {
    // Create World
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _mouseJoint = NULL;

    // Create Play Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    _groundBody = _world->CreateBody(&groundBodyDef);
    // Bottom Left
    this->createEdge(0, 10, 210, 10, 0);
    // Bottom Right
    this->createEdge(w - 210, 10, w, 10, 0);
    // Bottom Center
    this->createEdge(210, 10, w - 210, 10, -10);
    
    // Top Left
    this->createEdge(0, h - 10, 210, h - 10, 0);
    // Top Right
    this->createEdge(w - 210, h - 10, w, h - 10, 0);
    // Top Center
    this->createEdge(210, h - 10, w - 210, h - 10, -10);
    
    // Left
    this->createEdge(10, 0, 10, h, 0);
    // Right
    this->createEdge(w - 10, h, w - 10, 0, 0);
    // Center
    this->createEdge(0, h/2, w, h/2, -10);
    
    
    // Create Players and Puck
    _humanPlayer = Ball::create(this, humanPlayer, "Player/Mallet1_2.png");
    _humanPlayer->setStartPos(ccp(w/2, _humanPlayer->getRadius()*2));
    _humanPlayer->setSpritePosition(_humanPlayer->getStartPos());
    this->addChild(_humanPlayer);
    _aiPlayer = Ball::create(this, aiPlayer, "Player/Mallet2_2.png");
    _aiPlayer->setStartPos(ccp(w/2, h - _aiPlayer->getRadius()*2));
    _aiPlayer->setSpritePosition(_aiPlayer->getStartPos());
    this->addChild(_aiPlayer);
    _puck = Ball::create(this, puck, "Player/Puck.png");
    _puck->setStartPos(ccp(w/2, h/2));
    _puck->setSpritePosition(_puck->getStartPos());
    this->addChild(_puck);
}

void GameLayer::createEdge(float x1, float y1, float x2, float y2, int groupIndex) {
    b2EdgeShape groundEdgeShape;
    groundEdgeShape.Set(b2Vec2(x1 / PTM_RATIO, y1 / PTM_RATIO),
                        b2Vec2(x2 / PTM_RATIO, y2 / PTM_RATIO));
    b2FixtureDef groundEdgeDef;
    groundEdgeDef.shape = &groundEdgeShape;
    groundEdgeDef.filter.groupIndex = groupIndex;
    _groundBody->CreateFixture(&groundEdgeDef);

}

#pragma mark UPDATE
void GameLayer::update(float dt) {
    _world->Step(dt, 8, 3);
    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
        }
    }
}

bool GameLayer::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    CCLOG("Touched");
    if (_mouseJoint != NULL) return false;
    CCPoint tap = touch->getLocation();
    b2Vec2 target = this->ptm(tap);
    CCRect humanPlayerRect = _humanPlayer->boundingBox();
    if (humanPlayerRect.containsPoint(tap)) CCLOG("Touched");
    
    b2MouseJointDef md;
    md.bodyA =  _groundBody;
    md.bodyB = _humanPlayer->getBody();
    md.target = target;
    md.collideConnected = true;
    md.maxForce = 100000.0f * _humanPlayer->getBody()->GetMass();
    md.dampingRatio = 0;
    md.frequencyHz = 100000;
    
    _humanPlayer->setSpritePosition(tap);
    _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
    _humanPlayer->getBody()->SetAwake(true);
    return true;
}

void GameLayer::ccTouchMoved(CCTouch *touch, CCEvent *event) {
    if (_mouseJoint == NULL) return;
    CCPoint tap = touch->getLocation();
    b2Vec2 target = this->ptm(tap);
    _mouseJoint->SetTarget(target);
}

void GameLayer::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    if (_mouseJoint == NULL) return;
    _world->DestroyJoint(_mouseJoint);
    _mouseJoint = NULL;
}

//
//#pragma mark TOUCHES HANDLE
//void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
//    CCLOG("Touched");
//    if (_mouseJoint != NULL) return;
//    CCTouch *touch = (CCTouch *)touches->anyObject();
//    CCPoint tap = touch->getLocation();
//    b2Vec2 target = this->ptm(tap);
//    CCRect humanPlayerRect = _humanPlayer->boundingBox();
//    if (humanPlayerRect.containsPoint(tap)) CCLOG("Touched");
//    
//    b2MouseJointDef md;
//    md.bodyA =  _groundBody;
//    md.bodyB = _humanPlayer->getBody();
//    md.target = target;
//    md.collideConnected = true;
//    md.maxForce = 100000.0f * _humanPlayer->getBody()->GetMass();
//    md.dampingRatio = 0;
//    md.frequencyHz = 1000;
//    
//    _humanPlayer->setSpritePosition(tap);
//    _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
//    _humanPlayer->getBody()->SetAwake(true);
//}
//
//void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {
//    if (_mouseJoint == NULL) return;
//    CCTouch *touch = (CCTouch *)touches->anyObject();
//    CCPoint tap = touch->getLocation();
//    b2Vec2 target = this->ptm(tap);
//    _mouseJoint->SetTarget(target);
//}
//
//void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
//    if (_mouseJoint == NULL) return;
//    _world->DestroyJoint(_mouseJoint);
//    _mouseJoint = NULL;
//}

b2Vec2 GameLayer::ptm(CCPoint point) {
    return b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO);
}
b2Vec2 GameLayer::ptm2(float x, float y) {
    return b2Vec2(x / PTM_RATIO, y / PTM_RATIO);
}


CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}
