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

