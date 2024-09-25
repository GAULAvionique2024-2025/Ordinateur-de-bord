# 🖥️ **Drivers de l'ordinateur de bord**

Cette section présente une description des drivers programmés pour l'implémentation des composantes avec le IMU.

## Table des Matières
1.  [Driver GPIO](#driver-gpio)
2.  [Driver SPI](#driver-spi)
3.  [Driver USART](#driver-usart)
4.  [Driver CRC](#driver-crc)
5.  [Driver Baromètre (BMP280)](#driver-baromètre-bmp280)
6.  [Driver Buzzer](#driver-buzzer)
7.  [Driver Multiplexeur (CD74HC4051)](#driver-multiplexeur-cd74hc4051)
8.  [Driver Accéléromètre (ICM20602)](#driver-accéléromètre-icm20602)
9.  [Driver GPS (L76LM33)](#driver-gps-l76lm33)
10. [Driver Lecteur de carte SD (MEM2067)](#driver-lecteur-de-carte-sd-mem2067)
11. [Driver NMEA0183](#driver-nmea0183)
12. [Driver RFD900x](#driver-rfd900x)
13. [Driver Pyro](#driver-pyro)


---

## **Driver GPIO**:

Ce driver permet de gérer les ports GPIO (General Purpose Input/Output) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser les broches GPIO, lire leur état, écrire des valeurs et basculer leur état.

### Configuration des Broches GPIO

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

## **Driver USART**:

Ce driver permet de gérer les communications série via les périphériques USART (Universal Synchronous/Asynchronous Receiver-Transmitter) sur un microcontrôleur STM32. Il fournit des fonctions pour initialiser le périphérique USART, envoyer et recevoir des données en mode de sondage (polling).

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

Le registre de contrôle (CR1) est configuré pour activer la transmission (TE) et la réception (RE).
La vitesse de transmission est définie par le registre BRR, calculée à partir de la fréquence d'horloge et du baudrate spécifié.
Le format de données est configuré pour 8 bits sans parité, avec un bit d'arrêt.
Un timeout est utilisé pour éviter que le programme ne se bloque indéfiniment lors de l'attente de la disponibilité des registres.
Des messages d'erreur peuvent être gérés en vérifiant la valeur de retour des fonctions de transmission et de réception.

## **Driver CRC**:

Ce driver permet de calculer un code de contrôle de redondance cyclique sur 16 bits (CRC16) sur un tableau de données. Le CRC est une méthode utilisée pour détecter les erreurs dans les données transmises ou stockées.

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

## **Driver Multiplexeur (CD74HC4051)**:

Ce driver permet de contrôler un multiplexeur CD74HC4051 pour lire des tensions à partir de différents canaux d'entrée ainsi que pour tester des circuits pyrotechniques. Il utilise un microcontrôleur STM32 et communique avec des composants via des GPIO.

### Définition broche GPIO

Le driver utilise plusieurs constantes pour définir la configuration du multiplexeur :

CHANNEL_0 à CHANNEL_7 : Identifiants pour les différents canaux du multiplexeur.

PYRO_CHANNEL_0 et PYRO_CHANNEL_1 : Identifiants pour les différents pyrodispositifs.

PYRO_CONTINUITY_THRESHOLD : Seuil pour déterminer si un pyrodispositif détecte une countinuité ou non.

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

🔙 Retour à la [page principale](../README.md).
