// socket programming
// Server socket code:

// 1. Create a socket
// 2. Bind the socket
// 3. Listen for connections
// 4. Accept the connection
// 5. Send and receive data
// 6. Close the connection
// 7. Close the socket
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define inputSize 1000
int PORT = 5001;
// function get char array print it and return char array:
#define Buffer 1000
#include <ctype.h>
void *charE(void *u);
void *charI(void *u);
void *charO(void *u);
void *charU(void *u);
int sum_ = 0;
char key_[30] = "1101";
int getResult(char *CRC_input, char *CRC_key)
{
    int i, j, keylen, msglen, reciver_ = 1;
    char temp[3000], quot[1000], rem[3000], key1[3000];

    keylen = strlen(CRC_key);
    msglen = strlen(CRC_input);
    strcpy(key1, CRC_key);
    for (i = 0; i < keylen - 1; i++)
    {
        CRC_input[msglen + i] = '0';
    }
    for (i = 0; i < keylen; i++)
        temp[i] = CRC_input[i];
    for (i = 0; i < msglen; i++)
    {
        quot[i] = temp[0];
        if (quot[i] == '0')
            for (j = 0; j < keylen; j++)
                CRC_key[j] = '0';
        else
            for (j = 0; j < keylen; j++)
                CRC_key[j] = key1[j];
        for (j = keylen - 1; j > 0; j--)
        {
            if (temp[j] == CRC_key[j])
                rem[j - 1] = '0';
            else
                rem[j - 1] = '1';
        }
        rem[keylen - 1] = CRC_input[i + keylen];
        strcpy(temp, rem);
    }
    strcpy(rem, temp);

    for (i = 0; i < keylen - 1; i++)
    {
        if (rem[i] == '1' && reciver_ == 1)
        {
            printf("Error in reciving data");
            return 0;
        }

    }

    return 1;
}

void *sum(void *u)
{
    char *ptr = (char *)u;
    int i, sum = 0, n = 0;

    for (i = 0; i <= Buffer; ++i)
    {
        if (ptr[i] >= '0' && ptr[i] <= '9')
        {
            int val = ptr[i] - '0';
            sum = sum + val;
        }
        else
        {
            n++;
        }
    }
    sum_ = sum;
    printf("The sum is %d\n\n ", sum);
}

void *charA(void *u)
{
    char *ptr = (char *)u;
    int n = 0;
    pthread_t CRC_thread2;
    for (int i = 0; i <= Buffer; i++)
    {
        if (ptr[i] == 'a')
        {
            ptr[i] = 'A';
        }
        else
        {
            //printf(" ");
        }
    }

    pthread_create(&CRC_thread2, NULL, &charE, ptr);
    pthread_join(CRC_thread2, NULL);
}

void *charE(void *u)
{
    char *ptr = (char *)u;
    int n = 0;
    pthread_t CRC_thread3;
    for (int i = 0; i <= Buffer; i++)
    {
        if (ptr[i] == 'e')
        {
            ptr[i] = 'E';
        }
        else
        {
           // printf(" ");
        }
    }

    pthread_create(&CRC_thread3, NULL, &charI, ptr);
    pthread_join(CRC_thread3, NULL);
}

void *charI(void *u)
{
    char *ptr = (char *)u;
    int n = 0;
    pthread_t CRC_thread4;
    for (int i = 0; i <= Buffer; i++)
    {
        if (ptr[i] == 'i')
        {
            ptr[i] = 'I';
        }
        else
        {
           // printf(" ");
        }
    }

    pthread_create(&CRC_thread4, NULL, &charO, ptr);
    pthread_join(CRC_thread4, NULL);
}

void *charO(void *u)
{
    char *ptr = (char *)u;
    int n = 0;
    pthread_t CRC_thread5;
    for (int i = 0; i <= Buffer; i++)
    {
        if (ptr[i] == 'o')
        {
            ptr[i] = 'O';
        }
        else
        {
           // printf(" ");
        }
    }

    pthread_create(&CRC_thread5, NULL, &charU, ptr);
    pthread_join(CRC_thread5, NULL);
}

void *charU(void *u)
{
    char *ptr = (char *)u;
    int n = 0;
    for (int i = 0; i <= Buffer; i++)
    {
        if (ptr[i] == 'u')
        {
            ptr[i] = 'U';
        }
        else
        {
            //printf(" ");
        }
    }

}

int main(int argc, char *argv[])
{
    int id;
    pthread_t CRC_thread;
    pthread_t CRC_thread1;
    pthread_t CRC_thread2;
    pthread_t CRC_thread4;
    pthread_t CRC_thread5;
    if (argv[1] > 0)
    {
        char buf[30];
       // snprintf(buf, 1800, "fuser -k %d/tcp", atoi(argv[1]));

        PORT = atoi(argv[1]);
        system(buf);
    }
    else
    {
        // kill port if it is already in use
        system("fuser -k 5001/tcp");
    }
    
    // Socket code is written by taking the reference of project document 3
    
    int sockfd, newsockfd, clilen, n;
    char buffer[1800];
    struct sockaddr_in serv_addr, cli_addr;

    // 1. Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Bind the socket
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    // 3. Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // 4. Accept the connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }

    // 5. Send and receive data
    bzero(buffer, 1800);
    n = read(newsockfd, buffer, 1800);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    else
    {
        int result = getResult(buffer, key_);
        {
            if (result == 1)
            {
                printf("CRC Communication succeeded\n");
            }
            else
            {
                printf("CRC Communication failed\n");
            }
        }
        bzero(buffer, 1800);
        n = read(newsockfd, buffer, 1799);
        pthread_create(&CRC_thread, NULL, &sum, &buffer);
        pthread_create(&CRC_thread1, NULL, &charA, &buffer);

        pthread_join(CRC_thread, NULL);
        pthread_join(CRC_thread1, NULL);
         printf("New buffer : %s", buffer);


        n = write(newsockfd, buffer, 1800);
        n = write(newsockfd, "\nSum is: ", 9);
        // sum_ (int) to char conversion:
        //--------------------
        char sum_string[10];
        sprintf(sum_string, "%d", sum_);
        n = write(newsockfd, sum_string, 10);
        //--------------------
        n = write(newsockfd, "\nend", 3);
        // sleep(2);
    }


    // 6. Close the connection
    close(newsockfd);

    // 7. Close the socket
    close(sockfd);
    return 0;
}
