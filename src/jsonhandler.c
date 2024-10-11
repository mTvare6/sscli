#include "jsonhandler.h"
#include <stdlib.h>
#include <json-c/json.h>
#include <curl/curl.h>
const char* easycat(const char *l, const char *h, size_t hlen)
{
  char *comb = malloc(hlen + strlen(h) + 1);
  if(comb==NULL){
    perror("malloc()");
    exit(MALLOC_FAILED);
  }
  strcpy(comb, l);
  strcat(comb, h);
  return comb;
}


void strinit(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t strcurlwrite(void *ptr, size_t size, size_t nmemb, struct string *s){
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

struct string latestjson(){
  CURL *curl;
  CURLcode res;
  struct string jsonblob;
  strinit(&jsonblob);
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "https://ap-south-1.aws.realm.mongodb.com/api/client/v2.0/app/data-rgzxa/auth/providers/api-key/login");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{ \"key\" : \"BFXDWTSqEFOXbk5kzCYsqV50vL4YjUQywDUfcE5wy0cD01SsJ7nLFduSzjE4Or3W\" }");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, strcurlwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonblob);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    json_object *root = json_tokener_parse(jsonblob.ptr);
    const char *access_token = json_object_get_string(json_object_object_get(root, "access_token"));
    size_t t = 22 + strlen(access_token) + 1;
    char *he = malloc(t);
    strcpy(he, "Authorization: Bearer ");
    strcat(he, access_token);
    he[t-1] = '\0';
    headers = curl_slist_append(headers, he);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, "https://ap-south-1.aws.data.mongodb-api.com/app/data-rgzxa/endpoint/data/v1/action/find");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"dataSource\":\"Cluster0\",\"database\":\"student_search\",\"collection\":\"student_search\",\"filter\":{},\"limit\":30000}");
    free(jsonblob.ptr);
    strinit(&jsonblob);
    res = curl_easy_perform(curl);
    json_object_put(root);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return jsonblob;
}

