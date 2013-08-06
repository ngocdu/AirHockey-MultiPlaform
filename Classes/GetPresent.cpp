//
//  GetPresent.cpp
//  AirHockey
//
//

#include "GetPresent.h"

CCScene* GetPresent::scene() {
    CCScene *scene = CCScene::create();
    GetPresent *layer = GetPresent::create();
    scene->addChild(layer);
    return scene;
}

bool GetPresent::init() {
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;

    CCSize editBoxSize = CCSizeMake((w - 100) * SIZE_RATIO, 60 * SIZE_RATIO);
    
    CCLabelTTF *congrats = CCLabelTTF::create("TOP 10 SCORE !!!", FONT, 64 * SIZE_RATIO);
    CCLabelTTF *emailLabel1 = CCLabelTTF::create("Please enter your Email", FONT, 36 * SIZE_RATIO);
    CCLabelTTF *emailLabel2 = CCLabelTTF::create("to get Presents:", FONT, 36 * SIZE_RATIO);
    
    congrats->setPosition(ccp(w/2, h*6/8));
    congrats->setColor(ccYELLOW);
    emailLabel1->setPosition(ccp(w/2, h*5/8));
    emailLabel2->setPosition(ccp(w/2, emailLabel1->getPositionY() - 45*SIZE_RATIO));
    
    this->addChild(emailLabel1);
    this->addChild(congrats);
    this->addChild(emailLabel2);
    
    m_pUserEmail = extension::CCEditBox::create(editBoxSize, extension::CCScale9Sprite::create("WhiteBox.png"));
    m_pUserEmail->setPosition(ccp(w/2, emailLabel2->getPositionY() - 60*SIZE_RATIO));
    m_pUserEmail->setFontSize(40 * SIZE_RATIO);
    m_pUserEmail->setMaxLength(30);
    m_pUserEmail->setFontColor(ccBLACK);

    m_pUserEmail->setMaxLength(40);
    m_pUserEmail->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserEmail->setInputMode(kEditBoxInputModeEmailAddr);
    m_pUserEmail->setDelegate(this);
    this->addChild(m_pUserEmail);
    
    // Email Fail Message
    emailFailMsg = CCLabelTTF::create("Invalid Email !! Please Try Again !!", FONT, 24 * SIZE_RATIO);
    emailFailMsg->setPosition(ccp(m_pUserEmail->getPosition().x,
                                  m_pUserEmail->getPosition().y - 50*SIZE_RATIO));
    emailFailMsg->setColor(ccRED);
    emailFailMsg->setVisible(false);
    this->addChild(emailFailMsg);
    
    // name
    CCLabelTTF *nameLabel = CCLabelTTF::create("please choose your username:", FONT, 36 * SIZE_RATIO);
    nameLabel->setPosition(ccp(w/2, h*3/8));
    this->addChild(nameLabel);
    m_pUserName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("WhiteBox.png"));
    m_pUserName->setPosition(ccp(w/2, nameLabel->getPositionY() - 60*SIZE_RATIO));
    m_pUserName->setFontSize(40 * SIZE_RATIO);
    m_pUserName->setFontColor(ccBLACK);
    m_pUserName->setMaxLength(10);
    m_pUserName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserName->setInputMode(kEditBoxInputModeAny);
    m_pUserName->setDelegate(this);
    this->addChild(m_pUserName);
    
    nameFailMsg = CCLabelTTF::create("please enter your username !!", FONT, 24 * SIZE_RATIO);
    nameFailMsg->setPosition(ccp(m_pUserName->getPosition().x,
                                  m_pUserName->getPosition().y - 50 * SIZE_RATIO));
    nameFailMsg->setColor(ccRED);
    nameFailMsg->setVisible(false);
    this->addChild(nameFailMsg);
    
    CCMenuItemImage *sendMenuItem = CCMenuItemImage::create("Buttons/SubmitButton.png",
                                                            "Buttons/SubmitButtonOnClicked.png",
                                                            this, menu_selector(GetPresent::menuSendEmail));
    sendMenuItem->setScale(SIZE_RATIO);
    sendMenuItem->setPosition(ccp(w/2, 100));
    
    pMenu = CCMenu::create(sendMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    return true;
}
void GetPresent::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void GetPresent::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void GetPresent::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox,
                                    const std::string& text)
{
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void GetPresent::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p was returned !");
    GameManager::sharedGameManager()->setEmail(m_pUserEmail->getText());
}

#pragma mark valid Email
bool GetPresent::is_email(std::string const& address) {
    size_t at_index = address.find_first_of('@', 0);
    return at_index != std::string::npos

      && address.find_first_of('.', at_index) != std::string::npos;
    
}
int GetPresent::spc_email_isvalid(const char *address) {
    int        count = 0;
    const char *c, *domain;
    static char *rfc822_specials = "()<>@,;:\\\"[]!#$&%|*+}{?\'";
    
    /* first we validate the name portion (name@domain) */
    for (c = address;  *c;  c++) {
        if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
                           '\"')) {
            while (*++c) {
                if (*c == '\"') break;
                if (*c == '\\' && (*++c == ' ')) continue;
                if (*c == '&' || *c == '%' || *c == '$' || *c == '#' || *c == '!'
                    || *c <= ' ' || *c >= 127) return 0;
            }
            if (!*c++) return 0;
            if (*c == '@') break;
            if (*c != '.') return 0;
            continue;
        }
        if (*c == '@') break;
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    }
    if (c == address || *(c - 1) == '.') return 0;
    
    /* next we validate the domain portion (name@domain) */
    if (!*(domain = ++c)) return 0;
    do {
        if (*c == '.') {
            if (c == domain || *(c - 1) == '.') return 0;
            count++;
        }
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    } while (*++c);
    
    return (count >= 1);
}

void GetPresent::menuSendEmail(CCObject *pSender)
{
    int i = 0;
    int j = 0;
    
    string email = m_pUserEmail->getText();
    if (!this->isValidEmail(email)) {
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
        emailFailMsg->setVisible(true);
        emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    } else i = 1;

    if (strcmp(m_pUserName->getText(), "") == 0) {
        CCLOG("wrong username");
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
        nameFailMsg->setVisible(true);
        nameFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    } else j = 1;
    
    if (i*j == 1) {
        char * n =(char*) m_pUserName->getText();
        standardizeName(n);
        removeSpace(n);
        GameManager::sharedGameManager()->setName(n);
        CCHttpRequest* request = new CCHttpRequest();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        request->setUrl((ipAddr+":3000/users.json").c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, callfuncND_selector(GetPresent::onHttpRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
}
void GetPresent::menuBack(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}
void GetPresent::insertChar(char *xau,int index)
{
    for(int i=strlen(xau)+1;i>index&&i>0;i--) xau[i]=xau[i-1];
    xau[index]=' ';
}
void GetPresent::standardizeName(char *xau)
{
    int i,j=0;
    for(i=0;i<strlen(xau);i++)
    {
        if (j==0&&strchr(",.\!;:?",xau[i])) continue;
        else if (i&&j&&strchr(",.\!;:?",xau[i-1])&&xau[i]!=' ') insertChar(xau,i);
        
        if (j&&strchr(",.\!;:?",xau[i])&&xau[j-1]==' ')  xau[j-1]=xau[i],xau[j]=' ';
        else if ((j==0&&xau[i]!=' ')||(j&&xau[j-1]==' '&&xau[i]!=' ')) xau[j++]=toupper(xau[i]);
        else if ((j&&xau[i]!=' ')||(j&&xau[i-1]!=' '&&xau[i]==' ')) xau[j++]=xau[i];
    }
    xau[j-1*(j&&xau[j-1]==' ')]=NULL;
}
void GetPresent::removeSpace(char *xau) {
    int len = 0;
    int i = 0;
    len=strlen(xau);
    for(i=0;i<len;i++)
    {
        if(xau[i] == ' ')
        {
           xau[i] = '_';
        }
    }
}

void GetPresent::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCSize w = CCDirector::sharedDirector()->getWinSize();
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
        CCLabelTTF *notConnectLabel =
        CCLabelTTF::create("Can't load Data",
                           "Time new roman",
                           20 * SIZE_RATIO);
        notConnectLabel->setPosition(ccp(w.width/2, w.height/2));
        this->addChild(notConnectLabel);
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

    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            string email_server = document[i]["email"].GetString();
            CCLOG("%s", name.c_str());
            CCLOG("%s", m_pUserName->getText());
            char * n =(char*) m_pUserName->getText();
            standardizeName(n);
            removeSpace(n);
            CCLOG("text name : %s", n);
            if (strcmp(n, name.c_str()) == 0) {
               if(strcmp(m_pUserEmail->getText(), "") != 0 && strcmp(m_pUserEmail->getText(), email_server.c_str()) == 0 ){
                   CCFiniteTimeAction *showAction = CCFadeIn::create(0.2f);
                   CCFiniteTimeAction *hideAction = CCFadeOut::create(0.2f);
                   emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
                   return;
               } 
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    
    if (strcmp(m_pUserEmail->getText(), "") != 0 && this->isValidEmail(m_pUserEmail->getText()) &&
        this->spc_email_isvalid(m_pUserEmail->getText())) {
        CCHttpRequest * request = new CCHttpRequest();
        string name = GameManager::sharedGameManager()->getName();
        int p = GameManager::sharedGameManager()->getPoint();
        char strP[20] = {0};
        sprintf(strP, "%i", p);
        string email  = GameManager::sharedGameManager()->getEmail();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        int reward    = GameManager::sharedGameManager()->getReward();
        string url;
        if (reward != 0) {
            url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email;
        } else {
            url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email;
        }
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpPost);
        CCHttpClient::getInstance()->send(request);
        request->release();
        GameManager::sharedGameManager()->setEmail(m_pUserEmail->getText());
        
        char * n =(char*) m_pUserName->getText();
        standardizeName(n);
        GameManager::sharedGameManager()->setName(n);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
    } else {
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.2f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(0.2f);
        emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    }
    d-=1;
}
bool GetPresent::isValidEmail(std::string email){
    int length = email.length();
    if (length >= 9 && ((email[0] > 64 && email[0] < 91) ||
                        (email[0] > 96 && email[0] < 123)) && email[length - 1] != '.') {
        int count1 = 0;
        int count2 = 0;
        int count3 = 0;
        int count4 = 0;
        for (int i = 0; i < length; i++) {
            if (email[i] == '@') {
                count3++;
            }
            if ((email[i] < 48 && email[i] != 46) || (email[i] > 57 && email[i] < 64) ||
                (email[i] >90 && email[i] < 97 && email[i] != 95) || (email[i] >122)) {
                count4++;
            }
            if (email[i] == '.') {
                if (i < email.find('@')) {
                    count1++;
                } else {
                    count2++;
                }
            }
        }
        if (count3 == 1 && count4 == 0 && count1 < 2 && count2 > 0 &&
            ( email.find('@') > 3 && email.find('@') < 32)) {
            return true;
        } else {
            return false;
        }
        
    } else {
        return false;
    }

}


