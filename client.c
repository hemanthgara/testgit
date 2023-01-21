// socket programming
// client socket code:

// 1. Create a socket
// 2. Connect to the server
// 3. Send and receive data
// 4. Close the connection
// 5. Close the socket

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include<fcntl.h>

int PORT = 5001;
char key_variable[3000] = "1101";
char input__variable[3200] = "101101110";
char result_variable[3200];
char fileRead[1000];
// establishing CRC communication by understanding the reference from https://www.way2techin.com/2018/01/cyclic-redundancy-check.html

void CRC_communication(char *CRC_input, char *CRC_key)
{
    int i, j, key_length, messagelength;
    char temp[3000], quot[1000], CRC_reminder[3000], key1[3000];
    key_length = strlen(CRC_key);
    messagelength = strlen(CRC_input);
    strcpy(key1, CRC_key);
    for (i = 0; i < key_length - 1; i++)
    {
        CRC_input[messagelength + i] = '0';
    }
    for (i = 0; i < key_length; i++)
        temp[i] = CRC_input[i];
    for (i = 0; i < messagelength; i++)
    {
        quot[i] = temp[0];
        if (quot[i] == '0')
            for (j = 0; j < key_length; j++)
                CRC_key[j] = '0';
        else
            for (j = 0; j < key_length; j++)
                CRC_key[j] = key1[j];
        for (j = key_length - 1; j > 0; j--)
        {
            if (temp[j] == CRC_key[j])
                CRC_reminder[j - 1] = '0';
            else
                CRC_reminder[j - 1] = '1';
        }
        CRC_reminder[key_length - 1] = CRC_input[i + key_length];
        strcpy(temp, CRC_reminder);
    }
    strcpy(CRC_reminder, temp);
    for (i = 0; i < messagelength; i++)
    {
        result_variable[i] = CRC_input[i];
    }
    for (i = 0; i < key_length - 1; i++)
    {
        result_variable[i + messagelength] = CRC_reminder[i];
    }
}
int filehandling()
{
    int fp = open("text.txt",O_RDONLY);
    printf("fiel pointer is %d\n",fp);
    read(fp,fileRead,1000);
    
}
int main(int argc, char *argv[])
{

    if (argv[1] > 0)
    {
        PORT = atoi(argv[1]);
    }
    int socketID, n;
    char buffer[3000];
    struct sockaddr_in serv_addr;

    // 1. Create a socket
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    // 2. Connect to the server
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (connect(socketID, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }
    CRC_communication(input__variable, key_variable);
    n = write(socketID, result_variable, strlen(result_variable));
    // 3. Send and receive data
    filehandling();
        printf("File read : %s\n", fileRead);
        n = write(socketID, fileRead, strlen(fileRead));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    bzero(buffer, 3000);
    // getData:
    int noofinput = 0;
    do
    {
        bzero(buffer, 3000);
        n = read(socketID, buffer, 3000);
        printf("final data recieved at client side is : %s\n",buffer);
        int opf = open("result.txt",O_WRONLY | O_CREAT);
            write(opf,buffer,2000);
        
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }


        noofinput--;
    } while (noofinput > 0);
    // 4. Close the connection
    close(socketID);

    return 0;
}

// 5. Close the socket