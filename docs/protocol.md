# RFC-RT001: R-TYPE Protocol v2.1

**Status**: Standard  
**Date**: 2025-09-23  
**Authors**: Akandé Philippe, Aïmane Alassane  
**Category**: Network Protocol

## Abstract

This document specifies the network communication protocol for R-Type, a a horizontally scrolling shooter arcade video game. The protocole use TCP for reliability and UDP for real time data transmission, with a binary serialization system.

## Table of contents

1. [Introduction](#1-introduction)
2. [General architecture](#2-general-architecture)  
3. [Message format](#3-format-des-messages)
4. [Protocol commands](#4-commandes-du-protocole)
5. [Interaction sequences](#5-séquences-dinteraction)
6. [Error handling](#6-gestion-derreurs)
7. [Security](#7-sécurité)
8. [Versioning](#8-versioning)

## 1. Introduction

### 1.1. Objectives

R-Type protocol enables communication between a central server and multiple clients to synchronize the player's state, objects and environments.

### 1.2. Convention
The keywords "MUST", "SHOULD", "MAY" in this document are interpreted according to RFC 2119.

---
## 2. General architecture
The protocol uses a hybrid approach:

- **TCP (Port 7890)**: Critical commands, authentication, game state
- **UDP (Port 7891)**: Real-time position updates, quantum physics
- **WebSocket (Port 7892)**: Web client support (optional)

### 2.2 Modèle de communication

```
Client                          Server
  |                               |
  |-- TCP Connection Setup ------>|
  |<-- Welcome + Session ID ------|  
  |-- Authentication ------------>|
  |<-- Auth Success/Failure -----|
  |                               |
  |-- UDP Registration ---------->|
  |<-- UDP Acknowledgment --------|
  |                               |
  |<== Game State Updates ======>| (UDP Stream)
  |-- Game on Actions ----------->|
  |<-- Action Results ------------|

```

### 2.3 Sessions et états

Each client hold a **session** identified by an UUID 128-bits. The possible state are :

- `DISCONNECTED`: Not connected
- `CONNECTING`: TCP connection in progress
- `AUTHENTICATING`: Authentication in progress
- `LOBBY`: Connected, waiting for a game
- `INGAME`: Participating in an active game
- `SPECTATING`: Observing a game

---

## 3. Message Format

### 3.1 General Structure (TCP)


| Type | Valeur | Description | Direction |
|------|---------|------------|-----------|
| HELLO | 0x01 | Initialisation connexion | C → S |
| WELCOME | 0x02 | Réponse d'accueil | S → C |
| AUTH | 0x03 | Authentification | C → S |
| AUTH_RESULT | 0x04 | Résultat authentification | S → C |
| JOIN_GAME | 0x05 | Rejoindre une partie | C → S |
| GAME_STATE | 0x06 | État complet du jeu | S → C |
| MODE_SETH | 0x07 | Mode activé | C → S |
| PLAYER_INFO | 0x08 | Informations du joueurs | S → C |
| PLAYER_UPDATE | 0x09 | Position joueur | Bidirectionnel |
| DISCONNECT | 0x0A | Déconnexion propre | Bidirectionnel |
| ERROR | 0x0F | Message d'erreur | S → C |


## 4. Commandes du protocole

### 4.1 HELLO (0x01) - Connexion initiale

**Direction** : Client → Serveur  
**Payload** :

```c
struct HelloPayload {
    int16_t  version;     // Version du protocole client
    int16_t  mode;            // 0=Normal, 1=Debug
    char     client_name[32];      // Nom du client (null-terminated)
};
```

### 4.2 WELCOME (0x02) - Réponse serveur

**Direction** : Serveur → Client  
**Payload** :

```c
struct WelcomePayload {
    uint128_t session_id;          // ID de session assigné
    int16_t   server_version;      // Version du serveur
    int16_t  max_sessions;         // Nombre max de sessions
    char      server_name[64];     // Nom du serveur
};
``` 

### 4.3 AUTH (0x03) - Authentification

**Direction** : Client → Serveur  
**Payload** :

```c
struct AuthPayload {
    int16_t  auth_type;            // 0=Guest, 1=Username/Pass, 2=Session Token
    char     username[32];         // Nom d'utilisateur (null terminated string)
    char     credential[64];       // Mot de passe hashé ou token
    uint128_t client_hash;          // Hash du client pour anti-cheat
};
```
Les noms des guests vont être assignés dynamiquement. Player1, Player2, ..., Player234

**Types d'authentification** :
- `0` (Guest) : Connexion anonyme sans credentials
- `1` (UserPass) : Username + mot de passe SHA256
- `2` (Token) : Token JWT ou OAuth2
Dans le cas où le client est déconnecté en pleine partie (crash du serveur/autre), il peut tout simplement
fournir son token de session pour qu'on lui restaure sa partie si son token est toujours valide

### 4.4 AUTH_RESULT (0x04) - Authentication Result

**Direction** : Serveur → Client
**Payload** :

```c
struct AuthResultPayload {
    int16_t  status_code;       // 0 = Success, 1 = Invalid credentials, 2 = Token expired, 3 = Error
    uint128_t session_token;    // Unique token for session resumption (only valid if status_code == 0)
    char auth_token[64];        // Authentication token for secured requests 
    uint32_t player_id;         // Unique ID assigned to the player by the server
    char     welcome_msg[64];   // Optional message ("Welcome player!", "Auth failed", etc.)
};

```
uint128_t : UUID

### 4.5 HELP COMMAND (0x05) - Get commands available

**Direction** : Client → Serveur
**Payload** :

```c
struct HelpRequest {
    int16_t type = 0x05;   // HELP command
};
```

### 4.6 HELP RESPONSE (0x06) - Get commands available

**Direction** : Serveur → Client
**Payload** :

```c
struct HelpResponse {
    int16_t type = 0x10;
    char available_commands[512];  
};
```
Available commands:
- NAME [new_name]
- CREATE_SESSION
- LAUNCH_SESSION [session_id]
- JOIN_SESSION [session_token]
- REGISTER [username, password]
- AUTH [username, password]
- DISCONNECT
- PROFILE [username]
- LIST_USERS

### 4.7 NAME (0x07) - Change username

**Direction** : Client → Serveur
**Payload** :

```c
struct NameRequest {
    int16_t type;               // 0x07
    char auth_token[64];        // Authentication token
    char new_name[32];          // Desired new username
};
```

### 4.7 NAME (0x08) - Name result

**Direction** : Serveur → Client
**Payload** :

```c
struct NameResponse {
    int16_t type;               // 0x08
    int16_t status_code;        // 0=Success, 1=Invalid auth_token, 2=Name Taken
};
```

### 4.8 CREATE_SESSION (0x09) — Create a Game Session

**Direction** : Client → Serveur
**Payload** :

```c
struct CreateSessionRequest {
    int16_t type;               // 0x09
    char auth_token[64];        // Auth token
    char session_name[32];      // Optional session name
};
```

### 4.9 SESSION_RESPONSE (0x10) — Game Session

**Direction** : Serveur → Client
**Payload** :

```c
struct CreateSessionResponse {
    int16_t type;               // 0x10
    int16_t status_code;        // 0=Success, 1=Invalid auth_token
    uint32_t session_id;        // Unique ID assigned to the session (for creator only)
    char join_code[8];          // Code to share with other players
};
```

### 4.10 LAUNCH_SESSION (0x11) — Launch a Game Session

**Direction** : Client → Serveur
**Payload** :

```c
struct LaunchSessionRequest {
    int16_t type;               // 0x11
    char auth_token[64];        // Auth token of session creator
    uint32_t session_id;        // ID of the session (creator only)
};
```

### 4.11 LAUNCH_SESSION (0x12) — Server Response

**Direction** : Serveur → Client
**Payload** :

```c
struct LaunchSessionResponse {
    int16_t type;               // 0x12
    int16_t status_code;        // 0=Success, 1=Invalid auth_token, 2=Not session owner, 3=Not enough players
};
```

### 4.12 JOIN_SESSION (0x13) — Join an Existing Session

**Direction** : Client → Serveur
**Payload** :

```c
struct JoinSessionRequest {
    int16_t type;               // 0x13
    char auth_token[64];        // Auth token
    char join_code[8];          // Code provided by session creator
};
```

### 4.13 JOIN_SESSION (0x14) — Join an Existing Session

**Direction** : Serveur → Client
**Payload** :

```c
struct JoinSessionResponse {
    int16_t type;               // 0x14
    int16_t status_code;        // 0=Success, 1=Invalid auth_token, 2=Invalid join_code, 3=Session Full
    uint32_t session_id;        // Assigned session ID for the client
};
```

### 4.14 REGISTER (0x15) — Create Account

**Direction** : Client → Serveur
**Payload** :

```c
struct RegisterRequest {
    int16_t type;               // 0x15
    char username[32];
    char password_hash[64];     // SHA256 hashed password
};
```

### 4.15 REGISTER (0x16) — Create Account

**Direction** : Serveur → Client
**Payload** :

```c
struct RegisterResponse {
    int16_t type;               // 0x16
    int16_t status_code;        // 0=Success, 1=Username Taken, 2=Invalid Data
    char auth_token[64];        // Provided for immediate use
};
```

### 4.16 DISCONNECT (0x17) — Disconnect

**Direction** : Client → Serveur
**Payload** :

```c
struct DisconnectRequest {
    int16_t type;               // 0x17
    char auth_token[64];
};
```

### 4.17 DISCONNECT (0x18) — Disconnect

**Direction** : Serveur → Client
**Payload** :

```c
struct DisconnectResponse {
    int16_t type;               // 0x18
    int16_t status_code;        // 0=Success, 1=Invalid auth_token
};
```

### 4.18 PROFILE (0x19) — Check Profile

**Direction** : Client → Serveur
**Payload** :

```c
struct ProfileRequest {
    int16_t type;               // 0x19
    char auth_token[64];
};
```

### 4.19 PROFILE (0x20) — Check Profile

**Direction** : Serveur → Client
**Payload** :

```c
struct ProfileResponse {
    int16_t type;               // 0x20
    int16_t status_code;        // 0=Success, 1=Invalid auth_token
    char username[32];
    uint32_t player_id;
    uint32_t games_played;
    uint32_t games_won;
};
```

### 4.20 LIST_USERS (0x21) — List Active Users

**Direction** : Client → Serveur
**Payload** :

```c
struct ListUsersRequest {
    int16_t type;               // 0x21
    char auth_token[64];
};
```

### 4.21 LIST_USERS (0x22) — List Active Users

**Direction** : Serveur → Client
**Payload** :

```c
struct ListUsersResponse {
    int16_t type;               // 0x22
    uint32_t user_count;
    char usernames[32][32];     // Max 32 users
};
```

### 4.22 PLAYER_MOVE (0x24) — Indicate that the player moves --> Broadcast to all team members

**Direction** : Client → Serveur
**Payload** :

```c
struct MoveResponse {
    int8_t type;               // 0x24
    int16_t player_id;
    Vector2D direction;
};
```

### 4.23 ENEMY_MOVED (0x37) — Indicate to all team members that an enemy moved 

**Direction** : Serveur → Client
**Payload** :

```c
struct EnemyMovedResponse {
    int8_t type; // 0x37
    int16_t enemy_id;
    enum enemy_type(1|2|3|4);
    Vector2D position;
};
```

### 4.24 NBR_ENTITY (0x38) — Indicate the numbers of entities 

**Direction** : Serveur → Client
**Payload** :

```c
struct NbrEntity {
    int8_t type; // 0x38
    int16_t nbr;
};
```

### 4.25 ACTION_RESPONSE (0x25) — Indicate if an action like (shoot, ..) is make

**Direction** : Client → Serveur
**Payload** :

```c
struct ActionResponse {
    int8_t type; // 0x25
    int16_t player_id;
    enum Action input;
};
```