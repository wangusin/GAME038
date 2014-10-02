#ifndef __HELLOWORLD_FunctionMenu_H__
#define __HELLOWORLD_FunctionMenu_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "constants.h"

#include "CCScrollView.h"
#include "ShareData.h"
#include "TouchSprite.h"

#define BellPictureName "p_bt02.png"
#define PreScenePictureName "p_bt01.png"
#define NextScenePictureName "p_bt01.png"

#define StoryBoardPictureName "p_bt03.png"
#define SpeakerPictureName "p_bt040%d.png"
#define LanguagePictureName "p_bt05.png"
#define HintPictureName "p_bt06.png"

#define LANGUAGE_BACKGROUND "01/Button020402.png"
#define LANGUAGE_HIGHLIGHT "01/Button020403.png"
#define LANGUAGE_CHINESE_T "01/Button020404.png"
#define LANGUAGE_CHINESE_S "01/Button020405.png"
#define LANGUAGE_ENGLISH "01/Button020406.png"

#define SELECT_SCENE_BACKGROUND "01/p_bt0301.png"
#define SELECT_SCENE_PICTURE "01/p_bt0301000%d.png"

#define HINT_BACKGROUND_PICTURE "01底/p_bt0601.png"
#define HINT_WORDS_PICTURE "02文字/p_bt0602%s.png"

#define FUNC_CLOSE_BUTTON "p_read02.png"

#define CLICKED_BUTTON_SOUND      "HB0026SE0102.mp3"      //按鈕音

USING_NS_CC;
USING_NS_CC_EXT;

enum FunctionMenuStatue
{
    FUNCTION_MENU_CLOSING = 0,
    FUNCTION_MENU_OPENING = 1,
};

enum SpeakerStatus
{
    TURN_ON = 1,
    TURN_OFF = 2,
};

enum NumberOfFunction
{
    SCENE_SHORTCUT_NUMBER = 11000,
};


using namespace cocos2d;
using namespace CocosDenshion;

class FunctionMenu : public cocos2d::CCLayer, public CCScrollViewDelegate
{
public:
    FunctionMenu();
    ~FunctionMenu();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    
    void setInitialObject(int nowScene, CCObject *target, SEL_CallFunc preSceneCallBackFunc, SEL_CallFunc nextSceneCallBackFunc, SEL_CallFuncND choseSceneClickedCallFunc, SEL_CallFuncND functionStatusCallBackFunc, SEL_CallFunc languageSelectCallBackFunc);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(FunctionMenu);
    
    void releaseObject();
    
protected:
    
    CCSprite* bell;
    CCSprite* preScene;
    CCSprite* nextScene;
    
    CCLayer* subMenuLayer;
    CCSprite* storyBoard;
    CCSprite* speaker;
    CCSprite* language;
    CCSprite* hint;
    
    CCSpriteFrameCache* imageCache;
    
    void setSprite(CCSprite* target, CCPoint position, int level, const char* name);
    
    void setAllAction();
    
    CCObject *target;
    SEL_CallFunc preSceneCallBackFunc;
    SEL_CallFunc nextSceneCallBackFunc;
    SEL_CallFuncND choseSceneClickedCallFunc;
    SEL_CallFuncND functionStatusCallBackFunc;
    SEL_CallFunc languageSelectCallBackFunc;
    
    CCSequence* bellNormalAction;
    CCSequence* bellClickedAction;
    
    void setBellAction();
    
    CCMenu* mainMenu;
    //子選單按鈕
    CCMenu* subMenu;
    
    void onBellClicked();
    
    bool isFunctionMenuOpening;
    
    void onBellPlayed();
    
    void setSpriteObjectAtSubMenuLayer(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name);
    
    CCSequence* subMenuMoveUp;
    CCSequence* subMenuGoDown;
    
    void setSubMenuAction();
    
    void onSubMenuMovedUp();
    
    void onSubMenuWentDown();
    
    bool isBellDoOpeningAction;
    
    void checkAndDoForSubMenu();
    
    CCMenu* functionCloseMenu;
    
    void onCloseButtonClicked();
    
    //語言選單
    CCLayer* languageMenuLayer;
    
    CCMenu* languageMenu;
    
    CCMenuItemSprite* languageItemTW;
    CCMenuItemSprite* languageItemCN;
    CCMenuItemSprite* languageItemEN;
    
    CCSprite* languageTW;
    CCSprite* languageCN;
    CCSprite* languageEN;
    
    CCSprite* highLight;
    
    void setSpriteObjectAtTargetLayer(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* contents);
    
    CCPoint languagePosition[4];
    
    static const int languageCount = 3;
    
    CCSprite* languageBackground[languageCount];
    
    void onLanguageClicked();
    
    CCSequence* languageShowAction[languageCount];
    
    CCSequence* languageHideAction[languageCount];
    
    void setLanguageMenuAction();
    
    bool isLanguageOpening;
    
    void onLanguageShoeDone();
    
    void onLanguageHideDone();
    
    bool isLanguageRunning;
    
    bool isSoundTurnOn;
    
    void setSoundSwitch(bool onOff);
    
    void onSoundClicked();
    
    bool checkIsCanDoAction();
    
    //分鏡表
    CCScrollView* selectSceneView;
    
    CCSprite* selectSceneBackground;
    
    CCLayer* selectSceneLayer;
    
    void setSelectScene();
    
    static const int sceneNumber = 7;
    
    void onSceneClicked();
    
    bool isSelectSceneOpening;
    
    void setAllFunctionMenu(bool enable);
    
    void setSelectViewObject(bool enable);
    
    void onCloseSceneClicked();
    
    CCLayer* sceneLayer;
    
    int scrollCount;
    
    int lastScrollCount;
    
    bool isScroll;
    
    void checkIsScrolling();
    
    float idleTime;
    
    void checkIsWaitingOverTimeLimit();
    
    void onSelectSceneClicked(cocos2d::CCObject *pSender);
    
    
    //排程
    static const float timeLimitOfWaiting = 5.0f;
    
    static const float perDuringTime = 0.5f;
    
    void functionSchedule();
    
    void checkIsFunctionMenuOpened();
    
    float functionMenuWaitingTime;
    
    //播音效
    int playSoundEffect(const char *soundName, bool isRepeat);
    
    int nowScene;
    
    //換景
    void onPreSceneClicked();
    
    void onNextSceneClicked();
    
    void onHintClicked();
    
    CCLayer* hintLayer;
    
    void createHintObjects();
    
    void closeHineLayer();
    
    void changeLanguage();
    
    CCSprite* backgroundWordsButton;
    
    CCMenuItemSprite* createLanguageButton(const char* frameName, int tag);
    
    void onLanguageTouched(TouchSprite* target);
    
    void setAllLanguageTouchCanTouchable(bool touchable);
    
    CCMenu* blockMenu;
    
    void nextSceneEvent();
    void preSceneEvent();
    void languageEvent();
    
};

#endif // __HELLOWORLD_FunctionMenu_H__
