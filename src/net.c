#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <netdb.h>
/* SSL Library */
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
/* Original */
#include <users.h>

/* src/message.c */
extern int WriteMessage_SSL(SSL *ssl, struct userinfo user);
extern int ReadMessage_SSL(SSL *ssl);

/* デバイスからIPアドレスを取得 */
char *GetMyIpAddress(const char *device) {
  int sock;
  struct ifreq ifr;
  struct sockaddr_in net;

  memset(&net, 0, sizeof(struct sockaddr_in));

  ifr.ifr_addr.sa_family = AF_INET;
  strcpy(ifr.ifr_name, device);

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == EOF) {
    perror("socket(2)");
    exit(EXIT_FAILURE);
  }

  if(ioctl(sock, SIOCGIFADDR, &ifr) == EOF) {
    perror("ioctl(2)");
    close(sock);
    exit(EXIT_FAILURE);
  }

  close(sock);

  memcpy(&net, &ifr.ifr_ifru.ifru_addr, sizeof(struct sockaddr_in));

  return inet_ntoa(net.sin_addr);
}

int CreateSocket(const char* Url, struct sockaddr_in *Net) {
  int sock, err = 1;

  /* ホスト, サービスの情報を格納する構造体 */
  struct hostent *Host;
  struct servent *Service;

  /* 構造体の初期化 */
  bzero(&Host, sizeof(Host));
  bzero(&Service, sizeof(Service));

  /* ホスト情報の取得 */
  if((Host = gethostbyname(Url)) == NULL) {
    perror("gethostbyname(3)");
    return EOF;
  }

  /* サービス情報の取得 */
  if((Service = getservbyname("https", "tcp")) == NULL) {
    Net->sin_port = htons(443);
    err = 0;
  }

  printf("\tOfficial Service Name\t:%s\n", Service->s_name);
  printf("\tPort Number\t\t:%d\n", 443);
  printf("\tProtocol\t\t:%s\n", Service->s_proto);

  /* 接続する際の情報を格納 */
  Net->sin_family = AF_INET;
  bcopy(Host->h_addr_list[0], (char *)&Net->sin_addr, Host->h_length);
  if(err) {
    Net->sin_port = Service->s_port;
  }

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == EOF) {
    perror("socket(2)");
    return EOF;
  }

  return sock;
}

int SSL_Socket(int sock, struct userinfo user, const int flag) {
  int err = 0;
  SSL *ssl;
  SSL_CTX *ctx;

  /* 通常の接続 */
  if(connect(sock, (struct sockaddr *)&user.uNet, sizeof(struct sockaddr_in)) == EOF) {
    perror("connect(2)");
    return EOF;
  }

  /* メッセージの簡読化 */
  SSL_load_error_strings();
  SSL_library_init();

  if((ctx = SSL_CTX_new(SSLv3_client_method())) == NULL) {
    ERR_print_errors_fp(stderr);
    return EOF;
  }

  if((ssl = SSL_new(ctx)) == NULL) {
    ERR_print_errors_fp(stderr);
    return EOF;
  }

  /* SSL ソケットの作成 */
  if((err = SSL_set_fd(ssl, sock)) == 0) {
    ERR_print_errors_fp(stderr);
    return EOF;
  }

  /* No Required ?*/
  RAND_poll();
  while(RAND_status() == 0) {
    unsigned short rand_ret = rand() % 65536;
    RAND_seed(&rand_ret, sizeof(rand_ret));
  }


  if((err = SSL_connect(ssl)) != 1) {
    ERR_print_errors_fp(stderr);
    return EOF;
  }

  printf("\n\tSSL Connected!\n");

  if(WriteMessage_SSL(ssl, user) == EOF) {
    fprintf(stderr, "\tSSL :書き込みに失敗.\n");
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();
    return EOF;
  }

  if(flag) {
    if(ReadMessage_SSL(ssl) == EOF) {
      fprintf(stderr, "\tSSL :読み込みに失敗.\n");
    }
  }

  /* 後処理 */
  SSL_shutdown(ssl);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
  ERR_free_strings();

  return 1;
}
