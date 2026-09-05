// Toda vida que ver esse #ifndef e basicamente a proteção contra inclusão dupla ao chamar essa classe
#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <iostream>
#include <string>
// A biblioteca abaixo e necessaria para o uso do int8_t
#include <cstdint>

class BigInt{

//Atributos de dados privados
private:
    bool neg;
    int nDig;
    int8_t* d;

//Metodos privados
private:
    // Construtor privado
    BigInt(bool IsNeg, int Size);

    // O mwtodo privado correct para corrigir um BigInt caso seja gerado um resultado intermediario que nao respeite a especificacao
    void correct();

    // O metodo privado increment para incrementar os dígitos de um BigInt
    void increment();

    // O metodo privado decrement para decrementar os dígitos de um BigInt
    void decrement();
public:
    // Construtor default
    BigInt();

    // Destrutor
    ~BigInt();

    //Construtor por Copia
    BigInt(const BigInt& B);

    //Construtor por Movimento
    BigInt(BigInt &&B) noexcept;

    //Operador de Atribuicao por copia
    BigInt& operator=(const BigInt &B);


    //Operador de Atribuicao por Movimento
    BigInt& operator=(BigInt &&B) noexcept;

    // Funções de impressao e recepçao
    friend std::ostream &operator<<(std::ostream &O, const BigInt &B);
    friend std::istream &operator>>(std::istream &I, BigInt &B);


    ///Operadores de incremento

    //Pre-fixados □a
    BigInt& operator++();
    BigInt& operator--();
    //Pos-fixados a□

    BigInt operator++(int);
    BigInt operator--(int);


    ///Construtores específicos
    BigInt(long long int N);
    explicit BigInt(const std::string &S);

    //metodo division
    void division(const BigInt &D, BigInt &Q, BigInt &R) const;

    //metodo de conversão
    long long int toInt() const;

    //Funções de consulta (implementadas inline)
    bool isNeg() const{return neg;}
    int size() const{return nDig;}
    bool isZero() const{return (nDig == 1 && d[0] == 0);}
    //Funções de consulta (implementado no .cpp)
    int operator[](int i) const;

//Modulo (abs)
friend BigInt abs(const BigInt &X);
//Operadores unarios
const BigInt& operator+() const {return *this;};
BigInt operator-() const;
BigInt operator!() const;
//Operadores matemáticos
friend BigInt operator+(const BigInt &A,const BigInt &B);
friend BigInt operator-(const BigInt &A,const BigInt &B);
friend BigInt operator*(const BigInt &A,const BigInt &B);

friend BigInt operator/(const BigInt &A,const BigInt &B);
friend BigInt operator%(const BigInt &A,const BigInt &B);

//Deslocamento a esquerda
friend BigInt operator<<(const BigInt &A,int N);
//Descolamento a direita
friend BigInt operator>>(const BigInt &A,int N);
};
///Declarações de Funções clássicas
//Operadores de comparação
bool operator==(const BigInt &A,const BigInt &B);
bool operator!=(const BigInt &A,const BigInt &B);
bool operator<(const BigInt &A,const BigInt &B);
bool operator>(const BigInt &A,const BigInt &B);
bool operator<=(const BigInt &A,const BigInt &B);
bool operator>=(const BigInt &A,const BigInt &B);

#endif
