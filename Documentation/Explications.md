# 🛰️ Explication de l'Ordinateur de Bord

Ce document décrit le fonctionnement du code de l'ODB1 utilisé pour la fusée du GAUL.

## 🌳 **Diagramme d'implémentation**

Voici le diagramme qui montre la liaisons entres les différents modules de l'ordinateur de bord:

Ajouter diagramme...

## 🛠️ **Données principales**

Voici les différentes variables suivies par l'ordinateur de bord pendant le vol:

```bash
- header.flightmode        : Mode de vol (toujours 01)
- header.pyro0             : 0 (igniters non connecté -> 0 sinon 1)
- header.pyro1             : 0 (igniters non connecté -> 0 sinon 1)
- header.accelerometer     : 0 si erreur avec l'ICM à l'initialisation, sinon 1
- header.barometer         : 0 si erreur avec le BMP280 à l'initialisation, sinon 1
- header.gps_fix           : 1 si GPS a un fix avec les satellites
- header.sd                : 0 si erreur avec la carte SD à l'initialisation, sinon 1

- altitude_filtered (m)    : Altitude filtrée (float)
- temperature (°C)         : Température interne (float)
- latitude (°)             : Latitude (float)
- longitude (°)            : Longitude (float)
- gyroX (deg/s)            : Vitesse de rotation sur l'axe X (float)
- gyroY (deg/s)            : Vitesse de rotation sur l'axe Y (float)
- gyroZ (deg/s)            : Vitesse de rotation sur l'axe Z (float)
- accX (G)                 : Accélération sur l'axe X (float)
- accY (G)                 : Accélération sur l'axe Y (float)
- accZ (G)                 : Accélération sur l'axe Z (float)
- angle_roll_acc (°)       : Angle de roulis (yaw) (float)
- angle_pitch_acc (°)      : Angle de tangage (pitch) (float)
```

## ⚙️ **Fonctionnement**

### **Accéléromètre (ICM20602)**
L'accéléromètre est utilisé pour déterminer le mouvement de la fusée et son orientation en temps réel sur les trois axes (X, Y, Z).

### **Baromètre (BMP280)**
Le baromètre est utilisé pour déterminer si la fusée est en montée ou en descente. Il mesure également la température à l'intérieur de la fusée. Lorsque la fusée dépasse une certaine vitesse, le **Mach lock** est activé, désactivant le baromètre pendant la phase supersonique. 

### **Mach Lock** 
Le Mach lock est un mécanisme qui désactive le capteur barométrique lorsque la fusée dépasse Mach 1, rendant le baromètre instable à ces vitesses. Une fois la vitesse redescendue sous Mach 1, le baromètre se réactive.

### **Déclenchement des charges pyrotechniques**
- **Pyro0 (Drogue chute)** : Déclenché à l'apogée lorsque la fusée commence sa descente.
- **Pyro1 (Main chute)** : Déclenché à basse altitude pour ralentir la fusée avant l'atterrissage.

## 📄 **Documentation complémentaire**
- [Composantes](./Composantes.md) : Détails sur les composants matériels utilisés.
- [Drivers](./Drivers.md) : Explication du code des pilotes logiciels.

## 🪂 **Informations sur les parachutes**

### **Drogue Chute**
Le **drogue chute** est un petit parachute qui est déployé à l'apogée pour stabiliser la fusée pendant la descente sans provoquer trop de dérive à cause des vents en haute altitude.

### **Main Chute**
Le **parachute principal** est déployé à basse altitude pour garantir un atterrissage en douceur et sécurisé près du site de récupération.

## 📘 **Références**
- [Explication du Mach Lock](https://www.rocketryforum.com/threads/mach-buster-rocket-kit-go-fast-on-a-budget.140413/)

🔙 Retour à la [page principale](../README.md)
