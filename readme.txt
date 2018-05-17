Projeto que visa implementar um servidor FTP, baseado na RFC 959. Para esta implementação, foram definidas uma lista de comandos básicos visando o cliente alvo do servidor (cliente padrão dos sistemas Linux).
Lista de comandos implementados:
Exemplo:
- <comando_FTP_passado_ao_servidor> (<comando_utilizado_no_cliente_FTP): <explicação>

-------
- USER (enviado automaticamente quando a conexão se inicia): que serve para o usuário fazer login no sistema e ser direcionado para a sua pasta específica, onde poderá manipulá-la como quiser.
- PASS: que serve para o usuário autenticar sua senha e, assim, completar o Login e ser direcionado para sua pasta raiz.
- PWD (pwd): mostra ao usuário o caminho dentro do sistema onde o servidor está rodando que leva para sua pasta onde ele atualmente está.
- LIST (ls): permite que o usuário veja quais arquivos e pastas estão presentes na pasta remota onde ele está.
- CWD (cd): faz com que o usuário acesse uma subpasta dentro da sua pasta raiz. Ele pode usar indefinidamente o comando CWD, porém precisa sempre ter uma pasta para acessar.
- CDUP (cdup): faz com que o usuário se mova para cima na sua árvore de diretórios. Ele não pode usar indefinidamente: o usuário jamais conseguirá sair da sua pasta raiz e acessar os arquivos básicos do sistema onde está rodando o servidor.
- MKD (mkd): permite ao usuário criar uma subpasta dentro da pasta onde ele atualmente está.
- RMD (rmd): permite ao usuário remover uma subpasta dentro da pasta onde ele atualmente está. A pasta removida precisa estar vazia, caso contrário não será removida.
- DELE (delete): permite que o usuário delete um arquivo qualquer dentro da pasta remota onde está. Contudo, ele não pode excluir uma pasta utilizando este comando, para isso utiliza-se o comando RMD.
- SYST: utilizada, principalmente, logo após a autenticação de Usuário e Senha, serve para informar ao cliente qual sistema está rodando por baixo do servidor. Então, quando passada ao servidor, o servidor responderá com um código e o nome do sistema.
- NOOP: comando que faz o servidor retornar um código juntamente com um "Okay" ao cliente. É usada quando, após muito tempo de silêncio de ambas as partes, o cliente precisa de uma confirmação se a conexão ainda está ativa.
- STOR (put): permite que o cliente envie um arquivo para o servidor. O arquivo é salvo exatamente na pasta que o cliente está acessando remotamente e com o nome passado pelo cliente como parâmetro do comando.
- RETR (get): permite que o cliente receba um arquivo existente na pasta remota que está acessando. O nome do arquivo é passado como parâmetro e, como segundo parâmetro, o nome que será salvo quando chegar ao cliente.
- PORT (enviado junto com os comandos que abrem conexão de dados [get, put, ls]): permite que o cliente envie a porta que estará aguardando a conexão de dados ao servidor. Com esta porta, o servidor será capaz de iniciar uma conexão de dados no IP do cliente e na porta passada.
- PASV (passive - comando executado, porém não manda nenhum comando ao servidor): assim como o PORT, o PASV faz com que o cliente espere em uma porta específica por uma conexão. Contudo, quem define a porta que o cliente esperará é o servidor. O servidor responde o comando PASV com dois valores de portas, que, posteriormente, serão convertidos em hexadecimal e concatenados, para gerar, então, um valor decimal correspondente à uma porta.
- QUIT (bye ou quit): faz com que o servidor defina a saída do cliente e retorne seu status para 'aguardando conexão'. Com o QUIT, o cliente, que estava logado, retorna para 'status = 0' e o sistema volta para a pasta raiz do servidor (sai da pasta do cliente). Quando receber uma nova conexão, o servidor entrará direto na pasta do novo cliente conectado.

*** Caso o cliente não possua cadastro ainda: quando tentar login, o servidor criará um cadastro para o cliente (caso não reconheça o username) e adicionará à lista de clientes, juntamente com a sua senha. Sempre que isso acontece, invés de "Logado com sucesso." aparece "Cadastrado com sucesso.". 

*** Caso o cliente não possua uma pasta: é esperado que todos os clientes já cadastrados possuam uma pasta. Contudo, caso isso não aconteça, servidor cria uma pasta, na raiz de seus sistema, de nome "pasta_<username_cliente>". A pasta inicialmente estará vazia e poderá ser manipulada como o cliente bem entender, baseado nos comandos à disposição. 


-----------------------------------------------------------------------------------------------

O protocolo FTP possui muitos outros comandos que não foram implementados nessa versão. Já na RFC 959, existem inúmeros comandos, contudo, foram implementados apenas os comandos que estão listados na seção 5.1 do documento. Estes são os comandos mínimos necessários para uma utilização plena de um sistema FTP.
Porém, três comandos foram removidos da lista de comandos mínimos, já que o cliente alvo da implementação (o cliente padrão dos sistemas Linux) evita que se faça uma modificação com esses comandos.
Os três comandos removidos, que serão citados à seguir, são comandos de configuração entre servidor e cliente. Contudo, o cliente padrão do Linux tem como padrão que as configurações sejam específicamente as que ele prevê.

- TYPE: comando que altera o tipo de arquivo que será recebido pelo cliente. O cliente prevê que sejam apenas arquivos ASCII Non-print. Não é possível alterar essa configuração pelo cliente FTP.
- MODE: serve para alterar o modo de transmissão dos arquivos entre o servidor e o cliente. O cliente padrão prevê que o modo de transmissão seja apenas o Stream Mode, sendo, então, impossível modificar essa configuração.
- STRU: define a estrutura de arquivos do servidor. 

-----------------------------------------------------------------------------------------------
MODO DE COMPILAÇÃO e EXECUÇÃO:

* Requisitos do sistema: 
    - Estar rodando um sistema Linux;
    - Possuir o compilador GCC para arquivos .c.

* Sistema utilizado para o passo a passo: Linux Ubuntu v18.02 LTS

* Passo a passo de como compilar:
    1. Abra o terminal (CTRL + ALT + T [padrão]);
    2. Mova-se, utilizando o comando 'cd', até a pasta onde estão localizados os arquivos .c que compõem o servidor;
    3. Estando na pasta, digite no terminal do Linux o seguinte comando:
        "gcc *.c header.h -o Server" - sem as aspas.
    4. Após executar a compilação, será criado um arquivo executável chamado "Server" dentro da pasta onde está localizado;
    5. Pronto, o servidor já está compilado.

* Executar o servidor:
    1. Utilize, na pasta onde está o executável "Server", o comando:
        "./Server"
    2. O servidor será, então, iniciado e estará pronto para receber conexões.

* Passo a passo de como conectar:
    1. Abra um novo terminal (pode ser em outro computador que esteja ligado na mesma rede do servidor);
    2,1. Caso seja o mesmo computador:
        ~ digite no terminal o comando
            "ftp localhost 8080"
    2,2. Caso seja em outro computador:
        ~ ao ser iniciado, o servidor apresentou um endereço IP de sua rede local;
        ~ Copie este endereço IP no comando à seguir:
            "ftp <endereco_ip> 8080
    3. Ao conectar-se ao servidor, será solicitado um nome de usuário;
    4. Após, será solicitada a senha para o usuário;
    5. Caso o usuário ainda não esteja cadastrado, o servidor criará um cadastro e pasta para o usuário na pasta raiz do servidor;
    6. Está pronto para ser utilizado, conforme os comandos implementados no servidor. 
    
-----------------------------------------------------------------------------------------------
*** OBSERVAÇÕES:
    - O servidor está, atualmente, configurado para receber a conexão de controle na porta 8080. Para alterar essa configuração, basta redefinir a definição dentro do arquivo "server.c", linha 14. O valor da porta deve ser utilizado para a conexão com o comando "ftp <endereco_ip> <porta>".
    
    - Os arquivos que compõem o servidor, atualmente, na versão 6.34, são:
        ~ server.c   // arquivo base, recebe as conexões e faz o login do usuário, é onde recebe os comandos enviados
        ~ func.c     // contém as funções de todos os comandos implementados (exceto USER)
        ~ login.c    // usado para o gerenciamento de login e dos usuários
        ~ porta.c     // usado para as operações específicas dos comandos PORT e PASV
        ~ connection.c   // usado para fazer a conexão de dados
        ~ apoio.c      // possui algumas funções de apoio utilizadas por diferentes funções dentro do servidor
        ~ getMyIp.c    // possui a função que identifica o Ip de rede para facilitar na conexão externa
        ~ header.h     // apresenta a lista de funções globais do sistema (funções de um arquivo que são chamadas por funções de outros)
        ~ pastasClientes (pasta)  // pasta base, impossível rodar o servidor sem ela, possui todas as pastas dos clientes cadastrados e o arquivo de gerenciamento de cadastros
            ~ clientesCadastrados.txt    // arquivo de gerenciamento de cadastros, contém todos os usuários cadastrados e suas senhas.
            
    - Atualmente, por um bug na senha, o servidor pede para o cliente a senha, porém não executa a verificação, liberando o login mesmo que a senha esteja inválida. (v6.34)
    
    - O servidor só estará recebendo conexões se aparecer a mensagem "SERVIDOR ONLINE", caso contrário, ocorreu algum erro e ele estará fechado para conexões.
    
    - Alguns erros internos do servidor irão parar em uma função chamada "loopErro" que fará com que o servidor continue recebendo conexões e respondendo que está impossibilitado para receber clientes. É extremamente raro parar nessa função, já que ela só é chamada por um valor interno do servidor que foi atribuído indevidamente (até hoje, fora quando for forçado a ir para ela, nenhuma vez o servidor ficou dessa forma). Porém esse mecanismo existe e mantém o servidor online, mesmo com um erro gravíssimo, só que impossibilitado de fazer qualquer coisa.
    
-----------------------------------------------------------------------------------------------
REFERẼNCIAS:
    Algumas funções presentes no servidor tiveram suas lógicas copiadas exatamente da internet, porém adaptadas para funcionarem junto com as outras. Todas as funções que se encaixam nisso possuem, ao lado de suas declarações, um comentário dizendo que foi copiada da internet e que o link de referência está no arquivo "Links". Além do arquivo "Links", os links podem ser encontrados abaixo:
    
    * Converter um valor decimal em uma string Hexadecimal ('convertToHex' no arquivo 'porta.c'): https://stackoverflow.com/questions/16928517/converting-a-decimal-to-a-hexadecimal-numbers
    
    * Ler um arquivo em um vetor de bytes ('readFileBytes' no arquivo 'apoio.c'): http://www.codecodex.com/wiki/Read_a_file_into_a_byte_array
    
    * Get my Ip (arquivo 'getMyIp.c'): https://www.binarytides.com/get-local-ip-c-linux/
    
    Nos links acima, estão também especificados o arquivo onde se encontra a função e o nome exato da função implementada.


    
    
        
    
