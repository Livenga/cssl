#define URL "auth-nac2.ntwk.dendai.ac.jp"

/* ユーザの情報を格納する構造体 */

struct userinfo {
  char name[32];
  char device[16];
  char ip[32];
  char pass[32];
  struct sockaddr_in uNet;
};
