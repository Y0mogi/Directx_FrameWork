//=============================================================================
// Contents   : AABBでの処理を行う当たり判定コンポーネント
//              
// Author     : 髙橋 脩也
// LastUpdate : 2024/06/26
// Since      : 2024/06/01
//
// === 更新ログ ===
// 
// 6 / 26 詳細なコメントの追加
//
//
//=============================================================================
#pragma once

#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H




#include "component.h"
#include "collision_base.h"

/// <summary>
/// ボックスコリジョンコンポーネント
/// </summary>
class BoxCollision : public Component, public Collision_Base
{
public:
    // 初期化
    void Init()     override; 

    // 更新
    void Update()   override; 

    // コンポーネントの情報表示
    void CompInfo() override; 

    /// <summary>
    /// 自身との当たり判定の交差判定
    /// </summary>
    /// <param name="other">判定したい当たり判定のポインタ</param>
    /// <returns>True:交差している False:交差していない</returns>
    bool Intersects(Collision_Base* other) const override;

    /// <summary>
    /// 自身との当たり判定のの内包判定
    /// </summary>
    /// <param name="other">判定したい当たり判定のポインタ</param>
    /// <returns>True:内包している False:内包していない</returns>
    bool Contains(Collision_Base* other) const override;
    
    // gettter

    BoundingBox& GetCollision() { return m_Collision; }

    // コンストラクタ・デストラクタ

    inline BoxCollision() { SetCollisionType(CollisionType::AABB); }
    inline ~BoxCollision() override {}

    

private:
    BoundingBox m_Collision;
    Transform*  m_Transform{};
};

#endif // BOXCOLLISION