# 🖥️ **Documentation des Composants de l'Ordinateur de Bord**

Cette section fournit une vue d'ensemble détaillée des composants utilisés dans la conception de l'ordinateur de bord de la fusée **ODB1**. Vous trouverez ici les spécifications techniques et les descriptions des principaux modules, capteurs, et systèmes intégrés à l'ODB1.

> 🔧 **Schéma électrique :** Consultez le [schéma électrique de l'ODB1](./Composantes/shematic_v2.pdf) pour une visualisation complète de l'intégration des composants.

---

## **Liste des Composants**

### 🖧 **MCU - [STM32F103C8](./Composantes/stm32f103c8.pdf)**

Le microcontrôleur **STM32F103C8** assure la gestion centrale des opérations à bord, y compris la communication avec les capteurs et les modules périphériques.
<img src="./Composantes/STM32.png" alt="STM32" width="200"/>

---

### 🌡️ **Capteur de pression/température - [BMP280](./Composantes/bst-bmp280-ds001.pdf)**

Le **BMP280** mesure la pression atmosphérique et la température, informations essentielles pour les calculs d'altitude en vol.
<img src="./Composantes/BMP280.jpg" alt="BMP280" width="200"/>

---

### 🌀 **Capteur d'accélération/gyroscope - [ICM-20602](./Composantes/DS-000176-ICM-20602-v1.0.pdf)**

Le **ICM-20602** permet de mesurer les accélérations linéaires et les rotations angulaires sur trois axes, données cruciales pour le suivi du comportement de la fusée.
<img src="./Composantes/ICM.png" alt="ICM20602" width="200"/>

---

### 📡 **Module GPS - [L76LM33](./Composantes/Quectel_L76_Series_Hardware_Design_V3.1.pdf)**

Le module **L76LM33** assure le suivi de la position en temps réel avec des informations précises de latitude et de longitude.
<img src="./Composantes/L76L.jpg" alt="GPS L76LM33" width="200"/>

---

### 🔀 **Multiplexeur - [CD74HC4051-EP](./Composantes/cd74hc4051-ep.pdf)**

Le multiplexeur **CD74HC4051-EP** permet de sélectionner entre plusieurs signaux d'entrée analogiques, optimisant ainsi l'utilisation des broches GPIO du MCU.
<img src="./Composantes/Multiplexer.jpg" alt="Multiplexeur CD74HC4051" width="200"/>

---

### 💾 **Lecteur de carte SD - [MEM2067](./Composantes/MEM2067.pdf)**

Le lecteur **MEM2067** stocke les données de vol recueillies par les capteurs, assurant une récupération facile après le vol.
<img src="./Composantes/MEM.png" alt="Lecteur SD MEM2067" width="200"/>

---

### 📶 **Module Radio - [RFD900x](./Composantes/RFD900x%20DataSheet%20V1.2.pdf)**

Le module radio **RFD900x** transmet les données de télémétrie à la station au sol en temps réel, garantissant une surveillance continue des paramètres de vol.
<img src="./Composantes/RFD.png" alt="Module radio RFD900x" width="200"/>

---

### 🧨 **Igniters**

Les igniters sont responsables du déclenchement des charges pyrotechniques pour déployer les parachutes lors de la phase de descente.

---

### 📲 **Module Bluetooth - [HM10_BLE](./Composantes/11.%20HM10%20BLE_FTDI.pdf)**

Le module **HM10** permet une communication courte portée pour la configuration et le diagnostic de l'ODB via Bluetooth.
<img src="./Composantes/BLE.jpg" alt="Module Bluetooth HM10" width="200"/>

---

### 🔊 **Buzzer - [GT-0930RP2](./Composantes/GT-0930RP2-14-Feb-2019.pdf)**

Le buzzer **GT-0930RP2** est utilisé pour émettre des alertes sonores indiquant les états critiques ou les erreurs du système.
<img src="./Composantes/Buzzer.jpg" alt="Buzzer GT-0930RP2" width="200"/>

---

### 💡 **LED RGB**

La LED RGB fournit des signaux visuels pour indiquer différents états de fonctionnement de la fusée.

---

### 🔋 **Module de gestion de batterie Li - [MAX1898](./Composantes/max1898.pdf)**

Le **MAX1898** gère la charge et la distribution de l'alimentation depuis la batterie Li, assurant une autonomie optimale de l'ODB.

---

### 🔘 **Bouton poussoir - [LTC2954](./Composantes/2954fb.pdf)**

Le bouton poussoir **LTC2954** permet d'activer ou désactiver certaines fonctions de l'ODB manuellement.

---

Retrouvez la documentation technique complète de chaque composant [ici](./Composantes/).

---

🔙 **Retour à la [page principale](../README.md)**
