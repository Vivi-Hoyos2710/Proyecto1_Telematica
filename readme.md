# Proyecto 1 de telematica

## 1. Introduccion

Este es nuestro proyecto 1 de la materia telamatica STO255-4513 con el profesor Juan Carlos. este proyecto se basa en crear un servidor web el cual permite procesar peticiones del protocolo HTTP/1.1 y de esta forma entregar los diferentes recursos solicitador por el cliente Este proyecto fue desarrollado por Viviana Hoyos y Juan Esteban Jaramillo

## 2. Desarrollo
### 2.1 Api Sockets 
Al principio empezamos a entender la api de sockets con un codigo en python que el profesor nos paso para pasar texto de un lado a otro
### 2.2 C++
El segundo paso fue empezar fue dialogar entre nosotros para saber que lenguaje queriamos usar que en este caso fue c++ y replicar ese codigo que nos dieron en python a c++ para pasar textos de un cliente a un servidor y asi poder tener las conexiones bien
### 2.3 Peticion-Request
Este paso consistio en que desde la consola nos conectabamos a nuestros servidor usando el comando ```telnet``` y empezamos a enviar peticiones tipo ```GET /prueba HTTP/1.1``` para ya en el codigo recibir ese texto y empezar a separar que tipo de peticion era que path tenia y si la version de HTTP era la correcta que en este caso era HTTP/1.1
### 2.4 Postman
Luego empezamos a usar postman para crear la peticion y empezar a procesar las headers que traian las peticiones que haciamos desde postman. Cuando terminamos de guardar los encabezados. Empezamos a enviar respuestas sencillas usando la funcion de ```send()``` donde respondiamos son el protocolo que es ```HTTP/1.1 200``` ya mas abajo van los headers y despues va el body
### 2.5 Archivos
Para este paso ya podiamos recibir peticiones y procesarlas pero no enviabamos ni recibiamos ningun tipo de archivo asi que lo primero que hicimos que implementar la peticion de GET con path para que asi desde postman pudieramos solicitar un archivo de html o una imagen o cualquier tipo de archivo con sus respectivos headers. el siguiendo paso fue hacer la peticion HEAD que puede que envie los headers respectivos de dicha peticion
### 2.6 POST
Este fue uno de los pasos mas conflictivos que tuvimos ya que cuando estabamos haciendo el codigo para recibir el post nos estaba funcionando para recibir archivos de texto plano tipo: html, json, css, etc. pero cuando intentabamos enviar una imagen el binario a la hora de escribirlo en un nuevo archivo se nos estaba corrompiendo por varios problemas. Primero el buffer que teniamos era muy pequeño y no recibia el archivo completo. despues nos dimos cuenta que el while con el estabamos recibiendo el binario de la imagen tampoco estaba alcanzando para recibirla toda y asi hasta que pudimos encontrar la forma de guardarla bien

## 3. Como inciar el servidor
Primero colocas la ip que quieres usar segun donde lo vayas a trabajar si vas a usar una tu maquina colocas el localhost que es 127.0.0.1 en el archivo de constants_server.h donde dice IP_SERVER 

Despues Compilas el programa con el siguiente comando
```make```

Despues de haber compilado el codigo lo pones a correr con el siguiente comando
```./server {puerto} {archivo de logs} {carpeta de recursos}```

En este caso en especifico es 

```./server 8080 {archivo de logs} documentRootFolder```


Para probar los diferentes casos de prueba tenemos diversas carpetas y en el navegador pones tu la ip que tu hayas puesto en las constantes si es localhost pones 
```127.0.0.1:8080/CASO1/index.html```

