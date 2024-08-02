#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include "main.h"

/// <summary>
/// �N�I�[�^�j�I��
/// </summary>
class Quaternion
{
public:
    // === �N�H�[�^�j�I���̐��� === 
    
    /// <summary>
    /// �w�肳�ꂽ���𒆐S�Ɏw�肳�ꂽ�p�x�ŉ�]����N�H�[�^�j�I���𐶐�
    /// </summary>
    /// <param name="angle">��]�p�x (���W�A��)</param>
    /// <param name="axis">��]���̕����������x�N�g��</param>
    /// <returns>�������ꂽ�N�H�[�^�j�I��</returns>
    static Quaternion AngleAxis(float angle, const XMFLOAT3& axis);

    /// <summary>
    /// �I�C���[�p (�s�b�`�A���[�A���[��) ����N�H�[�^�j�I���𐶐�
    /// </summary>
    /// <param name="pitch">�s�b�`�p (X�����̉�])</param>
    /// <param name="yaw">���[�p (Y�����̉�])</param>
    /// <param name="roll">���[���p (Z�����̉�])</param>
    /// <returns>�������ꂽ�N�H�[�^�j�I��</returns>
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll);

    /// <summary>
    /// 2�̃N�H�[�^�j�I���Ԃ����ʐ��`��Ԃ��ĐV�����N�H�[�^�j�I���𐶐�
    /// </summary>
    /// <param name="q1">�n�_�̃N�H�[�^�j�I��</param>
    /// <param name="q2">�I�_�̃N�H�[�^�j�I��</param>
    /// <param name="t">��ԌW��</param>
    /// <returns>�������ꂽ�N�H�[�^�j�I��</returns>
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float dt);

    /// <summary>
    /// 2�̃N�H�[�^�j�I���Ԃ���`��Ԃ��ĐV�����N�H�[�^�j�I���𐶐�
    /// </summary>
    /// <param name="q1">�n�_�̃N�H�[�^�j�I��</param>
    /// <param name="q2">�I�_�̃N�H�[�^�j�I��</param>
    /// <param name="t">��ԌW��</param>
    /// <returns>�������ꂽ�N�H�[�^�j�I��</returns>
    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float dt);

    // ===================

    // �R���X�g���N�^
    Quaternion() : _quaternion(XMQuaternionIdentity()) {}
    Quaternion(float x, float y, float z, float w) : _quaternion(XMVectorSet(x, y, z, w)) {}
    Quaternion(const XMVECTOR& quaternion) : _quaternion(quaternion) {}
    Quaternion(const XMFLOAT4& quaternion) : _quaternion(XMLoadFloat4(&quaternion)) {}


    // �N�H�[�^�j�I���̋t
    Quaternion Inverse() const {
        return Quaternion(XMQuaternionInverse(_quaternion));
    }

    // �N�H�[�^�j�I���̐��K��
    Quaternion Normalize() const {
        return Quaternion(XMQuaternionNormalize(_quaternion));
    }

    // ��]�s��ւ̕ϊ�
    XMMATRIX ToRotationMatrix() const {
        return XMMatrixRotationQuaternion(_quaternion);
    }

    // XMVECTOR���擾
    XMVECTOR ToXMVector() const {
        return _quaternion;
    }

    // XMVECTOR��FLOAT4�ɕϊ����Ď擾
    XMFLOAT4 ToXMFloat4() const {
        return ToXMFLOAT4(_quaternion);
    }



    // ���Z�q�I�[�o�[���[�h
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(XMQuaternionMultiply(_quaternion, other._quaternion));
    }

    /// <summary>
    /// CompInfo�p
    /// </summary>
    XMVECTOR& GetRotation() {
        return _quaternion;
    }

private:
    XMVECTOR _quaternion;
};

inline Quaternion Quaternion::AngleAxis(float angle, const XMFLOAT3& axis)
{
    XMVECTOR axisVector = ToXMVECTOR(axis); // float3��vector�ϊ�
    XMVECTOR quaternion = XMQuaternionRotationAxis(axisVector, angle);
    return Quaternion(quaternion);
}

inline Quaternion Quaternion::FromEulerAngles(float pitch, float yaw, float roll)
{
    XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
    return Quaternion(quaternion);
}

inline Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    XMVECTOR result = XMQuaternionSlerp(q1._quaternion, q2._quaternion, t);
    return Quaternion(result);
}

inline Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    XMVECTOR result = XMVectorLerp(q1._quaternion, q2._quaternion, t);
    return Quaternion(XMQuaternionNormalize(result));
}

#endif // QUATERNION_H
