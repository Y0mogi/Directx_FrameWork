#pragma once

/// <summary>
/// �p�[�e�B�N���𔭐�������N���X
/// </summary>
class ParticleEmitter {
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


private:
	std::list<ParticleBase*> m_Particles;
};