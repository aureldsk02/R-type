/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20 [WSL: Ubuntu-24.04]
** File description:
** ECS Display Handler System with 3D Raycasting
*/

#ifndef DISPLAY_HANDLER_HPP_
#define DISPLAY_HANDLER_HPP_

#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

// Display resource structures
struct TextureSource {
    Texture2D texture;
    std::string name;
    std::string pathfile;
    bool isLoaded = false;
    Vector2 size = {0, 0};

    TextureSource() : texture{}, name(""), pathfile(""), isLoaded(false), size{0, 0} {}
};

struct FontSource {
    Font font;
    std::string name;
    std::string pathfile;
    bool isLoaded = false;
    int fontSize = 20;

    FontSource() : font{}, name(""), pathfile(""), isLoaded(false), fontSize(20) {}
};

// 2D Components (keeping existing ones)
struct AnimationComponent {
    std::vector<Rectangle> frames;
    int currentFrame = 0;
    float frameTime = 0.1f;
    float timer = 0.0f;
    bool loop = true;
    bool playing = false;
    bool pingpong = false;
    int direction = 1; // 1 = forward, -1 = backward

    AnimationComponent() : frames{}, currentFrame(0), frameTime(0.1f), 
                          timer(0.0f), loop(true), playing(false) {}
};

struct SpriteComponent {
    AnimationComponent animation;
    std::string textureName;
    Rectangle sourceRect = {0, 0, 32, 32};
    Vector2 position = {0, 0};
    Vector2 origin = {0, 0};
    float scale = 1.0f;
    float rotation = 0.0f;
    Color tint = WHITE;
    int layer = 0;
    bool visible = false;
    std::string type = "";

    SpriteComponent() : textureName(""), sourceRect{0, 0, 32, 32}, position{0, 0}, 
                       origin{0, 0}, scale(1.0f), rotation(0.0f), tint(WHITE), 
                       layer(0), visible(true), type("") {}
};

struct TextComponent {
    std::string content;
    std::string fontName;
    Vector2 position = {0, 0};
    int fontSize = 20;
    Color color = WHITE;
    int layer = 0;
    bool visible = false;

    TextComponent() : content(""), fontName("default"), position{0, 0},
                     fontSize(20), color(WHITE), layer(0), visible(false) {}
};

struct RectangleComponent {
    Vector2 position = {0, 0};
    Vector2 size = {50, 50};
    Color color = RED;
    int layer = 0;
    bool visible = true;

    RectangleComponent() : position{0, 0}, size{50, 50}, color(RED),
                          layer(0), visible(true) {}
};

struct StageComponent;

// Display event structure
struct DisplayEvent {
    enum Type {
        TEXTURE_LOADED,
        TEXTURE_UNLOADED,
        FONT_LOADED,
        FONT_UNLOADED,
        MODEL_LOADED,
        MODEL_UNLOADED,
        SCREEN_RESIZED,
        RAYCAST_HIT,
        NONE
    } type;
    std::string resourceName;
    bool success;

    DisplayEvent() : type(NONE), resourceName(""), success(false) {}
};

// ECS Display Handler System with 3D Raycasting
class DisplayHandlerSystem {
    private:
        // Resource management
        std::unique_ptr<TextureSource> currentTexture;
        std::unique_ptr<FontSource> currentFont;
        std::unordered_map<std::string, TextureSource> textures;
        std::unordered_map<std::string, FontSource> fonts;
        
        // 2D Components
        std::vector<SpriteComponent> sprites_Player;
        std::vector<SpriteComponent> sprites_Enemy;
        std::vector<SpriteComponent> sprites_Background;
        std::vector<SpriteComponent> sprites_Fire;
        std::vector<TextComponent> texts;
        std::vector<RectangleComponent> rectangles;
        
        // Screen properties
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        std::string windowTitle = "R-Type";
        bool fullscreen = true;
        bool vsync = true;
        
        // 2D Camera (for UI overlay)
        Camera2D camera2D = {{0, 0}, {0, 0}, 0.0f, 1.0f};
        bool camera2DEnabled = true;
        
        // Lighting
        Shader lightingShader = {};
        bool lightingEnabled = true;
        Vector3 ambientLight = {0.2f, 0.2f, 0.2f};
        
        // Display state
        Color backgroundColor = {20, 30, 40, 255}; // Dark blue-gray
        DisplayEvent currentEvent;
        bool initialized = false;

    public:
        DisplayHandlerSystem()
            : currentTexture(nullptr),
              currentFont(nullptr),
              initialized(false) {}
        ~DisplayHandlerSystem();

        // System lifecycle
        void Initialize();
        void LoadSpritesPlayerFromFile();
        void LoadSpritesEnemyFromFile();
        void LoadSpritesBackgroundFromFile();
        void LoadSpritesFireFromFile();
        std::vector<SpriteComponent> GetSpritesPlayer() { return sprites_Player; }
        std::vector<SpriteComponent> GetSpritesEnemy() { return sprites_Enemy; }
        std::vector<SpriteComponent> GetSpritesBackground() { return sprites_Background; }
        std::vector<SpriteComponent> GetSpritesFire() { return sprites_Fire; }
        void Update(float deltaTime);
        void Render();
        void Cleanup();
        bool IsInitialized() const { return initialized; }
        bool ShouldClose() const { return WindowShouldClose(); }
        bool ShouldCloseIgnoreEscape() const { return IsKeyPressed(KEY_F4) && IsKeyDown(KEY_LEFT_ALT); }
        bool ShouldCloseIgnoreEscapeOnly() const { 
            // Allow window close button, Alt+F4, but ignore ESC key
            return (WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) || (IsKeyPressed(KEY_F4) && IsKeyDown(KEY_LEFT_ALT)); 
        }

        // Resource management
        bool LoadTexture(const std::string& name, const std::string& path);
        bool LoadFont(const std::string& name, const std::string& path);
        void UnloadTexture(const std::string& name);
        void UnloadFont(const std::string& name);
        void UnloadAllResources();
        void SetCamera2D(Camera2D cam) { camera2D = cam; camera2DEnabled = true; }
        void EnableCamera2D() { camera2DEnabled = true; }
        void DisableCamera2D() { camera2DEnabled = false; }
        Camera2D& GetCamera2D() { return camera2D; }

        // Component management
        void AddSprite(std::vector<SpriteComponent> &list_entity, const SpriteComponent& sprite);
        void AddText(const TextComponent& text);
        void AddRectangle(const RectangleComponent& rect);        
        void ClearSprites() { sprites_Player.clear();
                                sprites_Enemy.clear();
                                sprites_Background.clear();
                                sprites_Fire.clear(); }
        void ClearTexts() { texts.clear(); }
        void ClearRectangles() { rectangles.clear(); }

        //drawing function
        void DrawSprite(const SpriteComponent& sprite);
        void DrawText(const TextComponent& text);
        void DrawRectangle(Vector2 position, Vector2 size, Color color);
        void DrawCircle(Vector2 position, float radius, Color color);
        void DrawLine(Vector2 start, Vector2 end, Color color);

        // Parallax background rendering
        void DrawParallaxBackground(const SpriteComponent& sprite, float offsetX, float speed = 1.0f);

        // Utility functions
        SpriteComponent& GetSprite(std::vector<SpriteComponent> &list_sprite, int id);

        // Get sprite by textureName
        SpriteComponent& GetSprite(std::vector<SpriteComponent> &list_sprite, const std::string &name);
        DisplayEvent GetCurrentEvent();
        Vector2 GetScreenSize() const { return {(float)screenWidth, (float)screenHeight}; }
        void UpdateText(size_t index, const std::string& newContent);
        void UpdateRectangle(size_t index, const RectangleComponent& rect);
        void UpdateSprite(std::vector<SpriteComponent> &list_sprite, const std::string name, const SpriteComponent& sprite);
        
        // Animation functions
        Rectangle UpdateAnimations(float deltaTime, SpriteComponent sprite);
        void EndRender();
    private:
        void BeginRender();
        void Render2D();
        void SortRenderables();
        void RenderSprites();
        void RenderTexts();
        void RenderRectangles();
};

#endif /* !DISPLAY_HANDLER_HPP_ */