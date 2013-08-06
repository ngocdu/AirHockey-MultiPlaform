//
//  Difficulty.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#include "Difficulty.h"

CCScene* Difficulty::scene() {
    CCScene *scene = CCScene::create();
    Difficulty *layer = Difficulty::create();
    scene->addChild(layer);
    return scene;
}

bool Difficulty::init() {
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;
    
    CCSprite *background = CCSprite::create("BackGrounds/BackGround3.png");
    background->setPosition(ccp(w/2, h/2));
    background->setScaleY(h/background->getContentSize().height);
    background->setScaleX(w/background->getContentSize().width);
    this->addChild(background);

    CCMenuItemFont *easyMenuItem;
    easyMenuItem->setFontName(FONT2);
    easyMenuItem = CCMenuItemFont::create("EASY", this, menu_selector(Difficulty::menuEasy));
    easyMenuItem->setPosition(ccp(w/2, h/2));
    easyMenuItem->setFontSizeObj(70*SIZE_RATIO);
    easyMenuItem->setFontSize(70*SIZE_RATIO);

    CCMenuItemFont *mediumMenuItem =
    CCMenuItemFont::create("MEDIUM", this, menu_selector(Difficulty::menuMedium));
    mediumMenuItem->setPosition(ccp(w/2, h*2/5));
    
    CCMenuItemFont *hardMenuItem =
    CCMenuItemFont::create("HARD", this, menu_selector(Difficulty::menuHard));
    hardMenuItem->setPosition(ccp(w/2, h*3/10      ));

    CCMenuItemImage *back =
    CCMenuItemImage::create("Buttons/BackButton.png", "Buttons/BackButtonOnClicked.png",
                            this, menu_selector(Difficulty::clickBtBack));
    back->setScaleX(SIZE_RATIO_X);
    back->setScaleY(SIZE_RATIO_Y);
    back->setPosition(ccp(w/2, h/8));
    
    pMenu = CCMenu::create(easyMenuItem, mediumMenuItem, hardMenuItem, back, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    return true;
}

void Difficulty::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void Difficulty::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void Difficulty::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox,
                                    const std::string& text)
{
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void Difficulty::editBoxReturn(cocos2d::extension::CCEditBox* editBox) {
//    GameManager::sharedGameManager()->setName(m_pEditName->getText());
}


// Game Level Easy=1, Medium=2, Hard=3
void Difficulty::menuHard(CCObject *pSender) {
//    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(3);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::menuMedium(CCObject *pSender) {
//    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(2);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::menuEasy(CCObject *pSender) {
//    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(1);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::clickBtBack(cocos2d::CCObject *pScene) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
}
