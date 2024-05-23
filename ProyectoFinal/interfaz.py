import tkinter as tk
import tkinter.ttk as ttk
import serial.tools.list_ports
from random import randrange
from PIL import ImageTk, Image
import threading
import time

global isOpen 
isOpen = False
dataList = []
global userN 
userN = ""
global saldo
saldo = 0
global Total
Total = 0
global Usuario
Usuario = ""

#  ------------------ FUNCIONES GENERALES -------------------
 
# funciones para conexión con el ESP32 por serial 
def serial_ports():
    if not serial.tools.list_ports.comports():
        return "Sin_Puertos"
    else:
        global ports
        ports = serial.tools.list_ports.comports()
        return ports
    
def connect():
    global serial_com
    if(cb.get() == "Sin_Puertos"):
        print("No se puede conectar")
    else:
        serial_com = serial.Serial(str(ports[cb.current()][0]),115200)
        global isOpen
        isOpen = True 
        port_connect_button["state"] = "disabled"
        port_disconnect_button["state"] = "normal"

def disconnect():
    global serial_com
    serial_com.close()
    global isOpen 
    isOpen = False 
    port_connect_button["state"] = "normal"
    port_disconnect_button["state"] = "disabled"


# funciones para la dinámica de los botones 
def on_hover(event):
    event.widget.configure(bg = "#624466")

def on_default(event):
    event.widget.configure(bg = "#AD3FBB")

# funciones que controla el ESP32

def lectura():
    numeros = ""
    if isOpen:
        global serial_com
        global saldo
        global userN 
        data = serial_com.readline().decode('ascii')
        data = data.rstrip()
        for i in data:
            if i == "I":
                userN = int(data[1])
                break
        for i in range(3, len(data)):
            numeros += data[i]
        
        numeros = numeros.replace(" ","")
        saldo = int(numeros)
        actualizacion()

    threading.Timer(1, lectura).start()

lectura()

def actualizacion():
    global Usuario
    if userN == 1:
        Usuario = "Tomás Muñoz"
        nombreUsuario.delete(1.0, tk.END)  
        nombreUsuario.insert(tk.END, Usuario)
        nombreUsuario.tag_config("center",justify = "center", font= "Magneto")
        nombreUsuario.tag_add("center", "1.0", "end")
        nombreUsuario.place(x = 28, y = 45)

        valor.delete(1.0, tk.END)  
        valor.insert(tk.END, "$"+str(saldo))
        valor.tag_config("center",justify = "center", font= "Magneto")
        valor.tag_add("center", "1.0", "end")
        valor.place(x = 28, y = 190)

        total.delete(1.0, tk.END)
        total.insert(tk.END, "$0")
        total.tag_config("center",justify = "center", font= "Magneto")
        total.tag_add("center", "1.0", "end")
        total.place(x = 20, y = 190)
    elif userN == 2:
        Usuario = "Santiago López"
        nombreUsuario.delete(1.0, tk.END)  
        nombreUsuario.insert(tk.END, Usuario)
        nombreUsuario.tag_config("center",justify = "center", font= "Magneto")
        nombreUsuario.tag_add("center", "1.0", "end")
        nombreUsuario.place(x = 28, y = 45)

        valor.delete(1.0, tk.END)  
        valor.insert(tk.END, "$"+str(saldo))
        valor.tag_config("center",justify = "center", font= "Magneto")
        valor.tag_add("center", "1.0", "end")
        valor.place(x = 28, y = 190)

        total.delete(1.0, tk.END)
        total.insert(tk.END, "$0")
        total.tag_config("center",justify = "center", font= "Magneto")
        total.tag_add("center", "1.0", "end")
        total.place(x = 20, y = 190)


def borrar():
    Usuario = "---"
    nombreUsuario.delete(1.0, tk.END)  
    nombreUsuario.insert(tk.END, Usuario)
    nombreUsuario.tag_config("center",justify = "center", font= "Magneto")
    nombreUsuario.tag_add("center", "1.0", "end")
    nombreUsuario.place(x = 28, y = 45)

    valor.delete(1.0, tk.END)  
    valor.insert(tk.END, "$0")
    valor.tag_config("center",justify = "center", font= "Magneto")
    valor.tag_add("center", "1.0", "end")
    valor.place(x = 28, y = 190)

    total.delete(1.0, tk.END)
    total.insert(tk.END, "$0")
    total.tag_config("center",justify = "center", font= "Magneto")
    total.tag_add("center", "1.0", "end")
    total.place(x = 20, y = 190)


def actualizar():
    global saldo
    global serial_com
    global isOpen
    dato = int(entrada.get())
    entrada.delete(0, tk.END)
    saldo += dato
    # valorNuevo = "V"+str(saldo)
    print(saldo)
    if isOpen:
        serial_com.write(saldo)
    total.delete(1.0, tk.END)
    total.insert(tk.END, "$"+str(saldo))
    total.tag_config("center",justify = "center", font= "Magneto")
    total.tag_add("center", "1.0", "end")
    total.place(x = 20, y = 190)


# ----------------    INTERFAZ   --------------------

# ventana de usuario 
root = tk.Tk()
root.title("Proyecto de Aula - Microcontroladores 2")
root.geometry("700x500")
root.configure(bg="#17161B")
# root.resizable(0,0)


frame = tk.Frame(root)
frame.pack()
frame.configure(bg = "#17161B")

# vista el selector y los botones de la comunicación serial
# serial port
serial_info_frame = tk.LabelFrame(frame, text = "Conexion Serial", bg = "#17161b", fg = "#fff")
serial_info_frame.grid(row = 0, column = 0, pady=15)

cb = ttk.Combobox(serial_info_frame, state = "readonly", value = serial_ports())
cb.current(0)
cb.grid(row = 0, column = 1)

actualizar_button = tk.Button(serial_info_frame, text= "Actualizar", bg = "#AD3FBB", fg="#fff", command=lambda: cb.configure(value=serial_ports()))
actualizar_button.grid(row= 0, column= 0)
actualizar_button.bind('<Enter>', on_hover)
actualizar_button.bind('<Leave>', on_default)


port_connect_button = tk.Button(serial_info_frame, text = "CONECTAR", bd = 1, fg = "#fff", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command = connect)
port_connect_button.grid(row = 0, column = 2)
port_connect_button.bind('<Enter>', on_hover)
port_connect_button.bind('<Leave>', on_default)

port_disconnect_button = tk.Button(serial_info_frame, text = "DESCONECTAR", bd = 1, fg = "#fff", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command = disconnect)
port_disconnect_button.grid(row = 0, column = 3)
port_disconnect_button["state"] = "disabled"
port_disconnect_button.bind('<Enter>', on_hover)
port_disconnect_button.bind('<Leave>', on_default)

for widget in serial_info_frame.winfo_children():
    widget.grid_configure(padx = 10, pady = 5)


# usuario y recarga 
secondFrame = tk.Frame(frame)
secondFrame.grid(row = 1, column = 0)
secondFrame.configure(bg = "#17161B")


# Info del usuario 
userFrame = tk.LabelFrame(secondFrame, bg = "#AE7CC0", fg = "#fff", highlightcolor = "#00044A", width = 300, height = 250)
userFrame.grid(row = 0, column = 0, padx = 30, pady = 50)


titulo = tk.Text(userFrame, bg= "#AE7CC0", width = 30, height = 13,  border= 0, pady=2)
titulo.insert(tk.END, "Usuario:")
titulo.configure(state="disabled")
titulo.tag_config("center",justify = "center", font= "Garamond")
titulo.tag_add("center", "1.0", "end")
titulo.place(x = 28, y = 20)




nombreUsuario = tk.Text(userFrame, bg= "#AE7CC0", width = 30, height = 13,  border= 0, pady=2)
nombreUsuario.insert(tk.END, Usuario)
# nombreUsuario.configure(state="disabled")
nombreUsuario.tag_config("center",justify = "center", font= "Magneto")
nombreUsuario.tag_add("center", "1.0", "end")
nombreUsuario.place(x = 28, y = 45)

imagenPlace = tk.Text(userFrame, bg= "#AE7CC0", width = 30, height = 13,  border= 0, pady=2)
imagenPlace.tag_config("center",justify = "center", font= "Magneto")
imagenPlace.tag_add("center", "1.0", "end")
imagenPlace.configure(state="disabled")
myimg = Image.open('usuario.png')
redim = myimg.resize((70,70))
photo = ImageTk.PhotoImage(redim)
imagenPlace.image_create(tk.END, image=photo)
imagenPlace.place(x = 108, y = 80)

titulo2 = tk.Text(userFrame, bg= "#AE7CC0", width = 30, height = 13,  border= 0, pady=2)
titulo2.insert(tk.END, "Actual:")
titulo2.configure(state="disabled")
titulo2.tag_config("center",justify = "center", font= "Garamond")
titulo2.tag_add("center", "1.0", "end")
titulo2.place(x = 28, y = 170)

valor = tk.Text(userFrame, bg= "#AE7CC0", width = 30, height = 13,  border= 0, pady=2)
valor.insert(tk.END, "$"+str(saldo))
valor.tag_config("center",justify = "center", font= "Magneto")
valor.tag_add("center", "1.0", "end")
valor.place(x = 28, y = 190)

# Para la recarga

recargaFrame = tk.LabelFrame(secondFrame, bg = "#B4D2E5", fg = "#fff", highlightcolor = "#00044A", width = 200, height = 250, )
recargaFrame.grid(row = 0, column = 2, padx = 30, pady = 50)


titulo3 = tk.Text(recargaFrame, bg= "#B4D2E5", width = 20, height = 13,  border= 0, pady=2)
titulo3.insert(tk.END, "Recarga:")
titulo3.configure(state="disabled")
titulo3.tag_config("center",justify = "center", font= "Garamond")
titulo3.tag_add("center", "1.0", "end")
titulo3.place(x = 20, y = 60)

entrada = tk.Entry(recargaFrame, width=10, justify="center")
entrada.place(x = 67, y =100)

titulo4 = tk.Text(recargaFrame, bg= "#B4D2E5", width = 20, height = 13,  border= 0, pady=2)
titulo4.insert(tk.END, "TOTAL:")
titulo4.configure(state="disabled")
titulo4.tag_config("center",justify = "center", font= "Garamond")
titulo4.tag_add("center", "1.0", "end")
titulo4.place(x = 20, y = 170)

total = tk.Text(recargaFrame, bg= "#B4D2E5", width = 20, height = 13,  border= 0, pady=2)
total.insert(tk.END, "$0")
total.tag_config("center",justify = "center", font= "Magneto")
total.tag_add("center", "1.0", "end")
total.place(x = 20, y = 190)


# Botones de borrar y cargar 

thirdFrame = tk.Frame(frame)
thirdFrame.grid(row = 2, column = 0)
thirdFrame.configure(bg = "#17161B")

borrarBtn = tk.Button(thirdFrame, text = "BORRAR", bd = 1, fg = "#00044A", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command = borrar)
borrarBtn.grid(row = 0, column = 0, padx = 160)
borrarBtn.bind('<Enter>', on_hover)
borrarBtn.bind('<Leave>', on_default)


recargaBtn = tk.Button(thirdFrame, text = "RECARGAR", bd = 1, fg = "#00044A", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command= actualizar)
recargaBtn.grid(row = 0, column = 1, padx = 100)
recargaBtn.bind('<Enter>', on_hover)
recargaBtn.bind('<Leave>', on_default)



# Centrar la ventana en la pantalla al inciarce 
#  Obtenemos el largo y  ancho de la pantalla
wtotal = root.winfo_screenwidth()
htotal = root.winfo_screenheight()
#  Guardamos el largo y alto de la ventana
wventana = 700
hventana = 500

#  Aplicamos la siguiente formula para calcular donde debería posicionarse
pwidth = round(wtotal/2-wventana/2)
pheight = round(htotal/2-hventana/2)

#  Se lo aplicamos a la geometría de la ventana
root.geometry(str(wventana)+"x"+str(hventana)+"+"+str(pwidth)+"+"+str(pheight))


root.mainloop()