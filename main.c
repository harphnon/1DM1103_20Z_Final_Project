#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define N 12

struct pola
{
    int x1; int y1; char *type1;
    int x2; int y2; char *type2;
    int x3; int y3; char *type3;
};

struct mapa
{
    char *pola[N][N];
};

typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    Memory *mem = (Memory *) userp;
    char *ptr = NULL;

    if (mem->response != NULL)
        ptr = realloc(mem->response, mem->size + realsize + 1);
    else
        ptr = malloc(mem->size + realsize + 1);

    if (ptr == NULL)
        return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char * make_request(char *url){
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        return chunk.response;
        free(chunk.response);
        curl_easy_cleanup(curl);

    }
}

char * info() { 
    char *res;
    char *url = "w";
    res = make_request(url);
    return res;
}

char * move() {
    char *res;
    char *url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/qwerty_11";
    res = make_request(url);
    return res;
}

char * explore() {
    char *res;
    char *url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/qwerty_11";
    res = make_request(url);
    return res;
}

char * rotate(char *site) {
    char *res;
    char *url;
    if(strcmp(site, "right") == 0){
        url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_11/right";
    }
    else if (strcmp(site, "left") == 0){
        url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_11/left";
    }
    res = make_request(url);
    return res;
}

struct pola get_data(char *expl){
    
    struct pola p;

    strtok(expl, "{");
    strtok(NULL, "{");
    strtok(NULL, "{");
    
    char *firste = strtok(NULL, "}");
    strtok(NULL, "{");
    
    char *seconde = strtok(NULL, "}");
    strtok(NULL, "{");
    
    char *thirde = strtok(NULL, "}");
        
    strtok(firste, " ");
    char *x1 = strtok(NULL, ",");
    p.x1 = x1[0] - '0';
    strtok(NULL, " ");
    char *y1 = strtok(NULL, ",");
    p.y1 = y1[0] - '0';
    p.y1 = N - p.y1;
    strtok(NULL, ":");
    strtok(NULL, "\"");
    p.type1 = strtok(NULL, "\"");

    strtok(seconde, " ");
    char *x2 = strtok(NULL, ",");
    p.x2 = x2[0] - '0';
    strtok(NULL, " ");
    char *y2 = strtok(NULL, ",");
    p.y2 = y2[0] - '0';
    p.y2 = N - p.y2;
    strtok(NULL, ":");
    strtok(NULL, "\"");
    p.type2 = strtok(NULL, "\"");

    strtok(thirde, " ");
    char *x3 = strtok(NULL, ",");
    p.x3 = x3[0] - '0';
    strtok(NULL, " ");
    char *y3 = strtok(NULL, ",");
    p.y3 = y3[0] - '0';
    p.y3 = N - p.y3;
    strtok(NULL, ":");
    strtok(NULL, "\"");
    p.type3 = strtok(NULL, "\"");

    return p;
}

struct mapa generate_map(){
    struct mapa m; 
    for ( int i = 0 ; i < N ; i++ ){
        for ( int j = 0 ; j < N ; j++ ){
            m.pola[i][j] = " ";
        }
    }
    return m;
}

void print_map(struct mapa m){
    for ( int i = 0 ; i < N ; i++ ){
        printf("%.2d ", i);
        for ( int j = 0 ; j < N ; j++ ){
            printf(" %s ", m.pola[j][i]);
        }
        printf("\n");
    }
}

struct mapa update_map(struct mapa m, struct pola p){
    struct mapa mnew = m;
    if(strcmp(p.type1, "sand") == 0){
        mnew.pola[p.x1][p.y1] = "S";
    }
    else if(strcmp(p.type1, "grass") == 0){
        mnew.pola[p.x1][p.y1] = "G";
    }
    else if(strcmp(p.type1, "wall") == 0){
        mnew.pola[p.x1][p.y1] = "W";
    }
    if(strcmp(p.type2, "sand") == 0){
        mnew.pola[p.x2][p.y2] = "S";
    }
    else if(strcmp(p.type2, "grass") == 0){
        mnew.pola[p.x2][p.y2] = "G";
    }
    else if(strcmp(p.type2, "wall") == 0){
        mnew.pola[p.x2][p.y2] = "W";
    }
    if(strcmp(p.type3, "sand") == 0){
        mnew.pola[p.x3][p.y3] = "S";
    }
    else if(strcmp(p.type3, "grass") == 0){
        mnew.pola[p.x3][p.y3] = "G";
    }
    else if(strcmp(p.type3, "wall") == 0){
        mnew.pola[p.x3][p.y3] = "W";
    }
    return mnew;
}

void main(int argc, char **argv)
{
    struct pola p;
    struct mapa map = generate_map();

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "info") == 0){
            printf("\ninformacje\n");
            printf("%s\n", info());
        }
        else if(strcmp(argv[i], "m") == 0){
            printf("\nruch\n");
            printf("%s\n", move());
        }
        else if(strcmp(argv[i], "e") == 0){
            char *expl = explore();
            struct mapa m;
            printf("\nrozgladanie sie\n");
            printf("%s\n", expl);
            p = get_data(expl);
            // printf("%d\n", p.x1);
            // printf("%d\n", p.y1);
            // printf("%s\n\n", p.type1);
            // printf("%d\n", p.x2);
            // printf("%d\n", p.y2);
            // printf("%s\n\n", p.type2);
            // printf("%d\n", p.x3);
            // printf("%d\n", p.y3);
            // printf("%s\n\n", p.type3);
            map = update_map(map, p);
            
        }
        else if(strcmp(argv[i], "r") == 0){
            printf("\nobrot w prawo\n");
            printf("%s\n", rotate("right"));
        }
        else if(strcmp(argv[i], "l") == 0){
            printf("\nobrot w lewo\n");
            printf("%s\n", rotate("left"));
        }
    print_map(map);
    }
}

