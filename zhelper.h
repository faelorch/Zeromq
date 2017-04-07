#ifndef _ZHELPERS_H_INCLUDED_
#define _ZHELPERS_H_INCLUDED_
#include <zmq.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#  include <sys/time.h>
//genere un nombre aleatoire entre 0 et num
#define randof(num)  (int)((float)(num)*random()/(RAND_MAX +1.0))
#if (!defined (WIN32))
#  include <sys/time.h>
#endif
#if (defined (WIN32))
#	include <windows.h>
#endif
#if (defined (WIN32))
#	define srandom  srand
#	define random  rand
#endif
#if ZMQ_VERSION_MAJOR == 2
#	error "vesion vielle passez sur ZeroMQ/3.2 pour cet example"
#endif
//genere un nombre aleatoire entre 0 et num

//envoi d'un message de socket 
static int s_send(void* socket,char *string){
	int size = zmq_send(socket,string,strlen(string),0);
	return size;
}

static int s_sendmore(void* socket,char* string){
	int size = zmq_send(socket,string,strlen(string),ZMQ_SNDMORE);
	return size;
}
//reception de messages dans socket 
static char * s_recv(void*socket){
	char buffer[256];
	int size = zmq_recv(socket,buffer,255,0);
	if(size == -1)
		return NULL;
	return strndup(buffer,sizeof(buffer)-1);//retourner une copie du buffeur 
}

static void s_dump(void*socket){
	int rc;
	zmq_msg_t message ;
	rc =zmq_msg_init(&message);//initialise un message_t et si ca marche retourne 0 
	assert(rc == 0);
	puts("------------------------------------");
	do{
		int size =zmq_msg_recv(&message,socket,0);//reception d'un message 
		assert(size >= 0);//si le message a bien ete recu
		char *data =(char*)zmq_msg_data(&message);//retourne un pointeur du contenu du message 
		assert(data != 0);
		int is_text =1;
		int char_nbr;
		for(char_nbr =0;char_nbr < size;char_nbr++){
			if((unsigned char)data[char_nbr]<32 ||(unsigned char) data[char_nbr]>126){
				
				is_text=0;
			}
		}
		printf("[%03d]",size);//on affiche la taille du message recu en trois chiffre significatifs
		for(char_nbr=0;char_nbr< size;char_nbr++){
			if(is_text){
				printf("%c",data[char_nbr]);
			}else{
				printf("%02X",(unsigned char)data[char_nbr]);
			}
		}
			printf("\n");
			
	}while(zmq_msg_more(&message));
	
	rc=zmq_msg_close(&message);
	assert(rc == 0);
		
}
#if (!defined (WIN32))
//affecter à une socket un identifiant aleatoire 
static void s_set_id(void*socket){
	char identity[10];
	sprintf(identity,"%04X-%04X",randof(0x10000),randof(0x10000));
	zmq_setsockopt(socket,ZMQ_IDENTITY,identity,strlen(identity));
	
}
#else
static void s_set_id(void*socket,intptr_t id){
	char identity[10];
	sprintf(identity,"%04X",(int)id);
	zmq_setsockopt(socket,ZMQ_IDENTITY,identity,strlen(identity));
	
}
#endif
//attendre msecs millisecondes
static void s_sleep(int msecs){
#if (defined (WIN32))
	sleep(msecs);
#else
	struct timespec t;
	t.tv_sec = msecs / 1000;
	t.tv_nsec=(msecs % 1000) *1000000;
	nanosleep(&t,NULL);
#endif
}

//retourne le nombre de secondes actuelles
static int64_t s_clock(void){
#if (defined (WIN32))
	SYSTEMTIME st;
	GetSystemTime(&st);
	return(int64_t)st.wSecond *1000 +st.wMilliseconds;
#else
	struct timeval tv;
	gettimeofday(&tv ,NULL);
	return(int64_t)(tv.tv_sec *1000 +tv.tv_usec/1000);
#endif
}

//affiche la date complet
static void s_console(const char *format, ...){
	time_t curtime = time(NULL);
	struct tm*loctime =localtime(&curtime);
	char *formatted =(char*)malloc(20);
	strftime(formatted,20,"%y-%m-%d %H:%M:%S",loctime);
	printf("%s",formatted);
	free(formatted);
	va_list argptr;
	va_start(argptr,format);
	vprintf(format,argptr);
	va_end(argptr);
	printf("\n");
}



#endif
