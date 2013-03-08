remote-authentication-with-open-ssl
===================================

Enunciado
-----------------------------------

El ejercicio se basa en programar una versión segura y una insegura de una aplicación (cliente-servidor) que solicita datos privados por la red, y verificar el funcionamiento de cada una con un analizador de tráfico. Los programas se desarrollarán en C.

Cuando un cliente se conecta al servidor, éste retorna un mensaje solicitando "nombre de usuario", espera la respuesta, y luego envía "clave". El servidor debe tener un archivo con los usuarios y sus claves para poder verificarlo.

Si el usuario existe en el sistema y la clave es correcta, el servidor responde con un mensaje "Usuario autorizado"; en caso contrario debe responder "Acceso denegado".

Las claves de los usuarios deben guardarse utilizando algoritmos criptográficos con el fin de disminuir las posibilidades de violación de seguridad.

Comandos
-----------------------------------

Los comandos para ejecutar cliente y servidor, son:

1. Cliente:

    ```bash
    acceso-rem -s ip-servidor -p puerto-servidor   # version insegura
    ```
    
    ```bash
    acceso-rem-seg -s ip-servidor -p puerto-servidor   #  version con OpenSSL
    ```
    
    donde `ip-servidor` y `puerto-servidor` son el IP de la maquina y el puerto, respectivamente, donde esta conectado el servidor

2. Servidor

    ```bash
    serv-acceso -p puerto
    ```
    
    ```bash
    serv-acceso-seg -p puerto
    ```
    
    Donde `puerto` es el número del puerto por el que atiende peticiones el servidor. 

Versión insegura
-------------------

Se desarrollará un cliente "cliente-eco" y un servidor "eco" que implemente el protocolo descrito arriba utilizando la librería sockets con TCP.

Versión TLS
-------------------

La versión segura utilizará la librería OpenSSL con TLS. Se deberá generar un certificado X.509 "auto-firmado" para el servidor.
