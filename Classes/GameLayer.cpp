//
//  GameLayer.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __FRAMGIA__ 2013年. All rights reserved.
//

#include "GameLayer.h"

int gameWriter(char *data, size_t size, size_t nmemb, string *buffer) {
    int result = 0;
    if (buffer != NULL) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}


#pragma mark SCENE
CCScene* GameLayer::scene() {
    CCScene *scene = CCScene::create();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    
    int _level;
    if (GameManager::sharedGameManager()->getLevel()) {
        _level = GameManager::sharedGameManager()->getLevel();
    } else {
        _level = 1;
    }
    CCSprite *spaceBackground;
    if (_level == 1) {
        spaceBackground = CCSprite::create("BackGrounds/bg-01.jpg");
    } else if (_level == 2) {
        spaceBackground = CCSprite::create("BackGrounds/bg-02.jpg");
    } else {
        spaceBackground = CCSprite::create("BackGrounds/bg-03.jpg");
    }
    spaceBackground->setPosition(ccp(w/2,h/2));
    spaceBackground->setScaleX(w/spaceBackground->getContentSize().width);
    spaceBackground->setScaleY(h/spaceBackground->getContentSize().height);
    GameLayer *layer = new GameLayer();
    scene->addChild(spaceBackground, SPACE_BACKGROUND_ZORDER);
    scene->addChild(layer);
    layer->release();
    return scene;
}

#pragma mark CONSTRUCTOR
GameLayer::GameLayer() {
    
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    setAccelerometerEnabled(true);
    
    if (GameManager::sharedGameManager()->getLevel()) {
        _level = GameManager::sharedGameManager()->getLevel();
    } else {
        _level = 1;
    }

    CCAction *playIntro = CCCallFuncN::create(this, callfuncN_selector(GameLayer::playIntro));
    CCAction *delayTime = CCDelayTime::create(1.60f);
    CCAction *playBGM = CCCallFuncN::create(this, callfuncN_selector(GameLayer::playBGM));
    CCArray *actionList = new CCArray(3);
    actionList->autorelease();
    actionList->insertObject(playIntro, 0);
    actionList->insertObject(delayTime, 1);
    actionList->insertObject(playBGM, 2);
    CCAction *BGM = CCSequence::create(actionList);
    this->runAction(BGM);
    
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;

    _score1 = 0;
    _score2 = 0;
    lastHit = -1200;
    point = 0;
    pointCal = 0;
    pointUnit = 0;
    
    _isPlaying = false;
    _isPaused = false;
    _isEnded = false;
    
    centerX = 0;
    centerY = 0;
    centerZ = 0;
//    this->getCamera()->getCenterXYZ(&x, &y, &z);
//    this->getCamera()->setCenterXYZ(centerX, centerY + 0.0000001, centerZ);
//    this->getCamera()->getUpXYZ(&x, &y, &z);
//    this->getCamera()->setUpXYZ(0.0f, 1.0f, 100000.0f);
//    this->getCamera()->getEyeXYZ(&x, &y, &z);
//    this->getCamera()->setEyeXYZ(0, -30, 40);
//    this->getCamera()->getCenterXYZ(&x, &y, &z);

    _controlLayer = new CCLayer();
//    _controlLayer->setPosition(ccp(w/2, h/2));
    _controlLayer->getCamera()->setEyeXYZ(0, -7, 10);
    this->addChild(_controlLayer);
    
    CCSprite *backGroundImg;
    if (_level == 1) {
        backGroundImg = CCSprite::create("BackGrounds/BackGround.png");
    } else if (_level == 2) {
        backGroundImg = CCSprite::create("BackGrounds/BackGround.png");
    } else {
        backGroundImg = CCSprite::create("BackGrounds/BackGround.png");
    }
    backGroundImg->setPosition(ccp(w/2, h/2));
    backGroundImg->setScaleY(h/backGroundImg->getContentSize().height);
    backGroundImg->setScaleX(w/backGroundImg->getContentSize().width);
    _controlLayer->addChild(backGroundImg);
    
    // Goal
    CCSprite *humanGoal = CCSprite::create("GameLayer/HumanGoal.png");
    CCSprite *aiGoal = CCSprite::create("GameLayer/AIGoal.png");
    humanGoal->setPosition(ccp(w/2, 25*SIZE_RATIO_Y));
    aiGoal->setPosition(ccp(w/2, h - 25*SIZE_RATIO_Y));
    humanGoal->setScaleX(w/backGroundImg->getContentSize().width);
    aiGoal->setScaleX(w/backGroundImg->getContentSize().width);
    humanGoal->setScaleY(h/backGroundImg->getContentSize().height);
    aiGoal->setScaleY(h/backGroundImg->getContentSize().height);
    _controlLayer->addChild(humanGoal, 2);
    _controlLayer->addChild(aiGoal, 2);
    
    GOAL_RADIUS = humanGoal->getContentSize().width*SIZE_RATIO_X/2;
    
    // Pause Button
    CCMenuItemImage *pauseButton = CCMenuItemImage::create("Buttons/PauseButton.png",
                                                           "Buttons/PauseButtonOnClicked.png",
                                                           this, menu_selector(GameLayer::onPauseClick));
    pauseButton->setScaleX(SIZE_RATIO_X);
    pauseButton->setScaleY(SIZE_RATIO_Y);
    pauseButton->setPosition(ccp(w - (pauseButton->getContentSize().width/2 +
                                 15) * SIZE_RATIO_X, h/2));
    
    CCMenu *pause = CCMenu::create(pauseButton, NULL);
    pause->setPosition(CCPointZero);
    this->addChild(pause, 5);
    
    // Score Counter
    CCSprite *humanScoreBG = CCSprite::create("BackGrounds/ScoreBG.png");
    CCSprite *aiScoreBG = CCSprite::create("BackGrounds/ScoreBG.png");
    
    humanScoreBG->setRotation(-90);
    aiScoreBG->setRotation(-90);
    
    humanScoreBG->setPosition(ccp(pauseButton->getPosition().x,
                                  h/2 - (humanScoreBG->getContentSize().width/2 + pauseButton->getContentSize().width) * SIZE_RATIO_Y));
    humanScoreBG->setScale(SIZE_RATIO);
    aiScoreBG->setPosition(ccp(pauseButton->getPosition().x,
                               h/2 + (aiScoreBG->getContentSize().width/2 + pauseButton->getContentSize().width) * SIZE_RATIO_Y ));
    aiScoreBG->setScale(SIZE_RATIO);
    this->addChild(humanScoreBG);
    this->addChild(aiScoreBG);

    _scoreLabel1 = CCLabelTTF::create("0", FONT, 48 * SIZE_RATIO);
    _scoreLabel2 = CCLabelTTF::create("0", FONT, 48 * SIZE_RATIO);
    _scoreLabel1->setRotation(90);
    _scoreLabel2->setRotation(90);
    _scoreLabel1->setPosition(humanScoreBG->getPosition());
    _scoreLabel2->setPosition(aiScoreBG->getPosition());
    this->addChild(_scoreLabel1);
    this->addChild(_scoreLabel2);
    
    // Timer
    CCSprite *timerBG = CCSprite::create("BackGrounds/TimerBG.png");
    timerBG->setRotation(90);
    timerBG->setPosition(ccp((timerBG->getContentSize().height/2 + 15) * SIZE_RATIO_X, h/2));
    timerBG->setScale(SIZE_RATIO);
    this->addChild(timerBG);

    _minutes = 3;
    _seconds = 00;
    _isPlaying = false;
    char timeBuf[20] = {0};
	sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);
    
    _timer = CCLabelTTF::create(timeBuf, FONT, 36 * SIZE_RATIO);
    _timer->setRotation(90);
	_timer->setPosition(timerBG->getPosition());
	this->addChild(_timer, CONTROL_LAYER_ZORDER);
    
    // Physics
    this->initPhysics();
    
    CCSprite *startGame = CCSprite::create("Buttons/OnStart.png");
    startGame->setPosition(ccp(-w/2, h/2));
    this->addChild(startGame, 9, 2);
    
    CCFiniteTimeAction *move1  = CCMoveTo::create(1, ccp(w/2, h/2));
    CCFiniteTimeAction *move2  = CCMoveTo::create(1, ccp(w*3/2, h/2));
    CCFiniteTimeAction *delay = CCDelayTime::create(0.2);
    CCFiniteTimeAction *start = CCCallFuncN::create(this, callfuncN_selector(GameLayer::onStart));
    startGame->runAction(CCSequence::create(move1, delay, move2, start, NULL));
    
    
    // Pause Layer
    _pauseLayer = CCSprite::create("BackGrounds/EndGameBG.png");
    _pauseLayer->setScale(SIZE_RATIO);
    _pauseLayer->setPosition(ccp(w/2, h/2));
    CCMenuItemImage *quitButton = CCMenuItemImage::create("Buttons/QuitButton.png",
                                                          "Buttons/QuitButtonOnClicked.png",
                                                          this, menu_selector(GameLayer::onQuitClick));
//    quitButton->setScale(SIZE_RATIO);
    quitButton->setPosition(_pauseLayer->convertToNodeSpace(ccp(w/2, h*4/10)));
    CCMenuItemImage *restartButton = CCMenuItemImage::create("Buttons/RestartButton.png",
                                                             "Buttons/RestartButtonOnClicked.png",
                                                             this, menu_selector(GameLayer::onRestartClick));
//    restartButton->setScale(SIZE_RATIO);
    restartButton->setPosition(_pauseLayer->convertToNodeSpace(ccp(w/2, h/2)));
    CCMenuItemImage *continueButton = CCMenuItemImage::create("Buttons/ContinueButton.png",
                                                              "Buttons/ContinueButtonOnClicked.png",
                                                              this, menu_selector(GameLayer::onContinueClick));
//    continueButton->setScale(SIZE_RATIO);
    continueButton->setPosition(_pauseLayer->convertToNodeSpace(ccp(w/2, h*6/10)));
    
    CCMenu *pauseMenu = CCMenu::create(quitButton,
                               restartButton, continueButton, NULL);
    pauseMenu->setPosition(ccp(0, 0));
    _pauseLayer->addChild(pauseMenu);
    this->addChild(_pauseLayer);
    _pauseLayer->setVisible(false);
    
    
    // EndGame Layer
    _endLayer = CCSprite::create("BackGrounds/EndGameBG.png");
    _endLayer->setPosition(ccp(w/2, h/2));
    _endLayer->setScale(SIZE_RATIO);
    this->addChild(_endLayer);
    
    _resultLabel    = CCLabelTTF::create("DRAW",FONT, 60);
    _scoreLabel     = CCLabelTTF::create("score: 0", FONT, 36);
    ew = _endLayer->getContentSize().width;
    eh = _endLayer->getContentSize().height;
    _resultLabel->setPosition(ccp(ew/2, eh - 50*SIZE_RATIO_Y));
    _scoreLabel->setPosition(ccp(ew/2, eh*3/4 - 15*SIZE_RATIO_Y));
    
    _endLayer->addChild(_resultLabel);
    _endLayer->addChild(_scoreLabel);
    
    CCMenuItemImage *quitButton2 = CCMenuItemImage::create("Buttons/QuitButton.png",
                                                           "Buttons/QuitButtonOnClicked.png",
                                                           this, menu_selector(GameLayer::onQuitClick));
//    quitButton2->setScale(SIZE_RATIO);
    quitButton2->setPosition(_endLayer->convertToNodeSpace(ccp(w/2, h*4/10)));
    CCMenuItemImage *restartButton2 = CCMenuItemImage::create("Buttons/RestartButton.png",
                                                              "Buttons/RestartButtonOnClicked.png",
                                                              this, menu_selector(GameLayer::onRestartClick));
//    restartButton2->setScaleX(SIZE_RATIO_X);
    restartButton2->setPosition(_endLayer->convertToNodeSpace(ccp(w/2, h/2)));
    
    CCMenu *endMenu = CCMenu::create(quitButton2, restartButton2, NULL);
    endMenu->setPosition(ccp(0, 0));
    _endLayer->addChild(endMenu);
    _endLayer->setVisible(false);
    
    // Scheduler
    scheduleUpdate();
    this->schedule(schedule_selector(GameLayer::Timer), 1);
    this->schedule(schedule_selector(GameLayer::handleProcess), 0.025);
}

#pragma mark DESTRUCTOR
GameLayer::~GameLayer() {
    delete _world;
    _world = NULL;
}

void GameLayer::onQuitClick() {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
}

void GameLayer::onRestartClick() {
    _isPlaying = true;
    point = pointCal = 0;
    this->resumeSchedulerAndActions();
    _resultLabel->stopAllActions();
    if (_isPaused) _pauseLayer->setVisible(false);
    if (_isEnded) _endLayer->setVisible(false);
    this->gameReset();
}

void GameLayer::onContinueClick() {
    _isPlaying = true;
    _isEnded = false;
    _isPaused = false;
    this->resumeSchedulerAndActions();
    _pauseLayer->setVisible(false);
}

void GameLayer::onPauseClick() {
    if (!_isEnded) {
        if (_pauseLayer->isVisible()) {
            _pauseLayer->setVisible(false);
            _isPaused = false;
            this->resumeSchedulerAndActions();
        }
        else {
            _pauseLayer->setVisible(true);
            _isPaused = true;
            this->pauseSchedulerAndActions();
        }
    } else {
        if (_endLayer->isVisible()) _endLayer->setVisible(false);
        else _endLayer->setVisible(true);
    }
}

void GameLayer::playIntro() {
    if (GameManager::sharedGameManager()->getBgm()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/PlayIntro.mp3");
    }
}

void GameLayer::playBGM() {
    if (GameManager::sharedGameManager()->getBgm()) {
        if (_level == 1 || _level == 2) {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/BG.mp3", true);
        } else {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/HardBG.mp3", true);
        }
    }
}

#pragma mark INIT PHYSICS
void GameLayer::initPhysics() {
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _mouseJoint = NULL;

    _contactListener = new MyContactListener();
    _world->SetContactListener(_contactListener);

    // Create Play Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    _groundBody = _world->CreateBody(&groundBodyDef);
    
    
    
    // Bottom Left
    this->createEdge(0, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y,
                     w/2 - GOAL_RADIUS, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y, 0);
    // Bottom Right
    this->createEdge(w/2 + GOAL_RADIUS, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y,
                     w, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y, 0);
    // Bottom Center
    this->createEdge(w/2 - GOAL_RADIUS, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y,
                     w/2 + GOAL_RADIUS, BORDER_WIDTH_BOTTOM*SIZE_RATIO_Y, -10);
    // Bottom goal
    this->createEdge(w/2 - GOAL_RADIUS, 45*SIZE_RATIO_Y, w/2 - GOAL_RADIUS, 0, 0);
    this->createEdge(w/2 + GOAL_RADIUS, 45*SIZE_RATIO_Y, w/2 + GOAL_RADIUS, 0, 0);
    
    // Top Left
    this->createEdge(0, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y,
                     w/2 - GOAL_RADIUS, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y, 0);
    // Top Right
    this->createEdge(w/2 + GOAL_RADIUS, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y,
                     w, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y, 0);
    // Top Center
    this->createEdge(w/2 - GOAL_RADIUS, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y,
                     w/2 + GOAL_RADIUS, h - BORDER_WIDTH_TOP*SIZE_RATIO_Y, -10);
    // Top goal
    this->createEdge(w/2 - GOAL_RADIUS, h - 45*SIZE_RATIO_Y, w/2 - GOAL_RADIUS, h, 0);
    this->createEdge(w/2 + GOAL_RADIUS, h - 45*SIZE_RATIO_Y, w/2 + GOAL_RADIUS, h, 0);
    
    // Left
    this->createEdge(BORDER_WIDTH_LEFT*SIZE_RATIO_X,
                     0, BORDER_WIDTH_LEFT*SIZE_RATIO_X, h, 0);
    // Right
    this->createEdge(w - BORDER_WIDTH_RIGHT*SIZE_RATIO_X, h,
                     w - BORDER_WIDTH_RIGHT*SIZE_RATIO_X, 0, 0);
    // Center
    this->createEdge(0, h/2, w, h/2, -10);
    
    // Create 2 Player and Puck
    _player1 = Ball::create(this, humanPlayer, "GameLayer/Mallet.png");
    _player1->setStartPos(ccp(w/2, _player1->getRadius()*2));
    _player1->setOpacity(0);
    _player1->setSpritePosition(_player1->getStartPos());
    human = CCSprite::create("GameLayer/Human.png");
    human->setScale(SIZE_RATIO);
//    human->setOpacity(30);
    human->setPosition(ccp(_player1->getPosition().x + 50*SIZE_RATIO_X,
                           _player1->getPosition().y + 100*SIZE_RATIO_Y));
    _controlLayer->addChild(human, 3);
    
    _player2 = Ball::create(this, aiPlayer, "GameLayer/Mallet.png");
    _player2->setStartPos(ccp(w/2, h - _player2->getRadius()*2));
    _player2->setOpacity(0);
    _player2->setSpritePosition(_player2->getStartPos());
    ai = CCSprite::create("GameLayer/AI.png");
    ai->setPosition(ccp(_player2->getPosition().x - 25*SIZE_RATIO_X,
                        _player2->getPosition().y + 100*SIZE_RATIO_Y));
    ai->setScale(SIZE_RATIO);
//    ai->setOpacity(30);
    _controlLayer->addChild(ai, 2);
    
    _puck = Ball::create(this, puck, "Player/Puck.png");
    _puck->setStartPos(ccp(w/2, h/2));
    _puck->setSpritePosition(_puck->getStartPos());
    
    _controlLayer->addChild(_player1);
    _controlLayer->addChild(_player2);
    _controlLayer->addChild(_puck);
}

void GameLayer::createEdge(float x1, float y1,
                           float x2, float y2,
                           int groupIndex) {
    b2EdgeShape groundEdgeShape;
    groundEdgeShape.Set(b2Vec2(x1 / PTM_RATIO, y1 / PTM_RATIO),
                        b2Vec2(x2 / PTM_RATIO, y2 / PTM_RATIO));
    b2FixtureDef groundEdgeDef;
    groundEdgeDef.shape = &groundEdgeShape;
    groundEdgeDef.filter.groupIndex = groupIndex;
    _groundBody->CreateFixture(&groundEdgeDef);
}

void GameLayer::onStart() {
    _isPlaying = true;
}

#pragma mark DRAW
void GameLayer::draw() {
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    _world->DrawDebugData();
    kmGLPopMatrix();
}

#pragma mark UPDATE
void GameLayer::update(float dt) {
    _world->Step(dt, 8, 3);
    if (_isPlaying) {
        _player1->update(dt);
        _player2->update(dt);
        _puck->update(dt);
        human->setPosition(ccp(_player1->getPosition().x + 50*SIZE_RATIO_X,
                               _player1->getPosition().y + 100*SIZE_RATIO_Y));
        ai->setPosition(ccp(_player2->getPosition().x - 25*SIZE_RATIO_X,
                            _player2->getPosition().y + 100*SIZE_RATIO_Y));
    }
    
    if ((_minutes == 0 && _seconds == 0) || _score1 == 3 || _score2 == 3) {
        _isPlaying = false ;
        _isEnded = true;
        _player1->reset();
        _player2->reset();
        human->setPosition(ccp(_player1->getPosition().x + 50*SIZE_RATIO_X,
                               _player1->getPosition().y + 100*SIZE_RATIO_Y));
        ai->setPosition(ccp(_player2->getPosition().x - 25*SIZE_RATIO_X,
                            _player2->getPosition().y + 100*SIZE_RATIO_Y));
        _puck->reset();
        this->endGame();
    }

    // Apply impluse when the puck is near the edges
    if (_puck->getPositionX() <= 15 + _puck->getRadius())
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(5*SIZE_RATIO, 0),
                                             _puck->getBody()->GetWorldCenter());

    if (_puck->getPositionX() >= w - 15 - _puck->getRadius())
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(-5*SIZE_RATIO, 0),
                                             _puck->getBody()->GetWorldCenter());
    
    if (_puck->getPositionX() <= w/GOALSIZE_RATIO || _puck->getPositionX() >= w - w/GOALSIZE_RATIO) {
        if (_puck->getPositionY() >= h - 15 - _puck->getRadius())
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, -5*SIZE_RATIO),
                                             _puck->getBody()->GetWorldCenter());
        if (_puck->getPositionY() <= 15 + _puck->getRadius())
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, 5*SIZE_RATIO),
                                             _puck->getBody()->GetWorldCenter());
    }
    
    // Gloal !!!
    if (_puck->getPositionY() > h + _puck->getRadius()) {
        SimpleAudioEngine::sharedEngine()->playEffect("Sounds/score.wav");
        this->scoreCounter(1);
        this->newTurn();
        _puck->setPosition(ccp(w/2, h/2 + 3 * _puck->getRadius()));
    }
    
    if (_puck->getPositionY() < -_puck->getRadius()) {
        SimpleAudioEngine::sharedEngine()->playEffect("Sounds/score.wav");
        this->scoreCounter(2);
        this->newTurn();
        _puck->setPosition(ccp(w/2, h/2 - 3 * _puck->getRadius()));
    }
    
    // Collision Detection
    std::vector<MyContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin();
        pos != _contactListener->_contacts.end(); ++pos) {
        MyContact contact = *pos;
        
        if (contact.fixtureA == _puck->getFixture() ||
            contact.fixtureB == _puck->getFixture()) {
            SimpleAudioEngine::sharedEngine()->playEffect("Sounds/hitPuck.wav");
        }
        
        if ((contact.fixtureA == _puck->getFixture()     &&
             contact.fixtureB == _player2->getFixture()) ||
            (contact.fixtureA == _player2->getFixture()  &&
             contact.fixtureB == _puck->getFixture())) {
            lastHit = 0;
            this->defenseCenter();
        }
    }
    if (lastHit >= 1200/_level) {
        this->defenseCenter();
    }
}

#pragma mark Get Game State Info
void GameLayer::getStateInfo() {
    x   = _puck->getPositionX();
    y   = _puck->getPositionY();
    px  = _player2->getPositionX();
    py  = _player2->getPositionY();
    vx  = _puck->getBody()->GetLinearVelocity().x;
    vy  = _puck->getBody()->GetLinearVelocity().y;
    vpx = _player2->getBody()->GetLinearVelocity().x;
    vpy = _player2->getBody()->GetLinearVelocity().y;
    pr = _puck->getRadius();
}

#pragma mark AI PLAYER
void GameLayer::handleProcess() {
    lastHit+= 25;
    this->getStateInfo();
    
    if (lastHit >= 1200/_level) {
        if ((y >= h/2 - pr && y <= 3*h/4) ||
            (y > 3*h/4 && x > w/2 && x < 3*w/4)) {
            this->attack();
        }
    } else {
        this->defenseCenter();
    }
}

void GameLayer::defenseLeft() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*SIZE_RATIO*_level*(w*3/8 - px),
                   15*SIZE_RATIO*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseRight() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*SIZE_RATIO*_level*(w*5/8 - px), 15*SIZE_RATIO*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseCenter() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*SIZE_RATIO*_level*(w/2 - px), 15*SIZE_RATIO*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::attack() {
    float cx = (h - 10 - pr - y)*vx/vy + x;
    if ((cx > w/4 && cx < w*3/4) || (vx < 10 && vy < 10))
        _player2->getBody()->ApplyLinearImpulse(
            b2Vec2(5*SIZE_RATIO*_level*(x - px), 5*SIZE_RATIO*_level*(10 + y - py)),
            _player2->getBody()->GetWorldCenter());
    else _player2->getBody()->SetLinearVelocity(b2Vec2((x/2 + w/4 - px)*SIZE_RATIO/5, 0));

    
}

#pragma mark TOUCHES HANDLE
bool GameLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (_isPlaying) {
        if (_mouseJoint != NULL) return false;
        CCPoint tap = touch->getLocation();
        b2Vec2 target = this->ptm(tap);
        
        if (tap.y < h/2 && tap.y > BORDER_WIDTH_BOTTOM &&
            tap.x > BORDER_WIDTH_LEFT  && tap.x < w - BORDER_WIDTH_RIGHT) {
            b2MouseJointDef md;
            md.bodyA =  _groundBody;
            md.bodyB = _player1->getBody();
            md.target = target;
            md.collideConnected = true;
            md.maxForce = 1000000.0f * _player1->getBody()->GetMass();
            md.dampingRatio = 0;
            md.frequencyHz = 100000;
            
            _player1->setSpritePosition(tap);
            _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
            _player1->getBody()->SetAwake(true);
        }
    }
    return true;
}

void GameLayer::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    if (_isPlaying) {
        if (_mouseJoint == NULL) {
            CCPoint tap = touch->getLocation();
            b2Vec2 target = this->ptm(tap);
            if (tap.y < h/2 && tap.y > BORDER_WIDTH_BOTTOM &&
                tap.x > BORDER_WIDTH_LEFT  && tap.x < w - BORDER_WIDTH_RIGHT){
            b2MouseJointDef md;
            md.bodyA =  _groundBody;
            md.bodyB = _player1->getBody();
            md.target = target;
            md.collideConnected = true;
            md.maxForce = 1000000.0f * _player1->getBody()->GetMass();
            md.dampingRatio = 0;
            md.frequencyHz = 100000;
            
            _player1->setSpritePosition(tap);
            _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
            _player1->getBody()->SetAwake(true);
            }
        } else {
            CCPoint tap = touch->getLocation();
            if (tap.y > h/2 || tap.y < 10 ||
                tap.x > w - 10 || tap.x < 10) {
                return;
            }
            b2Vec2 target = this->ptm(tap);
            _mouseJoint->SetTarget(target);
        }
    }
}

void GameLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark New Turn
void GameLayer::newTurn() {
    _player1->reset();
    _player2->reset();
    _puck->getBody()->SetLinearVelocity(b2Vec2_zero);
    
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark GAME RESET
void GameLayer::gameReset() {
    _player1->reset();
    _player2->reset();
    _puck->reset();
    
    _isEnded = false;
    _score1 = _score2 = 0;
    _minutes = 3;
    _seconds = 0;
    
    _scoreLabel1->setString("0");
    _scoreLabel2->setString("0");
    
    char timeBuf[20] = {0};
	if(_minutes < 10 && _seconds < 10)
        sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);
	else if(_minutes < 10 && _seconds >= 10)
        sprintf(timeBuf, "0%i:%i", _minutes, _seconds);
    
    _timer->setString(timeBuf);
    
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark SCORE COUNTER
void GameLayer::scoreCounter(int player) {
    char scoreBuf[10];
    
    if (player == 1) {
        _score1++;
        sprintf(scoreBuf, "%d", _score1);
        _scoreLabel1->setString(scoreBuf);
        _puck->setSpritePosition(ccp(w/2, h/2 + 2*_puck->getRadius()));
    }
    
    if (player == 2) {
        _score2++;
        sprintf(scoreBuf, "%d", _score2);
        _scoreLabel2->setString(scoreBuf);
        _puck->setSpritePosition(ccp(w/2, h/2 - 2*_puck->getRadius()));
    }
}

#pragma mark Timer
void GameLayer::Timer() {
    if(_isPlaying && _minutes >= 0) {
		if(_seconds > 0)	_seconds--;
		else {
			if(_minutes > 0) _minutes--;
			else  _minutes = 0;
			_seconds = 59;
		}
	}
    
	char timeBuf[20] = {0};
	if(_minutes < 10 && _seconds < 10)
        sprintf(timeBuf, "0%d:0%d", _minutes, _seconds);
	else if(_minutes < 10 && _seconds >= 10)
        sprintf(timeBuf, "0%d:%d", _minutes, _seconds);
    
    _timer->setString(timeBuf);
}

#pragma mark Check High Score
void GameLayer::checkHighScore() {
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr + "/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(GameLayer::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameLayer::upScore(int score) {
    string username = GameManager::sharedGameManager()->getName();
    this->convertName((char*)username.c_str());
    if (!username.empty()) {
        char scoreString[20] = {0};
        sprintf(scoreString, "%d", score);
        string email  = GameManager::sharedGameManager()->getEmail();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        string url = ipAddr + "/users?name="+username+"&point="+scoreString+"&email="+email;
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
            curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "account=kienbg");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS ,1);
            curl_easy_setopt(curl, CURLOPT_POST, true);
            
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == 0) {
                CCLOG("0 response OK");
            } else {
                CCLog("code: %d",res);
            }
        }
    }
}


void GameLayer::getTopRankingList(){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        string url = ipAddr + "/users.json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, gameWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuf);
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        if (res == 0) {
            CCLOG("0 response OK\n");
        } else {
            CCLOG("GET DATA FAILED");
        }
    }
}


void GameLayer::checkScore(int score) {
    this->getTopRankingList();
    rapidjson::Document document;
    if(dataBuf.c_str() != NULL && !document.Parse<0>(dataBuf.c_str()).HasParseError()) {
        string name = GameManager::sharedGameManager()->getName();
        convertName((char*)name.c_str());
        if (document.Size() == 0) {
            int r = GameManager::sharedGameManager()->getReward();
            GameManager::sharedGameManager()->setReward(r + 1);
            if (name.empty()) CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
            else this->upScore(score);
        } else if (document.Size() <= 2) {
            if (score >= document[rapidjson::SizeType(0)]["point"].GetInt()) {
                int r = GameManager::sharedGameManager()->getReward();
                GameManager::sharedGameManager()->setReward(r + 1);
            }
            if (name.empty())
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
            else {
                this->upScore(score);
            }
        } else {
            for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
                if (score >= document[i]["point"].GetInt()) {
                    if (i == 0) {
                        int r = GameManager::sharedGameManager()->getReward();
                        GameManager::sharedGameManager()->setReward(r + 1);
                    }
                    if (name == "") {
                        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
                        break;
                    } else {
                        this->upScore(score);
                    }
                }
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
}

#pragma mark HTTP REQUEST
void GameLayer::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCHttpResponse *response = (CCHttpResponse*)data;
    if (!response) {
        CCLOG("Error !! Cannot get Respond !");
        return;
    }
    if (!response->isSucceed()) {
        CCLOG("Error !! Respond not succeeded !");
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
    int d = -1;
    for (unsigned int i = 0; i < buffer->size(); i++) {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    this->getTopRankingList();
    //-----------------------
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        string name = GameManager::sharedGameManager()->getName();
        convertName((char*)name.c_str());
        if (point == 0) point = _score1 * (_minutes * 60 + _seconds) *
            pow(10.0, GameManager::sharedGameManager()->getLevel() + 1.0);
        if (document.Size() == 0) {
            int r = GameManager::sharedGameManager()->getReward();
            GameManager::sharedGameManager()->setReward(r + 1);
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
        } else if (document.Size() <= 2) {
            if (point >= document[rapidjson::SizeType(0)]["point"].GetInt()) {
                int r = GameManager::sharedGameManager()->getReward();
                GameManager::sharedGameManager()->setReward(r + 1);
            }
            if (name == "")
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
            else {
                CCHttpRequest * request = new CCHttpRequest();
                string email  = GameManager::sharedGameManager()->getEmail();
                string ipAddr = GameManager::sharedGameManager()->getIpAddr();
                char strP[20] = {0};
                sprintf(strP, "%d", point);
                string url    = ipAddr + "/users?name="+name+"&point="+strP+"&email="+email;
                request->setUrl(url.c_str());
                request->setRequestType(CCHttpRequest::kHttpPost);
                CCHttpClient::getInstance()->send(request);
                request->release();
            }
        } else {
            for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
                if (point >= document[i]["point"].GetInt()) {
                    if (i == 0) {
                        int r = GameManager::sharedGameManager()->getReward();
                        GameManager::sharedGameManager()->setReward(r + 1);
                    }
                    if (name == "") {
                        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
                        break;
                    } else {
                        CCHttpRequest * request = new CCHttpRequest();
                        char strP[20] = {0};
                        sprintf(strP, "%d", point);
                        string email  = GameManager::sharedGameManager()->getEmail();
                        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
                        string url    = ipAddr + "/users?name="+name+"&point="+strP+"&email="+email;
                        request->setUrl(url.c_str());
                        request->setRequestType(CCHttpRequest::kHttpPost);
                        CCHttpClient::getInstance()->send(request);
                        request->release();
                        break;
                    }                    
                }
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    
    free(data2);
}

void GameLayer::endGame() {
    _endLayer->setVisible(true);
    if (_score1 > _score2) {
        if (point == 0) {
            point = _score1 * (_minutes * 60 + _seconds) *
                pow(10.0, GameManager::sharedGameManager()->getLevel() + 1.0);
            GameManager::sharedGameManager()->setPoint(point);
            if (point >= GameManager::sharedGameManager()->getBestScore()) {
                GameManager::sharedGameManager()->setBestScore(point);
            }
            this->checkScore(point);
        }
        
        _resultLabel->setVisible(true);
        _resultLabel->setString("YOU WIN");
        _resultLabel->runAction(CCBlink::create(2.0f, 5.0f));
        if (pointCal < point) {
            if (pointUnit == 0) pointUnit = point/(_minutes*60 + _seconds);
            
            if(_minutes >= 0) {
                if(_seconds > 0) {
                    _seconds--;
                }
                else {
                    if(_minutes > 0) {
                        _minutes--;
                        _seconds = 59;
                    }
                }
                char timeBuf[20] = {0};
                if(_minutes < 10 && _seconds < 10)
                    sprintf(timeBuf, "0%d:0%d", _minutes, _seconds);
                else if(_minutes < 10 && _seconds >= 10)
                    sprintf(timeBuf, "0%d:%d", _minutes, _seconds);
                
                _timer->setString(timeBuf);
                
                pointCal += pointUnit;
                char scoreBuf[20];
                sprintf(scoreBuf, "score: %d",pointCal);
                _scoreLabel->setString(scoreBuf);
                _scoreLabel->setColor(ccYELLOW);
            }  
        } else {
            this->pauseSchedulerAndActions();
        }
    }
    else if (_score1 == _score2) {
        _resultLabel->setString("DRAW");
        this->pauseSchedulerAndActions();
    }
    else {
        _scoreLabel->setString("score: 0");
        _scoreLabel->setColor(ccWHITE);
        _resultLabel->setString("YOU LOSE");
        this->pauseSchedulerAndActions();
    }

}


void GameLayer::addEffect(CCPoint point) {
    CCParticleSystemQuad *pop = new CCParticleSystemQuad;
//    pop = CCParticleGalaxy::create();
    pop->setTexture(CCTextureCache::sharedTextureCache()->addImage("Star.png"));
    pop->setPosition(point);
    pop->setGravity(CCPointZero);
    
    pop->setAngle(140);
    pop->setAngleVar(360);
    
    pop->setSpeed(80);
    //pop->setSpeedVar(360);
    
    pop->setLife(2.0f);
    pop->setStartSize(40);
    
    pop->setPositionType(kCCPositionTypeRelative);
    
    pop->setRadialAccel(-50);
    //pop->setRadialAccelVar(-100);
    
    //pop->setTangentialAccel(-50);
    pop->setTotalParticles(9);
    //pop->setRotatePerSecond(0);
    pop->setAutoRemoveOnFinish(true);
    pop->setAtlasIndex(0);
    pop->setBlendAdditive(false);
    //pop->setOrderOfArrival(0);
    pop->setOpacityModifyRGB(false);
    pop->setDuration(0.5);
    pop->setEmissionRate(200);
    //pop->setEndRadius(50);
    this->addChild(pop, 100, 777);
}

void GameLayer::convertName(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++)
    {
        if(str_name[i] == ' ')
        {
            str_name[i] = '_';
        }
    }
}

b2Vec2 GameLayer::ptm(CCPoint point) {
    return b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO);
}

b2Vec2 GameLayer::ptm2(float x, float y) {
    return b2Vec2(x / PTM_RATIO, y / PTM_RATIO);
}
