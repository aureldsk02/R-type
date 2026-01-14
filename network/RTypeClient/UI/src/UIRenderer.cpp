#include "../include/UIRenderer.hpp"
#include "../include/utils.hpp"
#include "../../Managers/UIManager.hpp"

void UIRenderer::render(ClientUI &clientUI) {
    auto& uiManager = UIManager::getInstance();
    uiManager.updateTransition(clientUI.deltaTime);

    auto current = uiManager.getCurrentUI();

    if (current && current->getId() == "Dashboardpage") {
        auto elem = current->getElementById("Hour");
        auto cast = std::dynamic_pointer_cast<TextElement>(elem);

        std::string date = getHour();
        cast->setText(date);

        auto the_date = current->getElementById("date");
        auto date_cast = std::dynamic_pointer_cast<TextElement>(the_date);
        std::string dates = getDate();

        date_cast->setText(dates);
    } else if (current && current->getId() == "CustomParty") {
        auto elem = current->getElementById("mode_choice");
        auto cast = std::dynamic_pointer_cast<ChoiceFieldElement>(elem);

        auto text_diff = current->getElementById("choose_difficulty");
        auto text_cast = std::dynamic_pointer_cast<TextElement>(text_diff);
        auto diff_choice = current->getElementById("difficulty_choice");
        auto diff_choice_cast = std::dynamic_pointer_cast<ChoiceFieldElement>(diff_choice);
        auto path = current->getElementById("choose_file");
        auto path_cast = std::dynamic_pointer_cast<TextElement>(path);
        auto input = current->getElementById("file_path");
        auto input_cast = std::dynamic_pointer_cast<InputFieldElement>(input);


        if (cast->getSelectedIndex() == 0) {
            text_cast->setText("CHOOSE DIFFICULTY : ");
            diff_choice_cast->can_display = true;
            path_cast->setText("");
            input_cast->can_display = false;
        } else if (cast->getSelectedIndex() == 1) {
            text_cast->setText("");
            diff_choice_cast->can_display = false;
            path_cast->setText("");
            input_cast->can_display = false;
        } else if (cast->getSelectedIndex() == 2) {
            text_cast->setText("");
            diff_choice_cast->can_display = false;
            path_cast->setText("PATH TO YOUR FILE : ");
            input_cast->can_display = true;
        }
    }
    auto next = uiManager.getNextUI();

    if (!current && !next)
        return;

    if (current) {
        for (auto& el : current->elements) {
            el->update(clientUI.eventHandler, clientUI.window);
            el->display(clientUI.window);
        }
    }

    if (next && uiManager.isTransitioning()) {
        sf::RenderTexture temp;
        temp.create(clientUI.window.getSize().x, clientUI.window.getSize().y);
        temp.clear(sf::Color::Transparent);

        for (auto& el : next->elements) {
            el->update(clientUI.eventHandler, clientUI.window);
            el->display(temp);
        }

        temp.display();

        sf::Sprite fadeSprite(temp.getTexture());
        fadeSprite.setColor(sf::Color(255, 255, 255, static_cast<int>(uiManager.getAlpha() * 255)));
        clientUI.window.draw(fadeSprite);
    }
}