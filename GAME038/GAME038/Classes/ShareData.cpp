//
//  ShareData.cpp
//  RedHat
//
//  Created by Mac-8 on 13/7/2.
//
//

#include "ShareData.h"

using namespace cocos2d;

static ShareData* _instance = NULL;

ShareData* ShareData::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new ShareData();
        
        if (_instance)
        {
            _instance->init();
        }
        else
        {
            CC_SAFE_DELETE(_instance);
            
            return NULL;
        }
    }
    return _instance;
};

void ShareData::init()
{
    pictureName = "";
    pictureNameTs = "";

    CCString* languageInit = CCString::create("");
    
    isHasPhonetic = false;
    //語系初始化
    //取得系統語言.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //ios
    const char* plistPath = "InfoPlist.strings";
    const char* fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("InfoPlist.strings", plistPath);
    CCDictionary* plistDic = CCDictionary::createWithContentsOfFile(fullPath);
    languageInit = dynamic_cast<CCString*>(plistDic->objectForKey("LanguageInit"));
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //android
    
    ccLanguageType languagType=CCApplication::sharedApplication()->getCurrentLanguage();
    switch (languagType)
    {
        case cocos2d::kLanguageChinese:
            languageInit = CCString::createWithFormat("1");
            break;
        case cocos2d::kLanguageEnglish:
            languageInit = CCString::createWithFormat("2");
            break;
        case cocos2d::kLanguageTraditional:
            languageInit = CCString::createWithFormat("0");
            break;
        default:
            languageInit = CCString::createWithFormat("2");
            break;
    }
#endif
    
    if (!languageInit->isEqual(CCString::create(""))) {
        languageSetting = (GameLanguageType)languageInit->intValue();
        
        CCLog("languageSetting = %d", languageSetting);
    }
    else
    {
    
        languageSetting = TRADITIONAL_CHINESE;
    }
    
}


void ShareData::setLanguageOfScene(GameLanguageType languageType)
{
    languageSetting = languageType;
}

GameLanguageType ShareData::getLanguageOfScene()
{
    CCUserDefault *dataRecord = CCUserDefault::sharedUserDefault();
    languageSetting = (GameLanguageType)dataRecord->getIntegerForKey("Record_Language");
    
    return languageSetting;
}

void ShareData::setPictureName(const char *name)
{
    pictureName = name;
}

const char* ShareData::getPictureName()
{
    return pictureName.c_str();
}

void ShareData::setPictureNameTs(const char *name)
{
    pictureNameTs = name;
}

const char* ShareData::getPictureNameTs()
{
    return pictureNameTs.c_str();
}

void ShareData::showTime()
{
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    
    CCLog("now time = %d", now.tv_sec);
    
    struct tm *tm;
    time_t timep = now.tv_sec;
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour=tm->tm_hour;
    int min=tm->tm_min;
    int second=tm->tm_sec;
    
    CCLOG("ShowTime : %d/%d/%d %d:%d:%d", year, month, day, hour, min, second);
}

void ShareData::setPhoneticAlphabet(bool isHasPhonetic)
{
    this->isHasPhonetic = isHasPhonetic;
}

bool ShareData::getPhoneticAlphabet()
{
    return isHasPhonetic;
}
