# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)

import network          #Necesaria para crear el punto de acceso
from machine import Pin #Necesaria para controlar leds
import webrepl          #Necesario para usar herramienta webrepl

led = Pin(2, Pin.OUT) #Asignación al pin 2 (Led Azul) como pin de salida
led.on() #Encender led
ap = network.WLAN(network.AP_IF) # Crear Access Point
ap.active(True) # Activar
ap.config(essid='ESP32_Transmilenio') # Asignar nombre
ap.config(authmode=3, password='12345678') # Asignar contraseña

webrepl.start() #Habilitar acceso desde webrepl
led.off() #Apagar led
