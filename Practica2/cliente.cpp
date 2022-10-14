#include "cliente.h"

int main ( ){

	bool correctUsername=false; //Comprueba que sea correcto el nombre de usuario
	bool correctPassword=false; //Coprueba que sea correcta la contraseña
    bool playing=false;         //Comprueba si el usuario está jugando
    bool turn=false;            //Comprueba si es el turno de juego del usuario

	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	socklen_t len_sockname;
	fd_set readfds, auxfds;
	int salida;
	int fin=0;
	
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1){
		perror("-Err. No se puede abrir el socket cliente.\n");
    		exit (1);	
	}

    /* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2060);
	sockname.sin_addr.s_addr =  inet_addr("192.168.0.103");

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1){
		perror ("-Err. Error de conexión.\n");
		exit(1);
	}

    /* ------------------------------------------------------------------
		Inicializamos las estructuras
	-------------------------------------------------------------------*/
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

    /* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	start();
	do{
		auxfds=readfds;
		salida=select(sd+1,&auxfds, NULL, NULL, NULL);

		if(FD_ISSET(sd,&auxfds)){ //El servidor envía un mensaje
			bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0);

			printf("%s",buffer);

			if(strcmp(buffer, "-Err. Demasiados clientes conectados. Inténtelo de nuevo más tarde.\n")==0 || strcmp(buffer,"-Err. Servidor desconectado.\n")){
				fin=1;
			}
			if(strcmp(buffer, "+Ok. Nombre de usuario correcto.\n")==0){
				correctUsername=true;
				cout<<"Introduzca su contraseña (PASSWORD <contraseña>): ";
			}
			if(strcmp(buffer, "+Ok. Contraseña correcta. Sesión iniciada correctamente.\n")==0){
				correctPassword=true;
				cout<<buffer;
				gameOptions();
			}
            if(regex_match(buffer, (regex("(^\\+Ok\\. Empieza la partida\\.")))){ //TODO:
                gameInterface();
                playing=true;

                if(turn==true){
                    printf("+Ok. Turno de partida\n");
                }

                else{
                    printf("+Ok. Turno del otro jugador\n");
                }
            }
            if(strcmp(buffer, "+Ok. Petición Recibida. Esperando a más jugadores\n") == 0){
                turn=true;
            }
            
		}
	}while(fin==0);
		
	close(sd);
	return 0;
}