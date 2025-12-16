/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** FontManager - Centralized font management system
*/

#ifndef FONTMANAGER_HPP_
#define FONTMANAGER_HPP_

#include "Display_Handler.hpp"
#include <string>
#include <unordered_map>

class FontManager {
public:
  enum FontType { PIXEL_REGULAR, PIXEL_EXTRUDE, DEFAULT };

  enum FontSize {
    SMALL = 12,
    MEDIUM = 16,
    LARGE = 20,
    XLARGE = 24,
    TITLE = 32,
    HUGE_SIZE = 48,
    MASSIVE = 60,
    GIGANTIC = 80
  };

  static FontManager &getInstance();

  void Initialize(DisplayHandlerSystem &displayHandler);
  void LoadAllFonts(DisplayHandlerSystem &displayHandler);

  std::string GetFontName(FontType type) const;
  int GetFontSize(FontSize size) const;

  TextComponent CreateText(const std::string &content, Vector2 position,
                           FontType fontType = PIXEL_REGULAR,
                           FontSize fontSize = MEDIUM,
                           Color color = WHITE) const;

  TextComponent CreateTitleText(const std::string &content, Vector2 position,
                                Color color = WHITE) const;

  TextComponent CreateMenuText(const std::string &content, Vector2 position,
                               bool selected = false, Color normalColor = WHITE,
                               Color selectedColor = YELLOW) const;

private:
  FontManager() = default;
  ~FontManager() = default;
  FontManager(const FontManager &) = delete;
  FontManager &operator=(const FontManager &) = delete;

  std::unordered_map<FontType, std::string> fontNames;
  std::unordered_map<FontSize, int> fontSizes;
  bool initialized = false;
};

#endif /* !FONTMANAGER_HPP_ */