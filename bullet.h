#pragma once
#include <string>
#include "component.h"

class Bullet :public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

	//void CompInfo() override;

	Bullet(const XMFLOAT4 color, std::string path)
		:m_Color(color), m_Path(path){};

private:
	/// <summary>
	/// <para>テクスチャの読み込み</para>
	/// テクスチャの再度生成なので毎フレームの呼び出しは禁止
	/// </summary>
	void CreateTexture(const std::string& path);

	/// <summary>
	/// tmpPathへのパスの保存
	/// </summary>
	bool SaveFilePath(const std::string& path);
private:
	std::string					m_Path{}; // メインパス
	XMFLOAT4					m_Color = { 1.0f,1.0f,1.0f ,1.0f };

	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11VertexShader*			m_VertexShader{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11InputLayout*			m_VertexLayout{};
};

