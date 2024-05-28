#pragma once
#ifndef SPRITE_H
#define SPRITE_H


#include "component.h"
#include <string>

/// <summary>
/// 2Dスプライトを表示するコンポーネント
/// </summary>
class Sprite : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	/// <summary>
	/// スプライトカラーの設定
	/// </summary>
	/// <param name="color">描画時の色</param>
	void SetColor(const XMFLOAT4& color);

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="path">画像保存場所の相対パス</param>
	void LoadTexture(const std::wstring& path);

private:
	std::wstring _path{};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f ,1.0f };

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};

#endif // !SPRITE_H
