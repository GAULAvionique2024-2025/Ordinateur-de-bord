# 🖥️ **Documentation des Composants de l'Ordinateur de bord**

Cette section fournit une vue d'ensemble détaillée des composants utilisés dans la conception de l'ordinateur de bord de la fusée **ODB1**. Vous trouverez ici les spécifications techniques et les descriptions des principaux modules, capteurs, et systèmes intégrés à l'ODB1.

> 🔧 **Schéma électrique :** indisponible.
---

## **Liste des Composants**

### 🖧 **MCU - [STM32F412ZGT6](./Composantes/STM32F412ZGT6.pdf)**

Le microcontrôleur **STM32F412ZGT6** assure la gestion centrale des opérations à bord, y compris la communication avec les capteurs et les modules périphériques.

---

### 🌡️ **Capteur de pression/température - [MS580301BA01](./Composantes/MS580301BA01-00-TE.pdf)**

Le **MS580301BA01** mesure la pression atmosphérique et la température, informations essentielles pour les calculs d'altitude en vol.

---

### 🌀 **Capteur d'accélération/gyroscope - [ICM-20948](./Composantes/ICM20948.pdf)**

Le **ICM-20948** permet de mesurer les accélérations linéaires sur trois axes, les rotations angulaires sur trois axes, ainsi que les variations magnetiques sur 3 axes. Ces données sont cruciales pour le suivi du comportement de la fusée.

---

### 🛰️ **Module GPS - [L76LM33](./Composantes/L76LM33.pdf)**


Le module **L76LM33** assure le suivi de la position en temps réel avec des informations précises de latitude et de longitude.

---

### 💾 **Mémoire Flash - [W25Q512JV](./Composantes/W25Q512JV.pdf)**

Le **W25Q512JV** stocke les données de vol recueillies par les capteurs, assurant une récupération facile après le vol.

---

### 📶 **Module Radio - [RFD900x](./Composantes/RFD900x.pdf)**

Le module radio **RFD900x** transmet les données de télémétrie à la station au sol en temps réel, garantissant une surveillance continue des paramètres de vol.

---

### 🧨 **Igniters**

Les igniters sont responsables du déclenchement des charges pyrotechniques pour déployer les parachutes lors de la phase de descente.

---

### 📲 **Module Bluetooth - [HM10_BLE](./Composantes/HM10_BLE.pdf)**

Le module **HM10** permet une communication courte portée pour l'armement et le diagnostic de l'ODB via Bluetooth.

---

### 📡 **Module Émetteur-Récepteur RF - [RFM22B](./Composantes/RFM22B-S2.pdf)**

Le module **RFM22B** permet une communication courte portée pour signaler la position approximatives des parties de la fusée.

---

### 🔊 **Buzzer - [SMTB-0927-TW-R](./Composantes/SMTB-0927-TW-R.pdf)**

Le buzzer **SMTB-0927** est utilisé pour émettre des alertes sonores indiquant les états ou les erreurs du système lorsque la fusée est armée.

---

### 💡 **LEDs d'états - [150080GS75000](./Composantes/150080GS75000.pdf)**

Plusieurs LEDs fournissent un support visuel pour indiquer rapidement les différents états de fonctionnement de la fusée et de ses modules.

---

### ❌ **LEDs critique - [150080RS75000](./Composantes/150080RS75000.pdf)**

Cette LEDs fournit un support visuel pour indiquer rapidement un état critique de fonctionnement de l'ordinateur de bord ou de l'un de ses modules.

---

> Retrouvez la documentation technique complète de chaque composant [ici](./Composantes/).

---

🔙 **Retour à la [page principale](../../README.md)**
