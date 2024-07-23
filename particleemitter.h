#pragma once
#include "component.h"

class Particle_Base;

/// <summary>
/// �p�[�e�B�N���𔭐�������N���X
/// </summary>
class ParticleEmitter : public Component {
public:
	// �t�@�N�g�����\�b�h
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
	static const unsigned short PARTICLE_MAX = 1000;						// �p�[�e�B�N���ő吔
	std::vector<std::unique_ptr<Particle_Base>> m_Particles;	// �p�[�e�B�N��

	ID3D11Buffer*				m_VertexBuffer{};	// ���_�o�b�t�@
	ID3D11ShaderResourceView*	m_Texture{};		// �e�N�X�`�����\�[�X
	ID3D11VertexShader*			m_VertexShader{};	// ���_�V�F�[�_�[
	ID3D11PixelShader*			m_PixelShader{};	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*			m_VertexLayout{};	// ���_���C�A�E�g
	std::string					m_Path{};
	int							m_Count{};
};

// �t�@�N�g�����\�b�h�̎���
template<class T>
ParticleEmitter* ParticleEmitter::Factory(const std::string& path,const int& count,const XMFLOAT4& color) {
	auto emitter = new ParticleEmitter(path, count);
	emitter->m_Particles.resize(PARTICLE_MAX);
	for (auto& it : emitter->m_Particles) {
		it = std::make_unique<T>(emitter,color);
	}
	return emitter;
}