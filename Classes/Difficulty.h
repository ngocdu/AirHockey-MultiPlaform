//
//  Difficulty.h
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#ifndef __AirHockey__Difficulty__
#define __AirHockey__Difficulty__

#include "AppMacros.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameManager.h"
#include "RankingScene.h"
#include "GameLayer.h"

using namespace cocos2d;

class Difficulty : public CCLayer, public extension::CCEditBoxDelegate{
private:
    CCSize size;
    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    bool clickBackAble;
    extension::CCEditBox  *m_pEditName;
    CCLabelTTF *m_pTTFShowEditReturn;
    CCMenu *pMenu;
public:
    virtual bool init();
    
    static CCScene* scene();
    void toExtensionsMainLayer(CCObject *sender);
    virtual void editBoxEditingDidBegin(extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(extension::CCEditBox* editBox,
                                    const std::string& text);
    virtual void editBoxReturn(extension::CCEditBox* editBox);
    void menuMedium(CCObject *pSender);
    void menuEasy(CCObject  *pSender);
    void menuHard(CCObject  *pSender);
    void clickBtBack(CCObject* pScene);

    CREATE_FUNC(Difficulty);
};


#endif /* defined(__AirHockey__Difficulty__) */
