//creation d'un serveur et d'un client Hello World
//serveur recoit Hello envoi world
//utilisation de la bibliotheque zmq
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
int main(void){
	void *ctx = zmq_ctx_new();//creation d'un contexte
	void *resp =zmq_socket(ctx, ZMQ_REP);//creation d'une socket
	int rc = zmq_bind(resp , "tcp://*:5555");//assignment du numero de port 
	assert(rc ==0);//s'assurer que la connexion a etet faite 
	
	while(1){//boucle infinie 
		char buffer[5];
		zmq_recv(resp,buffer,5,0);//mettre les donnees recu dans une chaine  de caractere
		printf("Message recu : %s\n",buffer);//afficher la chaine de caractere
		sleep(1);
		zmq_send(resp,"world",5,0);//envoie du mot world
	}
	return 0;
}
