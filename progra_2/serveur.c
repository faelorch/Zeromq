/*********************************************************/
/*      Serveur qui envoie des donnees sur un port       */
/*      publie des messages à differents clients         */
/*      ce message contient des nombres aleatoire        */
/*      representant la temperature et l'humidité        */
/*                 d'une ville precise                   */
/*********************************************************/

#include "zhelper.h"



int main(void){
	
	void* context =zmq_ctx_new();
	void *pub= zmq_socket(context,ZMQ_PUB);
	//zmq_setsockopt(zmq.SNDHWM,0,0,0);
	//zmq_setsockopt(RCVHWM,0,0,0);
	int rc =zmq_bind(pub,"tcp://*:2223");
	assert(rc == 0);
	srandom(time(NULL));
	while(1){
		int code,temp,humid;
		code = randof(100000);//generer un code aleatoire
		temp =randof(215)-80;//generer une temperature aleatoire
		humid= randof(50)+10;
		char update[20];
		sprintf(update,"%05d %03d %d",code,temp,humid);//convertir les nombres generés en une chaine de caractère
		s_sleep(1000);
		printf("la chaine de caractère envoryé est : %s\n",update);
		//update[strlen(update)-1]='\0';
		//zmq_send(pub,update,20,0);//envoie des donnees avec fonction zmq
		s_send(pub,update);//envoie des donnees avec les fonction du ".h"
	}
	zmq_close(pub);
	zmq_ctx_destroy(context);
	return 0;
}
//ligne de compilation et d'excecution gcc -o serveur -lzmq serveur.c && clear && ./serveur
