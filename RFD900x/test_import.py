import odb_mavlink_v1

try:
    print(f"Succès ! L'ID du message est : {odb_mavlink_v1.MAVLINK_MSG_ID_ROCKET_TELEMETRY}")
    msg = odb_mavlink_v1.MAVLink_rocket_telemetry_message(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
    print("La classe MAVLink_rocket_telemetry_message est bien reconnue.")
except AttributeError:
    print("ERREUR : La constante MAVLINK_MSG_ID_ROCKET_TELEMETRY n'est toujours pas définie dans le fichier.")