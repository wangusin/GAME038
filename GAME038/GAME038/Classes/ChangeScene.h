//
//  ChangeScene.h
//  RedHat
//
//  Created by Mac-8 on 13/7/29.
//
//

#ifndef __RedHat__ChangeScene__
#define __RedHat__ChangeScene__

#include <iostream>
#include "cocos2d.h"

#include "TemplateScene.h"
#include "StartScene.h"

class ChangeScene
{
public:
    // 加入一個member function用來取得獨身
    static ChangeScene* getInstance();
    // 這個function必須是static function
    // 如此才可以不宣告物件實體取用這個函式
    
    void init();
    
    void changeToScene(int targetScene);
    
    void changeToTemplateScene(int targetScene);
    
    void changeSceneWithNormal(int targetScene);
    
};

#endif /* defined(__RedHat__ChangeScene__) */
