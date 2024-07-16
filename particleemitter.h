#pragma once
#include "component.h"

/// <summary>
/// パーティクルを発生させるクラス
/// </summary>
class ParticleEmitter : public Component{
public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()	override;
private:
	static const unsigned short PARTICLE_MAX = 1000;			// パーティクル最大数
	std::array<class Particle_Base*, PARTICLE_MAX> m_Particles;	// パーティクル

	ID3D11Buffer*				m_VertexBuffer{};	// 頂点バッファ
	ID3D11ShaderResourceView*	m_Texture{};		// テクスチャリソース
	ID3D11VertexShader*			m_VertexShader{};	// 頂点シェーダー
	ID3D11PixelShader*			m_PixelShader{};	// ピクセルシェーダー
	ID3D11InputLayout*			m_VertexLayout{};	// 頂点レイアウト
};