//
//  ShareData.h
//  RedHat
//
//  Created by Mac-8 on 13/7/2.
//
//

#ifndef __RedHat__ShareData__
#define __RedHat__ShareData__

#include <iostream>
#include "cocos2d.h"

#define BEST_TIME   "BestTime%d"
#define NINETH_BEST_SCORE   "NinethBestData"
#define ELEVENTH_BEST_SCORE   "EleventhBestData"
#define FOURTH_BEST_SCORE   "FourthBestData"

enum GameLanguageType
{
    TRADITIONAL_CHINESE = 0,
    SIMPLE_CHINESE,
    ENGLISH,
};

class ShareData
{
public:
    // 加入一個member function用來取得獨身
    static ShareData* getInstance();
    // 這個function必須是static function
    // 如此才可以不宣告物件實體取用這個函式
    
    void init();
    
    void setLanguageOfScene(GameLanguageType languageType);
    
    GameLanguageType getLanguageOfScene();
    
    void setPictureName(const char* name);
    
    const char* getPictureName();
    
    void setPictureNameTs(const char* name);
    
    const char* getPictureNameTs();
    
    void showTime();
    
    void setPhoneticAlphabet(bool isHasPhonetic);
    
    bool getPhoneticAlphabet();
    
private:
    
    GameLanguageType languageSetting;
    
    std::string pictureName;
    std::string pictureNameTs;
    
    bool isHasPhonetic;
    
};

#endif /* defined(__RedHat__ShareData__) */
