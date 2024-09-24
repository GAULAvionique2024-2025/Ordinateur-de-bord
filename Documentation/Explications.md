# Explication de l'ordinateur de bord

```
- header.flightmode (toujours 01)
- header.pyro0 (toujours 0 car les igniters ne sont pas installées pour launch canada)
- header.pyro1 (toujours 0 car les igniters ne sont pas installées pour launch canada)
- header.accelerometer (0 s'il y a une erreur avec l'ICM pendant l'initialisation de la fusée, sinon 1)
- header.barometer (0 s'il y a une erreur avec le BMP280 pendant l'initialisation de la fusée, sinon 1)
- header.gps_fix (en tout temps si le gps a un fix avec les satellites)
- header.sd (0 s'il y a une erreur avec la carte sd pendant l'initialisation de la fusée, sinon 1)

- altitude_filtered (metre, float)
- temperature (C, float)
- latitude (deg, float)
- longitude (deg, float)
- gyroX (deg/s, float)
- gyroY (deg/s, float)
- gyroZ (deg/s, float)
- accX (G, float)
- accY (G, float)
- accZ (G, float)
- angle_roll_acc (deg, float) (yaw de la fusée)
- angle_pitch_acc (deg, float) (pitch de la fusée)
```

L'accéléromètre (ICM) est utilisé pour déterminer si la fusée est en mouvement ainsi que l'orientation en temps réel sur les trois axes de la fusée.

Si la fusée est à l'arrêt (Mach lock* désactivé), le baromètre BMP280 est utilisé pour savoir si la fusée est en montée ou en descente, ainsi que pour mesurer la température à l'intérieur de l'appareil.

Si la fusée est au sol (altitude inférieure à un seuil déterminé par le baromètre), aucune action n'est entreprise.

Si la fusée est en montée et dépasse une certaine vitesse, le Mach lock est activé, désactivant ainsi le baromètre, devenu instable pendant la période du Mach lock.

Une fois l'apogée atteinte et la descente amorcée (Mach lock désactivé), la charge du pyro0 (drogue chute)** est déclenché.

Lorsque le drogue est déployé et que l'altitude passe en dessous d'une certaine hauteur déterminée par le baromètre, le charge du pyro1 (main chute)*** est déclenché.

Les détailles concernant l'implémentation des composantes au niveau logiciel [Drivers](./Drivers.md)

*Le Mach lock est un mécanisme utilisé principalement en aérospatiale pour désactiver les capteurs barométriques lorsque la fusée dépasse la vitesse du son (Mach 1). À ces vitesses supersoniques, les capteurs sensibles comme les baromètres peuvent devenir instables en raison des perturbations créées par les ondes de choc. Le Mach lock prévient l'acquisition de données erronées pendant cette phase. Une fois que la fusée ralentit sous Mach 1, le Mach lock est désactivé, et les capteurs peuvent reprendre des mesures fiables. Cela est souvent crucial pour les systèmes de mesure d'altitude ou de vitesse basés sur la pression. ​[Référence](https://www.rocketryforum.com/threads/mach-buster-rocket-kit-go-fast-on-a-budget.140413/)

**Le parachute de drogue est le premier parachute déployé par la fusée au moment de l’apogée. Son rôle est de ralentir la descente tout en minimisant la dérive causée par les vents en haute altitude. Sa petite taille permet de limiter le déplacement de la fusée loin du site de lancement.

***Le parachute principal est déployé après la descente initiale de la fusée, généralement à basse altitude. Sa grande taille permet de ralentir significativement la descente, garantissant un atterrissage en douceur et sécurisé près du site de récupération.

Retour à la [page principale](../README.md)