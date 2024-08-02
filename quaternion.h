#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include "main.h"

/// <summary>
/// クオータニオン
/// </summary>
class Quaternion
{
public:
    // === クォータニオンの生成 === 
    
    /// <summary>
    /// 指定された軸を中心に指定された角度で回転するクォータニオンを生成
    /// </summary>
    /// <param name="angle">回転角度 (ラジアン)</param>
    /// <param name="axis">回転軸の方向を示すベクトル</param>
    /// <returns>生成されたクォータニオン</returns>
    static Quaternion AngleAxis(float angle, const XMFLOAT3& axis);

    /// <summary>
    /// オイラー角 (ピッチ、ヨー、ロール) からクォータニオンを生成
    /// </summary>
    /// <param name="pitch">ピッチ角 (X軸回りの回転)</param>
    /// <param name="yaw">ヨー角 (Y軸回りの回転)</param>
    /// <param name="roll">ロール角 (Z軸回りの回転)</param>
    /// <returns>生成されたクォータニオン</returns>
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll);

    /// <summary>
    /// 2つのクォータニオン間を球面線形補間して新しいクォータニオンを生成
    /// </summary>
    /// <param name="q1">始点のクォータニオン</param>
    /// <param name="q2">終点のクォータニオン</param>
    /// <param name="t">補間係数</param>
    /// <returns>生成されたクォータニオン</returns>
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float dt);

    /// <summary>
    /// 2つのクォータニオン間を線形補間して新しいクォータニオンを生成
    /// </summary>
    /// <param name="q1">始点のクォータニオン</param>
    /// <param name="q2">終点のクォータニオン</param>
    /// <param name="t">補間係数</param>
    /// <returns>生成されたクォータニオン</returns>
    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float dt);

    // ===================

    // コンストラクタ
    Quaternion() : _quaternion(XMQuaternionIdentity()) {}
    Quaternion(float x, float y, float z, float w) : _quaternion(XMVectorSet(x, y, z, w)) {}
    Quaternion(const XMVECTOR& quaternion) : _quaternion(quaternion) {}
    Quaternion(const XMFLOAT4& quaternion) : _quaternion(XMLoadFloat4(&quaternion)) {}


    // クォータニオンの逆
    Quaternion Inverse() const {
        return Quaternion(XMQuaternionInverse(_quaternion));
    }

    // クォータニオンの正規化
    Quaternion Normalize() const {
        return Quaternion(XMQuaternionNormalize(_quaternion));
    }

    // 回転行列への変換
    XMMATRIX ToRotationMatrix() const {
        return XMMatrixRotationQuaternion(_quaternion);
    }

    // XMVECTORを取得
    XMVECTOR ToXMVector() const {
        return _quaternion;
    }

    // XMVECTORをFLOAT4に変換して取得
    XMFLOAT4 ToXMFloat4() const {
        return ToXMFLOAT4(_quaternion);
    }



    // 演算子オーバーロード
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(XMQuaternionMultiply(_quaternion, other._quaternion));
    }

    /// <summary>
    /// CompInfo用
    /// </summary>
    XMVECTOR& GetRotation() {
        return _quaternion;
    }

private:
    XMVECTOR _quaternion;
};

inline Quaternion Quaternion::AngleAxis(float angle, const XMFLOAT3& axis)
{
    XMVECTOR axisVector = ToXMVECTOR(axis); // float3をvector変換
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
