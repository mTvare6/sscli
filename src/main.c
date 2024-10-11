#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <json-c/json.h>
#include <pwd.h>
#include <sys/stat.h>
#include <wchar.h>
#include "c-flags.h"
#include "jsonhandler.h"
#define fieldstr(e, x) json_object_get_string(json_object_object_get((e), (x)))
bool file_exists (char *filename);
int main(int argc, char **argv){
  if (argc > 0)
    c_flags_set_application_name(argv[0]);

  c_flags_set_description("Student Search CLI for IITK");
  bool *help = c_flag_bool("help", "h", "show usage", false);
  bool *update = c_flag_bool("update", "u", "update jsondb", false);
  bool *csv = c_flag_bool("csv", "c", "enable csv mode", false);
  char **fname = c_flag_string("name", "n", "search by name", NULL);
  char **fhall = c_flag_string("hall", "l", "search by hall", NULL);
  char **fdept = c_flag_string("dept", "d", "search by dept", NULL);
  char **froll = c_flag_string("roll", "r", "search by roll", NULL);
  char **fprog = c_flag_string("prog", "p", "search by program", NULL);
  char **fhome = c_flag_string("home", "o", "search by hometown", NULL);
  int  *fyear = c_flag_int("year", "y", "search by year", 0);
  c_flags_parse(&argc, &argv, false);
  bool bname = *fname!=NULL;
  bool bhall = *fhall!=NULL;
  bool bdept = *fdept!=NULL;
  bool broll = *froll!=NULL;
  bool bprog = *fprog!=NULL;
  bool bhome = *fhome!=NULL;
  bool byear = *fyear!=0;

  if (*help) {
    c_flags_usage();
    return 0;
  }
  if(byear && *fyear < 10){
    perror("SS supports only Y > 10");
  }

  char *data_home = getenv("XDG_DATA_HOME");

  const char *homedir;
  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }

  if(data_home==NULL){
    data_home=malloc(strlen(homedir)+19);
    if(data_home==NULL){
      perror("malloc() returned NULL");
      exit(MALLOC_FAILED);
    }
    strcpy(data_home, homedir);
    strcat(data_home, "/.local/share/ss/");

  } else{
    char *j = data_home;
    data_home = malloc(strlen(data_home))+5;
    strcpy(data_home, j);
    if(data_home==NULL){
      perror("malloc() returned NULL");
      exit(MALLOC_FAILED);
    }
    strcat(data_home, "/ss/");
  }

  size_t data_home_len = strlen(data_home);
  const char* dbfile = easycat(data_home, "main.json", data_home_len);

  mkdir(data_home, S_IRWXU | S_IRWXG | S_IRWXO);

  if(*update) remove(dbfile);

  bool db_exists = file_exists((char *)dbfile);
  struct string jstring;
  strinit(&jstring);
  FILE *dbfp;
  json_object *root;

  if(!db_exists){
    dbfp=fopen(dbfile, "w+");
    if(dbfp==NULL){
      perror("Cannot open dbfile");
      exit(2);
    }
    jstring = latestjson();
    fprintf(dbfp, "%s", jstring.ptr);
  }
  else{
    dbfp = fopen(dbfile, "r");
    fseek(dbfp, 0, SEEK_END);
    jstring.len = ftell(dbfp);
    fseek(dbfp, 0, SEEK_SET);
    jstring.ptr = malloc(jstring.len + 1);
    fread(jstring.ptr, jstring.len, 1, dbfp);
    fclose(dbfp);
    jstring.ptr[jstring.len] = '\0';
  }
  root = json_tokener_parse(jstring.ptr);
  json_object *students = json_object_object_get(root, "documents");
  int temp_n = json_object_array_length(students);
  for (int i = 0; i < temp_n; i++){
    json_object *e = (json_object_array_get_idx(students, i));
    const char *home = fieldstr(e, "a");
    const char *bgrp = fieldstr(e, "b");
    const char *dept = fieldstr(e, "d");
    const char *genr = fieldstr(e, "g");
    const char *hall = fieldstr(e, "h");
    const char *roll = fieldstr(e, "i");
    const char *name = fieldstr(e, "n");
    const char *prog = fieldstr(e, "p");
    const char *room = fieldstr(e, "r");
    const char *user = fieldstr(e, "u");
    /*const char *sgrl = fieldstr(e, "s");*/
    /*json_object *children = (json_object_object_get(e, "c"));*/

    if(bname)
      if(strcasestr(name, *fname)==NULL)
        continue;
    if(broll)
      if(strcasestr(roll, *froll)==NULL)
        continue;
    if(bdept)
      if(strcasestr(dept, *fdept)==NULL)
        continue;
    if(bhall)
      if(strcasestr(hall, *fhall)==NULL)
        continue;
    if(bprog)
      if(strcasestr(prog, *fprog)==NULL)
        continue;
    if(bhome)
      if(strcasestr(home, *fhome)==NULL)
        continue;
    if(byear){
      size_t j=0;
      size_t tn = strlen(roll);
      for(;j<tn;){
        if(!('0' < roll[j] && roll[j] < '9')) j++;
        else break;
      }
      if(!( (roll[j]-'0')*10 + (roll[j+1]-'0') == *fyear))
        continue;
    }

    if(!*csv)
      printf("%-40s%-15s%-50s%-20s%-20s\n", name, roll, dept, user, home);
    else
      printf("%40s,%15s,%50s,%20s,%20s\n", name, roll, dept, user, home);
  }

}
bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}
