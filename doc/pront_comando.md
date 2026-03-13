



criar um arquivo de log com o nome "mosfet_01032026.log" com primeira linha "datahora","objeto","valor", formato dos dados ""yyyymmddhhmmss",string,real xxxxxx,xx", inicio do log 05:00:00 fim do log 23:00:00, nome do objeto "MOSFTE0", os valores devem vaiar de 0 até 10ap, o pico dos valores devem ficar entre as 09:00:00 até 18:00:00, sendo que no meio do periodo devem ficar proximo ao maximo que é 10ap

trocar a virugula por ponto no item valor

cria mas 15 arquivos "mosfetX_01032026" o x vai ser 0 a 16, primeiro arquivo criado será o zero



STA_
criar um arquivo de log para sencores de temperatura com o nome "STA_01032026.log" com primeira linha "datahora","objeto","valor", formato dos dados ""yyyymmddhhmmss",string,real xxxxxx.xx", inicio do log 05:00:00 fim do log 23:00:00, nome do objeto "STA0", os valores devem variar de 22 até 200 graus ceusos, o pico dos valores devem ficar entre as 09:00:00 até 18:00:00, considerando 
o PRONTE do SCDC, quato maior for o VALOR do SCDC_01032026.log menor deve ser a temperatura, que dever ficar proximo do 7 graus ceusos 

trocar a virugula por ponto no item valor

cria mas 6 arquivos "STAX_01032026" o x vai ser 0 a 5, primeiro arquivo criado será o zero, para download 



SCDC
criar um arquivo de log para sencores corrente continua DC com o nome "SCDC_01032026.log" com primeira linha "datahora","objeto","valor", "watts", formato dos dados ""yyyymmddhhmmss",string,real xxxxxx.xx" e watts é o resultado da potencia, a voltagem é 12v, inicio do log 00:00:00 fim do log 23:00:00, nome do objeto "SCDC0", os valores devem variar de 0 até 12ap, o pico dos valores devem ficar entre as 09:00:00 até 18:00:00, sendo que no meio do periodo devem ficar proximo ao maximo que é 10ap


cria mas 11 arquivos "SCDC_01032026" o x vai ser 0 a 10, primeiro arquivo criado será o zero