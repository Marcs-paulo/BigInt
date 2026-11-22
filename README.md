# BigInt — inteiros sem limite de dígitos

Projeto da disciplina **Programação Avançada** do Departamento de Engenharia de Computação e Automação (DCA) da **UFRN**, com o professor Adelardo Adelino Dantas de Medeiros.

A disciplina teve três projetos práticos, um por unidade:

| Unidade | Projeto | Assunto principal |
| :-- | :-- | :-- |
| 1 | **BigInt** (este) | classes, ponteiros e memória dinâmica |
| 2 | [Planejador](https://github.com/Marcs-paulo/Planejador) | exceções, arquivos e STL (algoritmo A*) |
| 3 | [Plotador](https://github.com/Marcs-paulo/Plotador) | interface gráfica com Qt |

## O que é

Uma classe C++, `BigInt`, que representa inteiros positivos, nulos ou negativos com **qualquer quantidade de dígitos**. Ela guarda os dígitos num array alocado com `new`, faz a entrada e saída pelo console e implementa as operações: soma, subtração, multiplicação, divisão, resto, fatorial, deslocamento de dígitos, incremento, decremento, troca de sinal e módulo.

O programa de exemplo (`bigint-main.cpp`, fornecido pelo professor) é uma calculadora de terminal que usa a classe:

```
Result: 25 (2 digitos)
Digite sua opcao [h para ajuda]: !
Result: 15511210043330985984000000 (26 digitos)
```

## Foco do projeto e o que buscamos aprender

- **Gerenciar memória dinâmica na mão**: alocar e liberar o array de dígitos com `new`/`delete`, mantendo-o sempre com o tamanho exato do número (sem zeros à esquerda).
- **Escrever uma classe completa**: construtores (default, específicos, por cópia e por movimento), destrutor e operadores de atribuição por cópia e por movimento.
- **Sobrecarregar operadores** para a classe se comportar como um tipo numérico: `+ - * / %`, comparações, `<< >>`, `++ --`, `!` (fatorial) e os operadores de stream.
- **Dividir o projeto em arquivos** (`.h` para a declaração, `.cpp` para a implementação).
- **Fazer os algoritmos da escola com dígitos**: o array guarda os dígitos ao contrário (`d[0]` é a unidade) justamente para facilitar soma e multiplicação.

A especificação proibia de propósito recursos que vieram depois na disciplina (exceções, contêineres e algoritmos da STL, herança e métodos virtuais), para o foco ficar em ponteiros e classes.

## O que foi pedido

O enunciado completo está em [`docs/BigInt-especificacao.pdf`](docs/BigInt-especificacao.pdf). Em resumo:

- A classe guarda três dados: `bool neg` (sinal), `int nDig` (quantidade de dígitos, sempre ≥ 1) e `int8_t* d` (os dígitos, de 0 a 9).
- O construtor default, o construtor por cópia e o `main` vieram prontos e não podiam ser alterados.
- Depois dos construtores e atribuições, nenhuma outra função podia usar `new` ou `delete` diretamente.
- Todas as funções descritas tinham de ser implementadas, mesmo as que o `main` não usa.

## Como compilar e rodar

Precisa só de um compilador C++17 (g++ ou clang).

```bash
g++ -std=c++17 -Wall -o bigint bigint-main.cpp bigint.cpp
./bigint
```

Também dá para abrir `BigInt.cbp` no **Code::Blocks**, que foi a IDE usada na disciplina.

## Arquivos

| Arquivo | Origem |
| :-- | :-- |
| `bigint.h`, `bigint.cpp` | classe `BigInt`, implementada por mim a partir da base do professor |
| `bigint-main.cpp` | programa de teste fornecido pelo professor (sem alterações) |
| `BigInt.cbp` | projeto do Code::Blocks |
| `docs/BigInt-especificacao.pdf` | enunciado do projeto |
