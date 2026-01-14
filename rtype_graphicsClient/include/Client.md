# Client

## Description
`Client` envoie les commandes du joueur (clavier) au serveur, et affiche les réponses.

## Responsabilités
- Lire les entrées utilisateur (`up`, `down`, `left`, `right`).
- Envoyer un `MoveRequest (0x23)` vers le serveur.
- Lire les réponses du serveur (`MoveResponse`, etc.).
- Afficher les informations reçues.

## Fonctionnement

### `Client(int p, std::string address)`
- Initialise un `NetworkManager` en mode client.
- Lance un thread pour écouter l’input utilisateur.
- Boucle principale :
  - `poll()` pour traiter les messages.
  - Décodage des réponses (ex: `MoveResponse`).

### `MoveRequest getMoveKey()`
Retourne le dernier mouvement enregistré.

---

## Exemple d’utilisation
```cpp
Client cli(8080, "127.0.0.1");

while (true) {
    
}
