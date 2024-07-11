//=============================================================================
// Contents   : AABB�ł̏������s�������蔻��R���|�[�l���g
//              
// Author     : ���� ����
// LastUpdate : 2024/06/26
// Since      : 2024/06/01
//
// === �X�V���O ===
// 
// 6 / 26 �ڍׂȃR�����g�̒ǉ�
//
//
//=============================================================================
#pragma once

#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H




#include "component.h"
#include "collision_base.h"

/// <summary>
/// �{�b�N�X�R���W�����R���|�[�l���g
/// </summary>
class BoxCollision : public Component, public Collision_Base
{
public:
    // ������
    void Init()     override; 

    // �X�V
    void Update()   override; 

    // �R���|�[�l���g�̏��\��
    void CompInfo() override; 

    /// <summary>
    /// ���g�Ƃ̓����蔻��̌�������
    /// </summary>
    /// <param name="other">���肵���������蔻��̃|�C���^</param>
    /// <returns>True:�������Ă��� False:�������Ă��Ȃ�</returns>
    bool Intersects(Collision_Base* other) const override;

    /// <summary>
    /// ���g�Ƃ̓����蔻��̂̓����
    /// </summary>
    /// <param name="other">���肵���������蔻��̃|�C���^</param>
    /// <returns>True:����Ă��� False:����Ă��Ȃ�</returns>
    bool Contains(Collision_Base* other) const override;
    
    // gettter

    BoundingBox& GetCollision() { return m_Collision; }

    // �R���X�g���N�^�E�f�X�g���N�^

    inline BoxCollision() { SetCollisionType(CollisionType::AABB); }
    inline ~BoxCollision() override {}

    

private:
    BoundingBox m_Collision;
    Transform*  m_Transform{};
};

#endif // BOXCOLLISION