[![MasterHead](Documentation/GAUL/logo-full.webp)](https://gaulspace.web.app/home)

<h1 align="center">Ordinateur de bord GAUL 2024-2025</h1>

<img align="right" src="https://api.visitorbadge.io/api/visitors?path=https%3A%2F%2Fgithub.com%2FGAULAvionique2024-2025%2FOrdinateur-de-bord&label=Visiteurs&labelColor=%23697689&countColor=%23f47373&style=flat" alt="Visiteurs" />

<p align="left">
  <a href="https://www.facebook.com/gaul.ul" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/facebook.svg" alt="GAUL Facebook" height="30" width="40" />
  </a>
  <a href="https://www.instagram.com/gaul.ul/" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/instagram.svg" alt="GAUL Instagram" height="30" width="40" />
  </a>
  <a href="https://www.facebook.com/groupeaerospatialul/" target="_blank">
    <img src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/youtube.svg" alt="GAUL Youtube" height="30" width="40" />
  </a>
</p>

## 🌟 **Main Repo pour l'ODB du GAUL**

Ce projet met en œuvre un **MCU** associé à divers capteurs et modules pour créer un ordinateur de bord performant, optimisé pour un lancement de fusée de type L4. Ce système embarqué est conçu pour collecter et analyser des données en temps réel, permettant ainsi une évaluation précise des performances de la fusée.

## 📦 **Composants Intégrés**

L'intégration inclut :

- **Accéléromètre / Gyroscope**
  
  Un capteur de mouvement qui fournit des données sur l'accélération et l'orientation de la fusée.

- **Baromètre**
  
  Un capteur de pression atmosphérique, permettant de calculer l'altitude.

- **Buzzer**
- 
  Utilisé pour signaler des événements critiques ou des alertes pendant le déboggage ou le démarrage de l'ordinateur de bord.

- **Lecteur de carte SD**
  
  Module d'emplacement mémoire SD qui permet d'enregistre les données de télémétrie pour une analyse ultérieure.

- **Module GPS**
  
  Module de localisation qui fournit des données de position pour suivre la trajectoire de la fusée.

- **Module Radio externe**
  
  Module de communication qui sssure la communication en temps réel avec la station de contrôle au sol.

- **Autres Optionnels**  
  ...

L’ensemble forme un système de télémétrie complet, idéal pour monitorer les performances et la trajectoire d’une fusée en temps réel. Pour des informations détaillées sur le fonctionnement de chaque composant, consultez [les explications détaillées](./Documentation/Explications.md).

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

- [Prise en main des logiciels](./Documentation/Logiciel.md)

## 🛠 **Roadmap**

Pour les fonctionnalités à venir et les améliorations prévues, veuillez consulter la [roadmap](./Documentation/Roadmap.md).

## 💡 **Liens Utiles**

- Pour voir la première itération de l'ordinateur de bord, consultez le projet [ODB1](https://github.com/GAULAvionique2023-2024/ODB1_Firmware).
- L'environnement de développement intégré pour STM32 est [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). Un document est disponible pour permettre une [prise en main](./Documentation/Logiciel.md) initiale pour le projet.
- [deepbluembedded](https://deepbluembedded.com/stm32-arm-programming-tutorials/) offre des tutoriels sur la programmation STM32.

## 👥 **Auteurs et Contributeurs**

- [@SamLol12](https://github.com/SamLol12)
- [@bestrider14](https://github.com/bestrider14)
- [@mathouqc](https://github.com/mathouqc)
- [Autres Participants](./Documentation/Participants.md)

---

🏠 Retour au la [page d'accueil](https://github.com/GAULAvionique2024-2025)
