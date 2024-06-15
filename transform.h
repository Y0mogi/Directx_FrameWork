#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "quaternion.h"
#include "main.h"


/// <summary>
/// オブジェクトの移動、回転、スケールを管理するコンポーネント
/// </summary>
class Transform :public Component
{
public:

	// ==========基本メンバ=========
	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
    Quaternion rotation;
	XMFLOAT3 scale{ 1.0f,1.0f,1.0f };
    
	// =============================	

public:

	// 使うか未定
	void Update() override {
      
	};

    
    void CompInfo()override {
        using namespace ImGui;
        SeparatorText("Transform");
        
        DragFloat3("Position", reinterpret_cast<float*>(&position));

        Text("Rotation is Can only display (2024/06/14)");
        XMFLOAT4 rotF4 = rotation.ToXMFloat4();
        DragFloat3("Rotation", reinterpret_cast<float*>(&rotF4));
        //rotation.FromEulerAngles(rotF4.x, rotF4.y, rotF4.z);

        DragFloat3("Scale", reinterpret_cast<float*>(&scale));
        
        Separator();
    }

	// ===方向ベクトルの取得===
   
    XMFLOAT3 GetForward() const {
            return GetDirectionVector(2);
        }
    XMFLOAT3 GetBack() const {
            XMFLOAT3 back = GetDirectionVector(2);
            return XMFLOAT3(-back.x, -back.y, -back.z);
        }
    XMFLOAT3 GetRight() const {
            return GetDirectionVector(0);
        }
    XMFLOAT3 GetLeft() const {
            XMFLOAT3 left = GetDirectionVector(0);
            return XMFLOAT3(-left.x, -left.y, -left.z);
        }
    XMFLOAT3 GetTop() const {
            return GetDirectionVector(1);
        }
    XMFLOAT3 GetUnder() const {
            XMFLOAT3 under = GetDirectionVector(1);
            return XMFLOAT3(-under.x, -under.y, -under.z);
        }
    
	// ========================


    // === 回転 ===
    
    // オイラー角での回転の設定
    void SetEulerAngles(const XMFLOAT3& angle) {
        rotation = Quaternion::FromEulerAngles(angle.x, angle.y, angle.z);
    }
    void SetEulerAngles(float pitch, float yaw, float roll) {
        rotation = Quaternion::FromEulerAngles(pitch, yaw, roll);
    }
    
    /// <summary>
    /// オイラー角での回転を取得します。
    /// </summary>
    XMFLOAT3 GetEulerAngles() const {
        return ToXMFLOAT3(XMQuaternionRotationMatrix(rotation.ToRotationMatrix()));
    }

    // 自転
    void Rotate(const XMFLOAT3& eulerAngles = {}) {
        Quaternion deltaRotation = Quaternion::FromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
        rotation = rotation * deltaRotation;
    }
    void Rotate(float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f) {
        Quaternion deltaRotation = Quaternion::FromEulerAngles(pitch, yaw, roll);
        rotation = rotation * deltaRotation;
    }

    /// 公転
    void RotateAround(const XMFLOAT3& point = {}, const XMFLOAT3& axis = {}, float angle ={}) {
        // 平行移動
        XMVECTOR pointVector = XMLoadFloat3(&point);
        XMVECTOR posVector = XMLoadFloat3(&position);
        posVector -= pointVector;

        // 回転
        Quaternion deltaRotation = Quaternion::AngleAxis(angle, axis);
        posVector = XMVector3Rotate(posVector, deltaRotation.ToXMVector());

        // 平行移動の復元
        posVector += pointVector;
        XMStoreFloat3(&position, posVector);

        // 回転の適用
        rotation = deltaRotation * rotation;
    }
    // ========================

	auto operator <=>(const Transform&) const = default;

private:

	// 回転行列の行を取得して正規化するヘルパー関数
	XMFLOAT3 GetDirectionVector(int row) const {
        XMMATRIX rotationMatrix = rotation.ToRotationMatrix();
        XMFLOAT3 dir;
        XMStoreFloat3(&dir, rotationMatrix.r[row]);
        return VectorNormalize(dir);
	}
};

#endif // !TRANSFORM_H