/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     UIHandler                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_UIHANDLER_HPP
    #define INCLUDED_UIHANDLER_HPP

/**
 * @file UIHandler.hpp
 * @author Aurel DOSSOU KOHI
 * @brief This file contains the definition of the UIHandler class
 * @version 0.1
 * @date 2025-11-05
 * 
 * @copyright Copyright (c) 2026 - Aurel DOSSOU KOHI
 * 
 */

#include <SFML/Graphics.hpp>

/**
 * @brief The UIHandler class is a customable class
 * It allows you to create diverses ui effects
 */
class UIHandler
{
    private:
        sf::RectangleShape fade_rect;
        sf::Text text;
        double t = 0.0;
    protected:
    public:
        /**
         * @brief A boolean to tell if the fade has ended
         * 
         */
        bool fade_ended = false;
        /**
         * @brief A boolean to tell if the fade has started
         * 
         */
        bool fade_started = false;
        /**
         * @brief Construct a new UIHandler object
         * 
         */
        UIHandler() {};
        /**
         * @brief Construct a new UIHandler object
         * 
         * @param text the text to display in the center of the screen
         * @param fade_in 
         */
        UIHandler(const std::string &text, bool fade_in);
        ~UIHandler() = default;

        /**
         * @brief Fades the screen to black
         * 
         * @param window the window
         * @param delta the amount of time elapsed since the previous frame
         */
        void fade_in(sf::RenderWindow &window, double delta);
        /**
         * @brief Fades out the screen from black
         * 
         * @param window the window
         * @param delta the amount of time elapsed since the previous frame
         */
        void fade_out(sf::RenderWindow &window, double delta);
        /**
         * @brief Draws your ui elements to the window
         * 
         * @param window the window to draw to
         */
        void draw(sf::RenderWindow &window);
};

#endif

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2026 Aurel DOSSOU KOHI                                                         *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */
