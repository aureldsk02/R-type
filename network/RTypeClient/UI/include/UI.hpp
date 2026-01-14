#ifndef UI_H
    #define UI_H
    #include "./librairies.hpp"
    #include "../components/include/UIElement.hpp"

class UI {
    public:
        std::string id;
        std::vector<std::shared_ptr<UIElement>> elements;

        UI(const std::string& id);
        void addElement(std::shared_ptr<UIElement> el);
        std::shared_ptr<UIElement> getElementById(const std::string& elementId);
        const std::string getId();
};

#endif