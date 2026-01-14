# NetworkManager

## Description
`NetworkManager` encapsule la gestion UDP via **ASIO** et fournit des méthodes pour envoyer, recevoir et gérer des messages asynchrones.

## Responsabilités
- Initialiser un socket UDP en mode serveur ou client.
- Envoyer des messages binaires (`send`).
- Réceptionner et stocker des messages (`receive`).
- Fournir une interface simple pour récupérer le dernier message reçu (`getLastMsg`).

## Méthodes principales

### `NetworkManager (int port, std::string address = "")`
- **Serveur** : si `address == "127.0.0.1"`, bind sur `port`.
- **Client** : sinon, bind sur un port libre.

### `void poll()`
Exécute les évènements réseau en attente (lecture/écriture).

### `void receive()`
Lance un `async_receive_from`, récupère les messages contenue dans un buffer.

### `void send (const int16_t* msg, size_t size, const asio::ip::udp::endpoint& client)`
Envoie un buffer de données à un destinataire.

### `std::pair <std::vector<int16_t>, asio::ip::udp::endpoint> getLastMsg()`
Retourne le dernier message reçu avec celui qui l'a envoyé.

### `asio::ip::udp::endpoint getLastSender()`
Retourne le dernier endpoint ayant envoyé un message.

---

## Exemple d’utilisation

```cpp

NetworkManager server(8080, "127.0.0.1");

while (true) {
    server.poll();
    auto msg = server.getLastMsg();
    if (!msg.first.empty()) {
        std::cout << "Message reçu (" << msg.first.size() << " bytes)" << std::endl;
    }
}
