#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
	printf("connexion a Hello World ...\n");
	void * ctx =zmq_ctx_new();// creation d'un contexte
	void * req =zmq_socket(ctx, ZMQ_REQ);//creation d'une socket
	zmq_connect(req,"tcp://localhost:5555");//connection du client 
	int req_nbr;
	for(req_nbr =0;req_nbr !=10;req_nbr++){//envoie du mot Hello 10 fois 
		char buffer[5];
		printf("envoie du hello numero : %d\n",req_nbr);
		zmq_send(req,"Hello",5,0);
		zmq_recv(req,buffer,5,0);
		printf("mot recu: %s\n",buffer);//affichage des donnees recu du serveur 
		
	}
	zmq_close(req);//fermeture de la requete
	zmq_ctx_destroy(ctx);
	return 0;
}
