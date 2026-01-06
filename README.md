[![MasterHead](Documentation/GAUL/logo-full.webp)](https://gaulspace.web.app/home)

<h1 align="center">Ordinateur de bord GAUL 2024-2026</h1>

<img align="right" src="https://api.visitorbadge.io/api/visitors?path=https%3A%2F%2Fgithub.com%2FGAULAvionique2024-2025%2FOrdinateur-de-bord&label=Visiteurs&labelColor=%23697689&countColor=%23f47373&style=flat" alt="Visiteurs" />

<p align="left">
  <a href="https://www.facebook.com/groupeaerospatialul" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/facebook.svg" alt="GAUL Facebook" height="30" width="40" />
  </a>
  <a href="https://www.instagram.com/gaul.ul/" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/instagram.svg" alt="GAUL Instagram" height="30" width="40" />
  </a>
  <a href="https://www.youtube.com/@projetGAUL" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/youtube.svg" alt="GAUL Youtube" height="30" width="40" />
  </a>
</p>

## 🌟 **Main Repo pour l'ODB du GAUL**

Ce projet met en œuvre un MCU associé à divers capteurs et modules pour créer un ordinateur de bord performant, optimisé pour un lancement de fusée de type L4. Ce système embarqué est conçu pour collecter et analyser des données en temps réel, permettant ainsi une évaluation précise des performances de la fusée.
De plus, l’ODB est responsable du déclenchement des charges pyrotechniques utilisées pour le déploiement des parachutes et la séparation des étages de la fusée, assurant ainsi la réussite et la sécurité des différentes phases du vol.
L’ensemble forme un système de télémétrie complet, capable non seulement de mesurer et enregistrer les données de vol, mais aussi d’agir activement sur les systèmes de la fusée au bon moment.

> Pour des explications détaillées sur le rôle et le fonctionnement de chaque composant, consultez :
> - [Explications ODB1](./Documentation/ODB1/Explications.md)
> - [Explications ODB2](./Documentation/ODB2/Explications.md)
> - [Explications ODB3 (ODBlix)](./Documentation/ODB2/Explications.md)

## ⚙️ Fonctionnalités de l’Ordinateur de bord (ODB)

Ce document présente une vue d’ensemble des fonctionnalités offertes par l’ordinateur de bord du GAUL.  
Il s’agit d’un résumé fonctionnel, indépendant des détails d’implémentation propres à chaque version.

### 📊 Acquisition et gestion des données
- Mesure des paramètres de vol (accélération, orientation, altitude, température, etc.)
- Acquisition des données à haute fréquence durant les phases critiques du vol
- Synchronisation temporelle des mesures
- Gestion simultanée de plusieurs capteurs
- Support de la redondance des sources de données

### 💾 Enregistrement et stockage
- Enregistrement complet des données de vol pour analyse post-lancement
- Stockage sur mémoire embarquée et/ou carte SD
- Structuration et horodatage des journaux de vol

### 📡 Télémétrie et communication
- Transmission des données de vol en temps réel vers la station au sol
- Communication radio longue portée
- Communication locale pour configuration, tests et débogage
- Support de protocoles de communication adaptés à l’avionique expérimentale

### 🚦 Gestion des événements de vol
- Détection automatique des différentes phases de vol (lancement, apogée, descente, atterrissage)
- Déclenchement conditionnel des événements critiques
- Commande des charges pyrotechniques (déploiement des parachutes, séparation des étages)
- Mécanismes de sécurité contre les déclenchements inattendus

### 🧭 Navigation et localisation
- Intégration des données de positionnement GPS
- Suivi de la trajectoire de vol
- Assistance à la récupération post-vol
- Compatibilité avec notre système de repérage externe ([IdéFIX](https://github.com/GAULAvionique/IdeFIX))

### 🔍 Diagnostic et supervision
- Surveillance de l’état du système en temps réel
- Indication des statuts de fonctionnement et des erreurs
- Outils d’aide au débogage pour les phases de test
- Retour d’information sonore et/ou visuel

### 🧪 Tests et validation
- Support des essais au sol
- Modes de test et de simulation
- Validation progressive avant intégration en vol
- Exploitation des données de vol pour amélioration continue

## 📚 **Documentation**

Toutes les informations sur les composants hardware / logiciel et les drivers sont disponibles ici :

### **Ordinateur de bord 1**
- [Composantes](./Documentation/ODB1/Composantes.md)
- [Drivers](./Documentation/ODB1/Drivers.md)

### **Ordinateur de bord 2**

- [Composantes](./Documentation/ODB2/Composantes.md)
- [Drivers](./Documentation/ODB2/Drivers.md)

### **Ordinateur de bord 3**

- [Composantes](./Documentation/ODB3/Composantes.md)
- [Drivers](./Documentation/ODB3/Drivers.md)

> [Prise en main des logiciels](./Documentation/Logiciel.md)

### 💡 **Liens Utiles**
- Pour voir la première itération de l'ordinateur de bord, consultez le projet [ODB1](https://github.com/GAULAvionique2023-2024/ODB1_Firmware).
- L'environnement de développement intégré pour STM32 est [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). Un document est disponible pour permettre une [prise en main](./Documentation/Logiciel.md) initiale pour le projet.
- [deepbluembedded](https://deepbluembedded.com/stm32-arm-programming-tutorials/) offre des tutoriels sur la programmation STM32.

> Ce projet essaye de suivre les règles de [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) afin de garder un historique clair et cohérent.

## 👥 **Auteurs et Contributeurs**

- [@SamLol12](https://github.com/SamLol12)
- [@bestrider14](https://github.com/bestrider14)
- [@mathouqc](https://github.com/mathouqc)
- [@vides119](https://github.com/vides119)
- [Autres Participants](./Documentation/Participants.md)

---

🏠 Retour à la [page d'accueil](https://github.com/GAULAvionique)
