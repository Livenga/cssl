#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/* SSL Library */
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
/* Original */
#include <users.h>

/* Options */
struct option long_opt[] = {
  {"help",    no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"debug",   no_argument, 0, 'g'},
  {"no_hide", no_argument, 0, 'n'},
  {"print",   no_argument, 0, 'p'},
  {"device", required_argument, 0, 'd'},
  {"user",   required_argument, 0, 'u'},
  {0, 0, 0, 0},
};

/* src/net.c */
extern char *GetMyIpAddress(const char *device); /* IP アドレスの取得 */
extern int CreateSocket(const char* Url, struct sockaddr_in *Net); /* 通常のソケット作成 */
extern int SSL_Socket(int sock, struct userinfo user, const int flag); /* SSL 関係の処理 */

/* src/pass.c */
extern int InputPass(char *Pass); /* パスワードを隠す */
extern int NoHideMode(char *Pass); /* 隠さない状態 */

/* src/main.c */
void print_help(const char *name);
void print_version(const char *name);

int main(int argc, char *argv[])
{
  int sock; /* ソケット変数 */
  int opt, optindex; /* getopt_long(3) */
  int f_help, f_ver, f_dev, f_usr, f_hide; /* オプションのフラグ */
  int f_deb, f_prn; /* オプションフラグ */

  struct sockaddr_in Net;
  struct userinfo user;

  f_help = f_ver = f_dev = f_usr = f_hide = f_deb = 0; /* オプションに対するフラグの初期化 */
  f_prn = 0; /* フラグの初期化 */

  /* 引数の確認 */
  if(argc < 2) {
    fprintf(stderr, "%s: Operand Error.\n", argv[0]);
    fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* オプションの設定 */
  while((opt = getopt_long(argc, argv, "hvgnpa:",
          long_opt, &optindex)) != EOF) {
    switch (opt) {
      case 'h': f_help = 1; break;
      case '?': f_help = 1; break;
      default : f_help = 1; break;
      case 'v': f_ver  = 1; break;
      case 'n': f_hide = 1; break;
      case 'g': f_deb  = 1; break;
      case 'p': f_prn  = 1; break;
      case 'd': f_dev  = 1;
                strcpy(user.device, optarg);
                break;
      case 'u': f_usr  = 1;
                strcpy(user.name, optarg);
                break;
    }
  }

  /* ヘルプ及びバージョンの処理 */
  if(f_help || f_ver) {
    if(f_help) print_help(argv[0]);
    if(f_ver)  print_version(argv[0]);
    exit(EXIT_SUCCESS);
  }

  /* 適切なフラクグの選択 */
  else if(f_dev && f_usr) {
    printf("\tUser Name\t\t:%s\n", user.name);
    printf("\tDevice Name\t\t:%s\n", user.device);

    strcpy(user.ip, GetMyIpAddress(user.device));
    printf("\tIP Address\t\t:%s\n", user.ip);

    if((sock = CreateSocket(URL, &Net)) == EOF) {
      fprintf(stderr, "\t[0]ソケットの作成に失敗.\n");
      exit(EXIT_FAILURE);
    }

    bcopy(&Net, &user.uNet, sizeof(struct sockaddr_in));

    /* ハイドモードではない */
    if(f_hide == 0) {
      if(InputPass(user.pass) == EOF) {
        close(sock);
        printf("パスワード系統のエラー.\n");
        exit(EXIT_FAILURE);
      }
    }

    /* ハイドモード */
    else if(f_hide) {
      NoHideMode(user.pass);
    }

    else {
      print_help(argv[0]);
      close(sock);
      exit(EXIT_FAILURE);
    }

    if(f_deb)
      printf("\tPassWord\t\t:%s\n", user.pass);

    /* SSLソケットの作成と諸々の処理 */
    if(SSL_Socket(sock, user, f_prn) == EOF) {
      fprintf(stderr, "\tSSL内部ので何かしらのエラーの発生.\n");
      close(sock);
      exit(EXIT_FAILURE);
    }

    close(sock);

    printf("\tSee You %s.\n", user.name);
  }

  /* 予期せぬエラー */
  else {
    print_help(argv[0]);
    exit(EXIT_FAILURE);
  }

  return 0;
}

void print_help(const char *name) {
  printf("Usage: %s [Options]\n\n \
      [Options]\n \
      -h, --help\t\t:ヘルプを表示.\n \
      -v, --version\t\t:バージョンの表示.\n \
      -g, --debug\t\t:デバッグモード.(推奨しない, 意味があまりない)\n \
      -n, --no_hide\t\t:パスワードを隠さない.\n \
      -p, --print\t\t:SSLの返してくるものを取得.\n \
          --device\t\t:イーサネットデバイスの指定(デバイス名の選択).\n \
          --user\t\t:学籍番号を指定.\n", name);
}

void print_version(const char *name) {
  printf("%s Version: 1.0.1 (C)Livenga\n", name);
}

