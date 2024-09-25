# **Ordinateur de bord GAUL 2024-2025**

![GAUL Banner](Documentation/GAUL/GAUL_Logo_Animated.gif)

## 🌟 **Main Repo pour l'ODB du GAUL**

Ce projet met en œuvre un **MCU** en association avec divers capteurs et modules pour créer un ordinateur de bord performant, optimisé pour un lancement de fusée de type L4. Ce système embarqué est conçu pour collecter et analyser des données en temps réel, permettant ainsi une évaluation précise des performances de la fusée.

## 📦 **Composants Intégrés**
L'intégration inclut :

- **Accéléromètre / Gyroscope**
  
  Un capteur de mouvement qui fournit des données sur l'accélération et l'orientation de la fusée.
  
- **Baromètre**

  Mesure la pression atmosphérique, permettant de calculer l'altitude.

- **Buzzer**
  
  Utilisé pour signaler des événements critiques ou des alertes pendant le vol.

- **Lecteur de carte SD**
  
  Permet d'enregistrer les données de télémétrie pour une analyse ultérieure.

- **Module GPS**
  
  Fournit des données de position pour suivre la trajectoire de la fusée.

- **Module radio externe**
  
  Assure la communication en temps réel avec la station de contrôle au sol.

- **Autres optionnels**
  
  ...

L’ensemble forme un système de télémétrie complet, idéal pour monitorer les performances et la trajectoire d’une fusée en temps réel. Pour plus d'informations détaillées sur le fonctionnement de chaque composant, consultez [les explications détaillées](./Documentation/Explications.md).

## 📷 Images de l'ODB
Découvrez une galerie illustrant le design et l'implémentation de l'ordinateur de bord :

[Voir la Showcase](./Documentation/Showcase.md)

<div style="display: flex; justify-content: space-around;">
  <img src="./Documentation/Showcase/ODB1_PCB.png" alt="ODB1" width="45%">
  <img src="./Documentation/Showcase/Rocket_Launch.jpg" alt="Rocket Launch" width="45%">
</div>

## 📚 Documentation
Toutes les informations sur les composants et les drivers sont disponibles ici :

- [Composantes](./Documentation/Composantes.md)
- [Drivers](./Documentation/Drivers.md)

## 🛠 Roadmap
Pour les fonctionnalités à venir et les améliorations prévues, veuillez consulter la [roadmap](./Documentation/Roadmap.md).

## 💡 Liens utiles
- Pour voir la première itération de l'ordinateur de bord, voir le projet [ODB1](https://github.com/GAULAvionique2023-2024/ODB1_Firmware).
- L'environnement de développement intégré pour STM32 est [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) et un documument est disponible pour permettre une [prise en main](./Documentation/Logiciel.md) initiale pour le projet.
- [deepbluembedded](https://deepbluembedded.com/stm32-arm-programming-tutorials/) pour ses tutoriels sur la programmation STM32.

## 👥 Auteurs et Contributeurs
- [@SamLol12](https://github.com/SamLol12)
- [@bestrider14](https://github.com/bestrider14)
- [@mathouqc](https://github.com/mathouqc)
- [Autres participants](./Documentation/Participants.md)
---
