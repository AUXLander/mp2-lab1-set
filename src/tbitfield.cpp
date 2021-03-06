// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include "iostream";
#include "string"
#include "tbitfield.h"

const size_t TSIZE = sizeof(TELEM) * 8;

TBitField::TBitField(int len = 0)
{
	if (len > 0) {
		BitLen = len;
		MemLen = (len / TSIZE + 1);
		pMem = new TELEM[MemLen];

		for (size_t i = 0; i < MemLen; i++) {
			pMem[i] = 0;
		}
	}
	else {
		throw "Incorrect data lenth";
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (size_t i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}
TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0 && n < BitLen) {
		return n / TSIZE;
	}
	else {
		throw "Incorrect index";
	}
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= 0 && n < BitLen) {
		return (TELEM)1 << n;
	}
	else {
		throw "Incorrect index";
	}
}

//// доступ к битам битового поля
//
int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen) {
		int indexInt = n / TSIZE;
		pMem[indexInt] = pMem[indexInt] | GetMemMask(n);
	}
	else {
		throw "Incorrect index";
	}

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen) {
		int indexInt = n / TSIZE;
		pMem[indexInt] = pMem[indexInt] & (~GetMemMask(n));
	}
	else {
		throw "Incorrect index";
	}
}


int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n <= BitLen){
		return ( pMem[n / TSIZE] & (1 << n) ) > 0;
	}
	else {
		throw "Incorrect index";
	}
}
//
//// битовые операции
//
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete[] pMem;
	pMem = new TELEM[bf.MemLen];

	for (size_t i = 0; i < bf.MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}

	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
	}

	for (size_t i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;

	TBitField temp(maxBitLen);

	for (size_t i = 0; i < minMemLen; i++) {
		temp.pMem[i] = pMem[i] | bf.pMem[i];
	}
  
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
	int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;

	TBitField temp(maxBitLen);

	for (size_t i = 0; i < minMemLen; i++) {
		temp.pMem[i] = pMem[i] & bf.pMem[i];
	}
  
	return temp;
}

TBitField TBitField::operator~(void) // отрицание 
{
	int temp = 0;

	for (int i = 0; i < BitLen % 32; i++){
		temp = temp + pow(2, i);
	}

	for (int i = 0; i < MemLen; i++){
		pMem[i] = ~pMem[i];

		if (i == MemLen - 1) {
			pMem[i] = pMem[i] & temp;
		}
	}

	return *this;
}

//// ввод/вывод
//
istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string str;
	istr >> str;

	reverse(str.begin(), str.end());

	for (size_t i = 0; i < str.size(); i++){
		if (str[i] == '1') {
			bf.SetBit(i);
		}	
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	string str;

	for (size_t i = 0; i < bf.BitLen; i++){
		str.push_back( bf.GetBit(i) + '0' );
	}

	reverse(str.begin(), str.end());

	return ostr << str;
}