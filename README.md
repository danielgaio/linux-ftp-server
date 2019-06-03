linux ftp server

Software produced by:
- Daniel E. Gaio
- Matheus H. Koch

Do uso do servidor
Compilação:  <br>
gcc main.c header.h server.c -o server <br>
Execução:
./server (ou: sudo ./server)

Do uso de cliente
No terminal digite (com o servidor já em execução):
ftp localhost

“Este projeto foi desenvolvido integralmente pela equipe, sem ajuda não autorizada de alunos não membros do projeto no processo de codificação”

Outros projetos e materiais utilizados:
  - //https://github.com/Siim/ftp
  - Port e pasv: https://slacksite.com/other/ftp.html
  - Apanhado geral do protocolo e alguns exemplos de sessões de transferencia de arquivos: http://danzig.jct.ac.il/tcp-ip-lab/ibm-tutorial/3376c44.html
  - The SYST, STAT, HELP, and NOOP verbs: https://cr.yp.to/ftp/syst.html
  - Como usar cliente ftp do terminal: https://www.howtoforge.com/tutorial/how-to-use-ftp-on-the-linux-shell/
  - FTP: a deeper look at a 'Passive' file transfer: https://www.ibm.com/developerworks/community/blogs/SterlingB2B/entry/FTP_a_deeper_look_at_PASSIVE_FTP?lang=en
  - Sobre o comandos PASV, PORT, RETR e REST: https://cr.yp.to/ftp/retr.html
