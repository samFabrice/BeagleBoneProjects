import Adafruit_BBIO.GPIO as GPIO
import time

# Définir les pins
triggerPin = "P8_16"  # P8_16 pour le trigger
echoPin = "P8_15"    # P8_15 pour l'écho

# Configurer les pins
GPIO.setup(triggerPin, GPIO.OUT)
GPIO.setup(echoPin, GPIO.IN)

# Fonction pour mesurer la distance
def get_distance():
    # Envoyer le signal de trigger
    print("Envoyer le signal de trigger...")
    GPIO.output(triggerPin, GPIO.HIGH)
    time.sleep(0.00001)  # 10 microsecondes de pulse
    GPIO.output(triggerPin, GPIO.LOW)

    # Attendre que l'écho soit reçu
    print("Attente de l'écho...")
    pulse_start = time.time()  # Initialiser pulse_start ici

    while GPIO.input(echoPin) == GPIO.LOW:
        # Afficher l'état du pin Echo
        print(f"État du pin Echo pendant l'attente: {GPIO.input(echoPin)}")
        if time.time() - pulse_start > 0.5:  # Ajouter un timeout de 0.5 seconde
            print("Timeout pendant l'attente de l'écho")
            return -1  # Retourner -1 pour indiquer une erreur

    print("Écho détecté, mesure du temps de réponse...")
    pulse_end = time.time() # Initialize pulse_end
    while GPIO.input(echoPin) == GPIO.HIGH:
        pulse_end = time.time()
        # Afficher l'état du pin Echo pendant la mesure
        print(f"État du pin Echo pendant la mesure: {GPIO.input(echoPin)}")
        if time.time() - pulse_end > 0.5: # Add a timeout of 0.5 second
            print("Timeout pendant la mesure de l'écho")
            return -1

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150  # Calculer la distance en cm
    return distance

try:
    while True:
        distance = get_distance()
        if distance != -1: # check the return value of get_distance()
            print(f"Distance mesurée: {distance:.2f} cm")
        else:
            print("Erreur de mesure de distance")
        time.sleep(1)

except KeyboardInterrupt:
    print("Programme interrompu")
finally:
    GPIO.cleanup()  # Nettoyer les pins
