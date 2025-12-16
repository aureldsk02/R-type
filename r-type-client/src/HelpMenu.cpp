/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** HelpMenu
*/

#include "HelpMenu.hpp"
#include "FontManager.hpp"

HelpMenu::HelpMenu() 
    : actionSelected(false), selectedAction(""), menuPosition{400, 300}, 
      fontSize(18), normalColor(WHITE), selectedColor(YELLOW), scrollOffset(0) {
    initializeHelpContent();
}

HelpMenu::~HelpMenu() {
}

void HelpMenu::Initialize() {
    actionSelected = false;
    selectedAction = "";
    scrollOffset = 0;
    initializeHelpContent();
}

void HelpMenu::initializeHelpContent() {
    helpContent.clear();
    
    helpContent.push_back("=== R-TYPE - GUIDE DE JEU ===");
    helpContent.push_back("");
    helpContent.push_back("OBJECTIF:");
    helpContent.push_back("Detruisez les vagues d'ennemis de l'Empire Bydo");
    helpContent.push_back("Survivez le plus longtemps possible");
    helpContent.push_back("Cooperez avec vos allies en mode multijoueur");
    helpContent.push_back("");
    helpContent.push_back("CONTROLES:");
    helpContent.push_back("Fleches / WASD  - Deplacer le vaisseau");
    helpContent.push_back("ESPACE          - Tirer des missiles");
    helpContent.push_back("ECHAP           - Pause / Menu");
    helpContent.push_back("F11             - Plein ecran");
    helpContent.push_back("");
    helpContent.push_back("GAMEPLAY:");
    helpContent.push_back("- Evitez les projectiles ennemis");
    helpContent.push_back("- Collectez les power-ups pour ameliorer vos armes");
    helpContent.push_back("- Chaque ennemi detruit rapporte des points");
    helpContent.push_back("- Attention aux boss en fin de niveau!");
    helpContent.push_back("");
    helpContent.push_back("MODES DE JEU:");
    helpContent.push_back("SOLO        - Jouez seul contre l'IA");
    helpContent.push_back("MULTIJOUEUR - Jusqu'a 4 joueurs en cooperation");
    helpContent.push_back("");
    helpContent.push_back("POWER-UPS:");
    helpContent.push_back("Rouge   - Ameliore la puissance de feu");
    helpContent.push_back("Bleu    - Missiles a tete chercheuse");
    helpContent.push_back("Vert    - Tir en rafale");
    helpContent.push_back("Jaune   - Bouclier temporaire");
    helpContent.push_back("");
    helpContent.push_back("ENNEMIS:");
    helpContent.push_back("Scouts  - Rapides mais fragiles");
    helpContent.push_back("Tanks   - Lents mais resistants");
    helpContent.push_back("Snipers - Tirent a longue portee");
    helpContent.push_back("Boss    - Enormes et dangereux!");
    helpContent.push_back("");
    helpContent.push_back("CONSEILS:");
    helpContent.push_back("- Restez en mouvement constant");
    helpContent.push_back("- Anticipez les patterns d'attaque");
    helpContent.push_back("- Utilisez les power-ups strategiquement");
    helpContent.push_back("- En multijoueur, coordonnez vos attaques");
    helpContent.push_back("");
    helpContent.push_back("SCORING:");
    helpContent.push_back("Scout detruit    : 100 points");
    helpContent.push_back("Tank detruit     : 250 points");
    helpContent.push_back("Sniper detruit   : 200 points");
    helpContent.push_back("Boss detruit     : 1000 points");
    helpContent.push_back("Bonus survie     : 50 points/seconde");
    helpContent.push_back("");
    helpContent.push_back("=== BONNE CHANCE, PILOTE! ===");
    helpContent.push_back("");
    helpContent.push_back("Appuyez sur ECHAP pour retourner aux parametres");
}

void HelpMenu::Update(InputHandlerSystem& inputHandler) {
    if (inputHandler.IsKeyPressed(InputKey::KEY_UP) || inputHandler.IsKeyPressed(InputKey::KEY_W)) {
        scrollOffset = std::max(0, scrollOffset - 1);
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_DOWN) || inputHandler.IsKeyPressed(InputKey::KEY_S)) {
        int maxScroll = std::max(0, static_cast<int>(helpContent.size()) - 20);
        scrollOffset = std::min(maxScroll, scrollOffset + 1);
    }
    else if (inputHandler.IsKeyPressed(InputKey::KEY_ESCAPE) || inputHandler.IsKeyPressed(InputKey::KEY_ENTER)) {
        actionSelected = true;
        selectedAction = "backToSettings";
        std::cout << "HelpMenu: Retour aux parametres" << std::endl;
    }
}

void HelpMenu::Render(DisplayHandlerSystem& displayHandler) {
    Vector2 screenSize = displayHandler.GetScreenSize();
    FontManager& fontManager = FontManager::getInstance();
    fontManager.Initialize(displayHandler);
    
    // Background gradient
    for (int i = 0; i < screenSize.y; i++) {
        float alpha = (float)i / screenSize.y;
        Color gradientColor = {
            static_cast<unsigned char>(5 + alpha * 10),
            static_cast<unsigned char>(10 + alpha * 15),
            static_cast<unsigned char>(20 + alpha * 25),
            255
        };
        displayHandler.DrawRectangle({0, (float)i}, {screenSize.x, 1}, gradientColor);
    }
    
    menuPosition.x = screenSize.x / 2;
    menuPosition.y = screenSize.y / 2;

    // Title with pixel font
    Vector2 titlePos = {screenSize.x / 2 - 200, 50};
    
    // Title shadow
    TextComponent titleShadow = fontManager.CreateTitleText("AIDE & CONTROLES", {titlePos.x + 3, titlePos.y + 3}, Color{0, 0, 0, 180});
    displayHandler.DrawText(titleShadow);
    
    // Main title
    TextComponent titleText = fontManager.CreateTitleText("AIDE & CONTROLES", titlePos, Color{150, 255, 150, 255});
    displayHandler.DrawText(titleText);

    // Help content panel
    int panelWidth = screenSize.x - 100;
    int panelHeight = screenSize.y - 200;
    Vector2 panelPos = {50, 130};
    
    displayHandler.DrawRectangle(panelPos, {(float)panelWidth, (float)panelHeight}, Color{15, 20, 35, 240});

    int lineHeight = 25;
    int maxVisibleLines = (panelHeight - 40) / lineHeight;
    
    for (int i = 0; i < maxVisibleLines && (i + scrollOffset) < helpContent.size(); i++) {
        int lineIndex = i + scrollOffset;
        Vector2 textPos = {panelPos.x + 30, panelPos.y + 20 + i * lineHeight};
        
        Color textColor = Color{200, 200, 200, 255};
        
        std::string line = helpContent[lineIndex];
        if (line.find("===") != std::string::npos) {
            textColor = Color{255, 255, 100, 255};
        } else if (line.find(":") != std::string::npos && line.length() < 20) {
            textColor = Color{100, 255, 150, 255};
        } else if (line.find("-") == 0) {
            textColor = Color{180, 180, 255, 255};
        }
        
        TextComponent lineText = fontManager.CreateText(line, textPos, FontManager::PIXEL_REGULAR, FontManager::MEDIUM, textColor);
        displayHandler.DrawText(lineText);
    }
    
    // Scroll bar
    if (helpContent.size() > maxVisibleLines) {
        int scrollBarHeight = panelHeight - 20;
        int scrollBarY = panelPos.y + 10;
        int scrollBarX = panelPos.x + panelWidth - 20;
        
        displayHandler.DrawRectangle({(float)scrollBarX, (float)scrollBarY}, {10, (float)scrollBarHeight}, Color{50, 50, 50, 200});
        
        float scrollRatio = (float)scrollOffset / (helpContent.size() - maxVisibleLines);
        int cursorY = scrollBarY + (int)(scrollRatio * (scrollBarHeight - 30));
        displayHandler.DrawRectangle({(float)scrollBarX, (float)cursorY}, {10, 30}, Color{100, 255, 100, 255});
    }

    // Instructions with pixel font
    displayHandler.DrawRectangle({screenSize.x / 2 - 250, screenSize.y - 40}, {500, 25}, Color{0, 0, 0, 150});
    
    TextComponent instructionsText = fontManager.CreateText("Haut/Bas pour defiler > ECHAP ou ENTREE pour retourner", 
                                                          {screenSize.x / 2 - 240, screenSize.y - 35}, 
                                                          FontManager::PIXEL_REGULAR, FontManager::MEDIUM, 
                                                          Color{180, 180, 180, 255});
    displayHandler.DrawText(instructionsText);
}

void HelpMenu::Cleanup() {
    helpContent.clear();
}

std::string HelpMenu::GetSelectedAction() const {
    return selectedAction;
}

bool HelpMenu::IsActionSelected() const {
    return actionSelected;
}

void HelpMenu::ResetSelection() {
    actionSelected = false;
    selectedAction = "";
}