#ifndef UIMANAGER
    #define UIMANAGER
    #include "../UI/include/librairies.hpp"
    #include "../UI/include/UI.hpp"

class UIManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<UI>> uis;
        std::shared_ptr<UI> currentUI;
        std::shared_ptr<UI> nextUI;
        float transitionAlpha = 1.f;
        bool transitioning = false;

        UIManager() = default;
    public:
        UIManager(const UIManager&) = delete;
        UIManager& operator=(const UIManager&) = delete;

        static UIManager& getInstance() {
            static UIManager instance;
            return instance;
        }
        void addUI(std::shared_ptr<UI> ui) { uis[ui->id] = ui; }
        
        void setUI(const std::string& id) {
            if (uis.find(id) != uis.end()) {
                nextUI = uis[id];
                transitioning = true;
                transitionAlpha = 0.f;
            }
        }
    
        std::shared_ptr<UI> getCurrentUI() { return currentUI; }
        std::shared_ptr<UI> getNextUI() { return nextUI; }
        bool isTransitioning() const { return transitioning; }
    
        void updateTransition(float deltaTime) {
            if (transitioning) {
                transitionAlpha += deltaTime * 2.f;
                if (transitionAlpha >= 1.f) {
                    transitionAlpha = 1.f;
                    currentUI = nextUI;
                    nextUI = nullptr;
                    transitioning = false;
                }
            }
            return;
        }
    
        float getAlpha() const { return transitionAlpha; }

        std::shared_ptr<UI> getUI(const std::string &id) {
            for (auto &ui : uis) {
                if (ui.first == id)
                    return ui.second;
            }
            return nullptr;
        }
};
#endif