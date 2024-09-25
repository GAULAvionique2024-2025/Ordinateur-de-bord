# 🧩 **Prise en main STM32CubeIDE**

## **Table des matières**
1. [Installation de STM32CubeIDE](#1-installation-de-stm32cubeide)
2. [Ouvrir un projet existant à partir de fichiers système](#2-ouvrir-un-projet-existant-à-partir-de-fichiers-système)
3. [Exploration de la structure des fichiers](#3-exploration-de-la-structure-des-fichiers)
4. [Consoles Problems et Tasks](##8-consoles-problems-et-tasks)
5. [Outils de Build](#4-outils-de-build)
6. [Débogage avec ST-Link](#5-débogage-avec-st-link)
7. [Exécution et Flash du programme](#6-exécution-et-flash-du-programme)
8. [Autres indications importantes pour STM32CubeIDE](#7-autres-indications-importantes-pour-stm32cubeide)

---

## ⬇️ **1. Installation de STM32CubeIDE**
Avant tout, vous devez installer STM32CubeIDE. Il est [téléchargeable](https://www.st.com/en/development-tools/stm32cubeide.html) gratuitement sur le site de STMicroelectronics. C'est un environnement de développement intégré (IDE) tout-en-un, qui inclut un compilateur, un éditeur de code, des outils de configuration de périphériques, ainsi que des outils de débogage.

## 📂 **2. Ouvrir un projet existant à partir de fichiers système**

### Cloner un dépôt Git
- Si vous travaillez à partir d’un dépôt Git, clonez-le dans le répertoire choisi en utilisant un client Git comme **Git Bash** ou un autre outil tel que **GitHub Desktop**.

### Ouvrir le projet dans STM32CubeIDE
- **Lancer STM32CubeIDE**.
- Allez dans **File > Open Projects from File System...**.
- Cliquez sur **Directory** et naviguez jusqu'au dossier cloné (celui contenant les fichiers de votre projet STM32).
- STM32CubeIDE analysera les fichiers et proposera les projets disponibles. Sélectionnez le projet pertinent.

### Projets basés sur `.ioc` (STM32CubeMX)
Si votre projet contient un fichier `.ioc`, c'est un fichier de configuration utilisé par **STM32CubeMX**, qui est intégré dans STM32CubeIDE. Ouvrez ce fichier pour reconfigurer les périphériques et générer des fichiers de code source (HAL, LL, etc.) automatiquement (utile au début, mais le projet est développé en low level **LL**).

## 🔍 **3. Exploration de la structure des fichiers**
Lorsque le projet est chargé, STM32CubeIDE organise les fichiers dans l'onglet **Project Explorer**, habituellement tout à gauche de l'interface :

### Header files (.h)
- Vous trouverez ces fichiers dans le dossier **Inc/** ou **include/**. Ils contiennent les définitions, les prototypes de fonctions et les déclarations des structures de données.

### Source files (.c)
- Les fichiers source se trouvent dans le dossier **Src/**. C’est là où vous écrivez le code source (implémentation des fonctions, logique principale, etc.).

### Drivers
- Ce dossier contient des fichiers relatifs aux bibliothèques STM32, telles que les fichiers HAL (Hardware Abstraction Layer), CMSIS (Cortex Microcontroller Software Interface Standard) et LL (Low Layer).

### Middlewares
- Si le projet utilise des bibliothèques de middleware comme FreeRTOS, USB, ou LwIP, ces fichiers seront stockés ici.

## 📋 **4. Consoles "Problems" et "Tasks"**

### Console "Problems"
- La console **Problems** affiche les erreurs et avertissements détectés dans votre projet. Elle vous aide à identifier rapidement les problèmes de compilation et de configuration. 
- Vous pouvez accéder à cette console via **Window > Show View > Problems**.

### Console "Tasks"
- La console **Tasks** vous permet de gérer les tâches, les rappels et les TODOs (//TODO) dans votre code. Vous pouvez y ajouter des notes ou des éléments à vérifier. 
- Accédez à cette console via **Window > Show View > Tasks**. Cela vous aide à garder une trace des éléments à améliorer ou des fonctionnalités à implémenter dans votre projet à travers le code.

## 🛠️ **5. Outils de Build**
STM32CubeIDE utilise **GNU Make** pour construire (compiler et lier) le projet. Voici les étapes pour réaliser le build de votre projet :

### Build simple
- Pour compiler le projet, cliquez sur le bouton 🔨 (en haut) ou utilisez le raccourci **CTRL+B**.
- Cela déclenchera le processus de compilation, et vous verrez les sorties dans la console de build (en bas de l'IDE). Si des erreurs ou des avertissements sont générés, vous pourrez les consulter ici.

### Configuration du build
- Vous pouvez ajuster les options de compilation dans **Project > Properties > C/C++ Build > Settings**. Vous pouvez, par exemple, modifier les optimisations du compilateur (par défaut -0n) ou ajuster les options de debug.

## 🐛 **6. Débogage avec ST-Link**

### Connexion du ST-Link
Le **ST-Link** est un outil de débogage et de programmation pour les microcontrôleurs STM32.

1. **Brancher** le ST-Link à la carte STM32 et à votre PC via USB.
2. STM32CubeIDE détecte normalement automatiquement le ST-Link connecté.

### Configuration de l'interface de débogage
- Allez dans **Run > Debug Configurations**.
- Choisissez **STM32 Cortex-M C/C++ Application** puis sélectionnez la configuration de débogage.
- Dans l'onglet **Debugger**, assurez-vous que le **ST-Link** est sélectionné comme interface de débogage.
  - Interface : **SWD (Serial Wire Debug)** est généralement le protocole utilisé.
  - Console : **SWV ITM Data Console** est utile lorsque des données sont **printf** sur le bus USB de l'ordinateur connecté au Debugger.

### Lancer le débogage
- Cliquez sur l'icône 🐞 pour démarrer le mode débogage. Cela compile le projet (si nécessaire), télécharge le binaire sur la carte, et arrête le processeur au point d'entrée (généralement dans `main()`).
- Vous pouvez ensuite utiliser des fonctionnalités comme les **points d'arrêt**, **step into/over** (avancer pas à pas dans le code), inspecter les **variables**, la **mémoire**, les **registres**, etc.

## 🏃‍♂️ **7. Exécution et Flash du programme**

### Lancer le programme sans débogage
- Pour exécuter directement votre programme sans mode debug, cliquez sur le bouton ▶️ (vert) ou utilisez **CTRL+F11**.
- Le binaire sera flashé sur la carte et le programme démarrera immédiatement après le téléchargement.

## 🔔 **8. Autres indications importantes pour STM32CubeIDE**

### Générer du code avec STM32CubeMX
- Si vous avez un fichier `.ioc`, vous pouvez l'ouvrir et ajouter des périphériques, ajuster la configuration (comme les GPIO, Timers, UART, I2C, etc.), et générer à nouveau le code. STM32CubeIDE synchronisera automatiquement les fichiers générés avec votre projet.

### Exploration de la documentation
- STM32CubeIDE fournit une intégration avec **la documentation CMSIS** et **les fichiers HAL/LL**. Faites un **CTRL+LMB** sur une fonction pour accéder directement à sa définition ou documentation dans le code.

---

🔙 Retour à la [page principale](../README.md)
