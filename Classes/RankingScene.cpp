//
//  RankingScene.cpp
//  AirHockey
//
//

#include "RankingScene.h"
#include "string"

USING_NS_CC;
USING_NS_CC_EXT;

int rankingWriter(char *data, size_t size, size_t nmemb, string *buffer)
{
    int result = 0;
    
    if (buffer != NULL)
    {
        //バッファ追記
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    
    return result;
}

CCScene* RankingScene::scene() {
    CCScene *scene = CCScene::create();
    RankingScene *layer1 = RankingScene::create();
    scene->addChild(layer1);
    return scene;
}

bool RankingScene::init() {
    this->upBestScore();
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;
    CCLOG("%f     %f        %f", SIZE_RATIO_X, SIZE_RATIO_Y, SIZE_RATIO);

    players = new CCArray();
    if (GameManager::sharedGameManager()->getBgm()) musicPlayed = true;
    else musicPlayed = false;
    introEnd = false;
    
    CCSprite *background = CCSprite::create("BackGrounds/RankingBG.png");
    background->setPosition(ccp(w/2, h/2));
    background->setScaleY(h/background->getContentSize().height);
    background->setScaleX(w/background->getContentSize().width);
    this->addChild(background);
    
    CCMenuItemImage *reward  = CCMenuItemImage::create("Present.png",
                                                       "PresentOnClicked.png",
                                                       this, menu_selector(RankingScene::reward));
    reward->setScaleX(SIZE_RATIO_X);
    reward->setScaleY(SIZE_RATIO_Y);
    reward->setPosition(ccp(w/5, h/8));

    string playerName = GameManager::sharedGameManager()->getName();
    if (playerName == "") reward->setVisible(false);
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("Buttons/StartButton.png", "Buttons/StartButtonOnClicked.png",
                                this, menu_selector(RankingScene::play));
    playItem->setScaleX(SIZE_RATIO_X);
    playItem->setScaleY(SIZE_RATIO_Y);
    playItem->setPosition(ccp(w/2, h/8));
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
    
    this->displayRanking();
    
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

void RankingScene::upBestScore() {
    this->getRanking();
    rapidjson::Document document;
    string nameLocal = GameManager::sharedGameManager()->getName();
    convertName2((char*)nameLocal.c_str());
    int pointLocalBest = GameManager::sharedGameManager()->getBestScore();
    int k = 0;
    if(dataBuf.c_str() != NULL && !document.Parse<0>(dataBuf.c_str()).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            
            string mail = document[i]["email"].GetString();
            string time = document[i]["updated_at"].GetString();
            int p = document[i]["point"].GetInt();
            if (pointLocalBest == p && name == nameLocal) {
                k++;
            }
            convertName((char*)name.c_str());
            if (p < pointLocalBest && i == 2 && k == 0) {
                char scoreString[20] = {0};
                sprintf(scoreString, "%i", pointLocalBest);
                string email  = GameManager::sharedGameManager()->getEmail();
                string ipAddr = GameManager::sharedGameManager()->getIpAddr();
                string url = ipAddr + "/users?name="+nameLocal+"&point="+scoreString+"&email="+email;
                this->postUrl(url);
            }
        }
    }
    dataBuf = "";
}
void RankingScene::postUrl(string url) {
    string username = GameManager::sharedGameManager()->getName();
    if (!username.empty()) {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            //133.242.203.251
            //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
            
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
            curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "account=kienbg");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            //        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS ,1);
            curl_easy_setopt(curl, CURLOPT_POST, true);
            
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == 0) {
                CCLOG("0 response OK");
            } else {
                CCLog("code: %i",res);
            }
        }
    }
}

void RankingScene::getRanking() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        //133.242.203.251
        //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
        
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        string url = ipAddr + "/users.json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rankingWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuf);
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        if (res == 0) {
            CCLOG("0 response OK\n");
        } else {
            CCLog("code: %i",res);
            CCLabelTTF *checkInternetMsg = CCLabelTTF::create("現在ランキングは閉じています", FONT, 30*SIZE_RATIO);
            checkInternetMsg->setPosition(ccp(w/2, h/2 - 30*SIZE_RATIO));
            checkInternetMsg->setColor(ccYELLOW);
            this->addChild(checkInternetMsg);
        }
    } else {
        CCLOG("no curl\n");
    }
    
}
void RankingScene::displayRanking() {
    this->getRanking();
    rapidjson::Document document;
    string nameLocal = GameManager::sharedGameManager()->getName();
    convertName2((char*)nameLocal.c_str());
    int pointLocalBest = GameManager::sharedGameManager()->getBestScore();
    int k = 0;
    if(dataBuf.c_str() != NULL && !document.Parse<0>(dataBuf.c_str()).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            
            string mail = document[i]["email"].GetString();
            string time = document[i]["updated_at"].GetString();
            int p = document[i]["point"].GetInt();
            int r = document[i]["reward"].GetInt();
            if (pointLocalBest == p && name == nameLocal) {
                k++;
            }
            convertName((char*)name.c_str());
            if (p < pointLocalBest && i == 2 && k == 0) {
                char scoreString[20] = {0};
                sprintf(scoreString, "%i", pointLocalBest);
                string email  = GameManager::sharedGameManager()->getEmail();
                string ipAddr = GameManager::sharedGameManager()->getIpAddr();
                string url = ipAddr + "/users?name="+nameLocal+"&point="+scoreString+"&email="+email;
                this->postUrl(url);
            }
            Player1 *player = new Player1(name, p, mail, time, r);
            players->addObject(player);
            CCString *point = CCString::createWithFormat("%d",player->getPoint());
            string s = RankingScene::scoreFormat(point->getCString());
            CCLabelTTF *pointLabel = CCLabelTTF::create(s.c_str(),
                                                        FONT, 35 * SIZE_RATIO);
            pointLabel->setAnchorPoint(CCPointZero);
            pointLabel->setPosition(ccp(250 * SIZE_RATIO_X,
                                        (560 - 100 * (players->count() - 1)) * SIZE_RATIO_Y));
            pointLabel->setTag(123);
            this->addChild(pointLabel);
            
            CCLabelTTF *nameLabel = CCLabelTTF::create(player->getName().c_str(),
                                                       FONT, 30 * SIZE_RATIO);
            nameLabel->setAnchorPoint(CCPointZero);
            nameLabel->setPosition(ccp(250 * SIZE_RATIO_X,
                                       (590 - 100 * (players->count() - 1)) * SIZE_RATIO_Y));
            this->addChild(nameLabel);
            
            char rankBuf[15];
            sprintf(rankBuf, "Numbers/%i.png", players->count());
            CCSprite *rank = CCSprite::create(rankBuf);
            rank->setScale(SIZE_RATIO);
            rank->setAnchorPoint(CCPointZero);
            rank->setPosition(ccp(100 * SIZE_RATIO_X,
                                  (550 - 100 * (players->count() - 1)) * SIZE_RATIO_Y));
            this->addChild(rank);
            
            CCSprite *line = CCSprite::create("line.png");
            line->setScaleX(SIZE_RATIO_X);
            line->setScaleY(SIZE_RATIO_Y);
            line->setPosition(ccp(w/2, (540- 100 * (players->count() - 1)) * SIZE_RATIO_Y));
            this->addChild(line);
        }
    } else {
        CCLog(document.GetParseError());
    }
    
    CCString *bestScore =
    CCString::createWithFormat("%d", GameManager::sharedGameManager()->getBestScore());
    string s = RankingScene::scoreFormat(bestScore->getCString());
    CCLabelTTF *bestScoreLabel = CCLabelTTF::create(s.c_str(),
                                                    FONT, 45 * SIZE_RATIO);
    bestScoreLabel->setAnchorPoint(CCPointZero);
    bestScoreLabel->setPosition(ccp(250 * SIZE_RATIO_X,
                                    220 * SIZE_RATIO_Y));
    this->addChild(bestScoreLabel);
    
    string username = GameManager::sharedGameManager()->getName();
    if (username.empty()) {
        username = "YOU";
    }
    CCLabelTTF *nameLabel = CCLabelTTF::create(username.c_str(),
                                               FONT, 30 * SIZE_RATIO);
    nameLabel->setAnchorPoint(CCPointZero);
    nameLabel->setPosition(ccp(250 * SIZE_RATIO_X,
                               260 * SIZE_RATIO_Y));
    this->addChild(nameLabel);
    dataBuf = "";
    players->removeAllObjects();
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
            CCAction *playIntro =
            CCCallFuncN::create(this, callfuncN_selector(RankingScene::playIntro));
            CCAction *delayTime = CCDelayTime::create(3.7f);
            CCAction *playBGM =
            CCCallFuncN::create(this, callfuncN_selector(RankingScene::playBGM));
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
void RankingScene::convertName(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++) {
        if(str_name[i] == '_') {
            str_name[i] = ' ';
        }
    }
}
void RankingScene::convertName2(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++) {
        if(str_name[i] == ' ') {
            str_name[i] = '_';
        }
    }
}
string RankingScene::scoreFormat(string score){
    string s = score;
    int i = 1;
    while ((i * 3) < score.length()) {
        s.insert(score.length() - i * 3, ",");
        i++;
    }
    return s;
}
Player::Player(string name, int point) {
    this->_point = point;
    this->_name = name;
}
Player::~Player(){}
