#ifndef __HELLOWORLD_ActorsLines_H__
#define __HELLOWORLD_ActorsLines_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "constants.h"
#include "ShareData.h"

#include "ShareMethod.h"

USING_NS_CC;

#define OS "%sHB0026OS0%d01c.mp3"

#define WORDS_PICTURE "p_Read000%d%s.png"
#define PHONETIC_ALPHABET_PICTURE "p_read010001.png"
#define NO_PHONETIC_ALPHABET_PICTURE "p_read010002.png"
#define CLOSE_BUTTON "p_read02.png"

#define WORDS_WITH_PHONETIC_PICTURE "p_Read000%dTs.png"

enum ActorsLinesNumber
{
    AL_WORDS_NUMBER = 99000,
    AL_CLOSE_BUTTON_NUMBER = 88000,
    AL_PHONETIC_ALPHABET_NUMBER = 77000,
    AL_WORDS_BACKGROUND_BUTTON_NUMBER = 66000,
    AL_FULL_SCREEN_BUTTON_NUMBER = 55000,
};

class ActorsLines : public cocos2d::CCLayer
{
public:
    ActorsLines();
    ~ActorsLines();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ActorsLines);
    
    void setInitialItmes(int chapterIndex, CCObject *target, SEL_CallFunc backgroundClickedCallBack, SEL_CallFunc OpeningClickedCallBack);
    
    void playWordsMoveDown();
    
    void releaseObject();
    
    void changeLanguage();
    
    const char* languageNameG;
    
protected:
    
    void setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content);
    
    CCSpriteFrameCache* imageCache;
    
    static const int ChapterCount = 6;
    
//    CCLayer* wordsLayer[ChapterCount];
    
    CCLayer* wordsLayer;
    
    CCPoint layerAtTopPosition[ChapterCount];
    
    CCObject *target;
    SEL_CallFunc OpeningClickedCallBack;
    SEL_CallFunc backgroundClickedCallBack;
    
    CCPoint phoneticAlphabetPosition[ChapterCount];
    CCPoint closePosition;
    
    void onPhoneticAlphabetClicked(CCObject *pSender);
    
    void onCloseClicked();
    
    void onWordsBackgroundClicked();
    
    CCSequence* wordsMoveUpAction[ChapterCount];
    CCSequence* wordsMoveDownAction;
    
    void setWordsAction();
    
    void wordsMovedUp();
    void wordsMoveDown();
    
    int chapterIndex;
    
    bool isClosing;
    
    void playOS();
    
    int playSoundEffect(const char *soundName, bool isRepeat);
    
    int nowPlayingOSId;
    
    void stopOS();
    
    int languageTypeWord;
    
    bool isPlayingOS;
    
    bool isWordsClosing;
    
    void whenOSPlayCompleted();
    
    void setBackgroundMenuVisible(bool visible);
    
    void onBackgroundClicked();
    
    CCMenu* backgroundMenu;
    
    bool isBackgroundClicked;
    
    CCRect wordsBackgroundZone[ChapterCount];
    CCPoint wordsBackgroundPosition[ChapterCount];

    bool isActionRunning;
    
    void setWord(int chapterIndex);
    
    CCPoint wordsPosition;
    
    CCSprite* word;
    
    const char* getLanguageName(bool isTwoType);
    
    CCSprite* phoneticAlphabetButton;
    CCMenuItemSprite* phoneticAlphabetSpriteItem;
    
    bool isShowPhonetic;
    
    void showPhoneticOrNot();
    
};

#endif // __HELLOWORLD_ActorsLines_H__
