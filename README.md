# 🛠️ Conception KiCad pour l'Ordinateur de Bord ODB1 🚀

Ce projet consiste à la conception d'une carte électronique pour l'ordinateur de bord de la fusée ODB1, réalisée avec **KiCad**. Voici un aperçu de l'approche utilisée pour la création du schéma et du circuit imprimé (PCB) de ce projet.

## 📦 Description du projet

L'ordinateur de bord ODB1 est conçu pour gérer et collecter des données provenant de différents capteurs embarqués dans la fusée. Cette carte doit être fiable, robuste et capable de transmettre des données en temps réel lors du vol. Pour ce faire, nous avons utilisé KiCad, un logiciel open-source qui permet de concevoir des schémas électroniques et de générer des circuits imprimés (PCB).

### 🔍 Étapes de conception

1. **Création du Schéma** : 
   Nous avons utilisé l'éditeur de schémas de KiCad pour définir les connexions entre les différents composants électroniques, tels que le microcontrôleur STM32, les capteurs de pression, les modules GPS et radio, etc. 
   
2. **Conception du PCB** :
   Après avoir finalisé le schéma, nous avons passé à la création du circuit imprimé. KiCad permet de positionner les composants sur la carte et de router les pistes électriques qui relient les différents éléments.
   
3. **Génération des fichiers Gerber** :
   Une fois le design terminé, nous avons généré les fichiers nécessaires à la fabrication du PCB, notamment les fichiers Gerber, qui contiennent toutes les informations pour produire la carte.

## 🧑‍💻 Objectif du projet

L'objectif principal est de réaliser une carte électronique compacte et performante, capable de supporter les conditions extrêmes d'un vol de fusée tout en maintenant une communication fluide avec la station au sol. Ce projet inclut également la transmission des données via radio et la collecte des mesures des capteurs embarqués.

## 🔧 Outils utilisés

- **KiCad** : Outil principal pour la conception du schéma et du PCB.

## 📅 Suivi de la progression

Les étapes de conception, de tests et de validation de la carte sont détaillées dans la documentation du projet. Chaque modification du schéma ou du PCB est suivie par des tests rigoureux pour garantir la qualité et la fiabilité de l'ordinateur de bord.

---

Pour en savoir plus sur les composants et les spécifications techniques, consultez [la documentation complète des composants ODB1](https://github.com/GAULAvionique2024-2025/Ordinateur-de-bord/tree/main/Documentation/ODB2)&#8203;:contentReference[oaicite:2]{index=2}.
