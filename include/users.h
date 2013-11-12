/* SSLに書き込む文章 */
char *Message_Head[] = {
  "POST /auth/perfigo_cm_validate.jsp https/1.1\r\n",
  "Host: auth-nac2.ntwk.dendai.ac.jp\r\n",
  "Connection: keep-alive\r\n",
  "Content-Length: 282\r\n",
  "Cache-Control: max-age=0\r\n",
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
  "Origin: https://auth-nac2.ntwk.dendai.ac.jp\r\n",
  "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36\r\n",
  "Content-Type: application/x-www-form-urlencoded\r\n",
  "Referer: httpss://auth-nac2.ntwk.dendai.ac.jp/\r\n",
  "Accept-Encoding: gzip,deflate,sdch\r\n",
  "Accept-Language: ja,en-US;q=0.8,en;q=0.6\r\n",
  "\r\n"
};

char *Message_Buttom[] = {
  "Host: auth-nac2.ntwk.dendai.ac.jp\r\n",
  "Connection: keep-alive\r\n",
  "Cache-Control: max-age=0\r\n",
  "Accept: image/webp,*/*;q=0.8\r\n",
  "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36\r\n",
  "Referer: https://auth-nac2.ntwk.dendai.ac.jp/auth/perfigo_cm_validate.jsp\r\n",
  "Accept-Encoding: gzip,deflate,sdch\r\n",
  "Accept-Language: ja,en-US;q=0.8,en;q=0.6\r\n",
  "\r\n",
  "GET /favicon.ico https/1.1\r\n",
  "Host: auth-nac2.ntwk.dendai.ac.jp\r\n",
  "Connection: keep-alive\r\n",
  "Accept: */*\r\n",
  "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36\r\n",
  "Accept-Encoding: gzip,deflate,sdch\r\n",
  "Accept-Language: ja,en-US;q=0.8,en;q=0.6\r\n",
  "\r\n",
};
