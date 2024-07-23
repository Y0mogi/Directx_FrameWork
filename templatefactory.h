//=============================================================================
// Contents   : �w�肵���N���X�̃C���X�^���X�𐶐�����N���X
//              
// Author     : ���� ����
// LastUpdate : 2024/07/22
// Since      : 2024/07/22
//
// === �X�V���O ===
//
// 
// 
//
//=============================================================================
#pragma once
#include <memory>

/// <summary>
/// �w�肵���N���X�̃C���X�^���X�𐶐�
/// </summary>
/// <typeparam name="T">�����������N���X</typeparam>
template<typename T>
class TemplateFactory
{
public:
    /// <summary>
    /// �w�肵�������ŃN���X�̃��j�[�N�|�C���^�C���X�^���X�𐶐�
    /// </summary>
    /// <typeparam name="Args">�C���X�^���X�̃R���X�g���N�^�ɓn�������̌^���X�g</typeparam>
    /// <param name="args">�C���X�^���X�����Ɏg�p�������</param>
    /// <returns>�w�肵���N���X�̃��j�[�N�|�C���^</returns>
    template<typename... Args>
    static std::unique_ptr<T> CreateInstance(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /// <summary>
    /// �w�肵�������ŃN���X�̋��L�|�C���^�C���X�^���X�𐶐�
    /// </summary>
    /// <typeparam name="Args">�C���X�^���X�̃R���X�g���N�^�ɓn�������̌^���X�g</typeparam>
    /// <param name="args">�C���X�^���X�����Ɏg�p�������</param>
    /// <returns>�w�肵���N���X�̋��L�|�C���^</returns>
    template<typename... Args>
    static std::shared_ptr<T> CreateInstance(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

};

