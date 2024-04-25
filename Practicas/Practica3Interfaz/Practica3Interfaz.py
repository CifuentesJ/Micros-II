import tkinter as tk
import tkinter.ttk as ttk
import serial.tools.list_ports
import matplotlib
matplotlib.use("TKAgg")
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from random import randrange
import time


# Variables globales

wide_graph = 100        #Define el ancho de la grádica con la cantidad de datos
global isOpen
isOpen = False
dataList = []


# Funciones

def serial_ports():
    if not serial.tools.list_ports.comports():
        return "NO-PORTS"
    else:
        global ports
        ports = serial.tools.list_ports.comports()
        return ports
    
def connect():
    global serial_com
    if(cb.get() == "NO-PORTS"):
        print("No se puede conectar")
    else:
        serial_com = serial.Serial(str(ports[cb.current()][0]),9600)
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


def led_off():
    global serial_com
    global isOpen
    if isOpen:
        serial_com.write(b"D0\n")

def led_on():
    global serial_com
    global isOpen
    if isOpen:
        serial_com.write(b"D1\n")

def slider_changed(event):
    data_slider.configure(text = get_current_value()+"%")
    global isOpen
    if isOpen:
        data = "S"+get_current_value()+"\n"
        serial_com.write(bytes(data,'utf-8'))
        print("Slide: "+data)

def get_current_value():
    return '{:.1f}'.format(slider.get())




# Inicialización de gráfica 

fig = plt.figure(figsize=(6,3))
fig.set_facecolor("#17161b")
ax = plt.axes()
ax.set_facecolor("#17161b")
ax.title.set_color("white")
ax.xaxis.label.set_color("white")
ax.yaxis.label.set_color("white")
ax.grid(alpha = 0.1)
ax.tick_params(axis = 'x', colors = "white")
ax.tick_params(axis = 'y', colors = "white")


# Buffer for data input

x = [None]*wide_graph
y = [0]*wide_graph
for i in range(0, wide_graph,1):
    x[i] = i
ln, = plt.plot(x,y, '-', color = "#CF27E6")

plt.axis([0, wide_graph, 0, 1100])
plt.ylabel("Valor lectura")
plt.title("Reading ...")




# Crear la función que recibe periodicamente los datos enviados desde el arduino con la información del potenciómetro y actualiza la vista de la gráfica

def update(frame):
    if isOpen:
        global serial_com
        global y 
        global x
        data = serial_com.readline().decode('ascii')
        print("Recibido: ", data)
        data = data.rstrip()
        data = data.replace("T","")
        y.append(float(data))

        #pop para evitar desbordamiento del buffer
        if len(y) > wide_graph:
            y.pop(0)

        y = y[-wide_graph:]
        x = x[-wide_graph:]

        ln.set_data(x, y)
        return ln
    
def on_hover(event):
    event.widget.configure(bg = "#624466")

def on_default(event):
    event.widget.configure(bg = "#AD3FBB")


# Crear la ventana de la interfaz de usuario
# main

root = tk.Tk()
root.title("Arduino Serial Data")
root.geometry("700x500")
root.configure(bg = "#17161B")
root.resizable(0,0)

frame = tk.Frame(root)
frame.pack()
frame.configure(bg = "#17161B")

canvas = FigureCanvasTkAgg(fig, master = root)
canvas.draw()

# Ubicando el canvas en la ventana de window
canvas.get_tk_widget().pack()

# Matplotlib Toolbar
toolbar = NavigationToolbar2Tk(canvas,root)
toolbar.update()

# Toolbar en la ventana de tkinder
#canvas.get_tk_widget().pack()



# Agregar a la vista el selector y los botones de la comunicación serial
# Serial port
serial_info_frame = tk.LabelFrame(frame, text = "Conexion Serial", bg = "#17161b", fg = "#fff")
serial_info_frame.grid(row = 0, column = 0)

cb = ttk.Combobox(serial_info_frame, state = "readonly", value = serial_ports())
cb.current(0)
cb.grid(row = 0, column = 1)


#Botón de actualizar 


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



# Agregar a la vista los botones de control del led 1 y el slider para controlar el led 2
#Send data 
send_info_frame = tk.LabelFrame(frame, text = "Envio de datos", bg = "#17161B", fg = "#fff", highlightcolor = "#00044A")
send_info_frame.grid(row = 1, column = 0, padx = 10, pady =10)

#ON button 
on1_button = tk.Button(send_info_frame, text = "ON", bd = 1, fg = "#00044A", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command = led_on)
on1_button.grid(row = 0, column = 0)
on1_button.bind('<Enter>', on_hover)
on1_button.bind('<Leave>', on_default)

#OFF button 
off1_button = tk.Button(send_info_frame, text = "OFF", bd = 1, fg = "#00044A", bg = "#AD3FBB", activebackground = "#624466", relief = "flat", command = led_off)
off1_button.grid(row = 0, column = 1)
off1_button.bind('<Enter>', on_hover)
off1_button.bind('<Leave>', on_default)

#Slider
current_value = tk.DoubleVar()
style = ttk.Style()
style.configure("TScale", background = "17161B")
slider = ttk.Scale(
    send_info_frame,
    from_= 0, 
    to = 100, 
    orient = 'horizontal',
    variable = current_value,
    command = slider_changed
)
slider.grid(row = 0, column = 2)
data_slider = tk.Label(send_info_frame, text = "0%", bg = "#17161B", fg = "#fff")
data_slider.grid(row = 0, column = 3)

for widget in send_info_frame.winfo_children():
    widget.grid_configure(padx=10, pady=5)



# Agregar una animación que llama cada 500ms la función update() para actualizar la gráfica y el inicio del loop
animation = FuncAnimation(fig, update, interval = 500, cache_frame_data=False)


root.mainloop()