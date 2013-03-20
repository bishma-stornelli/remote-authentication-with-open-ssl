#!/bin/bash

echo "Generando certificado"
openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout certificate/myserver.key -out certificate/myserver.crt
