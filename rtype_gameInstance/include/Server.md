# Server

## Description
`Server` utilise `NetworkManager` pour recevoir les requêtes des clients et y répondre selon le protocole **R-Type**.

## Responsabilités
- Écouter les messages des clients.
- Identifier le type de message (via `msg.first[0]`).
- Déléguer le traitement à une fonction (`recupMove`, `recupItem`, etc.).
- Envoyer la réponse appropriée.

## Principales fonctions

### `MoveResponse recupMove(...)`
- Décode un `MoveRequest (0x23)`.
- Affiche la direction du joueur.
- Retourne un `MoveResponse (0x24)`.

### `PickupItemResponse recupItem(...)`
- Génère une réponse `PickupItemResponse (0x27)` lorsqu’un joueur prend un objet.

### `PlayerStateResponse player_r(...)`
- Retourne un état de joueur (`0x28`) avec vie, score, niveau.

### `PlayerStateResponse2 player_r2(...)`
- Version étendue (`0x29`) avec état de partie (pause, en jeu).

---

## Exemple d’utilisation
```cpp

Server srv(8080);

while (true) {
    
}
