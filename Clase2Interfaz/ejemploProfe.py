import tkinter as tk
from tkinter import ttk
import serial
from serial.tools import list_ports

conectar = None

def obtener_puertos_com():
    puertos_com = [port.device for port in list_ports.comports()]
    return puertos_com

def mostrar_puertos_com():
    combo_puertos['values'] = obtener_puertos_com()
    combo_puertos.set("")

def conectar_o_desconectar():
    global conectar
    if conectar is None:
        puerto_seleccionado = combo_puertos.get()
        conectar = conectar_puerto(puerto_seleccionado)
    if conectar:
        btn_conectar.config(text="Desconectar")
    else:
        desconectar_puerto(conectar)
        conectar = None
        btn_conectar.config(text="Conectar")

def conectar_puerto(puerto):
    try:
        conectar = serial.Serial(puerto, baudrate=9600, stopbits=1, parity='N', bytesize=8)
        return conectar
    except serial.SerialException as e:
        return None

def desconectar_puerto(conectar):
    if conectar:
        conectar.close()




# Crea la ventana de la interfaz grafica
ventana = tk.Tk(className=" LEDs")
ventana.geometry("305x350")
ventana.config(bg="#405158")

# Etiqueta para mostrar el titulo del proyecto
etiqueta = tk.Label(ventana, text="Encender LEDs", font=("Arial", 14), bg="#5C6B72", fg="white")
etiqueta.pack(side=tk.TOP, fill="both")

# Crear ComboBox para mostrar los puertos COM disponibles
combo_puertos = ttk.Combobox(ventana, state='readonly')
combo_puertos.place(x=5, y=35)

# Botón para actualizar la lista de puertos COM disponibles
btn_actualizar = tk.Button(ventana, text="Actualizar", command=mostrar_puertos_com)
btn_actualizar.place(x=235, y=30)

# Botón para conectarse al puerto COM seleccionado
btn_conectar = tk.Button(ventana, text="Conectar", command=conectar_o_desconectar)
btn_conectar.place(x=155, y=30)


# Actualizar la lista de puertos COM disponibles al abrir la ventana
mostrar_puertos_com()

################################################################################################

# Etiqueta para mostrar led1
etiqueta_led1 = tk.Label(ventana, text="LED1", font=("Arial", 20), padx=1, pady=10, bg="#49646F", fg="white")
etiqueta_led1.place(x=10, y=80)

# Etiqueta para mostrar led2
etiqueta_led2 = tk.Label(ventana, text="LED2", font=("Arial", 20), padx=1, pady=10, bg="#49646F", fg="white")
etiqueta_led2.place(x=10, y=170)

# Etiqueta para mostrar led3
etiqueta_led3 = tk.Label(ventana, text="LED3", font=("Arial", 20), padx=1, pady=10, bg="#49646F", fg="white")
etiqueta_led3.place(x=10, y=260)

# Envia el comando por el puerto seleccionado
def led1_on():
    conectar.write(b"LED1_ON\n")

def led1_off():
    conectar.write(b"LED1_OFF\n")

def led2_on():
    conectar.write(b"LED2_ON\n")

def led2_off():
    conectar.write(b"LED2_OFF\n")

def led3_on():
    conectar.write(b"LED3_ON\n")

def led3_off():
    conectar.write(b"LED3_OFF\r\n")


# Botón para encender el led1
boton1_ON = tk.Button(ventana, text="ON", font=("Arial", 24), padx=1, pady=1, bg="green4", fg="white",
activebackground="green2", activeforeground="black", command=led1_on)
boton1_ON.place(x=110, y=80)

# Botón para apagar el led1
boton1_OFF = tk.Button(ventana, text="OFF", font=("Arial", 24), padx=1, pady=1, bg="red4", fg="white",
activebackground="red2", activeforeground="black", command=led1_off)
boton1_OFF.place(x=200, y=80)

# Botón para encender el led2
boton2_ON = tk.Button(ventana, text="ON", font=("Arial", 24), padx=1, pady=1, bg="green4", fg="white",
activebackground="green2", activeforeground="black", command=led2_on)
boton2_ON.place(x=110, y=170)

# Botón para apagar el led2
boton2_OFF = tk.Button(ventana, text="OFF", font=("Arial", 24), padx=1, pady=1, bg="red4", fg="white",
activebackground="red2", activeforeground="black", command=led2_off)
boton2_OFF.place(x=200, y=170)

# Botón para encender el led3
boton3_ON = tk.Button(ventana, text="ON", font=("Arial", 24), padx=1, pady=1, bg="green4", fg="white",
activebackground="green2", activeforeground="black", command=led3_on)
boton3_ON.place(x=110, y=260)

# Botón para apagar el led3
boton3_OFF = tk.Button(ventana, text="OFF", font=("Arial", 24), padx=1, pady=1, bg="red4", fg="white",
activebackground="red2", activeforeground="black", command=led3_off)
boton3_OFF.place(x=200, y=260)


################################################################################################

ventana.mainloop()