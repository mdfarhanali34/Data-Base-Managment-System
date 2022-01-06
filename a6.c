#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"


int entries( char *filebase )
{
  char filename[BUFFER];
  int ent;

  strcpy( filename, filebase );
  strcat( filename, ".idx" );  //doing string concat
  FILE *fp = fopen( filename, "rb" );
  fseek( fp, 0, SEEK_END );
  ent = ftell( fp ) / sizeof(long);
  fclose(fp);

  return ent;
}

char* getElement(char* basename, int index){

  char txtfile[BUFFER];
  char idxfile[BUFFER];
  char* buffer = malloc(10*sizeof(char));

  int* idx2 = malloc(sizeof(int));
  FILE *fptxt, *fpidx;

  strcpy( txtfile, basename );
  strcat( txtfile, ".txt" );

  strcpy( idxfile, basename );
  strcat( idxfile, ".idx" );

  fptxt = fopen( txtfile, "r" );
  fpidx = fopen( idxfile, "r" );

  fseek( fpidx, sizeof(long)*index, SEEK_SET );

  if ( fread( idx2, sizeof(long), 1, fpidx ) != 1 )
  {
    exit(-1);
  }

  fclose( fpidx );

  fseek( fptxt, *idx2, SEEK_SET );
  fgets( buffer, BUFFER, fptxt );

  fclose( fptxt );
  buffer[strlen(buffer) - 1] = '\0';

  return buffer;

}


int main(int argc, char **argv ){

if (argc != 3)
{
    fprintf( stderr, "Usage: %s building name room number\n", argv[0] );  //eroor checking comand line input
    exit(-1);
}

long hash_tableBuilding[HASHSIZE];
char *valueBuilding;
char *valueRoom;

FILE *txtfileBuilding = fopen( "building.txt" , "r" );
FILE *idxfileBuilding = fopen( "building.idx" , "r" );


init_hash_table( hash_tableBuilding );
get_hashtable( "building", hash_tableBuilding );

valueBuilding = argv[1];


int indexBuilding = hash_lookup( valueBuilding, hash_tableBuilding, idxfileBuilding, txtfileBuilding );  //Calling the hashlookup function and calculating the index

int nCode1 = entries( "code" );
int nBuilding2 = entries( "building" );
char *interCode_Building = malloc(nCode1); 


FILE *fpCode_Build = fopen( "code_building.rel", "rb" );
char *arrayBuilding = malloc( nCode1*nBuilding2 );
fread( arrayBuilding , 1, nCode1*nBuilding2, fpCode_Build );  //reading code and building relation file
fclose( fpCode_Build );

int iCode = -1;

  if ( (iCode == -1) && (indexBuilding>=0) )
  {
    for (int i=0;i<nCode1;i++)
    {
      int index = i*nBuilding2 +indexBuilding;
      interCode_Building[i] = arrayBuilding[index] ;   
    }
  }


  if ( (iCode > 0) && (indexBuilding == -1) )
  {
    for (int j=0;j<nBuilding2;j++)
    {
      int index = iCode*nBuilding2 + j;
      interCode_Building[j] = arrayBuilding[index] ;  
    }
  }

long hash_tableRoom[HASHSIZE];

FILE *txtfileRoom = fopen( "room.txt" , "r" );
FILE *idxfileRoom = fopen( "room.idx" , "r" );


init_hash_table( hash_tableRoom );
get_hashtable( "room", hash_tableRoom );

valueRoom = argv[2];

int indexRoom = hash_lookup( valueRoom, hash_tableRoom, idxfileRoom, txtfileRoom );  //calculating index for room hastable


int nRoom2 = entries( "room" );
char *interCode_Room = malloc(nCode1); 


FILE *fpCode_Room = fopen( "code_room.rel", "rb" );
char *arrayRoom = malloc( nCode1*nRoom2 );
fread( arrayRoom , 1, nCode1*nRoom2, fpCode_Room );  //reading the code and room relaton files
fclose( fpCode_Room );

 if ( (iCode == -1) && (indexRoom >=0) )
  {
    for (int i=0;i<nCode1;i++)
    {
      int index = i*nRoom2 +indexRoom;
      interCode_Room[i] = arrayRoom[index];  
    }
  }

  
  if ( (iCode > 0) && (indexRoom == -1) )
  {
    for (int j=0; j < nRoom2 ; j++)
    {
      int index = iCode*nRoom2 + j;
      interCode_Room[j] = arrayRoom[index];   
    }
  }


  char *interCode = malloc(nCode1); 

  for(int i=0; i< nCode1 ; i++)
  {
    interCode[i] = interCode_Building[i] && interCode_Room[i];  //finding the intersection array of two relation array
  }

char* finalStored = malloc(nCode1);
char* finalStoredPrev = malloc(nCode1);

for(int i=0; i< nCode1 ; i++){

if(interCode[i] == 1)
{  
   int indexCode = i;  //geting the index coresponding to a 1 in my intersection array

  int nsubject2 = entries( "subject" );
  FILE *fpCode_Subject = fopen( "code_subject.rel", "rb" );

  char *arraySubject = malloc( nCode1*nsubject2 );

  fread( arraySubject, 1, nCode1*nsubject2, fpCode_Subject );  //reading code and subject relation file

  fclose( fpCode_Subject );

  char *interCode_Subject = malloc(nCode1); 
  int subjectnegHolder = -1;
  char* holdSubject =  malloc(nCode1); 


  if ( (indexCode > 0) && (subjectnegHolder == -1) )
  {
    for (int j=0; j < nsubject2 ; j++)
    {
      int index = indexCode*nsubject2 + j;
      interCode_Subject[j] = arraySubject[index] ; 
      if(interCode_Subject[j] == 1){
        holdSubject = getElement("subject",j);   //extracting the subject out of the index
      }
    }
  
  }

  

  
int ncourseno2 = entries( "courseno" );
FILE *fpCode_Courseno = fopen( "code_courseno.rel", "rb" );

char *arrayCourseno = malloc( nCode1*ncourseno2 );

fread( arrayCourseno, 1, nCode1*ncourseno2, fpCode_Courseno );

fclose( fpCode_Courseno );

char *interCode_Courseno = malloc(nCode1); 
int coursenonegHolder = -1;
char* holdCourseno =  malloc(nCode1); 


 if ( (indexCode == -1) && (coursenonegHolder >=0) )
  {
    for (int i=0;i<nCode1;i++)
    {
      int index = i*ncourseno2 +coursenonegHolder;
      interCode_Courseno[i] = arrayCourseno[index];  
    }
  }

  if ( (indexCode > 0) && (coursenonegHolder == -1) )
  {
    for (int j=0; j < ncourseno2 ; j++)
    {
      int index = indexCode*ncourseno2 + j;
      interCode_Courseno[j] = arrayCourseno[index];
      if(interCode_Courseno[j] == 1){
   
        int indexForCourseno = j;

        holdCourseno = getElement("courseno",indexForCourseno);  //extracting the course no out of the index

      }
    }
  }

int nday2 = entries( "days" );
FILE *fpCode_Days = fopen( "code_days.rel", "rb" );

char *arrayDays = malloc( nCode1*nday2 );

fread( arrayDays , 1, nCode1*nday2, fpCode_Days );

fclose( fpCode_Days );

char *interCode_Days = malloc(nCode1); 
int daysnegHolder = -1;
char* holdDays =  malloc(nCode1); 

  if ( (indexCode > 0) && (daysnegHolder == -1) )
  {
    for (int j=0; j < nday2 ; j++)
    {

      int index = indexCode*nday2 + j;
      interCode_Days[j] = arrayDays[index];   
      if(interCode_Days[j] == 1){
        int indexForDays = j;
    
         holdDays = getElement("days",indexForDays);  //extracting the days out of the index
     
      }
    }

  }

int nfrom2 = entries( "from" );
FILE *fpCode_From = fopen( "code_from.rel", "rb" );

char *arrayFrom = malloc( nCode1*nfrom2 );

fread( arrayFrom , 1, nCode1*nfrom2, fpCode_From );

fclose( fpCode_From );

char *interCode_From = malloc(nCode1); 
int fromnegHolder = -1;
char* holdFrom =  malloc(nCode1); 
 

  if ( (indexCode > 0) && (fromnegHolder == -1) )
  {

    for (int j=0; j < nfrom2 ; j++)
    {

      int index = indexCode*nfrom2 + j;
      interCode_From[j] = arrayFrom[index];  
      if(interCode_From[j] == 1){

    
        int indexForFrom = j;
        holdFrom = getElement("from",indexForFrom);  //extracting the from out of the index
       
      }

    }

  }

int nto2 = entries( "to" );
FILE *fpCode_To = fopen( "code_to.rel", "rb" );

char *arrayTo = malloc( nCode1*nto2 );

fread( arrayTo , 1, nCode1*nto2, fpCode_To );

fclose( fpCode_To );

char *interCode_To = malloc(nCode1); 
int tonegHolder = -1;
char* holdTo =  malloc(nCode1); 

  if ( (indexCode > 0) && (tonegHolder == -1) )
  {
    for (int j=0; j < nto2 ; j++)
    {
      int index = indexCode*nto2 + j;
      interCode_To[j] = arrayTo[index];   

     if(interCode_To[j] == 1){
  
        int indexForTo = j;
        holdTo = getElement("to",indexForTo);  //extracting the to out of the index
       
      }
    }
  
  }

  strcpy(finalStoredPrev,finalStored);  //maintaining a previuos pointer element for later comparison
  snprintf(finalStored, nCode1,"%s %s %s %s %s",  holdSubject, holdCourseno, holdDays, holdFrom, holdTo );  // doing a string concatenation


    if(strcmp(finalStored,finalStoredPrev) !=0){

      printf("%s*%s %s %s - %s\n", holdSubject, holdCourseno, holdDays, holdFrom, holdTo ); 

      }

    }


  }
 
}

