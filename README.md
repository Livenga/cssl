## CSSL
東京電機大学, 有線承認を行うプログラム
   
ちなみにUser-Agentは,
  `User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36`
###Usage
`./cssl --device=<Device Name> --user=<User Name>`
   
Device Name: `ip link`で出てくるEthernetのデバイス名.
   
User Name: 学籍番号
***
環境によってはMakefile内のFLAGSに-lcryptonが必要.
   
`FLAGS = -g -Wall -I include/ -lssl -crypto`
