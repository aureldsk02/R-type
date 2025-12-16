/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Display Handler System Implementation with 3D Raycasting
*/

#include "Display_Handler.hpp"
#include "stages.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <rlgl.h>
#include <unistd.h>

DisplayHandlerSystem::~DisplayHandlerSystem() { Cleanup(); }

void DisplayHandlerSystem::Initialize() {
  if (!initialized) {
    InitWindow(screenWidth, screenHeight, windowTitle.c_str());
#ifdef __linux__
    usleep(100000); // 100ms
#endif
    SetWindowPosition(0, 0);
    SetTargetFPS(60);

    if (vsync) {
      SetConfigFlags(FLAG_VSYNC_HINT);
    }
    camera2D.target = {0, 0};
    camera2D.offset = {0.0f, 0.0f};
    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;
    initialized = true;
    std::cout << "DisplayHandlerSystem: Initialized with 3D support"
              << std::endl;
  }
  LoadSpritesPlayerFromFile();
  LoadSpritesBackgroundFromFile();
  LoadSpritesEnemyFromFile();
  LoadSpritesFireFromFile();
}

void DisplayHandlerSystem::LoadSpritesPlayerFromFile() {
  if (!initialized) {
    std::cerr
        << "DisplayHandlerSystem must be initialized before loading sprites."
        << std::endl;
    return;
  }
  std::string path = "res/config/player_sprites.json";
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open JSON file: Player_file" << std::endl;
    return;
  }

  nlohmann::json data;
  try {
    file >> data;
    for (const auto &spriteJson : data["sprites"]) {
      std::string texName = spriteJson["spriteComponent"]["textureName"];
      std::string texPath = "res/" + spriteJson["texture"].get<std::string>();
      if (textures.find(texName) == textures.end())
        LoadTexture(texName, texPath);

      SpriteComponent sprite;
      sprite.textureName = texName;
      auto pos = spriteJson["spriteComponent"]["position"];
      sprite.position = {pos[0], pos[1]};
      auto origin = spriteJson["spriteComponent"]["origin"];
      sprite.origin = {origin[0], origin[1]};
      sprite.scale = spriteJson["spriteComponent"]["scale"];
      sprite.rotation = spriteJson["spriteComponent"]["rotation"];
      auto tint = spriteJson["spriteComponent"]["tint"];
      sprite.tint = {tint[0], tint[1], tint[2], tint[3]};
      sprite.layer = spriteJson["spriteComponent"]["layer"];
      sprite.visible = spriteJson["spriteComponent"]["visible"];
      sprite.type = spriteJson["spriteComponent"]["type"];

      // Assign animation if present
      if (spriteJson.contains("animationComponent") &&
          spriteJson["animationComponent"].is_object()) {
        AnimationComponent anim;
        for (const auto &frame : spriteJson["animationComponent"]["frames"]) {
          anim.frames.push_back({frame[0], frame[1], frame[2], frame[3]});
        }
        anim.frameTime = spriteJson["animationComponent"]["frameTime"];
        anim.loop = spriteJson["animationComponent"]["loop"];
        anim.playing = spriteJson["animationComponent"]["playing"];
        anim.pingpong = spriteJson["animationComponent"]["pingpong"];
        sprite.animation = anim;
      }
      AddSprite(sprites_Player, sprite);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading player sprites: " << e.what() << std::endl;
  }
}

void DisplayHandlerSystem::LoadSpritesEnemyFromFile() {
  if (!initialized) {
    std::cerr
        << "DisplayHandlerSystem must be initialized before loading sprites."
        << std::endl;
    return;
  }
  std::string path = "res/config/enemy_sprites.json";
  std::cout << "DEBUG: Trying to load enemy sprites from: " << path
            << std::endl;
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open JSON file: " << path << std::endl;
    return;
  }

  nlohmann::json data;
  try {
    file >> data;
    for (const auto &spriteJson : data["sprites"]) {
      std::string texName = spriteJson["spriteComponent"]["textureName"];
      std::string texPath = "res/" + spriteJson["texture"].get<std::string>();
      if (textures.find(texName) == textures.end())
        LoadTexture(texName, texPath);

      SpriteComponent sprite;
      sprite.textureName = texName;
      auto pos = spriteJson["spriteComponent"]["position"];
      sprite.position = {pos[0], pos[1]};
      auto origin = spriteJson["spriteComponent"]["origin"];
      sprite.origin = {origin[0], origin[1]};
      sprite.scale = spriteJson["spriteComponent"]["scale"];
      sprite.rotation = spriteJson["spriteComponent"]["rotation"];
      auto tint = spriteJson["spriteComponent"]["tint"];
      sprite.tint = {tint[0], tint[1], tint[2], tint[3]};
      sprite.layer = spriteJson["spriteComponent"]["layer"];
      sprite.visible = spriteJson["spriteComponent"]["visible"];
      sprite.type = spriteJson["spriteComponent"]["type"];

      // Assign animation if present
      if (spriteJson.contains("animationComponent") &&
          spriteJson["animationComponent"].is_object()) {
        AnimationComponent anim;
        for (const auto &frame : spriteJson["animationComponent"]["frames"]) {
          anim.frames.push_back({frame[0], frame[1], frame[2], frame[3]});
        }
        anim.frameTime = spriteJson["animationComponent"]["frameTime"];
        anim.loop = spriteJson["animationComponent"]["loop"];
        anim.playing = spriteJson["animationComponent"]["playing"];
        sprite.animation = anim;
      }
      AddSprite(sprites_Enemy, sprite);
      std::cout << "DEBUG: Loaded enemy sprite '" << sprite.textureName
                << "' type='" << sprite.type << "'" << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading enemy sprites: " << e.what() << std::endl;
  }
  std::cout << "DEBUG: Total enemy sprites loaded: " << sprites_Enemy.size()
            << std::endl;
}

void DisplayHandlerSystem::LoadSpritesBackgroundFromFile() {
  if (!initialized) {
    std::cerr
        << "DisplayHandlerSystem must be initialized before loading sprites."
        << std::endl;
    return;
  }
  std::string path = "res/config/background.json";
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open JSON file: Background_file" << std::endl;
    return;
  }

  nlohmann::json data;
  try {
    file >> data;
    for (const auto &spriteJson : data["sprites"]) {
      std::string texName = spriteJson["spriteComponent"]["textureName"];
      std::string texPath = "res/" + spriteJson["texture"].get<std::string>();
      if (textures.find(texName) == textures.end())
        LoadTexture(texName, texPath);

      SpriteComponent sprite;
      sprite.textureName = texName;
      auto pos = spriteJson["spriteComponent"]["position"];
      sprite.position = {pos[0], pos[1]};
      auto origin = spriteJson["spriteComponent"]["origin"];
      sprite.origin = {origin[0], origin[1]};
      sprite.scale = spriteJson["spriteComponent"]["scale"];
      sprite.rotation = spriteJson["spriteComponent"]["rotation"];
      auto tint = spriteJson["spriteComponent"]["tint"];
      sprite.tint = {tint[0], tint[1], tint[2], tint[3]};
      sprite.layer = spriteJson["spriteComponent"]["layer"];
      sprite.visible = spriteJson["spriteComponent"]["visible"];
      sprite.type = spriteJson["spriteComponent"]["type"];

      // Assign animation if present
      if (spriteJson.contains("animationComponent") &&
          spriteJson["animationComponent"].is_object()) {
        AnimationComponent anim;
        for (const auto &frame : spriteJson["animationComponent"]["frames"]) {
          anim.frames.push_back({frame[0], frame[1], frame[2], frame[3]});
        }
        anim.frameTime = spriteJson["animationComponent"]["frameTime"];
        anim.loop = spriteJson["animationComponent"]["loop"];
        anim.playing = spriteJson["animationComponent"]["playing"];
        sprite.animation = anim;
      }

      // Parse sourceRect if present in JSON
      if (spriteJson["spriteComponent"].contains("sourceRect")) {
        auto srcRect = spriteJson["spriteComponent"]["sourceRect"];
        sprite.sourceRect = {srcRect[0], srcRect[1], srcRect[2], srcRect[3]};
      }
      AddSprite(sprites_Background, sprite);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading background sprites: " << e.what() << std::endl;
  }
}

void DisplayHandlerSystem::LoadSpritesFireFromFile() {
  if (!initialized) {
    std::cerr
        << "DisplayHandlerSystem must be initialized before loading sprites."
        << std::endl;
    return;
  }
  std::string path = "res/config/fire_sprites.json";
  std::cout << "DEBUG: Trying to load fire sprites from: " << path << std::endl;
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open JSON file: " << path << std::endl;
    return;
  }

  nlohmann::json data;
  try {
    file >> data;
    for (const auto &spriteJson : data["sprites"]) {
      std::string texName = spriteJson["spriteComponent"]["textureName"];
      std::string texPath = "res/" + spriteJson["texture"].get<std::string>();
      if (textures.find(texName) == textures.end())
        LoadTexture(texName, texPath);

      SpriteComponent sprite;
      sprite.textureName = texName;
      auto pos = spriteJson["spriteComponent"]["position"];
      sprite.position = {pos[0], pos[1]};
      auto origin = spriteJson["spriteComponent"]["origin"];
      sprite.origin = {origin[0], origin[1]};
      sprite.scale = spriteJson["spriteComponent"]["scale"];
      sprite.rotation = spriteJson["spriteComponent"]["rotation"];
      auto tint = spriteJson["spriteComponent"]["tint"];
      sprite.tint = {tint[0], tint[1], tint[2], tint[3]};
      sprite.layer = spriteJson["spriteComponent"]["layer"];
      sprite.visible = spriteJson["spriteComponent"]["visible"];

      // Handle missing type field safely
      if (spriteJson["spriteComponent"].contains("type")) {
        sprite.type = spriteJson["spriteComponent"]["type"];
      } else {
        sprite.type = "fire"; // default for fire sprites
      }

      // Assign animation if present
      if (spriteJson.contains("animationComponent") &&
          spriteJson["animationComponent"].is_object()) {
        AnimationComponent anim;
        for (const auto &frame : spriteJson["animationComponent"]["frames"]) {
          anim.frames.push_back({frame[0], frame[1], frame[2], frame[3]});
        }
        anim.frameTime = spriteJson["animationComponent"]["frameTime"];
        anim.loop = spriteJson["animationComponent"]["loop"];
        anim.playing = spriteJson["animationComponent"]["playing"];
        sprite.animation = anim;
      }

      // Parse sourceRect if present
      if (spriteJson["spriteComponent"].contains("sourceRect")) {
        auto srcRect = spriteJson["spriteComponent"]["sourceRect"];
        sprite.sourceRect = {srcRect[0], srcRect[1], srcRect[2], srcRect[3]};
      }
      AddSprite(sprites_Fire, sprite);
      std::cout << "DEBUG: Loaded fire sprite '" << sprite.textureName
                << "' type='" << sprite.type << "'" << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error loading fire sprites: " << e.what() << std::endl;
  }
  std::cout << "DEBUG: Total fire sprites loaded: " << sprites_Fire.size()
            << std::endl;
}

void DisplayHandlerSystem::Update(float deltaTime) {
  if (!initialized)
    return;
  // Check for window resize
  if (IsWindowResized()) {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    camera2D.offset = {0.0f, 0.0f};
  }
}

void DisplayHandlerSystem::Render() {
  if (!initialized)
    return;

  BeginRender();
  ClearBackground(backgroundColor);
  // RenderRectangles();

  EndRender();
}

void DisplayHandlerSystem::Cleanup() {
  if (initialized) {
    UnloadAllResources();

    if (IsShaderValid(lightingShader)) {
      UnloadShader(lightingShader);
    }

    ClearSprites();
    ClearTexts();

    CloseWindow();
    initialized = false;
    std::cout << "DisplayHandlerSystem: Cleaned up" << std::endl;
  }
}

bool DisplayHandlerSystem::LoadTexture(const std::string &name,
                                       const std::string &path) {
  TextureSource texture;
  texture.name = name;
  texture.pathfile = path;
  texture.texture = ::LoadTexture(path.c_str());
  texture.isLoaded = (texture.texture.id > 0);

  if (texture.isLoaded) {
    texture.size = {(float)texture.texture.width,
                    (float)texture.texture.height};
    textures[name] = texture;

    currentEvent.type = DisplayEvent::TEXTURE_LOADED;
    currentEvent.resourceName = name;
    currentEvent.success = true;

    std::cout << "DisplayHandlerSystem: Loaded texture '" << name << "' from "
              << path << std::endl;
    return true;
  } else {
    currentEvent.type = DisplayEvent::TEXTURE_LOADED;
    currentEvent.resourceName = name;
    currentEvent.success = false;

    std::cout << "DisplayHandlerSystem: Failed to load texture '" << name
              << "' from " << path << std::endl;
    return false;
  }
}

bool DisplayHandlerSystem::LoadFont(const std::string &name,
                                    const std::string &path) {
  FontSource font;
  font.name = name;
  font.pathfile = path;
  font.font = ::LoadFont(path.c_str());
  font.isLoaded = (font.font.texture.id > 0);

  if (font.isLoaded) {
    fonts[name] = font;

    currentEvent.type = DisplayEvent::FONT_LOADED;
    currentEvent.resourceName = name;
    currentEvent.success = true;

    std::cout << "DisplayHandlerSystem: Loaded font '" << name << "' from "
              << path << std::endl;
    return true;
  } else {
    currentEvent.type = DisplayEvent::FONT_LOADED;
    currentEvent.resourceName = name;
    currentEvent.success = false;

    std::cout << "DisplayHandlerSystem: Failed to load font '" << name
              << "' from " << path << std::endl;
    return false;
  }
}

void DisplayHandlerSystem::UnloadTexture(const std::string &name) {
  auto it = textures.find(name);
  if (it != textures.end()) {
    if (it->second.isLoaded) {
      ::UnloadTexture(it->second.texture);
    }
    textures.erase(it);
    std::cout << "DisplayHandlerSystem: Unloaded texture '" << name << "'"
              << std::endl;
  }
}

void DisplayHandlerSystem::UnloadFont(const std::string &name) {
  auto it = fonts.find(name);
  if (it != fonts.end()) {
    if (it->second.isLoaded) {
      ::UnloadFont(it->second.font);
    }
    fonts.erase(it);
    std::cout << "DisplayHandlerSystem: Unloaded font '" << name << "'"
              << std::endl;
  }
}

void DisplayHandlerSystem::UnloadAllResources() {
  for (auto &pair : textures) {
    if (pair.second.isLoaded) {
      ::UnloadTexture(pair.second.texture);
    }
  }
  textures.clear();

  for (auto &pair : fonts) {
    if (pair.second.isLoaded) {
      ::UnloadFont(pair.second.font);
    }
  }
  fonts.clear();
}

// Component management
void DisplayHandlerSystem::AddSprite(std::vector<SpriteComponent> &list_entity,
                                     const SpriteComponent &sprite) {
  list_entity.push_back(sprite);
}

void DisplayHandlerSystem::AddText(const TextComponent &text) {
  texts.push_back(text);
}

void DisplayHandlerSystem::AddRectangle(const RectangleComponent &rect) {
  rectangles.push_back(rect);
}

// Private helper functions
void DisplayHandlerSystem::BeginRender() { BeginDrawing(); }

void DisplayHandlerSystem::EndRender() { EndDrawing(); }

void DisplayHandlerSystem::Render2D() {
  if (camera2DEnabled) {
    BeginMode2D(camera2D);
  }

  RenderSprites();
  RenderTexts();

  if (camera2DEnabled) {
    EndMode2D();
  }
}

// Keep existing 2D drawing and animation functions...
void DisplayHandlerSystem::DrawSprite(const SpriteComponent &sprite) {
  if (!sprite.visible)
    return;

  auto it = textures.find(sprite.textureName);
  if (it == textures.end() || !it->second.isLoaded)
    return;

  Texture2D texture = it->second.texture;

  Rectangle source = sprite.sourceRect;
  Rectangle dest;

  if (sprite.type == "background") {
    // Scale background to fill the entire screen while maintaining aspect ratio
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    // Calculate scale factors for both dimensions
    float scaleX = screenWidth / source.width;
    float scaleY = screenHeight / source.height;

    // Use the larger scale factor to ensure background covers entire screen
    float scale = std::max(scaleX, scaleY) * sprite.scale;

    // Center the background
    float scaledWidth = source.width * scale;
    float scaledHeight = source.height * scale;
    float offsetX = (screenWidth - scaledWidth) * 0.5f;
    float offsetY = (screenHeight - scaledHeight) * 0.5f;

    dest = {sprite.position.x + offsetX, sprite.position.y + offsetY,
            scaledWidth, scaledHeight};

    std::cout << "Drawing background: screen=" << screenWidth << "x"
              << screenHeight << " source=" << source.width << "x"
              << source.height << " scale=" << scale << " dest=" << dest.width
              << "x" << dest.height << std::endl;
  } else {
    // For normal sprites, apply scaling logic
    const float baseWidth = 1920.0f;
    const float baseHeight = 1080.0f;
    float scaleFactor =
        std::min(GetScreenWidth() / baseWidth, GetScreenHeight() / baseHeight);
    float effectiveScale = sprite.scale * scaleFactor;

    dest = {sprite.position.x, sprite.position.y,
            sprite.sourceRect.width * effectiveScale,
            sprite.sourceRect.height * effectiveScale};
  }

  DrawTexturePro(it->second.texture, source, dest, sprite.origin,
                 sprite.rotation, sprite.tint);
}

void DisplayHandlerSystem::DrawParallaxBackground(const SpriteComponent &sprite,
                                                  float offsetX, float speed) {
  if (!sprite.visible)
    return;

  auto it = textures.find(sprite.textureName);
  if (it == textures.end() || !it->second.isLoaded)
    return;

  Texture2D texture = it->second.texture;

  // Get screen dimensions
  float screenWidth = static_cast<float>(GetScreenWidth());
  float screenHeight = static_cast<float>(GetScreenHeight());

  // Source rectangle - use sprite's sourceRect or full texture
  Rectangle source = sprite.sourceRect;
  if (source.width <= 0 || source.height <= 0) {
    source = {0, 0, static_cast<float>(texture.width),
              static_cast<float>(texture.height)};
  }

  // Calculate tile dimensions (scaled to screen)
  float tileWidth = screenWidth;
  float tileHeight = screenHeight;

  // Apply parallax offset with speed multiplier - NEGATIVE for left movement
  float parallaxOffset = -offsetX * speed;

  // Don't normalize offset to avoid jumps - just use modulo for positioning
  // This ensures smooth continuous scrolling without visible resets
  float normalizedOffset = fmod(parallaxOffset, tileWidth);

  // Draw tiles to cover the screen plus extras for seamless scrolling
  int tilesNeeded = 6; // More tiles to ensure no gaps during scrolling

  // Calculate starting position to ensure screen coverage
  float startX = normalizedOffset - (2 * tileWidth);

  for (int i = 0; i < tilesNeeded; i++) {
    float tilePositionX = startX + (i * tileWidth);

    Rectangle dest = {tilePositionX, 0, tileWidth, tileHeight};

    // Only draw if the tile is at least partially visible
    if (dest.x + dest.width >= 0 && dest.x <= screenWidth) {
      // Use simple repetition without mirroring for all backgrounds
      DrawTexturePro(texture, source, dest, {0, 0}, sprite.rotation,
                     sprite.tint);
    }
  }
}

void DisplayHandlerSystem::DrawText(const TextComponent &text) {
  if (!text.visible)
    return;

  auto it = fonts.find(text.fontName);
  if (it != fonts.end() && it->second.isLoaded) {
    DrawTextEx(it->second.font, text.content.c_str(), text.position,
               text.fontSize, 1, text.color);
  } else {
    ::DrawText(text.content.c_str(), text.position.x, text.position.y,
               text.fontSize, text.color);
  }
}

void DisplayHandlerSystem::DrawRectangle(Vector2 position, Vector2 size,
                                         Color color) {
  ::DrawRectangle(position.x, position.y, size.x, size.y, color);
}

void DisplayHandlerSystem::DrawCircle(Vector2 position, float radius,
                                      Color color) {
  ::DrawCircle(position.x, position.y, radius, color);
}

void DisplayHandlerSystem::DrawLine(Vector2 start, Vector2 end, Color color) {
  ::DrawLine(start.x, start.y, end.x, end.y, color);
}

DisplayEvent DisplayHandlerSystem::GetCurrentEvent() {
  DisplayEvent event = currentEvent;
  currentEvent = DisplayEvent(); // Reset current event
  return event;
}

void DisplayHandlerSystem::UpdateText(size_t index,
                                      const std::string &newContent) {
  if (index < texts.size()) {
    texts[index].content = newContent;
  }
}

SpriteComponent &
DisplayHandlerSystem::GetSprite(std::vector<SpriteComponent> &list_sprite,
                                const std::string &name) {
  auto it = std::find_if(
      list_sprite.begin(), list_sprite.end(),
      [&](const SpriteComponent &s) { return s.textureName == name; });
  if (it != list_sprite.end())
    return *it;
  throw std::out_of_range("Sprite not found: " + name);
}

SpriteComponent &
DisplayHandlerSystem::GetSprite(std::vector<SpriteComponent> &list_sprite,
                                int id) {
  if (id < 0 || id >= (int)list_sprite.size()) {
    throw std::out_of_range("Sprite not found: " + std::to_string(id));
  }
  return list_sprite[id];
}

void DisplayHandlerSystem::UpdateRectangle(size_t index,
                                           const RectangleComponent &rect) {
  if (index < rectangles.size()) {
    rectangles[index] = rect;
  }
}

void DisplayHandlerSystem::UpdateSprite(
    std::vector<SpriteComponent> &list_sprite, const std::string name,
    const SpriteComponent &sprite) {
  // auto it = std::find_if(list_sprite.begin(), list_sprite.end(), [&](const
  // SpriteComponent& s) {
  //     return s.textureName == name;
  // });
  // if (it != list_sprite.end()) {
  //     *it = sprite;
  // }
}

Rectangle DisplayHandlerSystem::UpdateAnimations(float deltaTime,
                                                 SpriteComponent sprite) {

  AnimationComponent &animation = sprite.animation;
  if (animation.playing && !animation.frames.empty()) {
    animation.timer += deltaTime;
    if (animation.timer >= animation.frameTime) {
      animation.timer = 0.0f;
      animation.currentFrame += animation.direction;

      if (animation.pingpong) {
        if (animation.currentFrame >= (int)animation.frames.size()) {
          animation.direction = -1;
          animation.currentFrame = animation.frames.size() - 2;
        } else if (animation.currentFrame < 0) {
          animation.direction = 1;
          animation.currentFrame = 1;
          // Do NOT set animation.playing = false here
        }
      } else if (animation.loop) {
        if (animation.currentFrame >= (int)animation.frames.size())
          animation.currentFrame = 0;
      } else {
        if (animation.currentFrame >= (int)animation.frames.size()) {
          animation.currentFrame = animation.frames.size() - 1;
          animation.playing = false;
        }
      }
    }
    sprite.sourceRect = animation.frames[animation.currentFrame];
  }
  return sprite.sourceRect;
}

void DisplayHandlerSystem::SortRenderables() {
  // Sort sprites by layer
  // std::sort(sprites.begin(), sprites.end(),
  //     [](const SpriteComponent& a, const SpriteComponent& b) {
  //         return a.layer < b.layer;
  //     });
}

void DisplayHandlerSystem::RenderSprites() { SortRenderables(); }

void DisplayHandlerSystem::RenderTexts() {
  for (const auto &text : texts) {
    DrawText(text);
  }
}

void DisplayHandlerSystem::RenderRectangles() {
  for (const auto &rect : rectangles) {
    if (rect.visible) {
      DrawRectangle(rect.position, rect.size, rect.color);
    }
  }
}
