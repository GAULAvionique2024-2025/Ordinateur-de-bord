# 🚀 **Sommaire de l'ordinateur de bord**

## **1. 🎯 Objectif du Projet**
Le projet vise à développer un **ordinateur de bord (ODB)** performant pour une fusée expérimentale. Cet ordinateur de bord intègre divers systèmes embarqués pour la gestion du vol, la détection de l’apogée, le déploiement des parachutes, et la collecte de données télémétriques en temps réel. L'objectif final est d'optimiser la fiabilité, la sécurité, et la performance énergétique du système embarqué.

## **2. 🔧 Composants Principaux**
### a. 🖥️ Microcontrôleurs STM32
Le projet utilise des microcontrôleurs **STM32** pour gérer les différentes fonctions embarquées comme le déclenchement des parachutes et la gestion des capteurs.

### b. 🪂 Parachutes
- **Drogue chute** : Un petit parachute déployé à l’apogée pour stabiliser la descente et réduire la dérive.
- **Main chute** : Le parachute principal déployé à basse altitude pour assurer un atterrissage en douceur.

### c. 📡 Systèmes de Télémétrie
Un module radio intégré permet la transmission de données vers une station au sol. Les données incluent la position, la vitesse, et d'autres informations critiques du vol. (Développement en cours...)

### d. 🧭 Capteurs
- **Capteurs de pression barométrique** : Utilisés pour détecter l’altitude et déterminer le moment optimal pour le déclenchement des parachutes.
- **Accéléromètres et gyroscopes** : Permettent de suivre la dynamique du vol (accélération, orientation, etc.).
- **Capteur GPS** : Pour la localisation précise de la fusée et la récupération après le vol.

### e. 🔔 Indicateurs Visuels et Sonores
- **LEDs d’état** : Utilisées pour indiquer les différents états du système (prêt au vol, apogée atteinte, chute libre, etc.).
- **Buzzer** : Sert à donner des indications sonores, par exemple pour signaler des anomalies ou des états spécifiques (défaillance, fin de mission).

### f. 🔋 Alimentation
- **Batteries lithium-polymère** : Fournissent l’énergie nécessaire pour les systèmes embarqués. La durée de vie et la gestion efficace de l’alimentation sont des aspects critiques du projet.

### g. 🔌 Connecteurs
Divers connecteurs pour assurer l'intégration entre les différents sous-systèmes et composants de l'ODB, garantissant une bonne connectivité physique et électrique.

## **3. 📅 Feuille de Route (Roadmap)**
### Phase 1 : 🔄 Développement Initial
1. **Gestion d'alimentation** : Assurer une alimentation fiable pour tous les composants embarqués.
   - [ ] Vérifier l'alimentation et les connexions.
   - [ ] Tester la durée de vie des batteries.

2. **Parachutes**
   - **Drogue chute** : Écrire le code pour le déclenchement à l’apogée et tester le mécanisme.
     - [ ] Développer et tester le code pour le déclenchement du parachute à l’apogée.
   - **Main chute** : Implémenter et tester le déclenchement à basse altitude.
     - [ ] Implémenter la logique pour le déclenchement à basse altitude.
   
3. **Indicateurs LED et Buzzer**
   - [ ] Développer le code pour les indications visuelles à l’aide des LEDs.
   - [ ] Créer des modèles sonores pour le buzzer, signalant différents états de l’ODB.
   
4. **Capteurs et Systèmes de Télémétrie**
   - [ ] Développer les routines de collecte de données pour les capteurs.
   - [ ] Tester la transmission des données par radio.

### Phase 2 : 🧪 Tests et Améliorations
1. **Itération des systèmes parachutes** : Affiner le fonctionnement des parachutes pour garantir un déploiement optimal selon l'altitude.
   - [ ] Vérifier le mécanisme de déploiement à différentes altitudes.
   
2. **Amélioration des communications radio** :
   - [ ] Augmenter la portée et le débit des données transmises.
   - [ ] Optimiser le module radio pour les conditions de vol.

3. **Optimisation des composants** :
   - Retirer des éléments non essentiels comme le module Bluetooth.
   - Modifier la gestion des LEDs RGB en faveur d'indicateurs visuels plus simples (LEDs d'états individuels).

4. **Amélioration de la puissance de traitement** :
   - Augmenter la puissance du processeur en utilisant la série F4 (STM32F4) pour des performances minimales requises.

### Phase 3 : 💡 Idées et Innovations
1. **Système de recharge** : Développer un système de recharge des batteries sur la plateforme de lancement.
2. **Module caméra** : Ajouter un module caméra au sol et à l'intérieur de la fusée pour capturer des images du vol.
3. **Répéteur radio** : Concevoir un système de répéteur radio via une charge utile éjectable.
4. **Systèmes modulaires CanBus** : Implémenter des sous-systèmes modulaires connectés via CanBus pour plus de flexibilité.

## **4. 🛠️ Outils utilisés**
- **STM32CubeIDE** : Un environnement de développement intégré pour la programmation et la configuration des microcontrôleurs STM32.
- **KiCad 8** : Un logiciel de conception de circuits imprimés (PCB), permettant de concevoir les circuits et de gérer les bibliothèques de composants.

## **5. 🛠️ Améliorations Préconisées (Version ODB2 par rapport à ODB1)**
- Améliorer la gestion des LEDs et du buzzer pour des indications d'état plus claires.
- Augmenter la puissance radio et améliorer le débit de transmission de données.
- Optimiser la durée de vie des batteries et améliorer la gestion de l'alimentation.
- Améliorer la robustesse des connecteurs et de la connectivité entre les composants.
