# 🛤️ **Roadmap de l'Ordinateur de Bord**

## Objectifs à Déterminer
- À déterminer...

## 🛠️ **Conception du PCB**
- [ ] Définir les spécifications du PCB
  - [ ] Établir les dimensions du PCB
  - [ ] Identifier les contraintes de puissance et de chaleur
- [ ] Sélectionner les composants
  - [ ] Choisir le microcontrôleur (ex: STM32F103C8)
  - [ ] Déterminer les capteurs nécessaires (ex: accéléromètre, baromètre)
  - [ ] Sélectionner les connecteurs et autres composants passifs
- [ ] Créer un schéma électrique
  - [ ] Intégrer les composants sélectionnés dans le schéma
  - [ ] Vérifier les connexions et les erreurs de circuit
- [ ] Concevoir le layout du PCB
  - [ ] Placer les composants sur le PCB
  - [ ] Tracer les pistes et optimiser le routage
  - [ ] Vérifier les règles de conception (DRC)
- [ ] Vérifier le design pour les erreurs
  - [ ] Réaliser une simulation de circuit si possible
  - [ ] Effectuer une revue de design avec l'équipe
- [ ] Commander le PCB
  - [ ] Choisir un fournisseur de PCB
  - [ ] Passer commande et suivre le statut
- [ ] Assembler le prototype
  - [ ] Solder les composants sur le PCB
  - [ ] Inspecter les soudures et vérifier les connexions
- [ ] Tester le prototype
  - [ ] Vérifier l'alimentation et les connexions
  - [ ] Tester chaque fonctionnalité (ex: capteurs, communication)

## 💻 **Programmation**
- [ ] Définir l'architecture logicielle
  - [ ] Établir l'organisation des fichiers et des modules
- [ ] Écrire le code pour le microcontrôleur
  - [ ] Initialiser les composants matériels
  - [ ] Implémenter les fonctions de base (ex: lecture de capteurs)
- [ ] Mettre en place la communication entre les modules
  - [ ] Configurer les protocoles de communication (ex: USART, SPI, I2C)
  - [ ] Tester la communication entre les différents modules
- [ ] Développer les drivers pour les composants
  - [ ] Écrire des drivers pour chaque capteur et module
  - [ ] Tester chaque driver individuellement
- [ ] Tester et déboguer le code
  - [ ] Effectuer des tests unitaires sur chaque fonction
  - [ ] Utiliser un débogueur pour identifier les problèmes
- [ ] Documenter le code et les fonctionnalités
  - [ ] Rédiger des commentaires dans le code
  - [ ] Créer un document de spécifications techniques
- [ ] Préparer le firmware pour la première itération
  - [ ] Créer un script de téléchargement du firmware
  - [ ] Tester le chargement du firmware sur le prototype
- [ ] Développer la boucle principale d'exécution pour le comportement de la fusée pendant un lancement
  - [ ] Créer une main loop
  - [ ] Établir une structure d'exécution des différents composants liés
  - [ ] Tester le chargement du firmware sur le prototype

---

🔙 Retour à la [page principale](../README.md)
