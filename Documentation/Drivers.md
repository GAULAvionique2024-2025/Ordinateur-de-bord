# 🔌 **Drivers de l'ordinateur de bord**

Cette section présente une description des drivers programmés pour l'implémentation des composantes avec le IMU.

## **Table des Matières**
1.  [Driver GPIO](#driver-gpio)
2.  [Driver SPI](#driver-spi)
3.  [Driver USART](#driver-usart)
4.  [Driver TIM](#driver-tim)
5.  [Driver CRC](#driver-crc)
6.  [Driver Baromètre (BMP280)](#driver-baromètre-bmp280)
7.  [Driver Buzzer](#driver-buzzer)
8.  [Driver Multiplexeur (CD74HC4051)](#driver-multiplexeur-cd74hc4051)
9.  [Driver Accéléromètre (ICM20602)](#driver-accéléromètre-icm20602)
10. [Driver GPS (L76LM33)](#driver-gps-l76lm33)
11. [Driver Lecteur de carte SD (MEM2067)](#driver-lecteur-de-carte-sd-mem2067)
12. [Driver NMEA0183](#driver-nmea0183)
13. [Driver RFD900x](#driver-rfd900x)
14. [Driver Pyro](#driver-pyro)
15. [Driver Filtre Kalman](#driver-filtre-kalman)

---

## **Driver GPIO**:

Ce driver permet de gérer les ports GPIO (General Purpose Input/Output) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser les broches GPIO, lire leur état, écrire des valeurs et basculer leur état.

### Configuration des GPIO:
- **1. Activer l'horloge pour le port GPIO** : Activer l'horloge dans `RCC->AHB1ENR` (pour STM32F4) ou le registre approprié pour votre microcontrôleur.
- **2. Configurer le mode des broches** : Définir chaque broche en entrée, sortie, fonction alternative ou analogique dans le registre MODER (`GPIOx->MODER`).
- **3. Configurer la vitesse des broches** : Régler la vitesse (faible, moyenne, élevée) dans le registre OSPEEDR (`GPIOx->OSPEEDR`).
- **4. Configurer le type de sortie** : Définir le type de sortie (Push-Pull ou Open-Drain) dans OTYPER (`GPIOx->OTYPER`).
- **5. Configurer les résistances pull-up/pull-down** : Activer les résistances pull-up ou pull-down dans PUPDR (`GPIOx->PUPDR`).
- **6. Lire ou écrire sur les broches** : Lire l’état de la broche dans le registre IDR (`GPIOx->IDR`). Écrire sur une broche dans ODR (`GPIOx->ODR`).

### Options des Broches GPIO

- **Direction**: Mode d'une broche GPIO (Input, Output).
- **Options d'une broche Input**: Type (Analogique, Push-Pull).
- **Options d'une broche Output**: Type (Push-Pull, Open-drain, Alternative).
- **États**: 
  - `LOW`: État bas
  - `HIGH`: État haut

### Fonctions et explications du driver

Voici une présentation des fonctions utilisées.

```
void Init_GPIO(GPIO_TypeDef *port, unsigned short pin, unsigned short dir, unsigned short opt)
```
Cette fonction initialise un port GPIO en spécifiant le port, la broche, la direction et l'option souhaitées.
```
Exemple

Init_GPIO(GPIOA, 5, OUT10, O_GP_PP);
```
Ici, la broche PA5 est configurée pour être une sortie à une vitesse de 10MHz en mode push-pull.

```
int Read_GPIO(GPIO_TypeDef *port, unsigned short pin)
```
Cette fonction lit l'état d'une broche GPIO en mode Input et renvoie HIGH(1) ou LOW(0).
```
Exemple

int state = Read_GPIO(GPIOA, 5);
if (state == HIGH) {
    // L'état est HIGH
}
```
Ici, on compare l'état de la broche PA5 à HIGH, par exemple, lorsqu'on appuie sur un bouton.

```
void Write_GPIO(GPIO_TypeDef *port, unsigned short pin, unsigned short state)
```
Cette fonction écrit un état HIGH(1) ou LOW(0) sur une broche GPIO.
```
Exemple

Write_GPIO(GPIOA, 5, HIGH);
```
Dans cet exemple, l'état de PA5 est mis à HIGH.

## **Driver SPI**:

Ce driver permet de configurer et d'utiliser le protocole de communication SPI (Serial Peripheral Interface) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique SPI, transmettre des données, recevoir des données, et effectuer des opérations de transmission/réception simultanées.

### Configuration du SPI:
- **1. Activer l'horloge du périphérique SPI** : Activer l'horloge dans le registre de configuration du bus (ex: `RCC->APB2ENR` pour SPI1 ou `RCC->APB1ENR` pour SPI2).
- **2. Configurer le mode SPI** : Régler le SPI en mode maître ou esclave dans le registre de contrôle CR1 (bit MSTR pour maître).
- **3. Configurer les broches GPIO pour SPI** : Configurer les broches MOSI, MISO et SCK en mode fonction alternative via le registre `GPIOx->MODER`.
- **4. Configurer la vitesse de l'horloge SPI** : Régler la vitesse de l'horloge via les bits BR (Baudrate) dans le registre `SPIx->CR1`.
- **5. Choisir la taille des données** : Utiliser les bits DFF (Data Frame Format) dans `SPIx->CR1` pour définir la taille des données (8 ou 16 bits).
- **6. Activer le SPI** : Mettre à jour le bit SPE dans `SPIx->CR1` pour activer la communication SPI.
- **7. Écrire ou lire les données** : Les données sont envoyées et reçues via le registre de données DR (`SPIx->DR`). Possibilité de vérifier les statuts de transmission/réception via le registre de statut SR (bits TXE pour Transmission Empty et RXNE pour Réception).

### Fonctions et explications du driver

```
void SPI_Init(SPI_TypeDef *SPIx)
```
Cette fonction initialise le périphérique SPI spécifié (SPI1 ou SPI2). Elle configure les broches GPIO nécessaires pour le fonctionnement du SPI, y compris le CS (Chip Select), le CLK (horloge), le MISO (Master In Slave Out) et le MOSI (Master Out Slave In).
```
Exemple

SPI_Init(SPI1);
```
Dans cet exemple, le périphérique SPI1 est initialisé avec les broches appropriées.

```
int SPI_TX(SPI_TypeDef *SPIx, uint8_t *data, int size)
```
Cette fonction transmet un tableau de données via SPI. Elle envoie chaque octet de données en attendant que le buffer TX soit vide avant d'écrire le prochain octet.
```
Exemple

uint8_t txData[] = {0xAA, 0xBB, 0xCC};
int result = SPI_TX(SPI1, txData, sizeof(txData));
if (result == 0) {
    // Transmission réussie
} else {
    // Erreur de transmission
}
```

```
int SPI_RX(SPI_TypeDef *SPIx, uint8_t *data, int size)
```
Cette fonction reçoit des données via SPI. Elle envoie un octet "dummy" pour générer l'horloge nécessaire à la réception des données.
```
Exemple

uint8_t rxData[3];
int result = SPI_RX(SPI1, rxData, sizeof(rxData));
if (result == 0) {
    // Réception réussie, les données sont dans rxData
} else {
    // Erreur de réception
}
```

```
int SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
```
Cette fonction permet de transmettre et de recevoir des données en même temps. Elle utilise les buffers TX et RX pour envoyer et recevoir des octets simultanément.
```
Exemple

uint8_t txData[] = {0x01, 0x02, 0x03};
uint8_t rxData[3];
int result = SPI_TransmitReceive(SPI1, txData, rxData, sizeof(txData), 200);
if (result == 0) {
    // Transmission et réception réussies, les données reçues sont dans rxData
} else {
    // Erreur lors de la transmission/réception
}
```

### Détails techniques

- **Réception des données**: La fonction `SPI_RX` envoie un octet "dummy" pour générer l'horloge SPI, ce qui permet de synchroniser la réception des données à partir du périphérique esclave.
- **Transmission/Réception simultanées**: La fonction `SPI_TransmitReceive` permet d'effectuer une communication pleine duplex en utilisant les buffers TX et RX, ce qui optimise les opérations de communication.
- **Gestion des erreurs**: Chaque fonction de communication retourne un code d'état pour indiquer le succès ou l'échec de l'opération, permettant ainsi une gestion des erreurs appropriée.

## **Driver USART**:

Ce driver permet de gérer les communications série via les périphériques USART (Universal Synchronous/Asynchronous Receiver-Transmitter) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique USART, envoyer et recevoir des données en mode de sondage (polling).

### Configuration de l'USART:
- **1. Activer l'horloge de l'USART** : Activer l'horloge dans `RCC->APB1ENR` (USART2) ou `RCC->APB2ENR` (USART1, USART6).
- **2. Configurer les broches GPIO pour l'USART** : Configurer les broches TX et RX en mode fonction alternative via le registre `GPIOx->AFR`.
- **3. Configurer le baudrate** : Calculer la valeur du baudrate et configurer le registre BRR (`USARTx->BRR`), qui dépend de la fréquence du bus.
- **4. Configurer le mode de communication** : Activer la transmission (TE) et la réception (RE) dans le registre de contrôle CR1 (`USARTx->CR1`).
- **5. Configurer les bits de parité et le format des données** : Ajuster la parité (bit PCE pour la parité activée) et le format des données (bits M pour la taille des données, 8 ou 9 bits) dans `USARTx->CR1`. Configurer les bits d'arrêt dans le registre de contrôle `CR2` (bits STOP).
- **6. Activer l'USART** : Mettre à jour le bit UE dans `USARTx->CR1` pour démarrer l'USART.
- **7. Transmettre et recevoir des données** : Écrire les données dans le registre DR (`USARTx->DR`) pour la transmission. Vérifier les statuts via le registre SR (bit TXE pour Transmission Empty, RXNE pour Données reçues).

### Fonctions et explications du driver

```
void USART_Init(USART_TypeDef *USARTx, uint16_t baudrate, uint16_t frequency_MHz)
```
Cette fonction initialise un périphérique USART avec les paramètres spécifiés : taux de transmission (baud rate) et fréquence d'horloge du système. Elle configure également les broches GPIO associées pour la transmission (TX) et la réception (RX).
```
Exemple

USART_Init(USART1, 9600, 8);
```
Avec cet exemple, on initialise le périphérique connecté à UART1 en définissant son baudrate (9600bps) et la vitesse de transfert (8MHz).

```
int8_t USART_TX(USART_TypeDef *USARTx, uint8_t *data, uint16_t size)
```
Cette fonction transmet des données via USART en mode de sondage. Elle attend que le registre de données soit prêt à accepter de nouvelles données avant de les écrire. Elle retourne 0 si la transmission est réussie et -1 en cas de timeout.
```
Exemple

uint8_t message[] = "Hello, USART!";
if (USART_TX(USART1, message, sizeof(message)) < 0) {
    // Gérer l'erreur de transmission
}
```

```
int8_t USART_RX(USART_TypeDef *USARTx, uint8_t *data, uint16_t size)
```
Cette fonction reçoit des données via USART en mode de sondage. Elle attend que des données soient disponibles dans le registre de données avant de les lire. Elle retourne 0 si la réception est réussie et -1 en cas de timeout.
```
Exemple

uint8_t buffer[20];
if (USART_RX(USART1, buffer, sizeof(buffer)) < 0) {
    // Gérer l'erreur de réception
}
```

### Détails techniques

- **Format de données** :  L'USART est onfiguré pour 8 bits sans parité, avec un bit d'arrêt.
- **Attente de diffusion des bits** : Un timeout est utilisé pour éviter que le programme ne se bloque indéfiniment lors de l'attente de la disponibilité des registres.

## **Driver TIM**:

Le driver Timer permet de gérer les timers intégrés dans un microcontrôleur STM32. Il fournit des fonctions pour configurer et utiliser les timers pour diverses applications telles que la génération de PWM, la mesure du temps, ou la gestion d'événements périodiques par interruption.

### Configuration du Timer:
- **1. Activer l'horloge du Timer** : Activer l'horloge dans le registre `RCC->APB1ENR` pour les Timers sur APB1 (par exemple, TIM2) ou dans `RCC->APB2ENR` pour ceux sur APB2 (par exemple, TIM1).
- **2. Configurer le prescaler** : Définir le prescaler dans le registre **PSC** (`TIMx->PSC`) pour diviser la fréquence de l'horloge et ajuster la résolution du timer.
- **3. Configurer la période du timer** : Spécifier la période désirée dans le registre d'auto-rechargement **ARR** (`TIMx->ARR`) pour définir l'intervalle après lequel une interruption ou un événement se produit.
- **4. Configurer le mode du Timer** : Choisir le mode de fonctionnement du Timer (mode compteur, PWM, etc.) dans le registre **CR1** (`TIMx->CR1`). Configurer d'autres options dans le registre **CR2** si nécessaire.
- **5. Configurer les interruptions** (optionnel) : Activer les interruptions via le registre **DIER** (`TIMx->DIER`) pour être notifié lorsque des événements liés au Timer surviennent (comme une mise à jour ou un overflow).
- **6. Démarrer le Timer** : Mettre à jour le bit **CEN** dans le registre **CR1** (`TIMx->CR1`) pour commencer à compter.
- **7. Lire le statut ou réinitialiser** : Vérifier le registre de statut **SR** (`TIMx->SR`) pour suivre les événements tels que la mise à jour (bit **UIF**). Réinitialiser ces événements en écrivant dans le même registre.

### Options des Timers

- **Mode de comptage** : Le Timer peut être configuré pour compter en montée, descente ou dans les deux directions (bits DIR et CMS dans le registre CR1).
- **Prescaler** : Permet de diviser la fréquence du timer pour ajuster la résolution du comptage.
- **Mode PWM** : Le Timer peut être utilisé pour générer des signaux PWM, en configurant les sorties associées.
- **Interruptions et DMA** : Le Timer peut générer des interruptions ou des requêtes DMA lorsque certains événements se produisent.

### Fonctions et explications du driver

Voici une présentation des fonctions typiquement utilisées dans un driver Timer.
```
void Init_Timer(TIM_TypeDef *timer, uint32_t prescaler, uint32_t period)
```
Cette fonction initialise un Timer en spécifiant le timer utilisé, le prescaler et la période souhaitée.
```
void Start_Timer(TIM_TypeDef *timer)
```
Cette fonction démarre le Timer en activant le bit **CEN** dans le registre CR1 du Timer.
```
void Stop_Timer(TIM_TypeDef *timer)
```
Cette fonction arrête le Timer en désactivant le bit **CEN** dans le registre CR1 du Timer.
```
void Timer_IRQHandler(void)
```
Cette fonction est appelée lorsqu'une interruption du Timer survient (par exemple, à la fin de la période). Le bit **UIF** dans le registre SR est vérifié pour savoir si une mise à jour s'est produite, et il est ensuite effacé.

### Détails techniques

- **Initialisation du Timer** : La fonction `Init_Timer` configure le registre de pré-scaler (PSC) pour ajuster la fréquence de comptage, ainsi que le registre d'auto-rechargement (ARR) pour définir la période.
- **Démarrage et arrêt du Timer** : Le démarrage et l'arrêt du Timer se font en réglant ou en effaçant le bit CEN dans le registre CR1 du Timer.
- **Gestion des interruptions** : Lorsqu'une interruption survient (comme un overflow ou la fin d'une période), l'indicateur approprié (comme UIF) doit être effacé pour éviter d'autres interruptions intempestives.
- **Modes de fonctionnement avancés** : Les timers peuvent être configurés pour des applications plus complexes telles que le mode PWM, l'input capture ou l'output compare, en réglant les registres appropriés (CCR, CCMR, etc.).

## **Driver CRC**:

Ce driver permet de calculer un code de contrôle de redondance cyclique sur 16 bits (CRC16) sur un tableau de données. Le CRC est une méthode utilisée pour détecter les erreurs dans les données transmises ou stockées.

- **1. Activer l'horloge du module CRC** : Activer l'horloge dans le registre approprié (ex: `RCC->AHB1ENR` pour STM32).
- **2. Réinitialiser le calcul du CRC** : Utiliser le registre de contrôle CR (`CRC->CR`) pour réinitialiser le calcul CRC.
- **3. Entrer les données pour le calcul** : Charger les données dans le registre DR (`CRC->DR`).
- **4. Lire le résultat du CRC** : Lire la valeur calculée dans le registre DR après que les données aient été introduites.

### Fonctions et explications du driver

```
uint16_t CRC16_Calculate(uint8_t *data, uint8_t size)
```
Cette fonction calcule le CRC16 d'un tableau de données. Elle initialise le registre CRC, applique le CRC à chaque octet des données, et renvoie le résultat dans un entier non-signé de 16 bits.
```
Exemple

int8_t message[] = {0x01, 0x02, 0x03, 0x04};
uint16_t crc = CRC16_Calculate(message, sizeof(message));
```

### Détails techniques

- **Initialisation du registre CRC**: La fonction commence par initialiser le registre CRC à une valeur prédéfinie (généralement 0xFFFF ou 0x0000) pour assurer un calcul correct.
- **Calcul du CRC**: Pour chaque octet du tableau de données, le CRC est mis à jour en appliquant une série d'opérations logiques (généralement XOR et décalages) basées sur un polynôme prédéfini, qui détermine la forme du CRC.
- **Retour du résultat**: À la fin du traitement de tous les octets, le résultat final du CRC est renvoyé sous forme d'un entier non-signé de 16 bits, prêt à être utilisé pour la vérification de l'intégrité des données.
- **Utilisation du CRC**: Le CRC calculé peut être comparé à un CRC pré-calculé lors de la réception des données pour vérifier leur intégrité. Si les deux valeurs sont identiques, cela indique que les données n'ont pas été altérées.

## **Driver Baromètre (BMP280)**:

Le BMP280 est un capteur barométrique utilisé pour mesurer la pression atmosphérique et déterminer l'altitude. Ce driver permet de communiquer avec le BMP280 via SPI pour obtenir des mesures de température et de pression, et calculer l'altitude en conséquence.

### Fonctions et explications du driver

```
uint8_t BMP280_Init(BMP280 *dev)
```
Cette fonction initialise le capteur BMP280. Elle configure la communication SPI, effectue une réinitialisation, vérifie l'identifiant du capteur, et lit les données de calibration nécessaires.
```
Exemple

BMP280 sensor;
sensor.cs_port = GPIOA; // Port pour CS
sensor.cs_pin = GPIO_PIN_4; // Pin pour CS
if (BMP280_Init(&sensor) != 0) {
    // Gestion de l'erreur
}
```

```
void BMP280_Read_Temperature_Pressure(BMP280 *dev)
```
Cette fonction lit la température et la pression à partir du BMP280 et effectue les calculs nécessaires pour compenser les mesures.
```
Exemple

BMP280_Read_Temperature_Pressure(&sensor);
float altitude = sensor.altitude_m;
```

```
float BMP280_PressureToAltitude(float pressure, float sea_level_pressure)
```
Cette fonction convertit une pression donnée en altitude.
```
Exemple

float altitude = BMP280_PressureToAltitude(sensor.pressure_Pa / 100.0f, 1013.25f);
```

```
void BMP280_Read_Calib_Data(BMP280 *dev)
```
Cette fonction lit les données de calibration du capteur BMP280 se trouvant dans les buffers internes du module.

```
void BMP280_Write(BMP280 *dev, uint8_t address, uint8_t value)
```
Cette fonction écrit une valeur dans un registre du BMP280 via SPI pour sa configuration.

```
void BMP280_Read(BMP280 *dev, uint8_t address, uint8_t *rxData[], uint8_t size)
```
Cette fonction lit des données à partir d'un registre du BMP280 via SPI.

```
uint8_t BMP280_SwapMode(uint8_t mode)
```
Cette fonction change le mode de fonctionnement du BMP280 en consommation normal ou basse.

### Détails techniques

- **Initialisation**: Lors de l'initialisation, le driver configure la communication SPI en s'assurant que le capteur est correctement connecté et que les broches CS, CLK, MISO et MOSI sont bien configurées.
- **Lecture des mesures**: Les fonctions de lecture récupèrent les valeurs de température et de pression en accédant aux registres internes du BMP280. Ces valeurs sont ensuite compensées en fonction des données de calibration lues précédemment.
- **Calcul de l'altitude**: La conversion de la pression en altitude utilise la formule barométrique, prenant en compte la pression au niveau de la mer. La pression est convertie de pascals à hPa (hectopascals) en divisant par 100.0f.
- **Gestion des modes**: Le BMP280 peut fonctionner en mode normal ou basse consommation. La fonction `BMP280_SwapMode` permet de changer entre ces modes, affectant la fréquence des mesures et la consommation d'énergie.
- **Erreurs potentielles**: Lors de l'initialisation et des lectures, il est important de gérer les erreurs, telles que les échecs de communication SPI ou des valeurs invalides lues à partir des registres.


## **Driver Buzzer**:

Ce driver permet de contrôler un buzzer via un signal PWM (Modulation de Largeur d'Impulsion) sur un microcontrôleur STM32. Il est configuré avec différentes routines de son, telles que STOP, START, PENDING, ARMED et CRASH, chacune ayant des paramètres spécifiques comme le nombre de bips, la fréquence de départ et de fin, ainsi que des délais de pause et de modulation.

### Fonctions et descriptions du driver

```
void Buzz(TIM_TypeDef *TIMx, uint32_t channel, buzzRoutines_t routine)
```
```
Exemple

Buzz(TIM2, LL_TIM_CHANNEL_CH1, START);
```
Cela va produire un son avec les paramètres définis pour la routine START.

### Détails techniques

- **Configuration PWM**: Le driver configure le Timer (TIMx) spécifié pour générer un signal PWM. Les canaux peuvent être configurés pour différents types de modulation afin de produire des sons variés en ajustant la fréquence et le rapport cyclique.

## **Driver Multiplexeur (CD74HC4051)**:

Ce driver permet de contrôler un multiplexeur CD74HC4051 pour lire des tensions à partir de différents canaux d'entrée ainsi que pour tester des circuits pyrotechniques. Il utilise un microcontrôleur STM32 et communique avec des composants via des GPIO.

### Définition configuration MUX

Le driver utilise plusieurs constantes pour définir la configuration du multiplexeur :

- CHANNEL_0 à CHANNEL_7 : Identifiants pour les différents canaux du multiplexeur.
- PYRO_CHANNEL_0 et PYRO_CHANNEL_1 : Identifiants pour les différents pyrodispositifs.
- PYRO_CONTINUITY_THRESHOLD : Seuil pour déterminer si un pyrodispositif détecte une countinuité ou non.

### Fonctions et explications du driver

```
uint8_t CD74HC4051_Init(ADC_HandleTypeDef *hadc);
```
Cette fonction initialise le multiplexeur CD74HC4051 et configure les broches GPIO nécessaires pour son fonctionnement. Elle effectue également la calibration de l'ADC.
```
Exemple

ADC_HandleTypeDef hadc1;
CD74HC4051_Init(&hadc1);
```
Dans cet exemple, le multiplexeur est initialisé avec l'ADC spécifié.

```
uint16_t CD74HC4051_AnRead(ADC_HandleTypeDef *hadc, uint8_t channel, uint8_t pyro_channel, float vref);
```
Cette fonction lit une valeur analogique à partir d'un canal spécifique du multiplexeur. Elle active le pyrodispositif correspondant si nécessaire et retourne la valeur convertie en millivolts.
```
Exemple

uint16_t voltage = CD74HC4051_AnRead(&hadc1, CHANNEL_1, PYRO_CHANNEL_0, 3.3);
```
Dans cet exemple, la valeur de tension du canal 1 est lue et retournée en millivolts.

```
bool Pyro_Check(ADC_HandleTypeDef *hadc, uint8_t pyro_channel);
```
Cette fonction vérifie l'intégrité d'un pyrodispositif en mesurant la continuité à travers celui-ci et retourne un booléen indiquant si le test a réussi ou non.
```
Exemple

bool isFunctional = Pyro_Check(&hadc1, PYRO_CHANNEL_1);
```
Dans cet exemple, l'intégrité du pyrodispositif 1 est vérifiée.

### Détails techniques

- **Type de Multiplexeur** : CD74HC4051 est un multiplexeur/démultiplexeur analogique 8:3 qui permet de sélectionner un des 8 canaux de sortie.
- **Configuration GPIO** : Lors de l'initialisation (`CD74HC4051_Init`), les GPIO nécessaires pour le contrôle du multiplexeur et des dispositifs pyrotechniques sont configurées. Les pins de sélection de canal (MUL_S0, MUL_S1, MUL_S2) sont configurées en sortie, et la pin de contrôle **MUL_E** est configurée avec une résistance de pull-up, ce qui la maintient à un état **HIGH** par défaut. Cela permet de désactiver le multiplexeur lorsque le système est en repos. Seuls les canaux valides (0-6) sont utilisés, les canaux 1 et 7 non.
- **Contrôle des dispositifs pyrotechniques** : Avant de lire un canal, le driver peut activer un dispositif pyrotechnique associé. La fonction `Pyro_Check` permet de tester la continuité d'un canal pyro en utilisant le multiplexeur et en vérifiant la valeur ADC retournée. Cela garantit que le dispositif fonctionne comme prévu.
- **Lecture de l'ADC** : Pour lire les valeurs analogiques, la fonction `ADC_Sampling` est utilisée après avoir réactivé le multiplexeur. La valeur lue est ensuite convertie en millivolts en utilisant la référence de tension fournie (`vref`).

## **Driver Accéléromètre (ICM20602)**:

Ce driver permet de configurer et d'utiliser le capteur d'accéléromètre et de gyroscope ICM20602 sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique, mettre à jour les données des capteurs, calibrer les capteurs et lire/écrire des données via SPI.

### Fonctions et explications du driver

```
uint8_t ICM20602_Init(ICM20602 *dev)
```
Cette fonction initialise le périphérique ICM20602. Elle configure les registres nécessaires, réinitialise le capteur et vérifie si le capteur est correctement connecté.
```
Exemple

ICM20602 mySensor;
if (ICM20602_Init(&mySensor) != 0) {
    // Erreur d'initialisation
}
```

```
void ICM20602_Update_All(ICM20602 *dev)
```
Cette fonction met à jour toutes les données du capteur (accéléromètre et gyroscope). Elle lit les valeurs brutes des capteurs et effectue les conversions nécessaires pour obtenir les valeurs en degrés et en G.
```
Exemple

ICM20602_Update_All(&mySensor);
```
Les données du struct du baromètre (mySensor) sont maintenant mises à jour.

```
void ICM20602_Calibrate(ICM20602 *dev, int8_t p_Sense)
```
Cette fonction calibre le gyroscope du capteur. Elle ajuste les offsets des axes en fonction des valeurs lues jusqu'à ce qu'elles soient dans une plage acceptable.
```
Exemple

ICM20602_Calibrate(&mySensor, 5);
```
Calibre le gyroscope avec une précision de 5 digits.

```
int8_t ICM20602_Data_Ready(ICM20602 *dev)
```
Cette fonction vérifie si de nouvelles données sont disponibles à lire depuis le capteur.
```
Exemple

if (ICM20602_Data_Ready(&mySensor)) {
    // Données disponibles
}
```

```
void ICM20602_Read(ICM20602 *dev, uint8_t address, uint8_t rxData[], uint8_t size)
```
Cette fonction lit des données à partir d'un registre spécifique du capteur en utilisant SPI.

```
void ICM20602_Write(ICM20602 *dev, uint8_t address, uint8_t value)
```
Cette fonction écrit une valeur dans un registre spécifique du capteur en utilisant SPI.

## **Driver GPS (L76LM33)**:

Ce driver permet d'interagir avec le module GNSS L76LM33, utilisé pour obtenir la position (latitude, longitude) d'une fusée. Il gère la lecture des données via UART, le parsing des phrases NMEA, et le stockage des données dans une structure.

### Fonctions et explications du driver

```
int8_t L76LM33_Init(L76LM33 *L76_data, UART_HandleTypeDef *huart)
```
Cette fonction initialise le capteur L76LM33. Elle configure le gestionnaire UART, initialise un tampon circulaire pour les données reçues, et envoie des commandes de configuration au module.
```
Exemple

L76LM33 gpsSensor;
if (L76LM33_Init(&gpsSensor, &huart1) != L76LM33_OK) {
    // Erreur d'initialisation
}
```

```
void L76LM33_RxCallback(L76LM33 *L76_data, UART_HandleTypeDef *huart)
```
Cette fonction est appelée lorsqu'une donnée UART est reçue. Elle ajoute le byte reçu au tampon circulaire et vérifie si une nouvelle ligne a été détectée.
```
Exemple

L76LM33_RxCallback(&gpsSensor, &huart1);
```

```
int8_t L76LM33_Read(L76LM33 *L76_data)
```
Cette fonction lit et parse une phrase NMEA GPRMC dans la structure de données. Elle doit être appelée fréquemment pour avoir les dernières données GPS disponibles.
```
Exemple

if (L76LM33_Read(&gpsSensor) == L76LM33_OK) {
    // Données GPS mises à jour
}
```

```
int8_t L76LM33_Read_Sentence(L76LM33 *L76_data)
```
Cette fonction lit une phrase NMEA à partir du tampon circulaire et la stocke dans un tampon NMEA.
```
Exemple

if (L76LM33_Read_Sentence(&gpsSensor) == L76LM33_OK) {
    // Phrase NMEA lue avec succès
}
```

```
int8_t L76LM33_Send_Command(L76LM33 *L76_data, char command[], uint8_t size)
```
Cette fonction envoie une commande à l'L76LM33 en utilisant les fonctions UART HAL.
```
Exemple

char command[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*35\r\n";
if (L76LM33_Send_Command(&gpsSensor, command, sizeof(command)) != L76LM33_OK) {
    // Erreur lors de l'envoi de la commande
}
```

### Détails techniques

- **Configuration PMTK** : Le module L76LM33 utilise les commandes PMTK pour configurer les paramètres de fonctionnement.
- **Conversion NMEA0183** : Les données reçues du L76LM33 sont formatées selon le standard NMEA 0183. Le message GPRMC fournit des informations essentielles telles que la date, l'heure, la latitude, la longitude, la vitesse et le cap. Le parsing de ces données implique l'extraction de chaque champ du message NMEA pour une utilisation ultérieure dans l'application.

## **Driver Lecteur de carte SD (MEM2067)**:

Le driver MEM2067 permet d'interagir avec une carte SD, en facilitant les opérations de lecture, écriture, et gestion des erreurs.

### Fonctions et explications du driver

```
uint8_t MEM2067_Mount(const char* filename);
```
Monte le système de fichiers de la carte SD en FAT. Si le fichier est vide, il écrit les en-têtes associées à chacune des données brutes à enregistrer pendant le vol.
```
Exemple

if (MEM2067_Mount("data.txt")) {
    // Monté avec succès
} else {
    // Échec du montage
}
```

```
void MEM2067_Write(const char *filename, const DataField data[], size_t num_fields);
```
Écrit des données dans un fichier spécifié.
```
Exemple

DataField data[] = {
    {DATA_TYPE_INT, {.i = 42}},
    {DATA_TYPE_FLOAT, {.f = 3.14}},
    {DATA_TYPE_STRING, {.str = "Hello"}}
};
MEM2067_Write("data.txt", data, 3);
```

```
char *MEM2067_Read(const char *filename);
```
Lit une ligne de données à partir d'un fichier spécifié.
```
Exemple

char* data = MEM2067_Read("data.txt");
```

```
void MEM2067_Unmount(void);
```
Démontre le système de fichiers de la carte SD et doit être placé avant l'arrêt de fonctionnement de l'ordinateur de bord.

```
const char* FATFS_ErrorToString(FRESULT result);
```
Convertit un code d'erreur FATFS en chaîne de caractères pour un affichage convivial.
```
Exemple

FRESULT result = f_mount(&fs, "/", 1);
if (result != FR_OK) {
    printf("Erreur: %s", FATFS_ErrorToString(result));
}
```

### Détails techniques

- **Intégration de FATFS dans les middleware du STM32** : Ce driver utilise la bibliothèque FATFS, intégrée dans les middleware du STM32, pour gérer le système de fichiers sur la carte SD. FATFS fournit une interface standard pour accéder aux fichiers et permet de travailler avec des systèmes de fichiers FAT. L'intégration dans les middleware facilite l'utilisation des fonctions de gestion de fichiers, offrant une abstraction de bas niveau pour interagir avec la carte SD sans avoir à gérer directement les détails matériels.
- **Format de fichier** : Le code génère par défaut un classeur .csv organiser selon le type de données pour facilité le traitement de toutes les données enregistrées.

## **Driver NMEA0183**:

Le driver NMEA0183 permet de parser une phrase NMEA de type RMC pour extraire l'heure, la latitude et la longitude.

### Fonctions et explications du driver

Ce driver permet de récupérer efficacement les informations de localisation à partir de phrases NMEA standard, facilitant l'intégration des données GPS dans votre projet.

## **Driver RFD900x**:

Le driver RFD900X permet d'initialiser et d'envoyer des données via un module de communication RFD900.

### Fonctions et explications du driver

```
uint8_t RFD900_Init(RFD900 *devRFD);
```
Cette fonction initialise la structure RFD900 en définissant les valeurs de l'en-tête, des données, du CRC, et de la taille à zéro.
```
Exemple

RFD900 rfd900_device;
RFD900_Init(&rfd900_device);
```

```
uint8_t RFD900_Send(RFD900 *devRFD);
```
Cette fonction envoie les données à travers le module RFD900 en utilisant le protocole de communication défini. Elle inclut un en-tête, les données, un CRC, et une fin de message.
```
Exemple

rfd900_device.header = 0x01; // Exemple d'en-tête
rfd900_device.data = "Hello"; // Exemple de données
rfd900_device.size = strlen(rfd900_device.data);
rfd900_device.crc = CalculateCRC(rfd900_device.data, rfd900_device.size);
RFD900_Send(&rfd900_device);
```

## **Driver Pyro**:

Le driver Pyro permet de contrôler des dispositifs pyrotechniques, comme des allumeurs, en utilisant des GPIO pour activer ou désactiver les signaux de déclenchement des parachûtes.

### Fonctions et explications du driver

```
void Pyro_Init(void);
```
Cette fonction initialise les GPIO nécessaires pour le contrôle des dispositifs pyrotechniques. Elle configure les pins en sortie et les met dans un état de repos.

```
uint8_t Pyro_Fire(bool armed, char pyro);
```
Cette fonction permet de tirer un dispositif pyrotechnique si le système est armé. Elle active le GPIO correspondant au pyrotechnique spécifié par l'argument pyro (0 ou 1), attend un court instant, puis désactive les GPIO.
```
Exemple

bool armed = true; // Exemple d'état armé
char pyro_to_fire = 0; // Exemple de pyrotechnique à tirer
Pyro_Fire(armed, pyro_to_fire);
```

### Détails techniques

- **Sécurité** : Le contrôle des dispositifs pyrotechniques nécessite des mesures de sécurité rigoureuses. Le driver inclut un mécanisme pour s'assurer que le système est en état "armé" avant de tirer. Cela évite des activations accidentelles. Il est crucial que la variable `armed` soit vérifiée avant d'envoyer un signal pour activer les dispositifs pyrotechniques.
- **Gestion des GPIO** : Le driver configure les GPIO utilisés pour le déclenchement des dispositifs. Les pins doivent être initialisées en mode sortie et mises dans un état de repos lors de l'initialisation. Cela permet d'assurer que les dispositifs ne s'activent pas de manière inattendue. Après le tir, les GPIO sont désactivées pour éviter toute activation continue.

## **Driver Filtre Kalman**

Ce driver implémente un filtre de Kalman pour les microcontrôleurs STM32, utilisé pour fusionner des données de capteurs, comme des gyroscopes et des accéléromètres. Il permet d'estimer précisément l'angle à partir des données bruitées.

### Détails techniques
- **Estimation de l'angle** : Le filtre de Kalman utilise les mesures du capteur pour estimer l'angle en prenant en compte le bruit et les erreurs des capteurs.
- **Covariance des erreurs** : Les matrices de covariance permettent de suivre les incertitudes des estimations du filtre.
- **Correction avec la mesure** : Le filtre utilise la mesure actuelle pour corriger ses prédictions et ainsi fournir une estimation plus précise.

---

🔙 Retour à la [page principale](../README.md).
