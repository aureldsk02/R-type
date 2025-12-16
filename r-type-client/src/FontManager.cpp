/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** FontManager implementation
*/

#include "FontManager.hpp"
#include <iostream>

FontManager &FontManager::getInstance() {
  static FontManager instance;
  return instance;
}

void FontManager::Initialize(DisplayHandlerSystem &displayHandler) {
  if (initialized)
    return;

  fontNames[PIXEL_REGULAR] = "pixel_regular";
  fontNames[PIXEL_EXTRUDE] = "pixel_regular";
  fontNames[DEFAULT] = "pixel_regular";

  fontSizes[SMALL] = 12;
  fontSizes[MEDIUM] = 16;
  fontSizes[LARGE] = 20;
  fontSizes[XLARGE] = 24;
  fontSizes[TITLE] = 32;
  fontSizes[HUGE_SIZE] = 48;
  fontSizes[MASSIVE] = 60;
  fontSizes[GIGANTIC] = 80;

  LoadAllFonts(displayHandler);
  initialized = true;

  std::cout << "FontManager initialized with pixel fonts!" << std::endl;
}

void FontManager::LoadAllFonts(DisplayHandlerSystem &displayHandler) {

  bool success =
      displayHandler.LoadFont("pixel_regular", "res/Fonts/Pixel Game.otf");

  if (success) {
    std::cout << "Loaded Pixel Game font successfully" << std::endl;
  } else {
    std::cout << "Failed to load Pixel Game font" << std::endl;
  }
}

std::string FontManager::GetFontName(FontType type) const {
  auto it = fontNames.find(type);
  return (it != fontNames.end()) ? it->second : "default";
}

int FontManager::GetFontSize(FontSize size) const {
  auto it = fontSizes.find(size);
  return (it != fontSizes.end()) ? it->second : 16;
}

TextComponent FontManager::CreateText(const std::string &content,
                                      Vector2 position, FontType fontType,
                                      FontSize fontSize, Color color) const {
  TextComponent text;
  text.content = content;
  text.fontName = GetFontName(fontType);
  text.position = position;
  text.fontSize = GetFontSize(fontSize);
  text.color = color;
  text.visible = true;
  return text;
}

TextComponent FontManager::CreateTitleText(const std::string &content,
                                           Vector2 position,
                                           Color color) const {
  return CreateText(content, position, PIXEL_REGULAR, MASSIVE, color);
}

TextComponent FontManager::CreateMenuText(const std::string &content,
                                          Vector2 position, bool selected,
                                          Color normalColor,
                                          Color selectedColor) const {
  Color textColor = selected ? selectedColor : normalColor;
  return CreateText(content, position, PIXEL_REGULAR, LARGE, textColor);
}