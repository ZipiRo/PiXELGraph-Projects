class Scene
{
private:
    friend class PiXELGraph;
    friend class SceneManager;
    
    virtual void Start() {}
    virtual void Event() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void Quit() {} 

public:
    int buildIndex;
};

class SceneManager
{
private:
    Scene *currentScene;
    std::vector<Scene *> scenes;

    SceneManager();

public:
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    static SceneManager &GetInstance();

    template <typename T>
    static void AddScene();
    
    static void LoadScene(int buildIndex);
    static Scene* GetScene();
};