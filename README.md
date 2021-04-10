> Trabalho de Compiladores I.
> Grupo: 
> Alexandre Saura
> Fábio Thomaz
> Ricardo Oliveira
> Rodrigo Suarez
> 
> Prof.: José Fontebasso Neto.
> UNISANTOS - março-abril de 2021.

# O Projeto   
Analisador léxico de uma calculadora com notação pós-fixada. A aplicação deverá analisar uma sequência de caractéres vindas do teclado e fazer sua análise léxica.  

## Execução no Code::blocks  
Para que a aplicação possa ser executada no *Code::blocks*, é necessário criar um projeto na IDE, onde todos os arquivos de código-fonte(.c) e os de header(.h) deverão ser incluídos no mesmo. Para que isso possa ser feito, deve-se ir, após a criação do projeto, em **Project** -> **Add files** e selecionar todos os arquivos citados acima.  
Após feito isso, basta compilar a aplicação e executar.

## Execução em um terminal Linux  
Navegue até o diretório do projeto e execute os comandos:  
```sh
make  
make run  
```

Após o término, para remover os arquivos-objeto(.o), execute o comando:  
```sh
make clean  
```

Para remover o arquivo executável da aplicação, execute:  
```sh
make mrproper  
```
