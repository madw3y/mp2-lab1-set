// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) : BitLen(len)
{
	if (len <= 0 )  throw -1; 
	MemLen = len / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];        
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

void TBitField::checkLen(int n) const {
	if (n < 0 || n >= BitLen) {
		throw -1;
	}
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	checkLen(n);
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	checkLen(n);
    return  1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}
 
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) return *this;

	if (BitLen != bf.BitLen) {
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) { return 0; }

	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i)) { return 0; }
	}

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) { return 1; }

	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i)) { return 1; }
	}

	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int resultLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;

	TBitField resultField(resultLen);

	for (int i = 0; i < resultField.MemLen; i++) {
		resultField.pMem[i] = pMem[i] | bf.pMem[i];
	}

    return resultField;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int resultLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;

	TBitField resultField(resultLen);

	for (int i = 0; i < resultField.MemLen; i++) {
		resultField.pMem[i] = pMem[i] & bf.pMem[i];
	}

    return resultField;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++) {
		result.pMem[i] = ~pMem[i];
	}
	int lastElementMask = (1 << BitLen % (sizeof(TELEM) * 8)) - 1;
	if (BitLen % (sizeof(TELEM) * 8) != 0)
		result.pMem[MemLen - 1] &= lastElementMask;
    return result;
}

// ввод/вывод
istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int bit;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> bit;
		if (bit) {
			bf.SetBit(i);
		} else {
			bf.ClrBit(i);
		}
	}
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	}
    return ostr;
}
