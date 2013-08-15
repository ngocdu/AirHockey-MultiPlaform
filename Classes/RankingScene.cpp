//
//  RankingScene.cpp
//  AirHockey
//
//

#include "RankingScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* RankingScene::scene() {
    CCScene *scene = CCScene::create();
    RankingScene *layer1 = RankingScene::create();
    scene->addChild(layer1);
    return scene;
}

bool RankingScene::init() {
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;

    players = new CCArray();
    if (GameManager::sharedGameManager()->getBgm()) musicPlayed = true;
    else musicPlayed = false;
    introEnd = false;
    
    CCSprite *background = CCSprite::create("BackGrounds/RankingBG.png");
    background->setPosition(ccp(w/2, h/2));
    background->setScaleY(h/background->getContentSize().height);
    background->setScaleX(w/background->getContentSize().width);
    this->addChild(background);
    
    CCSprite *topLine = CCSprite::create("line.png");
    CCSprite *bottomLine = CCSprite::create("line.png");
    
    topLine->setScale(SIZE_RATIO);
    bottomLine->setScale(SIZE_RATIO);
    
    topLine->setPosition(ccp(w/2, 602*SIZE_RATIO_Y));
    bottomLine->setPosition(ccp(w/2, 248*SIZE_RATIO_Y));
    
    topLine->setOpacity(70);
    bottomLine->setOpacity(70);
    this->addChild(topLine);
    this->addChild(bottomLine);
    
    CCMenuItemImage *reward  = CCMenuItemImage::create("Present.png",
                                                       "PresentOnClicked.png",
                                                       this, menu_selector(RankingScene::reward));
    reward->setScaleX(SIZE_RATIO_X);
    reward->setScaleY(SIZE_RATIO_Y);
    reward->setPosition(ccp(w/5, h/8));

    string playerName = GameManager::sharedGameManager()->getName();
    if (playerName == "") reward->setVisible(false);
    
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    string url    = ipAddr + ":3000/users/Ngoc_Du.json";
    request->setUrl((ipAddr+":3000/users.json").c_str());
//    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("Buttons/StartButton.png", "Buttons/StartButtonOnClicked.png",
                                this, menu_selector(RankingScene::play));
    playItem->setScaleX(SIZE_RATIO_X);
    playItem->setScaleY(SIZE_RATIO_Y);
    playItem->setPosition(ccp(w/2, h/8 - 10));
    //create bgmItem
    CCMenuItemImage *bgmItem =
        CCMenuItemImage::create("BgmOn.png", "BgmOn.png",
                                this, menu_selector(RankingScene::bgm));
    bgmItem->setScaleX(SIZE_RATIO_X);
    bgmItem->setScaleY(SIZE_RATIO_Y);
    bgmItem->setPosition(ccp(w*4/5, h/8));
    CCMenu* pMenu = CCMenu::create(playItem, bgmItem, reward, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
    

    bgm_off = CCSprite::create("BgmOff.png");
    bgm_off->setScaleX(SIZE_RATIO_X);
    bgm_off->setScaleY(SIZE_RATIO_Y);
    bgm_off->setPosition(bgmItem->getPosition());
    bgm_off->setVisible(!GameManager::sharedGameManager()->getBgm());
    
    if (!GameManager::sharedGameManager()->getBgm()) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        introEnd=  false;
    } else {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
        CCAction *playIntro = CCCallFuncN::create(this, callfuncN_selector(RankingScene::playIntro));
        CCAction *delayTime = CCDelayTime::create(3.7f);
        CCAction *playBGM = CCCallFuncN::create(this, callfuncN_selector(RankingScene::playBGM));
        CCArray *actionList = new CCArray(3);
        actionList->autorelease();
        actionList->insertObject(playIntro, 0);
        actionList->insertObject(delayTime, 1);
        actionList->insertObject(playBGM, 2);
        CCAction *BGM = CCSequence::create(actionList);
        this->runAction(BGM);
    }
    
    this->addChild(bgm_off);
    this->scheduleUpdate();
    return true;
}

void RankingScene::playIntro() {
    if (GameManager::sharedGameManager()->getBgm()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/StartIntro.mp3");
    }
}

void RankingScene::playBGM() {
    if (GameManager::sharedGameManager()->getBgm()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sounds/StartBG.mp3", true);
    }
}

void RankingScene::update(float dt) {
    if (GameManager::sharedGameManager()->getBgm()) {
        if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            scheduleOnce(schedule_selector(RankingScene::playBGM), true);
    }
}

void RankingScene::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response) {
        return;
    }

    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {0};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    
    if (!response->isSucceed()) {
        CCLabelTTF *checkInternetMsg = CCLabelTTF::create("「現在ランキングは閉じています」", FONT, 24*SIZE_RATIO);
        checkInternetMsg->setPosition(ccp(w/2, h/2 - 40*SIZE_RATIO));
        this->addChild(checkInternetMsg);
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
    int d = -1;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++) {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    //-----------------------

    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        string username = GameManager::sharedGameManager()->getName();
        string email = GameManager::sharedGameManager()->getEmail();
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            convertName((char*)name.c_str());
            string mail = document[i]["email"].GetString();
            string time = document[i]["updated_at"].GetString();
            int p = document[i]["point"].GetInt();
            int r = document[i]["reward"].GetInt();
            Player1 *player = new Player1(name,p, mail, time, r);
            players->addObject(player);
            
        }
    } else {
        CCLog(document.GetParseError());
    }
    free(data2);
    CCTableView *tableView = CCTableView::create(this, CCSizeMake(700*SIZE_RATIO_X,
                                                                350*SIZE_RATIO_Y));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(w/8, 250*SIZE_RATIO_Y));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView, 21);
    tableView->reloadData();
}

void RankingScene::reward(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RewardScene::scene());
}

void RankingScene::bgm(CCObject* pSender) {
    GameManager *game = GameManager::sharedGameManager();
    game->setBgm(!game->getBgm());
    bgm_off->setVisible(!game->getBgm());
    if (!game->getBgm()) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    } else {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
        if (musicPlayed) SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        else {
            musicPlayed = true;
            CCAction *playIntro = CCCallFuncN::create(this, callfuncN_selector(RankingScene::playIntro));
            CCAction *delayTime = CCDelayTime::create(3.7f);
            CCAction *playBGM = CCCallFuncN::create(this, callfuncN_selector(RankingScene::playBGM));
            CCArray *actionList = new CCArray(3);
            actionList->autorelease();
            actionList->insertObject(playIntro, 0);
            actionList->insertObject(delayTime, 1);
            actionList->insertObject(playBGM, 2);
            CCAction *BGM = CCSequence::create(actionList);
            this->runAction(BGM);
        }
    }
}

void RankingScene::play(CCObject* pSender) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, Difficulty::scene()));
}

void RankingScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize RankingScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return CCSizeMake(600*SIZE_RATIO_X, 80*SIZE_RATIO_Y);
}

CCTableViewCell* RankingScene::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell *cell = table->dequeueCell();
    cell = new CCTableViewCell();
    cell->autorelease();
    
    // Player Point
    Player * p = (Player*)players->objectAtIndex(idx);
    CCString *string = CCString::createWithFormat("%d",p->getPoint());

    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(),
                                                "Helvetica",
                                                35*SIZE_RATIO);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(550*SIZE_RATIO_X,0));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    
    // Player Name
    std::string name = p->getName();
    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(),
                                               "Helvetica",
                                               35*SIZE_RATIO);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(ccp(60*SIZE_RATIO_X, 0));
    cell->addChild(Namelabel);
    
    // Player Rank
    char rankBuf[15];
    sprintf(rankBuf, "Numbers/%i.png", idx+1);
    CCSprite *rank = CCSprite::create(rankBuf);
    rank->setScale(SIZE_RATIO);
    rank->setAnchorPoint(CCPointZero);
    rank->setPosition(CCPointZero);
    cell->addChild(rank);    
    return cell;
}

unsigned int RankingScene::numberOfCellsInTableView(CCTableView *table) {
    return players->count();
}
void RankingScene::convertName(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++)
    {
        if(str_name[i] == '_')
        {
            str_name[i] = ' ';
        }
    }
}
Player::Player(string name, int point) {
    this->_point = point;
    this->_name = name;
}
Player::~Player(){}
