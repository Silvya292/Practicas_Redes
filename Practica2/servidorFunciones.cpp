#include "servidor.h"

vector<Usuario> cargarUsuarios(){
    vector<Usuario> usuarios;
    Usuario aux=Usuario();
    string str;
    
    string filename("usuarios_Registrados.txt");
    ifstream file(filename);
    if(!file){
        EXIT_FAILURE;
    }

    getline(file,str,' ');

    while(!file.eof()){
        aux.setUsername(str);
        getline(file,str,'\n');
        aux.setPassword(str);
        usuarios.push_back(aux);
        getline(file,str,' ');
    }

    file.close();
    return usuarios;
}

bool registrarUsuario(Usuario usuario, vector<Usuario> usuarios){
    if(usuarioRepetido(usuario.getUsername(), usuarios)){
        return false;
    }

    string filename("usuarios_Registrados.txt");
    ofstream file(filename);
    file.open(filename, ios::app);
    if(!file){
        EXIT_FAILURE;
    }

    file<<"\n"<<usuario.getUsername()<<" "<<usuario.getPassword();
    file.close();

    return true;
}

bool comprobarPassword(string password, string usuario, vector<Usuario> usuarios){
    for(int i=0;i<usuario.size();i++){
        if(usuarios[i].getUsername()==usuario){
            if(usuarios[i].getPassword()==password){
                return true;
            }
        }
    }

    return false;
}

bool usuarioRepetido(string usuario, vector<Usuario> usuarios){
    for(int i=0;i<usuarios.size();i++){
        if(usuarios[i].getUsername()==usuario){
            return true;
        }
    }

    return false;
}

bool usuarioRegistrado(string usuario, vector<Usuario> usuariosIn){
    for(int i=0;i<usuariosIn.size();i++){
        if(usuariosIn[i].getUsername()==usuario){
            return true;
        }
    }

    return false;
}

void enviarMensaje(string mensaje, int sd){
    char buffer[256];
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, mensaje.c_str());
    send(sd, buffer, sizeof(buffer), 0);
}

//TODO: int partida(int sd, vector<Partida> partidas)

