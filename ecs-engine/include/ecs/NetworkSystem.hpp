/*
** EPITECH PROJECT, 2025
** network 
** File description:
** network
*/

#pragma once
#include "Registry.hpp"
#include "NetworkManager.hpp"

class NetworkSystem
{
    public:
        NetworkSystem(NetworkManager& net) : net(net)
    	{
    	}
        void update(Registry& registry, float dt);

    private:
        NetworkManager& net;
};
