# Relatorio Parcial -- Deposito de ideias

( Se a gente lembrar de ir preenchendo isso conforme pensa nas coisas a vida vai ser mais facil depois )

## Defender o TCP

< TODO >

## Dispachante ou Relacionamento MxN entre clientes e servidores?

Podemos ter um Dispachante que ouve todos os clientes e se conecta com todos os servidores, manejando as requisições e as repassando para os servidores corretos, observado o espaço de memória requerido.

Tambem podemos fazer com que cada cliente conheça cada um dos servidores e que o proprio cliente descubra qual é o servidor correto para receber sua requisição

A ideia do dispachante é boa pois possibilita o controle de atualizações futuros. 
Entretanto, é dificil ignorar o aumento de uma etapa de IO, além da introdução de um ponto unico de falha no sistema.

Ignorar o dispachante faz com que tenhamos que reproduzir algumas configurações tanto no cliente quanto no servidor,
e garantir que estas estejam sempre atualizadas, entretanto o ganho de performance e facilidade de implementação parece valer a pena.


## Qual é o processo logger mestre que pede aos outros loggers que mandem as suas parcelas?

Pode ser sempre o logger relacionado ao server 0, nao vejo porque alg mais complicado.

