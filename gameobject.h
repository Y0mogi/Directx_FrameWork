#pragma once

enum class Layer;
enum class Tag;
class Component;

/// <summary>
/// GameObject�N���X
/// </summary>
class GameObject
{
public:

	void Init();	// ������
	void Update();	// �X�V
	void Uninit();	// �J��
	void Draw();	// �`��
	inline bool IsDiscard(){
		if (this->m_Discard) {
			this->Uninit();
			return true;
		}
		else {
			return false;
		}
	}   

	inline void Discard() { this->m_Discard = true; };	// �폜�\��
	void OnCollisionEnter(GameObject* Collision);	// �Փˏ���
	void CompInfo();								// �R���|�[�l���g�̏��\��

	/// <summary>
	/// �A�^�b�`����Ă���R���|�[�l���g�̎擾
	/// </summary>
	/// <typeparam name="T">�擾�������R���|�[�l���g�̌^</typeparam>
	/// <returns>�R���|�[�l���g�̃|�C���^</returns>
	template<class T>
	T* GetComponent(){
		for (auto& it : this->componentList) {
			T* tmp = dynamic_cast<T*>(it);
			if (tmp != nullptr)
				return tmp;
		}
		return nullptr;
	}

	/// <summary>
	/// �I�u�W�F�N�g�ɃR���|�[�l���g���A�^�b�`
	/// </summary>
	/// <typeparam name="T">�ǉ��������R���|�[�l���g�̌^</typeparam>
	/// <returns>�ǉ������R���|�[�l���g�̃|�C���^</returns>
	template<class T>
	T* AddComponent(){
		T* tmp = new T();
		tmp->Parent = this;
		this->componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// �I�u�W�F�N�g�ɕ����̃R���|�[�l���g���A�^�b�`
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<class T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* tmp = new T(std::forward<Args>(args)...);
		tmp->Parent = this;
		this->componentList.push_back(tmp);
		return tmp;
	}

	/// <summary>
	/// �����Ă���R���|�[�l���g���폜����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>�폜������������</returns>
	template<class T>
	bool RemoveComponent() {
		for (auto it = this->componentList.begin(); it != this->componentList.end(); ++it) {
			if (typeid(**it) == typeid(T)) {  // typeid �Ō^���r
				Component* tmp = *it;
				it = this->componentList.erase(it);  // ���X�g����v�f���폜���A�C�e���[�^���X�V
				delete tmp;  // �����������
				return true;  // ����
			}
		}
		return false;  // ������Ȃ������ꍇ
	}

	// getter

	std::string&	GetObjectName()	{ return this->m_ObjectName;	}
	Layer&			GetLayer()		{ return this->m_Layer;		}
	Tag&			GetTag()		{ return this->m_Tag;			}


	// �R���X�g���N�^�E�f�X�g���N�^

	GameObject();
	GameObject(class Scene* scene);
	GameObject(const std::string& name,Layer layer,Tag tag,class Scene* scene);
	~GameObject();


public:
	std::list<Component*> componentList{};	// ���g�ɂ��Ă���R���|�[�l���g
	class Scene* scene = nullptr;			// ���g���Ǘ����Ă���V�[���̃|�C���^

private:
	bool		m_Discard = false;			// �폜�\��
	Tag			m_Tag{};					// ���g�̃^�O
	Layer		m_Layer{};					// �\�����C���[
	std::string m_ObjectName{};				// �I�u�W�F�N�g�̌ŗL����
};

/// <summary>
/// �Q�[���I�u�W�F�N�g�̕\���D��x
/// </summary>
enum class Layer : int {
	Layer_0,		// 3D�I�u�W�F�N�g�Ȃ�
	Layer_1,		// 3D�I�u�W�F�N�g�Ȃ�
	Layer_2,		// UI�Ȃǂ�2D�X�v���C�g�̕`��Ɏg�p

	Layer_Num,
};

/// <summary>
/// GameObject�N���X�̃^�O
/// </summary>
enum class Tag {
	None,
	Ground,
	Player,
	Enemy,

	Tag_Num,
};
