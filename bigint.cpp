#include "bigint.h"
#include <iostream>
/* ACRESCENTADO */
#include <cmath> // Biblioteca para usar o fabs e o log10
#include <cstdlib>    // abs
#include <utility>    // Para usar o move
#include <algorithm>  // Para usar o max
#include <cctype> //Ao analisar o codigo encontrei no que já estava implementado as função isdigit() ao pesquisar sobre ela descobri que usa essa lib
using namespace std;

/// Construtor default.
/// Inicializa com um inteiro de 1 digito, valor 0 (zero).
/// NAO PODE SER MODIFICADO
BigInt::BigInt()
  : neg(false)
  , nDig(1)
  , d(new int8_t[1]{0})
{}

/// Destrutor
/* ACRESCENTADO */

BigInt::~BigInt()
{
    delete[] d;
}


/// Construtor especifico PRIVADO que recebe o sinal e a quantidade de digitos
/* ACRESCENTADO */
BigInt::BigInt(bool IsNeg, int Size)
    : neg(IsNeg)
    // Criando teste contra erros USB (Basicamente o ternario serve para impedir que o tamanho do array seja nulo ou menor que isso)
    , nDig(Size < 1 ? 1 : Size)
    , d(new int8_t[nDig]{0})
{}


/// Construtor por copia.
/// Delega ao construtor especifico privado.
/// NAO PODE SER MODIFICADO.
BigInt::BigInt(const BigInt& B)
  : BigInt(B.isNeg(), B.size())
{
  // Copia os digitos
  for (int i=0; i<size(); ++i) d[i] = B.d[i];
}

/// Atribuicao por copia
/* ACRESCENTADO */
BigInt& BigInt::operator=(const BigInt &B) {
    if (this == &B) return *this;

    delete[] d;

    neg = B.neg;
    nDig = B.nDig;

    if(nDig>0){
        d = new int8_t[nDig];
        for(int i=0; i<nDig; ++i){
            d[i] = B.d[i];
        }
    }else{
        d= nullptr;
    }
    return *this;
}


/// Construtor por movimento
/* ACRESCENTADO */
BigInt::BigInt(BigInt &&B) noexcept
    : neg(B.neg)
    , nDig(B.nDig)
    , d(B.d)
{
    B.neg = false;
    B.nDig = 0;
    B.d = nullptr;
}
/// Atribuicao por movimento
/* ACRESCENTADO */
BigInt& BigInt::operator=(BigInt &&B) noexcept{
    if (this == &B) return *this;

    delete[] d;
    neg = B.neg;
    nDig = B.nDig;
    d = B.d;

    B.neg = false;
    B.nDig = 0;
    B.d = nullptr;

    return *this;
}

/// Construtor especifico a partir de inteiro longo.
/// Tambem conversor de long long int para BigInt.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(long long int N)
  /* ACRESCENTADO */
  :BigInt(N<0, (N ==0) ? 1 : 1+int(log10(fabs(N))))
{
  // Calcula os digitos, usando divisao inteira por 10
    for (int i=0; i<size(); ++i)
    {
        d[i] = abs(N%10); // Modulo do resto da divisao
        N /= 10;          // Divisao inteira
    }
}

/// Conversor de BigInt para long long int
/* ACRESCENTADO */
long long int BigInt::toInt() const{
    // inicia o valor de teste como 0
    long long int val = 0;

    for(int i=size()-1; i >=0; --i){
        val = 10*val+d[i];
        if(val<0){
            cerr << "Erro o valor do BigInt ultrapassa o maximo do long long int"<< endl;
            return 0;
        }
    }
    if(isNeg()){
        return -val;
    }

    return val;
}

//A maior parte das funções de consultas foram implementadas no.h por serem inline
// Aqui na .cpp ficou somenta a sobrecarga do operator []
int BigInt::operator[](int i) const{
    if(i<0 || i>= nDig){
        return 0;
    }
    /*
    Adicionei o "static_cast<int>()" apos ver la em baixo na documentacao do BigInt que os digitos do tipo int8_t, são equivalentes a
    um char e por indicação do propio documento ele pede para adicionar isso no operator[]
    */
    return static_cast<int>(d[i]);
}


/// ******************
/// * FIM DA PARTE 1 *
/// ******************

/// Funcao privada que corrige o numero, caso haja inconsistencias
/* ACRESCENTADO */

 void BigInt::correct(){
 // Elimina 0’s mais significativos
    int newSize = size();
    while(newSize>1 && d[newSize-1] ==0){
        newSize = newSize-1;
    }
    if(newSize !=size()){
        // criei essa nova variavel BigInt temporario para armazenar o valor corrigido, mantendo o sinal e alocando somente a memoria que ele precisa
        BigInt temp(neg, newSize);
        for(int i=0; i<newSize; ++i){
            temp.d[i] = d[i];
        }
        // transformo a variavel original na nova corrigida (isso chama meu operador de atribuicao por copia definido la atras)
        *this = move(temp);
    }
    // para impedir zero negativo
    if(isZero()){
        neg = false;
    }
 }


/// Construtor especifico a partir de string.
/// Nao eh conversor de string para BigInt.
/// Delega ao construtor default.
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS, APENAS
/// NAS PARTES INDICADAS POR /* ACRESCENTAR */
BigInt::BigInt(const string& S)
  : BigInt() // Valor inicial zero
{
  // Se string vazia, emite erro e permanece com valor inicial zero
  if (S.empty())
  {
    cerr << "empty string cannot create a BigInt\n";
    return;
  }

  // Posicao onde comecam os digitos, inicialmente zero
  size_t ini=0;
  // Leva em conta o sinal
  bool IsNeg = false;

  if (S[0]=='+' || S[0]=='-')
  {
    // Se nao tem nenhum digito alem do sinal, emite erro e permanece com valor inicial
    if (S.size()==1)
    {
      cerr << "sign-only string cannot create a BigInt\n";
      return;
    }
    IsNeg = (S[0]=='-');
    ++ini;
  }

  // Faz ter sinal (IsNeg) e numero de digitos (tamanho da string - ini) corretos
  /* ACRESCENTADO */
  // Aqui basicamente aproveitei o construtor já definido usamos ele para criar o BigInt com tamanho exato e o sinal correto
    BigInt temp(IsNeg, S.size()-ini);
    *this = move(temp);

  // Calculo dos digitos do BigInt
  for (int i=0; i<size(); ++i)
  {
    const char& c = S[S.size()-1-i]; // Referencia (apelido) para facilitar a notacao
    if (!isdigit(c))
    {
      *this = BigInt(); // = 0
      cerr << "string with invalid character cannot create a BigInt\n";
      return;
    }
    d[i] = static_cast<int8_t>(c-'0');
  }
  // Corrige eventuais numeros fora da especificacao
  correct();
}

/// ******************
/// * FIM DA PARTE 2 *
/// ******************

/// Insercao (impressao)
/* ACRESCENTADO */
ostream &operator<<(ostream &O, const BigInt &B){
    /*
    O sinal ‘-‘, se o número for negativo;
    */
    if(B.isNeg()){
        O << "-";
    }
    /*
    Os digitos, de d[size()-1] a d[0], desde que estejam entre 0 e 9; caso contrario, o
    que nunca deveria acontecer, deve ser impresso ‘#’ em vez do digito.
    */
    for(int i = B.size()-1; i>=0; --i){
        if(B.d[i]>= 0 && B.d[i]<= 9){
            O << B[i];
        }else{
            O << '#';
        }
    }
    return O;
}
/// Extracao (digitacao).
/// NAO PODE SER MODIFICADO NAS PARTES JAH IMPLEMENTADAS.
/// PODE (E PRECISA) RECEBER ACRESCIMOS NAS PARTES INDICADAS POR /* ACRESCENTAR */
std::istream& operator>>(istream& I, BigInt& B)
{
  // Valor inicial zero
  B = BigInt(); // = 0

  // Testa a stream de entrada e descarta eventuais separadores iniciais.
  // Em caso de erro, encerra a digitacao.
  istream::sentry s(I);
  if (!s) return I;

  // Inspeciona o primeiro caractere que serah lido
  char c = I.peek();

  // Testa se o primeiro caractere eh um sinal.
  // Se for, consome (elimina do buffer), processa e inspeciona o proximo caractere.
  if (c=='+' || c=='-')
  {
    // Consome da stream
    I.get(c);  // Valor de "c" permanece o mesmo...
    // Atribui o sinal
    B.neg = (c=='-');
    // Obtem o proximo caractere
    c = I.peek();
  }

  // Numero de digitos que foram digitados
  int numDigitos = 0;

  // Testa se eh um caractere valido: digitos 0 a 9
  while (isdigit(c))
  {
    // Consome da stream
    I.get(c);  // Valor de "c" permanece o mesmo...
    ++numDigitos;

    if (numDigitos>1)
    {
      // Faz o BigInt manter o sinal e passar a ter size()+1 digitos,
      // avancando todos para uma posicao mais significativa aa frente.
      /* ACRESCENTADO */
        BigInt temp(B.isNeg(), B.size()+1);
        for(int i=0; i< B.size(); ++i){
            temp.d[i+1] = B.d[i];
        }
        B = move(temp);
    }

    // Acrescenta o novo digito como sendo o primeiro (o menos significativo)
    B.d[0] = static_cast<int8_t>(c-'0');

    // Inspeciona o proximo caractere que vai ser lido
    c = I.peek();
  }

  // Assinala erro na stream se nenhum digito foi lido
  if (numDigitos==0) I.setstate(ios::failbit);

  // Corrige eventuais erros na digitacao
  B.correct();

  // Encerra a digitacao
  return I;
}

/// ******************
/// * FIM DA PARTE 3 *
/// ******************

/// Teste de igualdade
/* ACRESCENTADO */
//igualdade
bool operator==(const BigInt &A,const BigInt &B){
    if(A.isNeg() != B.isNeg()) return false;
    if(A.size() != B.size()) return false;

    for(int i=0; i<A.size(); ++i){
        if(A[i] != B[i]) return false;
    }
    return true;
}
//desigualdade
bool operator!=(const BigInt &A,const BigInt &B){
    return !(A == B);
}


/// Menor que
/* ACRESCENTADO*/
bool operator<(const BigInt &A,const BigInt &B){
    if(A.isNeg() != B.isNeg()) return A.isNeg();

    if(A.size() != B.size()){
        if(!A.isNeg()){
            return A.size() < B.size();
        }
        return B.size() < A.size();
    }
    for(int i = A.size()-1; i >= 0;--i){
        if(A[i] != B[i]){
            if(!A.isNeg()){
                return A[i] < B[i];
            }
            return B[i] < A[i];
        }
    }
    return false;
}


/// ******************
/// * FIM DA PARTE 4 *
/// ******************

/// Funcao privada que incrementa os digitos (o modulo) do numero
/* ACRESCENTADO */
void BigInt::increment(){
    int k =0;
    while(k<size()){
        if(d[k] <9){
            d[k] = d[k]+1;
            return;
        }
        d[k] = 0;
        ++k;
    }
    // Aumenta nº de dígitos. Exemplo:
    // Antes do método: [9 9 9 9]
    // Agora: [0 0 0 0]
    // Deve ficar: [0 0 0 0 1]

    // Usei o construtor para criar uma variavel temporaria com um espaço a mais de tamanho
    BigInt temp(isNeg(),size()+1);
    // O novo digito mais signignificativo vai receber 1
    temp.d[temp.size()-1] =1;
    //trannsformo essa variavel temporaria na minha principal usando meu operator =
    *this = move(temp);
}

/// Funcao privada que decrementa os digitos (o modulo) do numero
/* ACRESCENTADO */
void BigInt::decrement(){
    if(isZero()){
        *this = -1;
        return;
    }
    int k=0;
    while(k<size()){
        if(d[k] > 0){
            d[k]--;
            break;
        }
        d[k] = 9;
        ++k;
    }
    correct();
}

/// Operador de incremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator++()
{
  if (!isNeg()) increment();
  else decrement();
  return *this;
}

/// Operador de decremento pre-fixado
/// NAO PODE SER MODIFICADO
BigInt& BigInt::operator--()
{
  if (isNeg()) increment();
  else decrement();
  return *this;
}

/// Operador de incremento pos-fixado
/* ACRESCENTADO */
BigInt BigInt::operator++(int){
    BigInt temp = *this;
    ++(*this);
    return temp;
}


/// Operador de decremento pos-fixado
/* ACRESCENTADO */
BigInt BigInt::operator--(int){
    BigInt temp = *this;
    --(*this);
    return temp;
}


/// ******************
/// * FIM DA PARTE 5 *
/// ******************

/// Modulo (abs)
/* ACRESCENTADO */
BigInt abs(const BigInt &X){
    BigInt temp(X);
    temp.neg = false;
    return temp;
}

/// Negativo (unario)
/* ACRESCENTADO */
BigInt BigInt::operator-() const{
    if(isZero()) return *this;

    BigInt temp(*this);
    temp.neg = !isNeg();
    return temp;
}

/// Soma
/* ACRESCENTADO */
BigInt operator+(const BigInt &A,const BigInt &B){
    if(A.isZero()) return B;
    if(B.isZero()) return A;
    if(A.isNeg() != B.isNeg()) return A -(-B);

    BigInt C(A.isNeg(),1+max(A.size(),B.size()));

    int carry = 0;

    for(int i=0; i<C.size();++i){
        C.d[i]= A[i]+B[i]+carry;
        if(C[i]>9){
            C.d[i] = C.d[i] - 10;
            carry = 1;
        }else{
            carry = 0;
        }
    }
    C.correct();
    return C;
}
/// Subtracao
/* ACRESCENTADO */
BigInt operator-(const BigInt &A,const BigInt &B){
    if(A.isZero()) return -B;
    if(B.isZero()) return A;
    if(A.isNeg() != B.isNeg()) return A +(-B);

    if(abs(A) < abs(B)) return -(B-A);
    BigInt C(A.isNeg(),A.size());

    int borrow = 0;
    for(int i=0; i<C.size();++i){
        C.d[i]= A[i]-B[i]-borrow;
        if(C[i]<0){
            C.d[i] = C.d[i] + 10;
            borrow = 1;
        }else{
            borrow = 0;
        }
    }
    C.correct();
    return C;
}
/// ******************
/// * FIM DA PARTE 6 *
/// ******************

/// Multiplicacao
/* ACRESCENTADO */
BigInt operator*(const BigInt &A,const BigInt &B){
    if(A.isZero() || B.isZero()) return 0;

    BigInt C((A.isNeg() != B.isNeg()), A.size()+B.size());

    for(int i=0; i<A.size(); ++i){
        if(A.d[i] != 0){
            for(int j=0; j<B.size(); ++j){
                if(B.d[j] != 0){
                    int k = i+j;
                    C.d[k] = C.d[k] + A.d[i]*B.d[j];
                    while(C.d[k] >9){
                        int carry = C.d[k] / 10;
                        C.d[k] = C.d[k] % 10;
                        k++;
                        C.d[k] = C.d[k] + carry;
                    }
                }
            }
        }
    }
    C.correct();
    return C;
}
/// Fatorial
/* ACRESCENTADO */
BigInt BigInt::operator!() const{
    if(isNeg()){
        cerr << "Nao existe fatorial de numero negativo"<< endl;
        return 0;
    }
    BigInt C(1);
    for(BigInt N =2; N<=*this; ++N){
        C = C*N;
    }
    return C;
}
/// ******************
/// * FIM DA PARTE 7 *
/// ******************

/// Deslocamento aa esquerda
/* ACRESCENTADO */
BigInt operator<<(const BigInt &A,int N){
    if(N<=0 || A.isZero()) return A;

    BigInt C(A.isNeg(),A.size()+N);
    for(int i= N; i<C.size(); ++i){
        C.d[i] = A[i-N];
    }
    return C;
}
/// Deslocamento aa direita
/* ACRESCENTADO */
BigInt operator>>(const BigInt &A,int N){
    if(N<=0 || A.isZero()) return A;
    if(N>=A.size()) return 0;

    BigInt C(A.isNeg(),A.size()-N);
    for(int i= 0; i<C.size(); ++i){
        C.d[i] = A[i+N];
    }
    return C;
}

/// Divisao de *this por D.
void BigInt::division(const BigInt &D, BigInt &Q, BigInt &R) const{
    const BigInt &A =*this;
    if(A.isZero() || D.isZero()){
        if(D.isZero()){
                cerr << "Nao e possivel fazer divisao por zero"<< endl;
        }
        Q =0;
        R =0;
        return;
    }
    BigInt absD = abs(D);

    if(abs(A)< absD){
    Q=0;
    R=A;
    return;
    }

    Q = BigInt(A.isNeg() != D.isNeg(), A.size()-D.size()+1);

    R = abs(A >> (A.size()-D.size()));
    for(int i=Q.size()-1; i>=0; --i){
        int div = 0;
        while(R>=absD){
            R= R-absD;
            div++;
        }
        Q.d[i] = div;
        if(i>0){
            R = R<<1;
            R.d[0] = A[i-1];
        }
    }
    Q.correct();
    if(!(R.isZero())){
            R.neg = A.isNeg();
    }
}

/// Quociente da divisao inteira
/* ACRESCENTADO */
BigInt operator/(const BigInt &A,const BigInt &B){
    BigInt Q,R;
    A.division(B,Q,R);
    return Q;
}
/// Resto da divisao inteira
/* ACRESCENTADO */
BigInt operator%(const BigInt &A,const BigInt &B){
    BigInt Q,R;
    A.division(B,Q,R);
    return R;
}
/// ******************
/// * FIM DA PARTE 8 *
/// ******************
