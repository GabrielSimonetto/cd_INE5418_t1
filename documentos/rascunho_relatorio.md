# Relatorio Parcial -- Deposito de ideias

( Se a gente lembrar de ir preenchendo isso conforme pensa nas coisas a vida vai ser mais facil depois )


## Defender o TCP

< TODO >

# Numero de clientes possiveis

Ao criar as threads que servirão os diferentes clientes que se conectem, precisamos de uma lista para armazenar os endereços de todas as threads em atividade. Nesse ponto, podemos tomar a decisão de criar uma lista que cresce de forma dinâmica ou de uma lista de tamanho fixo. Num cenário real, existe a possibilidade de a decisão correta ser usar uma lista que cresce dinamicamente ao mesmo tempo que se monitora a rede para identificar se os recursos estao se tornando escassos para possivelmente reduzir a quantidade de clientes e começar a negar acesso a novos clientes. Um cenário que vem em mente, é se por exemplo cada cliente unico ocupa pouca memoria, dessa forma, não sendo um problema, ao mesmo tempo que cada cliente unico faz poucas requisições por vez.

É claro que mesmo nesse cenário existem muitas variáveis envolvidas, e provavelmente voce consegue prever quanto poder processamento um numero X de usuários irá exigir. Bom, feitas essas considerações, como nós não vamos implementar nenhum meio complexo de tratamento dinamico, vamos optar pela simplicidade de um numero definido de clientes possiveis por vez.

    // TODO: ta, mas ai eu teno que checar quais espaços dessa lista estao ocupados e quais nao?
                EHRMAGHERDS


## Dispachante ou Relacionamento MxN entre clientes e servidores?

Podemos ter um Dispachante que ouve todos os clientes e se conecta com todos os servidores, manejando as requisições e as repassando para os servidores corretos, observado o espaço de memória requerido.

Tambem podemos fazer com que cada cliente conheça cada um dos servidores e que o proprio cliente descubra qual é o servidor correto para receber sua requisição

A ideia do dispachante é boa pois possibilita o controle de atualizações futuros. 
Entretanto, é dificil ignorar o aumento de uma etapa de IO, além da introdução de um ponto unico de falha no sistema.

Ignorar o dispachante faz com que tenhamos que reproduzir algumas configurações tanto no cliente quanto no servidor,
e garantir que estas estejam sempre atualizadas, entretanto o ganho de performance e facilidade de implementação parece valer a pena.


## Qual é o processo logger mestre que pede aos outros loggers que mandem as suas parcelas?

Pode ser sempre o logger relacionado ao server 0, nao vejo porque alg mais complicado.

## Threads:

Um tempo absurdo foi perdido tentando fazer threads funcionarem, muitos loops infinitos foram enfrentados, em especifico, read e write nao esperavam por dados e simplesmente ficavam rodando, e portanto eu agora estou trocando pra recv e... pro outro la, vou catar agora, e, com alguma sorte, isso vai funcionar (>:

recv é analogo ao read, send é analogo ao write
pronto, eh o send entao.

Posteriormente pensamos que seria uma boa ter usado as threads em modo detached, ja que caso o main fosse interrompido novas conexões nao poderiam mais ser criadas, mas as que ja existissem poderiam rodar até o final.

## Interrompendo o server:

Nao foi encontrada uma forma elegante de interromper o serviço no server, e finalizar o serviço nos clientes.

## Arquitetura multi-server:

Diversas arquiteturas podem ser consideradas para gerenciar a gestão dos servers distribuidos,
1- conectar todos os clientes com todos os servidores, fazer a decisão de para onde a requisição vai no cliente
2- criar um dispachante que envia requisições dos clientes para os servidores, mas, fazer servidores enviarem diretamente a seus clientes
3- encadear os servidores numa fila, repassar a requisição por todos e aqueles que tiverem informação pertinente concatenarem na resposta final,
    o ultimo servidor ficando responsavel por devolver a resposta pro cliente
4- similar ao sistema dispachante, centralizar um servidor como o responsavel por receber requisições e pedir partes faltantes. (primary-secondary)

Cada uma dessas soluções vai ter diversas vantagens e desvantagens, num sistema de mundo real provavelmente o melhor sistema eh o 2,
sendo que isso facilita refatorações da configuração do sistema como por exemplo adicionar servers, alem de simplificar a requisição do lado do cliente
e a ligação ser diretamente do servidor com o cliente reduz um processo de IO caso passasse pelo dispachante, como é no primary-secondary.
Entretanto parece um pouco complicado fazer com que todos os servidores possuam uma conexão com o cliente para enviar dados diretamente,
e alem disso, o unico jeito de resolver uma requisição envolvendo multiplos servers seria enviar identificadores da ordem dos pacotes para que os proprios
clientes concatenassem a resposta final.

A solução adotada no trabalho será a arquitetura primary-secondary devido a facilidade de sua implementação, teremos 2 ciclos de IO, mas isso nos permite ter
somente uma conexao com o cliente e com o servidor, o que facilita a requisição, alem de que o servidor primário consegue gerenciar os multiplos pacotes
caso necessario (ainda pensando se vou fazer requisição serial concatenando ou distribuida e com identificadores.)

(Usar esse parágrafo se formos usar só uma porta) Estamos usando somente uma porta tambem, o que poderia vir a ser uma decisão
ruim para performance, isso poderia ser refatorado caso necessário.