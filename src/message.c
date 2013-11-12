#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <netdb.h>
/* SSL Library */
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
/* Original */
#include <users.h>
#include <SSL_Message.h>

#define BUFFER_SIZE 2048
#define MESSAGE_SIZE 65536

int WriteMessage_SSL(SSL *ssl, struct userinfo user) {
  int i;
  int err;
  int Header_Height = sizeof(Message_Head) / sizeof(Message_Head[0]);
  int Buttom_Height = sizeof(Message_Buttom) / sizeof(Message_Buttom[0]);

  char Content[MESSAGE_SIZE];

  for(i = 0; i < Header_Height; i++) {
    if((err = SSL_write(ssl, Message_Head[i], strlen(Message_Head[i]))) < 1) {
      return EOF;
    }
  }

  sprintf(Content, "reqFrom=perfigo_login.jsp&uri=&cm=&userip=%s&session=&pm=Linux+x86_64&index=2&pageid=-1&compact=false&registerGuest=NO&userNameLabel=UserID&passwordLabel=Password&guestUserNameLabel=Guest+ID&guestPasswordLabel=Password&username=%s&password=%s&provider=TDU+UserGET /auth/TDU_logo_A[320x42].jpg https/1.1\r\n", user.ip, user.name, user.pass);

  if((err = SSL_write(ssl, Content, strlen(Content))) < 1) {
    return EOF;
  }

  for(i = 0; i < Buttom_Height; i++) {
    if((err = SSL_write(ssl, Message_Buttom[i], strlen(Message_Buttom[i]))) < 1) {
      return EOF;
    }
  }


  return 1;
}

int ReadMessage_SSL(SSL *ssl) {
  int byte;
  char buffer[BUFFER_SIZE];

  while(1) {
    if((byte = SSL_read(ssl, buffer, BUFFER_SIZE)) == EOF) {
      perror("SSL_read(3)");
      return EOF;
    }
    if(byte == 0) break;
    if(write(1, buffer, byte) == EOF) {
      perror("write(2)");
      return EOF;
    }
  }

  return 1;
}
