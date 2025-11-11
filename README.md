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

## 📦 **Composants Intégrés**

L’intégration inclut :

- **Accéléromètre / Gyroscope**  
  Fournit des données de mouvement et d’orientation de la fusée.

- **Accéléromètre haute-G**  
  Mesure les accélérations extrêmes durant le lancement et la propulsion.

- **Altimètre**  
  Capteur de pression atmosphérique utilisé pour calculer l’altitude.

- **Capteur de température**  
  Permet de surveiller la température interne du compartiment avionique et des composants critiques.

- **Lecteur de carte SD**  
  Permet l’enregistrement de toutes les données de vol pour analyse post-lancement.

- **Mémoire externe**  
  Stockage complémentaire pour la sauvegarde de données critiques (redondance de télémétrie).

- **Module GPS + Breakout Board (lock/unlock switch)**  
  Fournit la position en temps réel, avec une **breakout board intégrée** GPS permettant de changer entre le module **lock** ou celui **unlock** avant le vol.

- **Module Radio externe**  
  Assure la communication en temps réel avec la station au sol pour la télémétrie et la supervision du vol.

- **Module Bluetooth**  
  Permet une communication locale rapide (configuration, test et debug sans fil).

- **Support Idéfix (Détecteur de balise RSSI)**  
  Intègre la compatibilité avec **Idéfix**, le système de repérage du GAUL, pour détecter les signaux radio des balises en phase de récupération.

- **Borniers de déclenchement (charges pyrotechniques)**  
  Sorties de puissance dédiées au **déploiement des parachutes** et à la **séparation des étages**.

- **Buzzer**  
  Sert à signaler les événements critiques ou à assister le débogage pendant les tests et le démarrage.

- **Autres modules optionnels**  
  ...

L’ensemble forme un **système de télémétrie complet**, capable non seulement de mesurer et enregistrer les données de vol, mais aussi d’agir activement sur les systèmes de la fusée au bon moment.

> Pour des explications détaillées sur le rôle et le fonctionnement de chaque composant, consultez :  
> - [Explications ODB2](./Documentation/ODB2/Explications.md)  
> - [Explications ODB1](./Documentation/ODB1/Explications.md)

## 📷 **Images de l'ODB**

Découvrez une galerie illustrant le design et l'implémentation de l'ordinateur de bord :

[Voir la Showcase](./Documentation/ODB1/Showcase.md)

<div style="display: flex; justify-content: space-around;">
  <img src="./Documentation/ODB1/Showcase/ODB1_PCB.png" alt="ODB1" width="45%">
  <img src="./Documentation/ODB1/Showcase/Rocket_Launch.jpg" alt="Rocket Launch" width="45%">
</div>

## 📚 **Documentation**

Toutes les informations sur les composants hardware / logiciel et les drivers sont disponibles ici :

### **Ordinateur de bord 1**
- [Composantes](./Documentation/ODB1/Composantes.md)
- [Drivers](./Documentation/ODB1/Drivers.md)

### **Ordinateur de bord 2**

- [Composantes](./Documentation/ODB2/Composantes.md)
- [Drivers](./Documentation/ODB2/Drivers.md)

> [Prise en main des logiciels](./Documentation/Logiciel.md)

## 🛠 **Roadmap**

Pour les fonctionnalités à venir et les améliorations prévues, veuillez consulter la [roadmap](./Documentation/Roadmap.md).

## 💡 **Liens Utiles**

- Pour voir la première itération de l'ordinateur de bord, consultez le projet [ODB1](https://github.com/GAULAvionique2023-2024/ODB1_Firmware).
- L'environnement de développement intégré pour STM32 est [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). Un document est disponible pour permettre une [prise en main](./Documentation/Logiciel.md) initiale pour le projet.
- [deepbluembedded](https://deepbluembedded.com/stm32-arm-programming-tutorials/) offre des tutoriels sur la programmation STM32.

> Ce projet suit les règles de [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) afin de garder un historique clair et cohérent.

## 👥 **Auteurs et Contributeurs**

- [@SamLol12](https://github.com/SamLol12)
- [@bestrider14](https://github.com/bestrider14)
- [@mathouqc](https://github.com/mathouqc)
- [Autres Participants](./Documentation/Participants.md)

---

🏠 Retour au la [page d'accueil](https://github.com/GAULAvionique)
