# Ordinateur de Bord GAUL — Branche ODBlix

Bienvenue dans la branche **ODBlix** du projet *Ordinateur de Bord* de GAULAvionique.  
Cette version propose une architecture alternative / expérimentale / allégée (ajuster selon la réalité) du système embarqué, avec une organisation de dossier particulière et des versions de firmware différentes.

---

## Table des Matières

1. [Description générale](#description-générale)  
2. [Composants intégrés](#composants-intégrés)  
3. [Structure de la branche / organisation des dossiers](#structure-de-la-branche--organisation-des-dossiers)  
4. [Détails des versions](#détails-des-versions)  
5. [Installation & Build](#installation--build)  
6. [Roadmap & Fonctionnalités à venir](#roadmap--fonctionnalités-à-venir)  
7. [Contributeurs](#contributeurs)

---

## Description générale

La branche **ODBlix** propose une déclinaison de l’Ordinateur de Bord standard, conçue pour :

- réduire la complexité / le poids / la consommation (ajuster selon le but),
- tester de nouvelles versions expérimentales de firmware / de drivers,
- faciliter les comparaisons entre versions (V1, V3, etc.) dans un même environnement.

Elle s’appuie sur les mêmes principes que l’Ordinateur de Bord principal : collecte de données, télémétrie, enregistrement, communication, etc.

---

## Composants intégrés

ODBlix intègre (liste à vérifier selon ce qui est actif) :

- Accéléromètre / gyroscope  
- Capteur de pression / altimètre  
- Module GPS (position)  
- Lecteur de carte SD pour enregistrement de données  
- Module de communication radio pour la transmission vers le sol  
- Buzzer ou indicateurs sonores pour alertes / statuts  
- (Optionnel) capteurs auxiliaires selon version (température, orientation additionnelle, etc.)

---

## Structure de la branche / organisation des dossiers

Voici les principaux dossiers / fichiers, et leur rôle respectif :

| Dossier / fichier | Description |
|---|---|
| `ODB_V1/` | Version 1 du firmware / hardware : fonctionnalités de base, stabilité, tests initiaux. |
| `ODB_V2/` | Version 2 du firmware / hardware : stabilité, correction des problèmes de la V1 |
| `ODB_V3/` | Version 3 : ajout de fonctionnalités avancées / optimisation ; comparatif avec V2. |

---

## Détails des versions

- **ODB_V1** : version de base. Priorité sur la fiabilité et sur la collecte de données simples. Moins de fonctionnalités, interface minimale et non fonctionnel.
- **ODB_V2** : version de base. Priorité sur la fiabilité et sur la collecte de données simples. Moins de fonctionnalités, interface minimale avec correctifs, mais non testé.    
- **ODB_V3** : version plus avancée. Ajout / amélioration de modules, optimisations (ex : consommation, vitesse, taille), peut-être nouvelles fonctionnalités / capteurs.
- 
---
