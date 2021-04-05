# Nota importante

O código ta bugado (: por alguma razao a informação chega corrompida no server

```
# usando como input o comando "escrever" ou "ler"

"""
    [bigode@coala cd_INE5418_t1]$ ./server 

    Dispatcher conectado number: 3

    Aguardando comando do cliente...

    Command before reading from dispatcher: 

    Command AFTER reading from dispatcher: @H��

    Comando nao reconhecido, aguardando novo comando.

    Aguardando comando do cliente...
"""
```

# Instruções para Uso

1) Compile usando `make all`
2) Levante um dispatcher usando `./dispatcher`
3) Levante um servidor em outro terminal usando `./server`
4) Levante um (ou varios) cliente em outro terminal usando `./client`