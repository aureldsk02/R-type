#include "../include/UI.hpp"
#include "../../Managers/UIManager.hpp"

UI::UI(const std::string& id) : id(id) {}
void UI::addElement(std::shared_ptr<UIElement> el) { elements.push_back(el); }
std::shared_ptr<UIElement> UI::getElementById(const std::string& elementId) {
    for (auto& el : elements) {
        if (el->id == elementId)
            return el;
    }
    return nullptr;
}
const std::string UI::getId() {return id;}
