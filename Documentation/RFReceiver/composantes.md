# 🖥️ **Documentation des Composants de l'Ordinateur de bord**

Cette section fournit une vue d'ensemble détaillée des composants utilisés dans la conception du receiver RF. Vous trouverez ici les spécifications techniques et les descriptions des principaux modules, capteurs, et systèmes intégrés au receiver.

## **Liste des Composants**

### 🖧 **MCU - [STM32F103RE](./Composantes/stm32f103re.pdf)**

Le microcontrôleur **STM32F103RE** assure la gestion centrale des opérations à bord, y compris la communication avec les capteurs et les modules périphériques.

---

### 🧭 **Capteur magnétique / boussole - [MLX90393](./Composantes/MLX90393.PDF)**

Le **MLX90393** permet de mesurer le champ magnétique terrestre et déterminer l'orientation du receiver. Il est encore incertain si nous allons vraiment utiliser ce module. Il pourrait être utile à des fins de navigation.

---

### 📶 **Module Radio - [RFM22B-433-S2](./Composantes/RFM22B-S2.pdf)**

Le module radio **RFM22B-433-S2** capte le signal de balise de la fusée. Il est possible que le signal comprenne les coordonnées GPS de celle-ci, dans un tel cas ce module permet de lire ces coordonnées. Ce module permet aussi la lecture du RSSI (received signal strength indicator), qui permet de juger la distance et la direction de la fusée.

---

### 📺 **Écran LCD - [CN0296D](./Composantes/CN026D.pdf)**

L'écran LCD **CN0296D** permet un interface facile à utiliser et descriptif. C'est un interface à 20 colonnes et 4 lignes. Il peut être utilisé en I2C.\n **ATTENTION** Il faut rendre le module compatible en 3.3V en suivant ce tutoriel (sinon utiliser des logic converters):
https://www.instructables.com/5V-LCD-With-Arduino-Due-33V-I2C/

---

### 🔊 **Buzzer - [SMTB-0927-TW-R](./Composantes/SMTB-0927-TW-R.pdf)**

Le buzzer **SMTB-0927** est utilisé pour émettre un son proportionnel à la puissance du signal reçu.

---

### ⌨️ **Boutons**

Des pushbuttons seront utilisés pour interagir avec le receiver et le configurer. Au moins quatres boutons seront utilisés : 2 navigation, 1 "enter", 1 "config" ou "prog"

---

### 💡 **LEDs d'états - [150080GS75000](./Composantes/150080GS75000.pdf)**

Plusieurs LEDs fournissent un support visuel pour indiquer rapidement les différents états de fonctionnement du receiver. Ces leds risquent plus de servir au débugage qu'à l'utilisateur.

> Retrouvez la documentation technique complète de chaque composant [ici](./Composantes/).

---

🔙 **Retour à la [page principale](../../README.md)**
