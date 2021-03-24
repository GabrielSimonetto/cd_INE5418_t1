# Geral

A gente passou bastante tempo discutindo as diferenças de usar um dispachante que
    conversasse com todos os clientes e servidores e resolvesse tudo centralizadamente

Mas no fim parece que fazer um relacionamento MxN de todos os clientes contra todos os servidores
    parece fazer mais sentido do ponto de vista de performance de IO
    alem de que... deixa o projeto mais simples.

Tambem conversamos um pouco sobre as configurações como
    numero de servidores, memoria total, etc.
    Eh bom que seja facil de configurar tudo pra testes,
    e o config.h eh um primeiro passo nesse sentido
    mas se sobrar tempo no fim do projeto era legal
    colocar uma forma de inputar essas coisas via txt
    e/ou CLI...

    Aparentemente um codigo mais ou menos assim resolve no txt:

    -- no config.txt
    num_servers = 3

    -- na leitura de configuração que precisa acontecer tanto no cliente quanto nos servidores
    (fd eh o arquivo)
    fscanf(fd, "num_servers = %d",&num_servers);

Ah sim, e os loggers provavelmente vao ser simples o suficiente
    pq eles sao basicamente clientes especializados
    que vao ter alguma prioridade em cima de requisições comuns


# Duvidas

0) Eu tinha uma duvida se tava tudo bem fazer multiplos clientes com forks (processos)
R: Aparentemente nao ter o espaço global de memoria vai complicar varias coisas na hora de coordenar tretas
    compartilhar mutexes/semaforos, coisas assim,
    entao parece fazer sentido trocar pra threads.


1) Tudo bem ser um fork (processo) e nao uma thread
    o que faz o servidor ouvir mais de um cliente?
    funcionaria da mesma forma com threads?

    R: respondido acima, vamo faze thread

2) Separação de servidores pode ser arbitraria?
    ex: 4 maquinas, 256 bytes, só pq sim.

    R: respondido acima, só deixar configuravel

3) A comunicação do dispachante com os servidores tambem
    deve ser via sockets certo?
        ou só criar os objetos "servidor"
        e fazer requisições diretamente por uma função
        é o suficiente?

    R: precisa ser via sockets, mas nao ter dispachante tambem resolve.


-- A nossa intuição geral tava toda certa tambem.
Intuição geral: (verificar)

O Server global(Dispachante) recebe requisições dos clientes
O Server global vai oferecer aos clientes requisições nos limites globais de memória
    Ex: 1024 bytes
O cliente, escolherá um range qualquer do que ele quer ver
    ler, 233, 15
O Server global vai checar qual maquina local possui aquela informação, e repassar a requisição
    Ex: 4 maquinas, 256 bytes
        A)   0 -  255
        B) 256 -  511
        C) 512 -  767
        D) 768 - 1023

    nesse caso 233-248 fica na maquina B

    caso o cliente executar: ler 233 100
        teremos que lidar com uma requisição lendo dados
        que existem nas maquinas B e C.
        (se bem que isso pode ser um extra no trabalho)
        (da pra soh impedir, num primeiro momento)
        (a menos que justamente fazer esse tratamento va ser
            mais dificil do que soh dar a resposta completa de uma vez)


Extra: (se sobrar tempo)

4) os loggers só precisam registrar no intervalo de tempo definido certo?
    nao entre cada requisição de escrita.

    R: Sim, um logger mestre vai mandar todo mundo agir a cada X segundos

5) a imagem da a entender que o log eh um arquivo a parte (log.txt)
    mas se eu to interessado somente em guardar a memoria global,
    faz mais sentido eu só ter um txt global
    e ai os loggers meio que viram uma requisição habitual?
        (com algum mecanismo de prioridade em cima de outras requisições)

    R: Isso ta certo, a mensagem ta confuso
        nao precisa criar .txt pra loggers parciais (os não-mestre)

6) onde escrever essa informação?

    R: Nao precisa se preocupar com isso,
        haveria uma preocupação com espaço de armazenamento num caso real
        nesse caso só defenda onde vc colocou e dale.
