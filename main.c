#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#define TESTS_NUMBER 5
#define DATA_SIZE 100


int num_of_digits(int num){
    int ret = 1;
    while (num /= 10) {
        ++ret;
    }
    return ret;
}

int test_counter = 0;
typedef int32_t str_len;
typedef int8_t* str;

typedef enum return_code return_code;
enum return_code{
    CUBE_OAUTH2_ERR_OK = 0,
    CUBE_OAUTH2_ERR_TOKEN_NOT_FOUND,
    CUBE_OAUTH2_ERR_DB_ERROR,
    CUBE_OAUTH2_ERR_UNKNOWN_MSG,
    CUBE_OAUTH2_ERR_BAD_PACKET,
    CUBE_OAUTH2_ERR_BAD_CLIENT,
    CUBE_OAUTH2_ERR_BAD_SCOPE,
};

char* error_str(return_code code){
    char *ret;
    if (code == 0) {
        ret = "";
        return ret;
    }
    else if (code == 1){
        ret = "token not found";
        return ret;
    }
    else if (code == 2){
        ret = "db error";
    }
    else if(code == 3){
        ret = "unknown svc message type";
    }
    else if(code == 4){
        ret = "bad packet";
    }
    else if(code == 5){
        ret = "bad client";
    }
    else {
        ret = "bad scope";
    }
    return NULL;
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
typedef struct string string;
struct string{
    str_len string_length;
    str string;
};
typedef struct data data;
struct data{
    string client_id;
    int32_t client_type;
    str username;
    int32_t expires_in;
    int64_t user_id;
    string tocken;
    string scope;
};
typedef struct header header;
struct header{
    int32_t svc_id;
    int32_t body_length;
    int32_t request_id;
};
int fill_userdata(){
    return 0;
}
data users_data[DATA_SIZE];

string* cube_string(str_len string_length){
    string *return_string = malloc(sizeof(string));
    if (return_string == NULL) {
        return NULL;
    }
    return_string->string = malloc(string_length * sizeof(int8_t));
    if (return_string->string == NULL) {
        free(return_string);
        return NULL;
    }
    
    return_string->string_length = string_length;
    return return_string;//???
}

void free_cube_string(string* str){
    while (str != NULL) {
        free(str->string);
        free(str);
    }
}



int fill_db(){
    for (int i = 0; i < DATA_SIZE; ++i) {
        char* client = "client";
        char* tocken = "tocken";
        char* scope = "scope";

        int8_t *client_id_str = malloc(sizeof(int8_t) * ( strlen(client) + num_of_digits(i) ) );
        int8_t *tocken_str = malloc(sizeof(int8_t) * ( strlen(tocken) + num_of_digits(i) ) );
        int8_t *scope_str = malloc(sizeof(int8_t) * ( strlen(scope) + num_of_digits(i) ) );

        sprintf(client_id_str, "client%d", i);
        sprintf(tocken_str, "tocken%d", i);
        sprintf(scope_str, "scope%d", i);

        users_data[i].client_id.string_length = strlen(client_id_str);
        users_data[i].client_id.string = client_id_str;
        
        users_data[i].expires_in = i*i*i % 213;
        users_data[i].user_id = i+100;

        users_data[i].client_type = i*i*762 % 874;
    }
    return 0;
}

void print_db(){
    for (int i = 0; i < DATA_SIZE; ++i) {
        printf("%s\n", users_data[i].client_id.string);
        printf("%d\n", users_data[i].expires_in);

    }
}

int check(string tocken, string scope){

    return 0;
}
int main(int argc, char *argv[])
{

    fill_db();
   // print_db();

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 1) {
        fprintf(stderr,"ERROR, enter port number\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    
    if (newsockfd < 0)
        error("ERROR on accept");
   
    header my_header;
    {
        memset(&my_header, 0, sizeof(header));
        
        int32_t rd = 0;
        int32_t left = sizeof(header);
        while (left) {
            ssize_t ret = read(newsockfd,((void*)&my_header) + rd, left);
            if (ret >= 0) {
                rd += ret;
                left -= ret;
            }
            else {
                perror("ERROR reading header\n");
                return -1;
            }
        }
    }
    printf("svc_id:%d \n body_length:%d\n request_id:%d\n ", my_header.svc_id,
           my_header.body_length, my_header.request_id);
    int8_t* body_buffer = malloc(sizeof(int8_t) * my_header.body_length);
    {
        int32_t rd = 0;
        int32_t left = my_header.body_length;
        while (left) {
            ssize_t ret = read(newsockfd,((void*)body_buffer) + rd, left);
            if (ret >= 0) {
                rd += ret;
                left -= ret;
            }
            else {
                error("ERROR reading body\n");
            }
        }
        for (int i = 0; i < my_header.body_length;  ++i) {
            printf("%X",body_buffer[i]);
        }
    }
   

    
//    int32_t svc_msg = ((int32_t *)body_buffer)[0];
//    int32_t token_len = ((int32_t *)body_buffer)[1];
//    ssize_t svc_tocken_len_size = 2 * sizeof(int32_t);
//    int8_t* tmp = body_buffer;
//    tmp = tmp + svc_tocken_len_size;
//    int8_t* tocken = malloc(sizeof(int8_t) * token_len );
//    int len = token_len;
//    printf("svc_msg:%d \n tocken_length:%d\n ", svc_msg, token_len);
//
//    for (int i = 0; i < len; ++i) {
//        tocken[i] = tmp[i];
//    }

    {
        int32_t wr = 0;
        int32_t left = sizeof(header);
        while (left) {
            ssize_t ret = write(newsockfd,((void*)&my_header) + wr, left);
            if (ret >= 0) {
                wr += ret;
                left -= ret;
            }
            else {
                perror("ERROR writing header\n");
                return -1;
            }
        }
    }
    {
        int32_t rd = 0;
        int32_t left = my_header.body_length;
        while (left) {
            ssize_t ret = write(newsockfd,((void*)body_buffer) + rd, left);
            if (ret >= 0) {
                rd += ret;
                left -= ret;
            }
            else {
                error("ERROR reading body\n");
            }
        }
    }
    
    close(newsockfd);
    close(sockfd);
    return 0;
}
