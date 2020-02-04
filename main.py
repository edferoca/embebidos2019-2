from machine import Pin #Libreria para acceder al hardware
from machine import UART #Libreria para acceder al hardware
import esp # Librería para liberar funciones que vienen por defecto
import utime #Librería para indicar tiempos de espera
import network # Librería que nos permite configurar redes
import machine
from umqttsimple import MQTTClient 
import ubinascii
pin1 = None #Se crea la variable pin1 vacía para luego asignarle una función
pin1 = Pin(2, Pin.OUT) #Se declara como variable de salida
pin1.value(1) #Se usa como indicador ya que es un led
uart = UART(1, 115200)    # init with given baudrate
pin1.value(0)   
esp.osdebug(None) # se desactivan mensajes del sistema
pin1.value(1)                     
uart.init(115200, bits=8, parity=None, stop=1, tx=17, rx=16) # Se iniciliza el uart por los pines tx y rx

wlan = network.WLAN(network.STA_IF) #Crea un objeto WLAN
wlan.active(True) #activa la red que acaba de definir
wlan.connect('Transmilenio','transmilenio') # Se conecta a una red con los argumentos dados
utime.sleep(1)
print(wlan.ifconfig())
# para crear el cliente mqtt es necesario definir unos parámetros
server = '192.168.0.15'
port=1880
user=''
passwd=''
CLIENT_ID = ubinascii.hexlify(machine.unique_id())
mqtt = MQTTClient(CLIENT_ID,server,port,user,passwd)
pin1.value(0)




# Se define un bucle infinito con el fin de que siempre se esté atento a cuando el uart reciba un dato
while True==True:
	pin1.value(1)
	val = uart.read() # Se crea la variable val con los datos que llegan por el uart. Esto es posible por la funcion .read()
	if val != None:
		print(val)
		mqtt.connect() # mediante connect y disconect garantizamos que solo cuando se tenga un dato que entregar se envíen los datos con mqtt 
		mqtt.publish('Datos',val) # publicamos en el tópico Datos el dato val con lo recibido del uart
		mqtt.disconect()

	else:
		utime.sleep(1)
	pin1.value(0)


​


