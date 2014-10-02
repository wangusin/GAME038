//
//  ShareMethod.cpp
//  RedHat
//
//  Created by Mac-8 on 13/7/2.
//
//

#include "ShareMethod.h"

using namespace cocos2d;

static ShareMethod* _instance = NULL;

ShareMethod* ShareMethod::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new ShareMethod();
        
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

void ShareMethod::init()
{
    centralPoint = CCPoint(752, 448);
}

CCPoint ShareMethod::getOffsetOfCenter(cocos2d::CCPoint position)
{
    CCPoint offset = ccpSub(position, centralPoint);
    
    return offset;
}

void ShareMethod::createMenuWithNoPictrue(cocos2d::CCMenu*& menu, cocos2d::CCPoint position, int level, cocos2d::CCNode *carrier, CCRect checkZoneSize, SEL_MenuHandler selector, CCNode* target)
{

    createMenuWithNoPictrueWithTag(menu, position, level, carrier, checkZoneSize, selector, target, 0);
    
}

void ShareMethod::createMenuWithNoPictrueWithTag(cocos2d::CCMenu *&menu, cocos2d::CCPoint position, int level, cocos2d::CCNode *carrier, cocos2d::CCRect checkZoneSize, SEL_MenuHandler selector, cocos2d::CCNode *target, int tag)
{
    CCSprite *checkZoneSpr = CCSprite::create();
    checkZoneSpr->setTextureRect(checkZoneSize);
    checkZoneSpr->setOpacity(0);
    CCMenuItemSprite *checkZoneItem = CCMenuItemSprite::create(checkZoneSpr, NULL);
    checkZoneItem->setAnchorPoint(ccp(0, 0));
    menu = CCMenu::createWithItem(checkZoneItem);
    menu->setPosition(position);
    carrier->addChild(menu,level);
    checkZoneItem->setTarget(target, selector);
    checkZoneItem->setTag(tag);
}

void ShareMethod::actionSettingWithBeginNumber(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber, CCSpriteFrameCache* imageCache)
{
    CCAnimation* targetAnimation = CCAnimation::create();
    for (int i = beginNumber ; i < count + beginNumber; i++) {
        const char* tempString = CCString::createWithFormat(name, i)->getCString();
        
        if (!imageCache->spriteFrameByName(tempString))
        {
            CCLog("missing tempString = %s", tempString);
        }
        
        targetAnimation->addSpriteFrame(imageCache->spriteFrameByName(tempString));
    }
    
    targetAnimation->setDelayPerUnit(delayPerUnit);
    
    targetAction = (CCRepeat::create(CCAnimate::create(targetAnimation), repeatCount));
    
}

void ShareMethod::actionSettingWithBeginNumberDecrement(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber, CCSpriteFrameCache* imageCache)
{
    CCAnimation* targetAnimation = CCAnimation::create();
    for (int i = beginNumber ; i > beginNumber - count; i--) {
        const char* tempString = CCString::createWithFormat(name, i)->getCString();
        
        if (!imageCache->spriteFrameByName(tempString))
        {
            CCLog("missing tempString = %s", tempString);
        }
        
        targetAnimation->addSpriteFrame(imageCache->spriteFrameByName(tempString));
    }
    
    targetAnimation->setDelayPerUnit(delayPerUnit);
    
    targetAction = (CCRepeat::create(CCAnimate::create(targetAnimation), repeatCount));
    
}

void ShareMethod::createMenuWithPicture(cocos2d::CCMenu *&menu, cocos2d::CCPoint position, int level, cocos2d::CCNode *carrier, const char* imageName, CCSpriteFrameCache* imageCache, SEL_MenuHandler selector, cocos2d::CCNode *target, int tag)
{
    CCSprite *checkZoneSpr = CCSprite::createWithSpriteFrameName(imageName);
    CCMenuItemSprite *checkZoneItem = CCMenuItemSprite::create(checkZoneSpr, NULL);
    checkZoneItem->setAnchorPoint(ccp(0, 0));
    menu = CCMenu::createWithItem(checkZoneItem);
    menu->setPosition(position);
    carrier->addChild(menu,level);
    checkZoneItem->setTarget(target, selector);
    checkZoneItem->setTag(tag);
}

void ShareMethod::createMenuWithPictureAddSelectImage(cocos2d::CCMenu *&menu, cocos2d::CCPoint position, int level, cocos2d::CCNode *carrier, const char* normalImageName, const char* selectImageName, CCSpriteFrameCache* imageCache, SEL_MenuHandler selector, cocos2d::CCNode *target, int tag)
{
    CCSprite *checkZoneSpr = CCSprite::createWithSpriteFrameName(normalImageName);
    CCSprite *selectSpr = CCSprite::createWithSpriteFrameName(selectImageName);
    CCMenuItemSprite *checkZoneItem = CCMenuItemSprite::create(checkZoneSpr, selectSpr);
    checkZoneItem->setAnchorPoint(ccp(0.5, 0.5));
    menu = CCMenu::createWithItem(checkZoneItem);
    menu->setPosition(position);
    carrier->addChild(menu,level);
    checkZoneItem->setTarget(target, selector);
    checkZoneItem->setTag(tag);
}

void ShareMethod::createMenuWithSprite(cocos2d::CCMenu *&menu, cocos2d::CCPoint position, int level, cocos2d::CCNode *carrier, CCSprite* button, SEL_MenuHandler selector, cocos2d::CCNode *target, int tag)
{
    CCMenuItemSprite *checkZoneItem = CCMenuItemSprite::create(button, NULL);
    checkZoneItem->setAnchorPoint(ccp(0, 0));
    menu = CCMenu::createWithItem(checkZoneItem);
    menu->setPosition(position);
    carrier->addChild(menu,level);
    checkZoneItem->setTarget(target, selector);
    checkZoneItem->setTag(tag);
}

void ShareMethod::getRandomAndNoRepeat(int *randomArray, int xCount, int yCount, int size)
{
    
    int quantity = xCount * yCount;
    int start = 0;
    int total = xCount * yCount;
    if (quantity > total) {
        CCLog("随机数错误");
    }
    int sequence[total];  //存放随机数的数组
    //int output[quantity]; //最终生成的不重复一系列随机数
    
    //将sequence 初始化
    for (int i = 0; i < total; i++) {
        sequence[i] = start + i;
    }
    
    //随机数种子
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(seed);
    
//    for (int i = 0; i < quantity; i++) {
//        int num = arc4random() % total;//在指定范围下产生随机数
//        randomArray[i] = sequence[num];//将产生的随机数存储
//        sequence[num] = sequence[total-1];//将最后个下标的值填充到随机产生的下标中
//        total--;//在指定范围 向前移
//    }
    
    for (int i = 0; i < size; i++) {
        int num = arc4random() % total;//在指定范围下产生随机数
        randomArray[i] = sequence[num];//将产生的随机数存储
        sequence[num] = sequence[total-1];//将最后个下标的值填充到随机产生的下标中
        total--;//在指定范围 向前移
    }
}

bool ShareMethod::isContainNumber(int targetNumber, int* array, int size)
{    
    for (int i = 0; i < size; i++)
    {
        if (targetNumber == array[i])
            return true;
    }
    
    return false;
}

bool ShareMethod::ccpIsEqual(cocos2d::CCPoint pos1, cocos2d::CCPoint pos2)
{
    //    CCLOG("pos1(%f, %f)    pos2(%f, %f)", pos1.x, pos1.y, pos2.x, pos2.y);
    
    if (pos1.x == pos2.x && pos1.y == pos2.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int ShareMethod::playLongSound(const char *soundName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int soundId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundName, false);
    
    return soundId;
    
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Android
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(minfo,
                                      CLASS_NAME,
                                      "playLongSound",
                                      "(Ljava/lang/String;)V"))
    {
        jstring StringArg1 = minfo.env->NewStringUTF(soundName);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
        minfo.env->DeleteLocalRef(StringArg1);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    
    return ANDROID_LONG_SOUND_ID;
#endif
}

void ShareMethod::stopLongSound(int soundId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(soundId);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(minfo,
                                      CLASS_NAME,
                                      "stopLongSound",
                                      "()V"))
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}

void ShareMethod::pauseLongSound(int soundId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Android
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(minfo,
                                      CLASS_NAME,
                                      "pauseLongSound",
                                      "()V"))
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    
#endif
}

void ShareMethod::resumeLongSound(int soundId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //Android
    JniMethodInfo minfo;
    if(JniHelper::getStaticMethodInfo(minfo,
                                      CLASS_NAME,
                                      "resumeLongSound",
                                      "()V"))
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    
#endif
}



