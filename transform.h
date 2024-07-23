//=============================================================================
// Contents   : GameObjectの位置、回転、スケールを扱うクラス
//              
// Author     : 髙橋 脩也
// LastUpdate : 2024/06/16
// Since      : 2024/06/01
//
// === 更新ログ ===
//
// 6 / 16   QuaternionクラスをDirectXTKのQuaternionクラスに変更
// 7 / 20	クリアメソッドの追加
//
//=============================================================================

#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "main.h"

/// <summary>
/// オブジェクトの移動、回転、スケールを管理するコンポーネント
/// </summary>
class Transform :public Component
{
public:

	// ==========基本メンバ=========

	XMFLOAT3						position{ 0.0f,0.0f,0.0f };
	DirectX::SimpleMath::Quaternion rotation{};
	XMFLOAT3						scale{ 1.0f,1.0f,1.0f };
	
	// =============================	

public:

	void CompInfo()override {
		using namespace ImGui;
		SeparatorText("Transform");
		if (TreeNode("TMInfo")) {

			DragFloat3("Position", reinterpret_cast<float*>(&position));

			Text("Rotation is Can only display (2024/06/14)");
			
			// クォータニオンを回転行列に変換
			auto rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
			// 回転行列からオイラー角を取得
			float pitch, yaw, roll;
			ExtractPitchYawRoll(rotationMatrix, pitch, yaw, roll);
			XMFLOAT3 rotF3 = XMFLOAT3(pitch, yaw, roll);
			DragFloat3("Rotation", reinterpret_cast<float*>(&rotF3));

			DragFloat3("Scale", reinterpret_cast<float*>(&scale));

			TreePop();
		}
	}

	// ===ローカルの方向ベクトルの取得===
   
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

	/// <summary>
	/// Positionの初期化
	/// </summary>
	/// <param name="clear">初期化する値</param>
	inline void ClearPosition(const float& clear = 0.0f) {
		this->position.x = 0.0f;
		this->position.y = 0.0f;
		this->position.z = 0.0f;
		
	}

	/// <summary>
	/// Rotationの初期化
	/// </summary>
	/// <param name="clear">初期化する値</param>
	inline void ClearRotation(const float& clear = 0.0f) {
		this->rotation.x = 0.0f;
		this->rotation.y = 0.0f;
		this->rotation.z = 0.0f;
	}

	/// <summary>
	/// Scaleの初期化
	/// </summary>
	/// <param name="clear">初期化する値</param>
	inline void ClearScale(const float& clear = 0.0f) {
		this->scale.x = 0.0f;
		this->scale.y = 0.0f;
		this->scale.z = 0.0f;
	}







	auto operator <=>(const Transform&) const = default;

private:

	// 回転行列の行を取得して正規化するヘルパー関数
	XMFLOAT3 GetDirectionVector(int row) const {
		XMMATRIX rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
		XMFLOAT3 dir;
		XMStoreFloat3(&dir, rotationMatrix.r[row]);
		return VectorNormalize(dir);
	}

	// 回転行列からオイラー角を抽出するヘルパー関数
	void ExtractPitchYawRoll(const DirectX::SimpleMath::Matrix& matrix, float& pitch, float& yaw, float& roll) const {
		yaw = atan2f(matrix._23, matrix._33);
		pitch = -asinf(matrix._13);
		roll = atan2f(matrix._12, matrix._11);
	}
};

#endif // !TRANSFORM_H