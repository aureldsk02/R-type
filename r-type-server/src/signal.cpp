/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-20
** File description:
** signal
*/
#include <csignal> // For signal handling functions and macros
#include <iostream> // For input/output operations
#include <cstdlib>  // For exit() function

void signalHandler(int signum) {
    std::cout << "Game end unexpectedly" << std::endl;
    exit(0);
}