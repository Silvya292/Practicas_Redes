#include "servidor.h"

using namespace std;

#define MSG_SIZE 250
#define MAX_CLIENTS 30

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
   
/*---------------------------------------------------- 
	Descriptor del socket y buffer de datos                
-----------------------------------------------------*/
int sd, new_sd;
struct sockaddr_in sockname, from;
char buffer[100];
socklen_t from_len;
struct hostent * host;

fd_set readfds, auxfds;
int salida;
int arrayClientes[MAX_CLIENTS];
int numClientes = 0;
int i,j,k;
int recibidos;
char identificador[MSG_SIZE];

int on, ret;

regex_t regex_;
int resultado;

vector<Usuario> usuariosRegistrados=cargarUsuarios(); //Carga todos los usuarios registrados del sistema
vector<Usuario> usuariosConectados; //Carga todos los usuarios que se encuentren activos
vector<int> cola; //Carga los usuarios que están en cola para jugar
// TODO: vector<Partida> partidas; //Lista de partidas simultáneas

int main(){

    resultado=regcomp(&regex_,"REGISTRO -u \\w* -p \\w*",REG_EXTENDED);

    /* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1){
		perror("-Err. No se puede abrir el socket cliente.\n");
    		exit (1);	
	}

    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2060);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1){
		perror("-Err. Error en la operación bind.\n");
		exit(1);
	}

    /*---------------------------------------------------------------------
		De las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
	from_len = sizeof (from);

	if(listen(sd,1) == -1){
		perror("-Err. Error en la operación de listen.\n");
		exit(1);
	}

    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);


    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);

    /*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
	while(1){

        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
        auxfds = readfds;
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);

        if(salida > 0){
            for(i=0; i<FD_SETSIZE; i++){
                //Buscamos el socket por el que se ha establecido la comunicación
                if(FD_ISSET(i, &auxfds)) {
                    if(i == sd){
                        if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                            perror("-Err. Error aceptando peticiones.");
                        }
                        else{
                            if(numClientes < MAX_CLIENTS){
                                cout<<"+Ok. Usuario conectado.\n";
                                strcpy(buffer, "+Ok. Usuario conectado.\n");

                                usuariosRegistrados=cargarUsuarios();

                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd,&readfds);        
                                send(new_sd,buffer,sizeof(buffer),0);
                            }
                            else{
                                bzero(buffer,sizeof(buffer));
                                strcpy(buffer,"-Err. Demasiados clientes conectados. Inténtelo de nuevo más tarde.\n");
                                send(new_sd,buffer,sizeof(buffer),0);
                                close(new_sd);
                            }
                        }
                    }
                    else if (i == 0){
                        //Se ha introducido información de teclado
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer),stdin);


                        if(strcmp(buffer,"SALIR\n") == 0){
                            manejador(SIGINT);
                        }
                    } 

                    //TODO: Distintos caso del programa
                    else{
                        bzero(buffer,sizeof(buffer));
                        recibidos = recv(i,buffer,sizeof(buffer),0);
                        
                        if(recibidos > 0){
                            resultado=regexec(&regex_, buffer, 0, NULL, 0);

                            if(strcmp(buffer,"SALIR\n") == 0){
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }

                            else if(!resultado){
                                string option;
                                option.pop_back();
                                string username=option.substr(option.find(" -u ")+4, option.find("-p"));
                                username=username.substr(0,username.find(" ")); //Almacena el nombre de usuario
                                string password=option.substr(option.find(" -p ")+4, option.length()+1); //Almacena la contraseña

                                if(usuarioRepetido(username, usuariosRegistrados)){
                                    strcpy(buffer, "-Err. El nombre de usuario indicado ya está registrado.");
                                    send(i,buffer,sizeof(buffer),0);
                                }

                                else{
                                    Usuario usuario;
                                    usuario.setUsername(username);
                                    usuario.setPassword(password);
                                    usuario.setSd(sd);

                                    registrarUsuario(usuario, usuariosRegistrados);
                                    usuariosRegistrados.push_back(usuario);
                                    usuariosConectados.push_back(usuario);
                                    strcpy(buffer, "+Ok. Usuario registrado correctamente.");
                                    send(i,buffer,sizeof(buffer),0);
                                }
                            }

                            else if(strncmp(buffer, "USUARIO ", strlen("USUARIO "))==0){
                                if (strncmp(buffer, "USUARIO \n", strlen("USUARIO \n"))==0){
                                    strcpy(buffer, "-Err. No se ha podido iniciar sesión\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else{
                                    string option=buffer;
                                    option.pop_back();
                                    int aux=option.find(" ");
                                    string usuario=option.substr(aux+1,option.length()+1);
                                    if(usuarioRepetido(usuario,usuariosRegistrados)){
                                        if(usuarioRegistrado(usuario,usuariosConectados)){
                                            strcpy(buffer,"-Err. No se ha podido iniciar sesión\n");
                                        }
                                        else{
                                            for(int i=0;i<usuariosRegistrados.size();i++){
                                                if(usuariosRegistrados[i].getUsername()==usuario){
                                                    usuariosRegistrados[i].setSd(sd);
                                                }
                                            }
                                            strcpy(buffer, "+Ok. Nombre de usuario correcto.\n");
                                        }
                                        send(i,buffer,sizeof(buffer),0);
                                    }
                                    else{
                                        strcpy(buffer, "-Err. Nombre de usuario incorrecto.\n");
                                        send(i,buffer,sizeof(buffer),0);
                                    }
                                }
                            }

                            else if(strncmp(buffer, "PASSWORD ", strlen("PASSWORD "))==0){
                                if (strncmp(buffer, "PASSWORD \n", strlen("PASSWORD \n"))==0){
                                    strcpy(buffer, "–Err. Error de validación.\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }

                                else{
                                    bool isLogged=false;
                                    string passwdOption=buffer;
                                    passwdOption.pop_back();
                                    int aux=passwdOption.find(" ");
                                    string password=passwdOption.substr(aux+1, passwdOption.length()+1);

                                    for(int pos=0;pos<usuariosRegistrados.size();pos++){
                                        if((usuariosRegistrados[pos].getPassword()==password) && (usuariosRegistrados[pos].getSd()==sd)){
                                            strcpy(buffer, "+Ok. Usuario validado.\n");
                                            send(i, buffer, sizeof(buffer),0);

                                            Usuario nuevoUsuario=usuariosRegistrados[pos];
                                            nuevoUsuario.setSd(i);
                                            usuariosConectados.push_back(nuevoUsuario);
                                            isLogged=true;
                                            break;
                                        }
                                    }

                                    if(!isLogged){
                                        strcpy(buffer, "–Err. Error de validación.\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                }
                            }

                            else if(strncmp(buffer, "INICIAR-PARTIDA",strlen("INICIAR-PARTIDA"))==0){
                                cola.push_back(i);
                                if(cola.size()==2){
                                    //TODO: Partida
                                }
                            }                                                                              
                        }

                        //Si el cliente introdujo ctrl+c
                        if(recibidos== 0){
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            //Eliminar ese socket
                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                        }
                    }
                }
            }
        }
	}
	close(sd);
	return 0;
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++){
        if(arrayClientes[j] == socket){
            break;
        }
    }
    for(; j < (*numClientes) - 1; j++){
        (arrayClientes[j] = arrayClientes[j+1]);
    }
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++){
        if(arrayClientes[j] != socket){
            send(arrayClientes[j],buffer,sizeof(buffer),0);
        }
    }
}


void manejador (int signum){
    printf("\nApagando el servidor...\n");
    signal(SIGINT,manejador);

    for(i=0;i<numClientes;i++){
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "-Err. Servidor desconectado.\n");
        send(arrayClientes[j], buffer, sizeof(buffer), 0);
        close(arrayClientes[j]);
        FD_CLR(arrayClientes[j], &readfds);
    }

    close(sd);
    exit(-1);
}