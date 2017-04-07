/******************************************************/
/*		client qui recupere 100 fois les donnees      */
/*		les donnees envoyees par le serveur un boucle */
/*		recupere le code la temperature et l'humidite */
/*      et calcule la temperature moyenne  			  */
/*                  et l'affiche                      */
/******************************************************/
#include "zhelper.h"


int main(int argc,char *argv[]){

	printf("collection des donnees relatives à la temperature\n ");
	void*ctx = zmq_ctx_new();
	void * sub= zmq_socket(ctx,ZMQ_SUB);
	zmq_setsockopt(sub,ZMQ_SUBSCRIBE,"",0);
	//mq_setsockopt(zmq_RCVHWM,0,,);
	int rc = zmq_connect(sub,"tcp://localhost:2223");
	
	assert(rc == 0);
	const char *filter= (argc >1)? argv[1]:"10001";
	rc = zmq_setsockopt(sub,ZMQ_SUBSCRIBE,filter,strlen(filter));
	assert(rc ==0);
	printf("connexion\n");
	int update_nbr;
	long total_temp =0;
	int code , temp,humid;
	for(update_nbr =0;update_nbr <100;update_nbr++){
		//char car[20];
		char*car =  s_recv(sub);
		car[strlen(car)-1]='\0';
		//zmq_recv(sub,car,20,0);
		printf("chaine de caractère recue : %s\n\n",car);
		sscanf(car,"%d %d %d",&code ,&temp,&humid);
		total_temp+=temp;
		temp++;
		free(car);
	}
	printf("la temperatur moyenne pour le zipcode '%s' (%d) est %dF\n",filter,code,(int)(total_temp / update_nbr));
	zmq_close(sub);
	zmq_ctx_destroy(ctx);
	
	
	return 0;
}
//gcc -o client -lzmq client.c && clear && ./client 63000
