#pragma once
#include "component.h"

class Particle_Base;

/// <summary>
/// パーティクルを発生させるクラス
/// </summary>
class ParticleEmitter : public Component {
public:
	// ファクトリメソッド
	template<class T>
	static ParticleEmitter* Factory(const std::string& path, const int& count, const XMFLOAT4& color);

	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()	override;

	ParticleEmitter(const std::string& path,const int& count)
		:m_Path(path),m_Count(count) {};
	~ParticleEmitter() override {};
private:
	static const unsigned short PARTICLE_MAX = 1000;						// パーティクル最大数
	std::vector<std::unique_ptr<Particle_Base>> m_Particles;	// パーティクル

	ID3D11Buffer*				m_VertexBuffer{};	// 頂点バッファ
	ID3D11ShaderResourceView*	m_Texture{};		// テクスチャリソース
	ID3D11VertexShader*			m_VertexShader{};	// 頂点シェーダー
	ID3D11PixelShader*			m_PixelShader{};	// ピクセルシェーダー
	ID3D11InputLayout*			m_VertexLayout{};	// 頂点レイアウト
	std::string					m_Path{};
	int							m_Count{};
};

// ファクトリメソッドの実装
template<class T>
ParticleEmitter* ParticleEmitter::Factory(const std::string& path,const int& count,const XMFLOAT4& color) {
	auto emitter = new ParticleEmitter(path, count);
	emitter->m_Particles.resize(PARTICLE_MAX);
	for (auto& it : emitter->m_Particles) {
		it = std::make_unique<T>(emitter,color);
	}
	return emitter;
}