#pragma once
#ifndef MANAGER_H
#define MANAGER_H

class Scene;

class Manager
{

public:
	static void Init();
	static void Uninit();
	static void Update(const float& dt);
	static void Draw();
	

	template<class T>
	inline static void SetScene() { m_NextScene = new T(); }

	static Scene* GetScene() { return m_NowScene; }

private:
	static Scene* m_NowScene;
	static Scene* m_NextScene;
};

#endif // MANAGER_H