Parte 1:
[ ] - Criar UM espaço de memória para UM cliente
    [ ] - Escrever
        [ ] - barras de espaço sao polemicas, resolver?
                (usar aspas?)
                (concordar em jamais usar barras de espaço?)

    [x] - Ler
        [ ] - Refatorar pra nao ser terrivel
                adicionar novas funcionalidades
                e etc.

[x] - Multiplos clientes
    (fork ok? ou thread?)
    R: Threads vao facilitar a vida pra usar o mesmo ambiente de memoria
        para compartilhar mutexes/semaforos vai ser mais simples.

[ ] - Separar em 4 servidores

[ ] - Evitar data races
    (melhor fazer isso antes ou depois de rachar os servidores?)
    (evitar data race escrevendo em mais de um server vai ser estranho)

        (extra)
[ ] - mudar o config.h pra nao ter que recompilar coisas
        -- colocar por CLI ou por .txt
        fscanf , fprintf

        n = 3
        (fd eh o arquivo)
        fscanf(fd, "n = %d",&num_servers);


Parte 2, logger:
[ ] - nem ideia como fazer isso aos poucos


Parte 3, relatorio:
[ ] - Reler a definição do trabalho
[ ] - Procurar pontos de decisão
