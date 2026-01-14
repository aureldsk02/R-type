/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * TextureManager *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef INCLUDED_TEXTUREMANAGER_HPP
#define INCLUDED_TEXTUREMANAGER_HPP

/**
 * @file ResourceManager.hpp
 * @author Aurel DOSSOU KOHI
 * @brief The ResourceManager file. Contains the declaration of the
 * ResourceManager class
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 *
 */

#include <iostream>
#include <map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

/**
 * @brief The resource_type enum. Contains the possible types of resources
 * stored by the ResourceManager. TEXTURE for textures and FONT for fonts
 */
typedef enum { TEXTURE, FONT } resource_type;

/**
 * @brief The ResourceManager singleton class. Manages textures and fonts.
 * It contains a map of textures and a map of fonts that associates unique names
 * to each texture or font. It provides methods to load textures and fonts from
 * files and to get the corresponding sf::Texture or sf::Font.
 */
class ResourceManager {
  private:
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, sf::Font> m_fontMap;

    ResourceManager() {}

  public:
    ~ResourceManager() {}

    /**
     * @brief Returns the singleton instance of the ResourceManager
     *
     * @return ResourceManager&
     */
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

    /**
     * @brief Loads a texture or font from a file
     *
     * @param fileName the path to the file
     * @param id the unique name to be associated with the resource
     * @param type the type of the resource (TEXTURE or FONT)
     * @return true if the resource was loaded successfully
     * @return false if there was an error
     */
    bool load(
        const std::string& fileName, const std::string& id, resource_type type
    ) {
        if (type == FONT) {
            sf::Font font;

            if (!font.loadFromFile(fileName)) {
                return false;
            }

            m_fontMap[id] = std::move(font);
            return true;
        } else if (type == TEXTURE) {
            sf::Texture texture;

            if (!texture.loadFromFile(fileName)) {
                return false;
            }

            m_textureMap[id] = std::move(texture);
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief Get the Texture associated with the id
     *
     * @param id the unique name associated with the texture
     * @return sf::Texture&
     */
    sf::Texture& getTexture(const std::string& id) {
        try {
            return m_textureMap.at(id);
        } catch (const std::out_of_range& e) {
            std::cerr << "Texture not found: " << id << std::endl;
            exit(84);
        }
    }

    /**
     * @brief Get the Font associated with the id
     *
     * @param id the unique name associated with the font
     * @return sf::Font&
     */
    sf::Font& getFont(const std::string& id) {
        try {
            return m_fontMap.at(id);
        } catch (const std::out_of_range& e) {
            std::cerr << "Font not found: " << id << std::endl;
            exit(84);
        }
    }
};

#endif

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2026 Aurel DOSSOU KOHI *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
