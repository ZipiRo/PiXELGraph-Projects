#include "Core/SceneManager.h"

SceneManager::SceneManager()
{ currentScene = nullptr; }

SceneManager &SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

template <typename T>
void SceneManager::AddScene()
{
    auto &instance = GetInstance();

    auto &scene = T();
    instance.scenes.push_back(scene);
} 

void SceneManager::LoadScene(int buildIndex)
{
    auto &instance = GetInstance();

    if (buildIndex < 0 || buildIndex >= instance.scenes.size())
        return;

    instance.currentScene = &instance.scenes[buildIndex];
    SceneManager::GetScene()->Start();
}

Scene *SceneManager::GetScene()
{
    auto &instance = GetInstance();
    return instance.currentScene;
}