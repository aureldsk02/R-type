# 🧩 Case Study : Snapshot-Based vs Event-Based Networking

## 1. Contexte général

Dans les applications réseau temps réel — notamment les jeux en ligne, les simulateurs, ou les systèmes de robotique distribuée — le serveur doit **synchroniser fréquemment l’état du monde** entre plusieurs clients.

Deux approches dominantes existent pour cette synchronisation :

1. **Event-Based Networking**  
   → Le serveur envoie uniquement les **changements d’état** (événements) survenus depuis la dernière mise à jour.  
   Exemple : *“Le joueur 3 s’est déplacé de (x, y) à (x+2, y)”*.

2. **Snapshot-Based Networking**  
   → Le serveur envoie **une photographie complète (snapshot)** de l’état du jeu à intervalles réguliers.  
   Exemple : *“Voici les positions actuelles de tous les joueurs et entités.”*

---

## 2. Description des deux méthodes

### 🔹 Event-Based (Delta Updates)

**Principe :**  
Seuls les changements d’état (events) sont envoyés : mouvement, tir, spawn, destruction, etc.  
Les clients mettent à jour leur propre copie de l’état à partir des deltas reçus.

**Avantages :**
- Bande passante réduite, car on ne transmet que les différences.  
- Idéal pour des connexions stables à faible latence.

**Inconvénients :**
- Complexité élevée côté client (doit reconstruire et maintenir l’état cohérent).  
- Si un paquet est perdu, l’état peut devenir **désynchronisé**.  
- Requiert souvent des mécanismes de correction (ACK, rollback, interpolation).

---

### 🔹 Snapshot-Based

**Principe :**  
À intervalles réguliers (ex. toutes les 100 ms), le serveur envoie **un snapshot complet** de l’état global (positions, vitesses, statuts, etc.).  
Le client **remplace** ou **interpole** son état local à partir du snapshot.

**Avantages :**
- Résilience élevée aux pertes de paquets : un nouveau snapshot corrige automatiquement les erreurs précédentes.  
- Implémentation plus simple côté client.  
- Synchronisation cohérente de tous les joueurs et entités.

**Inconvénients :**
- Consomme plus de bande passante.  
- Peut introduire des pics réseau si les snapshots sont volumineux.

---

## 3. Scénario de test (cas d’usage)

**Contexte :** jeu multijoueur 2D type *R-Type*  
- 1 serveur → 4 clients  
- Chaque client envoie sa position et ses actions (tir, déplacement)  
- L’état du monde contient environ 100 entités (ennemis, tirs, bonus)

### Test A : Event-Based

- Le serveur envoie uniquement les actions et changements (tirs, déplacements).  
- Bande passante moyenne : **25 KB/s**  
- Lors d’une perte de 10 % des paquets :
  - Désynchronisation visible : positions incorrectes, tirs fantômes.  
  - Le client doit recourir à des correctifs locaux.

### Test B : Snapshot-Based

- Le serveur envoie un snapshot complet toutes les **100 ms** (~10 par seconde).  
- Taille d’un snapshot : 2 KB → **20 KB/s** au total.  
- Même avec 10 % de pertes :
  - Le client se resynchronise automatiquement à chaque nouveau snapshot.  
  - Aucune désynchronisation persistante observée.  
  - Latence perçue légèrement plus élevée (interpolation requise).

---

## 4. Analyse comparative

| Critère | Event-Based | Snapshot-Based |
|----------|--------------|----------------|
| **Bande passante** | Faible | Moyenne à élevée |
| **Résilience aux pertes** | Faible | Excellente |
| **Complexité client** | Haute | Faible |
| **Synchronisation globale** | Difficile | Automatique |
| **Latence visuelle** | Faible | Légèrement plus élevée (interpolation) |
| **Maintenance à long terme** | Complexe | Simple et stable |

---

## 5. Interprétation & mise en valeur du Snapshot-Based

Le **snapshot-based networking** s’impose comme une solution **robuste et fiable** pour les systèmes en **temps réel tolérant une légère latence**, comme les jeux coopératifs, les simulations, ou les environnements partagés.

- Il **garantit la cohérence globale** du monde même en présence de pertes réseau.  
- Il **simplifie** considérablement la logique client, réduisant les bugs de synchronisation.  
- Avec des optimisations comme la **compression différentielle** (envoi uniquement des parties du snapshot modifiées), on peut réduire la bande passante sans perdre les avantages de robustesse.  
- C’est aussi la méthode privilégiée par des moteurs comme **Unreal Engine (replication snapshots)** ou **Apex Legends (snapshot delta compression)**.

---

## 6. Conclusion

En résumé :  
> L’approche **snapshot-based** offre une meilleure **cohérence**, **simplicité** et **robustesse** face aux conditions réelles du réseau, au prix d’une consommation légèrement supérieure de bande passante.

Elle est donc particulièrement adaptée :
- aux jeux temps réel **multiclients**,  
- aux environnements **instables en latence**,  
- et aux systèmes nécessitant une **synchronisation continue** sans logique complexe de correction.

