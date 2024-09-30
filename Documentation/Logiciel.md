# 🧩 **Prise en main STM32CubeIDE et Kicad 8**

Cette section vous guide à travers l'utilisation des deux principaux outils de développement utilisés dans le projet : STM32CubeIDE et KiCad 8. STM32CubeIDE est un environnement de développement intégré (IDE) puissant pour la programmation et la configuration des microcontrôleurs STM32. Il simplifie le processus de création de projets, depuis la configuration matérielle jusqu'au débogage du code.

KiCad 8, quant à lui, est un logiciel libre de conception de circuits électroniques. Il permet de créer des schémas, de concevoir des PCB (circuits imprimés) et de gérer les bibliothèques de composants. Ce guide vous aidera à comprendre les bases de ces outils, à configurer votre environnement de travail, et à démarrer vos premiers projets d’électronique embarquée et de conception de circuits.

## 📑 **Table des matières**
[STM32CubeIDE](#stm32cubeide)
  1. [Installation de STM32CubeIDE](#-installation-de-stm32cubeide)
  2. [Ouvrir un projet existant à partir de fichiers système](#-ouvrir-un-projet-existant-a-partir-de-fichiers-systeme)
  3. [Exploration de la structure des fichiers](#-exploration-de-la-structure-des-fichiers)
  4. [Consoles Problems et Tasks](#-consoles-problems-et-tasks)
  5. [Outils de Build](#-outils-de-build)
  6. [Débogage avec ST-Link](#-debugage-avec-st-link)
  7. [Exécution et Flash du programme](#-execution-et-flash-du-programme)
  8. [Autres indications importantes pour STM32CubeIDE](#-autres-indications-importantes-pour-stm32cubeide)

[Kicad 8](#kicad-8)
  1. [Installation de KiCad 8](#-installation-de-kicad-8)
  2. [Création d'un nouveau projet](#-creation-dun-nouveau-projet)
  3. [Exploration de l'interface utilisateur](#-exploration-de-linterface-utilisateur)
  4. [Gestion des bibliothèques de composants](#-gestion-des-bibliotheques-de-composants)
  5. [Conception du schéma](#-conception-du-schema)
  6. [Création du circuit imprimé (PCB)](#-creation-du-circuit-imprime-pcb)
  7. [Génération des fichiers de fabrication](#-generation-des-fichiers-de-fabrication)
  8. [Autres conseils utiles pour KiCad 8](#-autres-conseils-utiles-pour-kicad-8)

---

## **STM32CubeIDE**

<div style="display: flex; justify-content: space-around;">
  <img src="./Logiciel/STM32.png" alt="ODB1" width="25%">
</div>

STM32CubeIDE un environnement de développement intégré (IDE) tout-en-un, qui inclut un compilateur, un éditeur de code, des outils de configuration de périphériques, ainsi que des outils de débogage.

## ⬇️ **1. Installation de STM32CubeIDE**
Avant tout, vous devez installer STM32CubeIDE. Il est téléchargeable gratuitement sur le [site officiel](https://www.st.com/en/development-tools/stm32cubeide.html) de STMicroelectronics.
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
- La console **Tasks** vous permet de gérer les tâches, les rappels et les TODOs (`//TODO`) dans votre code. Vous pouvez y ajouter des notes ou des éléments à vérifier. 
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
- Pour exécuter directement votre programme sans mode debug, cliquez sur le bouton ▶️ (`vert`) ou utilisez **CTRL+F11**.
- Le binaire sera flashé sur la carte et le programme démarrera immédiatement après le téléchargement.

## 🔔 **8. Autres indications importantes pour STM32CubeIDE**

### Générer du code avec STM32CubeMX
- Si vous avez un fichier `.ioc`, vous pouvez l'ouvrir et ajouter des périphériques, ajuster la configuration (comme les GPIO, Timers, UART, I2C, etc.), et générer à nouveau le code. STM32CubeIDE synchronisera automatiquement les fichiers générés avec votre projet.

### Exploration de la documentation
- STM32CubeIDE fournit une intégration avec **la documentation CMSIS** et **les fichiers HAL/LL**. Faites un **CTRL+LMB** sur une fonction pour accéder directement à sa définition ou documentation dans le code.

---

## **Kicad 8**

<div style="display: flex; justify-content: space-around;">
  <img src="./Logiciel/Kicad.png" alt="ODB1" width="25%">
</div>

KiCad est un logiciel open source de conception assistée par ordinateur (CAO) dédié à la création de circuits imprimés (PCB).

## ⬇️ **1. Installation de KiCad 8**
Pour commencer, téléchargez KiCad 8 sur le [site officiel](https://www.kicad.org/download/). KiCad est un logiciel libre de conception de circuits imprimés qui inclut des outils pour la conception de schémas, la création de PCB, et la gestion des bibliothèques de composants.

## 📂 **2. Création d'un nouveau projet**
### Démarrer un projet
- **Lancez KiCad** et cliquez sur **New Project**.
- Choisissez un emplacement pour enregistrer votre projet et donnez-lui un nom.

## 🔍 **3. Exploration de l'interface utilisateur**
Lorsque vous ouvrez un projet, l'interface de KiCad se compose de plusieurs éléments clés :

### Barre d'outils
- En haut, vous trouverez la barre d'outils avec des icônes pour accéder rapidement aux différentes fonctionnalités.

### Zone de travail
- La zone de travail est où vous dessinerez vos schémas et vos circuits imprimés.

### Navigateur de projet
- À gauche, le navigateur de projet affiche tous les fichiers associés à votre projet, y compris les schémas et les fichiers PCB.

## 📋 **4. Gestion des bibliothèques de composants**
### Ajouter des bibliothèques
- KiCad utilise des bibliothèques pour stocker des composants. Pour ajouter des bibliothèques, allez dans **Preferences > Manage Footprint Libraries** et sélectionnez les bibliothèques nécessaires.

### Rechercher des composants
- Vous pouvez rechercher des composants via l'outil de recherche intégré lors de la conception de votre schéma.

## 🛠️ **5. Conception du schéma**
### Ouvrir l'éditeur de schéma
- Cliquez sur l'icône **Schematic Editor** pour ouvrir l'éditeur de schéma.

### Ajouter des composants
- Utilisez l'outil **Place Symbol** pour ajouter des composants à votre schéma.
- Connectez les composants avec l'outil **Place Wire**.

## 🖨️ **6. Création du circuit imprimé (PCB)**
### Passer à l'éditeur PCB
- Une fois le schéma terminé, ouvrez l'éditeur PCB en cliquant sur l'icône **PCB Layout Editor**.

### Placer les composants
- Importez votre schéma et placez les composants sur le PCB. Utilisez l'outil **Move** pour les repositionner.

### Routage
- Utilisez l'outil **Route** pour connecter les pads des composants selon le schéma.

## 📄 **7. Génération des fichiers de fabrication**
### Fichiers Gerber
- Une fois le PCB terminé, allez dans **File > Plot** pour générer les fichiers Gerber nécessaires à la fabrication de votre circuit imprimé.

### Exporter la liste des matériaux
- Utilisez l'outil **Generate Bill of Materials** pour créer une liste des matériaux nécessaires à la construction de votre projet.

## 🔔 **8. Autres conseils utiles pour KiCad 8**
### Sauvegarde fréquente
- Pensez à sauvegarder régulièrement votre travail pour éviter toute perte de données.

### Explorer la documentation
- KiCad propose une documentation complète accessible dans le menu d'aide ou via ce [site](https://docs.kicad.org/8.0/). Profitez-en pour vous familiariser avec toutes les fonctionnalités disponibles.

---

🔙 Retour à la [page principale](../README.md)
