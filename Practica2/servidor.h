#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <cstring>
#include <regex>
#include <ctype.h>
#include <regex.h>

using namespace std;

class Usuario{
    private:
        string userName;
        string password;
        bool isIn;
        int sd;

    public:
        Usuario(string username, string password, int sd){
            this->setUsername(username);
            this->setPassword(password);
            this->setSd(sd);
            this->isIn = false;
        }

        Usuario(){
            this->userName = "";
            this->password = "";
        }

        string getUsername(){
            return this->userName; 
        }

        string getPassword(){ 
            return this->password; 
        }

        int getSd(){ 
            return this->sd; 
        }

        bool isIn(){ 
            return this->isIn; 
        }

        void setUsername(string username){ 
            this->userName = username; 
        }

        void setPassword(string password){ 
            this->password = password; 
        }

        void setSd(int sd){ 
            this->sd = sd; 
        }

        void setisIn(bool isIn){ 
            this->isIn = isIn; 
        }
};

vector<Usuario> cargarUsuarios(); //Lista con los usuarios guardados en el fichero

bool registrarUsuario(Usuario usuario, vector<Usuario> usuarios); //Añade un nuevo usuario al fichero

bool comprobarPassword(string password, string usuario, vector<Usuario> usuarios); //Comprueba si la contraseña pertenece al nombre de usuario indicado

bool usuarioRepetido(string usuario, vector<Usuario> usuarios); //Comprueba si ya existe el nombre de usuario indicado

bool usuarioRegistrado(string usuario, vector<Usuario> usuariosIn); //Comprueba si el usuario se ha registrado previamente

void enviarMensaje(string mensaje, int sd); //Envía un mensaje a través del descriptor sd

//TODO: int partida(int sd, vector<Partida> partidas); //Comprueba en qué partida está el usuario

#endif