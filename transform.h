#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "main.h"
#include "XMFLOAT_Helper.h"

/// <summary>
/// オブジェクトの移動、回転、スケールを管理するコンポーネント
/// </summary>
class Transform :public Component
{
public:

	// ==========基本メンバ=========
	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scale{ 1.0f,1.0f,1.0f };
	// =============================


public:

	// 使うか未定
	void Update() override {
		

	};

	// ===方向ベクトルの取得===
	const XMFLOAT3 GetForward() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 foward;
		// 回転行列の3行目からZ軸方向ベクトルを取り出す
		XMStoreFloat3(&foward, rotationMatrix.r[2]);

		// 正規化しておく
		
		return VectorNormalize(foward);
	};
	const XMFLOAT3 GetBack() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 back;
		// 回転行列の3行目からZ軸方向ベクトルを取り出す
		XMStoreFloat3(&back, rotationMatrix.r[2]);

		
		return -VectorNormalize(back);
	};
	const XMFLOAT3 GetRight() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 right;
		// 回転行列の0行目からX軸方向ベクトルを取り出す
		XMStoreFloat3(&right, rotationMatrix.r[0]);

		

		return VectorNormalize(right);
	};
	const XMFLOAT3 GetLeft() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 left;
		// 回転行列の0行目からX軸方向ベクトルを取り出す
		XMStoreFloat3(&left, rotationMatrix.r[0]);

		

		return -VectorNormalize(left);
	};
	const XMFLOAT3 GetTop() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 top;
		// 回転行列の2行目からY軸方向ベクトルを取り出す
		XMStoreFloat3(&top, rotationMatrix.r[1]);

		

		return 
	};
	const XMFLOAT3 GetUnder() const {
		// オイラー角から回転行列を生成
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 under;
		// 回転行列の2行目からY軸方向ベクトルを取り出す
		XMStoreFloat3(&under, rotationMatrix.r[1]);

		

		return -VectorNormalize(under);
	};
	// ========================

	
	auto operator <=>(const Transform&) const = default;
};

#endif // !TRANSFORM_H