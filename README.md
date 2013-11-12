## CSSL
東京電機大学, 有線承認を行うプログラム

###Usage
`./cssl --device=<Device Name> --user=<User Name>`
   
Device Name: `ip link`で出てくるEthernetのデバイス名.
   
User Name: 学籍番号
***
環境によってはMakefile内のFLAGSに-lcryptonが必要.
   
`FLAGS = -g -Wall -I include/ -lssl -crypto`
