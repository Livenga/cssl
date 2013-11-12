#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* 端末設定の変更 */
int InputPass(char *Pass) {
  struct termios tty, tty_s;

  /* 端末情報の取得 */
  if(ioctl(1, TCGETS, &tty) == EOF) {
    perror("ioctl(2)");
    return EOF;
  }

  /* 端末情報の保存 */
  tty_s = tty;

  /* エコーモードに設定 */
  tty.c_lflag &= ~(ECHO);

  /* 変更した情報に設定 */
  if(ioctl(1, TCSETS, &tty) == EOF) {
    perror("ioctl(2)");
    return EOF;
  }

  printf("\tPassWord: ");
  fgets(Pass, 64, stdin);

  Pass[strlen(Pass) - 1] = '\0';
  putchar('\n');

  /* バックアップ状態に変更 */
  if(ioctl(0, TCSETS, &tty_s) == EOF) {
    perror("ioctl(2)");
    return EOF;
  }

  return 1;
}

int NoHideMode(char *Pass) {
  printf("\tPassWord: ");
  fgets(Pass, 64, stdin);

  Pass[strlen(Pass) - 1] = '\0';
  putchar('\n');

  return 1;
}
