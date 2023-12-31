#include <iostream>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <conio.h>
int Brows = 10, Bcolumns = 10;
const int mrows = 1000, mcol = 1000;
using namespace std;
enum turn { white, black };
const double pi = 3.14;
struct position
{
	int ri;
	int ci;
};
struct undo
{
	char U[9][9];
	char ucpw[9 * 2 + 1];
	char ucpb[9 * 2 + 1];
	position ud;
	position us;
	bool chw;
	bool chb;
	int i;
};
void DeleteMemoryDouble(char** B, char** BC, bool** bmap, int dim)
{
	for (int i = 0;i < dim;i++)
	{
		delete[]B[i];
		delete[]BC[i];
		delete[]bmap[i];
	}
	delete[]B;
	B = nullptr;
	delete[]BC;
	BC = nullptr;
	delete[]bmap;
	bmap = nullptr;
}
void DeleteMemorySingle(char* CPW, char* CPB)
{
	delete[]CPW;
	CPW = nullptr;
	delete[]CPB;
	CPB = nullptr;
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void Delay(double i)
{
	for (;i < 1000000;i += 0.1);
}
void PawnSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j < s / 4;j++)
	{
		gotoRowCol(cr, cc + j);
		cout << sym;
	}
	for (int j = 1;j <= s / 4;j++)
	{
		gotoRowCol(cr, cc - j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr - i, cc + (s / 4 - 1));
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr - s / 2, (cc + s / 4 - j - 1));
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr - s / 2) + i, cc - s / 4 - 1);
		cout << sym;
	}

}
void PawnSym2(int cr, int cc, int s, char sym = -37)
{
	for (int ln = 0;ln < s / 2;ln++)
	{
		gotoRowCol(cr + ln, cc - ln);
		for (int s = 0;s < ln * 2 + 1;s++)
			cout << sym;
	}
	for (int ln = 1;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ((s / 4) * 2) + 1;s1++)
		{
			gotoRowCol(cr - ln, (cc - (s / 4)) + s1);
			cout << sym;
		}
	}
}
void PromoPawnSymW(int cr, int cc, int s)
{
	SetClr(12);
	PawnSym2(cr, cc, s);
}
void PromoPawnSymB(int cr, int cc, int s)
{
	SetClr(5);
	PawnSym2(cr, cc, s);
}
void BishopSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j < s / 4;j++)
	{
		gotoRowCol(cr, cc + j);
		cout << sym;
	}
	for (int j = 1;j <= s / 4;j++)
	{
		gotoRowCol(cr, cc - j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr + i, cc + (s / 4 - 1));
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr + s / 2, (cc + s / 4 - j - 1));
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr + s / 2) - i, cc - s / 4 - 1);
		cout << sym;
	}
}
void BishopSym2(int cr, int cc, int s, char sym = -37)
{
	for (int ln = 0;ln < s / 2;ln++)
	{
		gotoRowCol(cr + ln, cc - ln);
		for (int s = 0;s < ln * 2 + 1;s++)
			cout << sym;
	}
	for (int ln = 0;ln < s / 2 - 1;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			gotoRowCol(cr - ((s / 2 - 1) - ln), (cc - ln) + s1);
			cout << sym;
		}
	}
}
void PromoBishopSymW(int cr, int cc, int s)
{
	SetClr(12);
	BishopSym2(cr, cc, s);
}
void PromoBishopSymB(int cr, int cc, int s)
{
	SetClr(5);
	BishopSym2(cr, cc, s);
}
void RookSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j < s / 4;j++)
	{
		gotoRowCol(cr, cc + j);
		cout << sym;
	}
	for (int j = 1;j <= s / 4;j++)
	{
		gotoRowCol(cr, cc - j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr - i, cc + (s / 4 - 1));
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr - s / 2, (cc + s / 4 - j - 1));
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr - s / 2) + i, cc - s / 4 - 1);
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr + j, (cc - s / 4 - 1) + j);
		cout << sym;
	}
}
void RookSym2(int cr, int cc, int s, char sym = -37)
{
	int count = 0;
	for (int ln = 1;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			gotoRowCol(cr + ln, cc - ln + s1);
			cout << sym;
		}
	}
	for (int ln = 0;ln < 1;ln++)
	{
		for (int s1 = 0;s1 < (ln + 1) * 2 + 1;s1++)
		{
			if (count % 2 == 0)
			{
				gotoRowCol(cr + ln, cc - 1 + s1);
				cout << sym;
			}
			count++;
		}
	}
	for (int ln = 2;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			gotoRowCol(cr - ln + 1, cc - ln + s1);
			cout << sym;
		}
	}
	for (int ln = 2;ln < s / 2;ln++)
	{
		count = 0;
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count % 2 == 0)
			{
				gotoRowCol(cr - ln, cc - ln + s1);
				cout << sym;
			}
			count++;
		}
	}
}
void PromoRookSymW(int cr, int cc, int s)
{
	SetClr(12);
	RookSym2(cr, cc, s);
}
void PromoRookSymB(int cr, int cc, int s)
{
	SetClr(5);
	RookSym2(cr, cc, s);
}
void LanceSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j <= s / 4;j++)
	{
		gotoRowCol(cr + s / 2, cc + j);
		cout << sym;
	}
	for (int j = 1;j <= s / 4;j++)
	{
		gotoRowCol(cr + s / 2, cc - j);
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr + s / 2) - i, cc - s / 4);
		cout << sym;
	}
}
void LanceSym2(int cr, int cc, int s, char sym = -37)
{

	for (int ln = 0;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (s1 == 0 || s1 == ln * 2 + 1 - 1)
			{
				gotoRowCol(cr + ln, cc - ln + s1);
				cout << sym;
			}
		}
	}
	for (int ln = 0;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (s1 == 0 || s1 == ln * 2 + 1 - 1)
			{
				gotoRowCol(cr - 2 + ln, cc - ln + s1);
				cout << sym;
			}
		}
	}
}
void PromoLanceSymW(int cr, int cc, int s)
{
	SetClr(12);
	LanceSym2(cr, cc, s);
}
void PromoLanceSymB(int cr, int cc, int s)
{
	SetClr(5);
	LanceSym2(cr, cc, s);
}
void KnightSym(int cr, int cc, int s, char sym = -37)
{
	for (int i = 0;i < s / 2;i++)
	{
		gotoRowCol(cr + i, cc + i);
		cout << sym;
	}
	for (int i = 0;i < s / 2;i++)
	{
		gotoRowCol(cr - i, cc - i);
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr - s / 2) + i, cc - s / 2);
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol((cr + s / 2) - i, cc + s / 2);
		cout << sym;
	}
}
void KnightSym2(int cr, int cc, int s, char sym = -37)
{
	for (int ln = 1;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 <= ln + 1;s1++)
		{
			gotoRowCol(cr + ln, cc - 2 + s1);
			cout << sym;
		}
	}
	for (int ln = 0;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (ln != s / 2 - 1)
			{
				gotoRowCol(cr - 2 + ln, cc - ln + s1);
				cout << sym;
			}
			else
			{
				if (cc - ln + s1 != cc)
				{
					gotoRowCol(cr - 2 + ln, cc - ln + s1);
					cout << sym;
				}
			}
		}
	}
}
void PromoKnightSymW(int cr, int cc, int s)
{
	SetClr(12);
	KnightSym2(cr, cc, s);
}
void PromoKnightSymB(int cr, int cc, int s)
{
	SetClr(5);
	KnightSym2(cr, cc, s);
}
void GoldGeneralSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j < s / 4;j++)
	{
		gotoRowCol(cr, cc + j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr + i, cc + (s / 4 - 1));
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr + s / 2, (cc + s / 4 - 1) - j);
		cout << sym;
	}
	for (int i = 1;i <= s;i++)
	{
		gotoRowCol(cr + s / 2 - i, (cc + s / 4 - 1) - s / 2);
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol((cr + s / 2 - s), ((cc + s / 4 - 1) - s / 2) + j);
		cout << sym;
	}
}
void GoldGeneralSym2(int cr, int cc, int s, char sym = -37)
{
	int count = 0;
	for (int ln = 0;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count != 2)
			{
				if (s1 == 0 || s1 == ln * 2 + 1 - 1)
				{
					gotoRowCol(cr - 2 + ln, cc - ln + s1);
					cout << sym;
				}
			}
			else
			{
				gotoRowCol(cr - 2 + ln, cc - ln + s1);
				cout << sym;
			}
		}
		count++;
	}
	count = 0;
	for (int ln = 1;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count != 1)
			{
				if (s1 == 0 || s1 == ln * 2 + 1 - 1)
				{
					gotoRowCol(cr + ln, cc - ln + s1);
					cout << sym;
				}
			}
			else
			{
				gotoRowCol(cr + ln, cc - ln + s1);
				cout << sym;
			}
		}
		count++;
	}
}
void SilverGeneralSym(int cr, int cc, int s, char sym = -37)
{
	for (int j = 0;j <= s / 4;j++)
	{
		gotoRowCol(cr, cc + j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr + i, cc + s / 4);
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr + s / 2, (cc + s / 4) - j);
		cout << sym;
	}
	for (int j = 1;j <= s / 4;j++)
	{
		gotoRowCol(cr, cc - j);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr - i, cc - s / 4);
		cout << sym;
	}
	for (int j = 1;j <= s / 2;j++)
	{
		gotoRowCol(cr - s / 2, (cc - s / 4) + j);
		cout << sym;
	}
}
void SilverGeneralSym2(int cr, int cc, int s, char sym = -37)
{
	int count = 0;
	for (int ln = 0;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{

			if (s1 == 0 || s1 == ln * 2 + 1 - 1)
			{
				gotoRowCol(cr - 2 + ln, cc - ln + s1);
				cout << sym;
			}
		}
		count++;
	}
	count = 0;
	for (int ln = 1;ln < s / 2;ln++)
	{
		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count != 1)
			{
				if (s1 == 0 || s1 == ln * 2 + 1 - 1)
				{
					gotoRowCol(cr + ln, cc - ln + s1);
					cout << sym;
				}
			}
			else
			{
				gotoRowCol(cr + ln, cc - ln + s1);
				cout << sym;
			}
		}
		count++;
	}
}
void PromoSilverGeneralW(int cr, int cc, int s)
{
	SetClr(12);
	SilverGeneralSym2(cr, cc, s);
}
void PromoSilverGeneralB(int cr, int cc, int s)
{
	SetClr(5);
	SilverGeneralSym2(cr, cc, s);
}
void KingSym(int cr, int cc, int s, char sym = -37)
{
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol((cr + 1) - i, cc + i);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr + i, cc + i);
		cout << sym;
	}
	for (int i = 0;i < s / 2;i++)
	{
		gotoRowCol(cr - i, cc);
		cout << sym;
	}
	for (int i = 1;i <= s / 2;i++)
	{
		gotoRowCol(cr + i, cc);
		cout << sym;
	}
}
void KingSym2(int cr, int cc, int s, char sym = -37)
{
	for (int ln = 2;ln < s / 2;ln++)
	{
		int count = 0;

		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count % 2 == 0)
			{
				gotoRowCol(cr - ln + 1, cc - ln + s1);
				cout << sym;
			}
			count++;
		}

	}
	for (int ln = 2;ln < s / 2;ln++)
	{

		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{

			gotoRowCol(cr - ln + 2, cc - ln + s1);
			cout << sym;

		}

	}
	for (int ln = 2;ln < s / 2;ln++)
	{
		int count = 0;

		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{
			if (count % 2 == 0)
			{
				gotoRowCol(cr + ln - 1, cc - ln + s1);
				cout << sym;
			}
			count++;
		}

	}
	for (int ln = 2;ln < s / 2;ln++)
	{

		for (int s1 = 0;s1 < ln * 2 + 1;s1++)
		{

			gotoRowCol(cr + ln, cc - ln + s1);
			cout << sym;

		}

	}
}
void RemoveRowsOnConsole(int rs, int re)
{
	gotoRowCol(rs, 0);
	for (int t = 0;t < re;t++)
	{
		for (int i = 0;i < 100;i++)
		{
			cout << " ";
		}
		cout << endl;
	}
	gotoRowCol(rs, 0);
}
bool IsPathClrDiagRtoL(position s, position d, char** B)
{
	int Chr = abs(s.ri - d.ri);
	if (s.ri < d.ri)
	{
		for (int i = 1;i < Chr;i++)
		{
			if (B[s.ri + i][s.ci - i] != '-')
				return false;
		}
		return true;
	}
	else
	{
		for (int i = 1;i < Chr;i++)
		{
			if (B[d.ri + i][d.ci - i] != '-')
				return false;
		}
		return true;
	}
}
bool IsPathClrDiagLtoR(position s, position d, char** B)
{
	int Chr = abs(s.ri - d.ri);
	if (s.ri < d.ri)
	{
		for (int i = 1;i < Chr;i++)
		{
			if (B[s.ri + i][s.ci + i] != '-')
				return false;
		}
		return true;
	}
	else
	{
		for (int i = 1;i < Chr;i++)
		{
			if (B[d.ri + i][d.ci + i] != '-')
				return false;
		}
		return true;
	}
}
bool IsPathClrVerti(position s, position d, char** B)
{
	int st, ed;
	if (s.ri < d.ri)
	{
		st = s.ri + 1;
		ed = d.ri;
	}
	else
	{
		st = d.ri + 1;
		ed = s.ri;
	}
	for (;st < ed;st++)
	{
		if (B[st][s.ci] != '-')
			return false;
	}
	return true;
}
bool IsPathClrHorizon(position s, position d, char** B)
{
	int st, ed;
	if (s.ci < d.ci)
	{
		st = s.ci + 1;
		ed = d.ci;
	}
	else
	{
		st = d.ci + 1;
		ed = s.ci;
	}
	for (;st < ed;st++)
	{
		if (B[s.ri][st] != '-')
			return false;
	}
	return true;
}
bool IsHorizonMove(position s, position d)
{
	return s.ri == d.ri;
}
bool IsVerticalMove(position s, position d)
{
	return  s.ci == d.ci;
}
bool IsDiagMove(position s, position d)
{
	int Chr = abs(s.ri - d.ri), Chc = abs(s.ci - d.ci);
	return Chr == Chc;
}
bool IsDiagMoveLtoR(position s, position d)
{
	return IsDiagMove(s, d) && d.ci > s.ci && d.ri > s.ri || IsDiagMove(s, d) && d.ci < s.ci&& d.ri < s.ri;
}
bool IsDiagMoveRtoL(position s, position d)
{
	return IsDiagMove(s, d) && d.ci < s.ci&& d.ri>s.ri || IsDiagMove(s, d) && d.ci > s.ci && d.ri < s.ri;
}
void DrawBox(int sr, int sc, int rows, int columns, int clr)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < columns;j++)
		{
			SetClr(clr);
			gotoRowCol(sr + i, sc + j);
			cout << char(-37);
		}
	}
}
void DrawBoard(int dim, int rows, int columns)
{

	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			if ((i + j) % 2 == 0)
				DrawBox(i * rows, j * columns, rows, columns, 7);
			else
				DrawBox(i * rows, j * columns, rows, columns, 8);
		}

	}
}
void Capitalize(char& sym)
{
	if (sym >= 'a' && sym <= 'z')
	{
		sym = sym - 32;
	}

}
void DeCapitalize(char& sym)
{
	if (sym >= 'A' && sym <= 'Z')
	{
		sym = sym + 32;
	}
}
bool InBoundD(position d, int dim)
{
	if (d.ri < 0 || d.ri >= dim || d.ci < 0 || d.ci >= dim)
		return false;
	return true;
}
bool InBoundS(position s, int dim)
{
	if (s.ri < 0 || s.ri >= dim || s.ci < 0 || s.ci >= dim)
		return false;
	return true;
}
bool IsRookMove(position s, position d, char** B)
{
	return IsHorizonMove(s, d) && IsPathClrHorizon(s, d, B) || IsVerticalMove(s, d) && IsPathClrVerti(s, d, B);
}
bool IsLanceMove(position s, position d, char** B, int turn)
{
	if (turn == white)
		return IsVerticalMove(s, d) && IsPathClrVerti(s, d, B) && d.ri < s.ri;
	else
		return IsVerticalMove(s, d) && IsPathClrVerti(s, d, B) && s.ri < d.ri;

}
bool IsBishopMove(position s, position d, char** B)
{
	return IsDiagMoveLtoR(s, d) && IsPathClrDiagLtoR(s, d, B) || IsDiagMoveRtoL(s, d) && IsPathClrDiagRtoL(s, d, B);
}
bool IsKingMove(position s, position d, char** B)
{
	int Chr = abs(s.ri - d.ri);
	int Chc = abs(s.ci - d.ci);
	return IsRookMove(s, d, B) && Chr <= 1 && Chc <= 1 || IsBishopMove(s, d, B) && Chr <= 1 && Chc <= 1;
}
bool IsPawnMove(position s, position d, char** B, int turn)
{
	int Chr = abs(s.ri - d.ri);
	return IsLanceMove(s, d, B, turn) && Chr == 1;
}
bool IsGoldGeneralMove(position s, position d, char** B, int turn)
{
	int Chr = abs(s.ri - d.ri);
	int Chc = abs(s.ci - d.ci);
	if (turn == white)
		return IsBishopMove(s, d, B) && Chr == 1 && Chc == 1 && d.ri < s.ri || IsRookMove(s, d, B) && Chr <= 1 && Chc <= 1;
	else
		return IsBishopMove(s, d, B) && Chr == 1 && Chc == 1 && s.ri < d.ri || IsRookMove(s, d, B) && Chr <= 1 && Chc <= 1;
}
bool IsSilverGeneralMove(position s, position d, char** B, int turn)
{
	int Chr = abs(s.ri - d.ri);
	int Chc = abs(s.ci - d.ci);
	return IsBishopMove(s, d, B) && Chr == 1 && Chc == 1 || IsPawnMove(s, d, B, turn);
}
bool IsKnightMove(position& s, position d, char** B, int turn)
{
	int Chr, Chc;

	if (turn == white)
	{
		s.ri = s.ri - 1;
		Chr = abs(s.ri - d.ri);
		Chc = abs(s.ci - d.ci);
		return IsDiagMove(s, d) && d.ri < s.ri&& Chr == 1 && Chc == 1;
	}
	else
	{
		s.ri = s.ri + 1;
		Chr = abs(s.ri - d.ri);
		Chc = abs(s.ci - d.ci);
		return IsDiagMove(s, d) && s.ri < d.ri&& Chr == 1 && Chc == 1;
	}
}
bool IsPromoBishopMove(position s, position d, char** B, int turn)
{
	return IsBishopMove(s, d, B) || IsKingMove(s, d, B);
}
bool IsPromoRookMove(position s, position d, char** B, int turn)
{
	return IsRookMove(s, d, B) || IsKingMove(s, d, B);
}
bool IsLegalMove(position s, position d, char** B, int turn)
{

	switch (B[s.ri][s.ci])
	{
	case'r':
	case'R':
		return IsRookMove(s, d, B);
	case'b':
	case'B':
		return IsBishopMove(s, d, B);
	case'k':
	case'K':
		return IsKingMove(s, d, B);
	case'p':
	case'P':
		return IsPawnMove(s, d, B, turn);
	case'l':
	case'L':
		return IsLanceMove(s, d, B, turn);
	case'g':
	case'G':
	case'q':
	case'Q':
	case'w':
	case'W':
	case't':
	case'T':
	case'e':
	case'E':
		return IsGoldGeneralMove(s, d, B, turn);
	case's':
	case'S':
		return IsSilverGeneralMove(s, d, B, turn);
	case'n':
	case'N':
		return IsKnightMove(s, d, B, turn);
	case'X':
	case'x':
		return IsPromoBishopMove(s, d, B, turn);
	case'Y':
	case'y':
		return IsPromoRookMove(s, d, B, turn);
	}
}
bool IsValidMoveBlackD(position d, char** B)
{
	if (B[d.ri][d.ci] >= 'A' && B[d.ri][d.ci] <= 'Z' || B[d.ri][d.ci] == '-')
		return true;
	return false;

}
bool IsValidMoveBlackS(position s, char** B)
{
	if (B[s.ri][s.ci] >= 'a' && B[s.ri][s.ci] <= 'z')
		return true;
	return false;

}
bool IsValidMoveWhiteD(position d, char** B)
{
	if (B[d.ri][d.ci] >= 'a' && B[d.ri][d.ci] <= 'z' || B[d.ri][d.ci] == '-')
		return true;
	return false;

}
bool IsValidMoveWhiteS(position s, char** B)
{
	if (B[s.ri][s.ci] >= 'A' && B[s.ri][s.ci] <= 'Z')
		return true;
	return false;

}
bool IsValidMoveS(position s, char** B, int turn)
{
	if (turn == white)
	{
		if (!IsValidMoveWhiteS(s, B))
			return false;
		return true;
	}
	else if (turn == black)
	{
		if (!IsValidMoveBlackS(s, B))
			return false;
		return true;
	}
}
bool IsValidMoveD(position d, char** B, int turn)
{
	if (turn == white)
	{
		if (!IsValidMoveWhiteD(d, B))
			return false;
		return true;
	}
	else if (turn == black)
	{
		if (!IsValidMoveBlackD(d, B))
			return false;
		return true;
	}
}
bool** BoolMap(char** B, position s, int dim, int turn)
{
	position d;
	bool** bmap;
	bmap = new bool* [dim];
	for (int i = 0;i < dim;i++)
		bmap[i] = new bool[dim] {};

	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			d.ri = i;
			d.ci = j;
			if (IsValidMoveD(d, B, turn) && IsLegalMove(s, d, B, turn))
				bmap[i][j] = true;
		}
	}
	return bmap;
}
void ColorSteps(int r, int c)
{
	r = (r * Brows) + (Brows / 2);
	c = (c * Bcolumns) + (Bcolumns / 2);
	r = (r / Brows) * Brows;
	c = (c / Bcolumns) * Bcolumns;
	SetClr(11);
	for (int j = c;j < c + Bcolumns;j++)
	{
		gotoRowCol(r, j);
		cout << char(-37);
	}
	for (int j = (c + Bcolumns) - 1, i = r;i < r + Brows;i++)
	{
		gotoRowCol(i, j);
		cout << char(-37);
	}
	for (int i = (r + Brows) - 1, j = (c + Bcolumns) - 1;j >= c;j--)
	{
		gotoRowCol(i, j);
		cout << char(-37);
	}
	for (int i = (r + Brows) - 1;i >= r;i--)
	{
		gotoRowCol(i, c);
		cout << char(-37);
	}
}
void DeColorSteps(int r, int c)
{
	int i = r, j = c;
	r = (r * Brows) + (Brows / 2);
	c = (c * Bcolumns) + (Bcolumns / 2);
	r = (r / Brows) * Brows;
	c = (c / Bcolumns) * Bcolumns;
	if ((i + j) % 2 == 0)
		SetClr(7);
	else
		SetClr(8);
	for (int j = c;j < c + Bcolumns;j++)
	{
		gotoRowCol(r, j);
		cout << char(-37);
	}
	for (int j = (c + Bcolumns) - 1, i = r;i < r + Brows;i++)
	{
		gotoRowCol(i, j);
		cout << char(-37);
	}
	for (int i = (r + Brows) - 1, j = (c + Bcolumns) - 1;j >= c;j--)
	{
		gotoRowCol(i, j);
		cout << char(-37);
	}
	for (int i = (r + Brows) - 1;i >= r;i--)
	{
		gotoRowCol(i, c);
		cout << char(-37);
	}

}
void ShowSteps(bool** bmap, int dim)
{
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			if (bmap[i][j] == true)
				ColorSteps(i, j);
		}
	}
}
void HideSteps(bool** bmap, int dim)
{
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			if (bmap[i][j] == true)
				DeColorSteps(i, j);
		}
	}
}
void TurnCh(int& turn)
{
	if (turn == white)
		turn = black;
	else
		turn = white;

}
position FindKing(char** B, int dim, int turn)
{
	position d;
	char king;
	if (turn == white)
		king = 'K';
	else
		king = 'k';
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			if (B[i][j] == king)
			{
				d.ri = i;
				d.ci = j;
				return d;
			}

		}
	}
}
void PlaceMoveCopy(position s, position d, char**& BC, char** B, int dim)
{
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			BC[i][j] = B[i][j];
		}
	}
	BC[d.ri][d.ci] = BC[s.ri][s.ci];
	BC[s.ri][s.ci] = '-';
}
bool IsCheck(int turn, char** B, int dim)
{

	position d;
	d = FindKing(B, dim, turn);
	TurnCh(turn);
	position s;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			s.ri = i;
			s.ci = j;
			if (IsValidMoveS(s, B, turn) && IsLegalMove(s, d, B, turn))
				return true;
		}
	}
	return false;
}
bool IsSelfCheck(int turn, char** BC, int dim)
{
	return IsCheck(turn, BC, dim);
}
bool IsDefend(int turn, int dim, char** B, char** BC)
{
	TurnCh(turn);
	position d;
	position s;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			s.ri = i;
			s.ci = j;
			if (IsValidMoveS(s, B, turn) && B[s.ri][s.ci] != 'K' || IsValidMoveS(s, B, turn) && B[s.ri][s.ci] != 'k')
			{
				for (int i1 = 0;i1 < dim;i1++)
				{
					for (int j1 = 0;j1 < dim;j1++)
					{
						d.ri = i1;
						d.ci = j1;
						if (IsValidMoveD(d, B, turn) && IsLegalMove(s, d, B, turn))
						{
							PlaceMoveCopy(s, d, BC, B, dim);
							if (!IsCheck(turn, BC, dim))
								return true;
						}

					}
				}
			}
		}
	}
	return false;
}
bool IsCheckmate(int turn, int dim, char** B, char** BC)
{
	int count = 0;
	TurnCh(turn);
	position s;
	s = FindKing(B, dim, turn);
	position d;
	if (IsCheck(turn, B, dim))
		count++;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			d.ri = i;
			d.ci = j;
			if (IsLegalMove(s, d, B, turn) && IsValidMoveD(d, B, turn))
			{
				PlaceMoveCopy(s, d, BC, B, dim);
				if (!IsCheck(turn, BC, dim))
					return false;
			}


		}
	}
	count++;
	if (count == 2)
		return true;
	else
		return false;
}
bool IsStalemate(int turn, int dim, char** B, char** BC)
{
	TurnCh(turn);
	position s;
	s = FindKing(B, dim, turn);
	position d;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			d.ri = i;
			d.ci = j;
			if (IsLegalMove(s, d, B, turn) && IsValidMoveD(d, B, turn))
			{
				PlaceMoveCopy(s, d, BC, B, dim);
				if (!IsCheck(turn, BC, dim))
					return false;
			}


		}
	}
	return true;
}
void init(char**& B, int& turn, int& dim, string PNames[], char**& BC, char*& CPW, char*& CPB, undo*& u)
{
	char save;
	cout << "Do you want to play saved game or a new game:" << endl;
	cout << "Press s to play last saved game and n for new:" << endl;
	cin >> save;
	Capitalize(save);
	dim = 9;
	u = new undo[1000];
	B = new char* [dim];
	BC = new char* [dim];
	CPW = new char[dim * 2 + 1];
	CPB = new char[dim * 2 + 1];
	for (int i = 0;i < dim;i++)
	{
		B[i] = new char[dim];
		BC[i] = new char[dim];
	}
	for (int i = 0;i <= dim * 2;i++)
	{
		CPW[i] = '-';
		CPB[i] = '-';
		u[0].ucpw[i] = '-';
		u[0].ucpb[i] = '-';
	}
	if (save == 'N')
	{
		ifstream cin1("new.txt");
		for (int i = 0;i < dim;i++)
		{
			for (int j = 0;j < dim;j++)
			{
				cin1 >> B[i][j];
				BC[i][j] = B[i][j];
			}
		}
		for (int i = 0;i < 2;i++)
		{
			cout << "Enter player " << i + 1 << " name:" << endl;
			cin >> PNames[i];
		}
		turn = white;
	}
	else if (save == 'S')
	{
		ifstream cin1("save.txt");
		for (int i = 0;i < 2;i++)
		{
			cin1 >> PNames[i];
		}
		cin1 >> turn;
		for (int i = 0;i < dim;i++)
		{
			for (int j = 0;j < dim;j++)
			{
				cin1 >> B[i][j];
				BC[i][j] = B[i][j];
			}
		}
		for (int i = 0;i <= dim * 2;i++)
		{
			cin1 >> CPW[i];
			u[0].ucpw[i] = CPW[i];
		}
		for (int i = 0;i <= dim * 2;i++)
		{
			cin1 >> CPB[i];
			u[0].ucpb[i] = CPB[i];
		}
	}



}
void PrintBoard(char** B, int dim)
{
	position s;
	for (int i = 0, r = Brows / 2;i < dim;i++, r = r + Brows)
	{
		for (int j = 0, c = Bcolumns / 2;j < dim;j++, c = c + Bcolumns)
		{
			s.ri = i;
			s.ci = j;
			if (IsValidMoveWhiteS(s, B))
			{
				gotoRowCol(r, c);
				SetClr(15);
				switch (B[s.ri][s.ci])
				{
				case 'B':
					BishopSym2(r, c, 6);
					break;
				case 'P':
					PawnSym2(r, c, 6);
					break;
				case'R':
					RookSym2(r, c, 6);
					break;
				case'L':
					LanceSym2(r, c, 6);
					break;
				case'N':
					KnightSym2(r, c, 6);
					break;
				case'G':
					GoldGeneralSym2(r, c, 6);
					break;
				case'S':
					SilverGeneralSym2(r, c, 6);
					break;
				case'K':
					KingSym2(r, c, 6);
					break;
				case'Q':
					PromoPawnSymW(r, c, 6);
					break;
				case'W':
					PromoLanceSymW(r, c, 6);
					break;
				case'E':
					PromoSilverGeneralW(r, c, 6);
					break;
				case'T':
					PromoKnightSymW(r, c, 6);
					break;
				case'X':
					PromoBishopSymW(r, c, 6);
					break;
				case'Y':
					PromoRookSymW(r, c, 6);
					break;
				}


			}
			else if (IsValidMoveBlackS(s, B))
			{
				gotoRowCol(r, c);
				SetClr(0);
				switch (B[s.ri][s.ci])
				{
				case 'b':
					BishopSym2(r, c, 6);
					break;
				case 'p':
					PawnSym2(r, c, 6);
					break;
				case'r':
					RookSym2(r, c, 6);
					break;
				case'l':
					LanceSym2(r, c, 6);
					break;
				case'n':
					KnightSym2(r, c, 6);
					break;
				case'g':
					GoldGeneralSym2(r, c, 6);
					break;
				case's':
					SilverGeneralSym2(r, c, 6);
					break;
				case'k':
					KingSym2(r, c, 6);
					break;
				case'q':
					PromoPawnSymB(r, c, 6);
					break;
				case'w':
					PromoLanceSymB(r, c, 6);
					break;
				case'e':
					PromoSilverGeneralB(r, c, 6);
					break;
				case't':
					PromoKnightSymB(r, c, 6);
					break;
				case'x':
					PromoBishopSymB(r, c, 6);
					break;
				case'y':
					PromoRookSymB(r, c, 6);
					break;
				}
			}
		}
	}
}
void TurnMsg(string PNames[], int turn, int dim)
{
	SetClr(15);
	RemoveRowsOnConsole(dim * Brows + 1, 1);
	gotoRowCol(Brows * dim + 1, 0);
	if (turn == white)
		cout << PNames[turn] << "'s turn (White)" << endl;
	else
		cout << PNames[turn] << "'s turn (Black)" << endl;
}
void RemoveTrail(position s, position d)
{
	int rd = (d.ri * Brows) + 5;
	int cd = (d.ci * Bcolumns) + 5;
	int rs = (s.ri * Brows) + 5;
	int cs = (s.ci * Bcolumns) + 5;
	rs = (rs / Brows) * Brows;
	cs = (cs / Bcolumns) * Bcolumns;
	rd = (rd / Brows) * Brows;
	cd = (cd / Bcolumns) * Bcolumns;
	if ((s.ri + s.ci) % 2 == 0)
		DrawBox(rs, cs, Brows, Bcolumns, 7);
	else
		DrawBox(rs, cs, Brows, Bcolumns, 8);
	if ((d.ri + d.ci) % 2 == 0)
		DrawBox(rd, cd, Brows, Bcolumns, 7);
	else
		DrawBox(rd, cd, Brows, Bcolumns, 8);

}
bool IsTherePiece(position d, char** BC)
{
	if (BC[d.ri][d.ci] == '-')
		return false;
	return true;
}
bool IsPInCol(char** BC, position d, int turn, int dim)
{
	char target;
	if (turn == white)
		target = 'P';
	else
		target = 'p';
	for (int i = 0;i < dim;i++)
	{
		if (BC[i][d.ci] == target)
			return true;
	}
	return false;
}
bool IsPGivingCheckmate(int turn, char** B, char** BC, int dim, position d, int i, char* CPW, char* CPB)
{
	bool ch = true;
	if (B[d.ri][d.ci] != '-')
		ch = false;
	if (ch == true)
	{
		if (turn == white)
			B[d.ri][d.ci] = CPB[i];
		else
			B[d.ri][d.ci] = CPW[i];
	}
	if (IsCheckmate(turn, dim, B, BC))
	{
		if (ch == true)
			B[d.ri][d.ci] = '-';
		for (int i = 0;i < dim;i++)
		{
			for (int j = 0;j < dim;j++)
			{
				BC[i][j] = B[i][j];
			}
		}
		BC[d.ri][d.ci] = '-';
		return true;
	}
	if (ch == true)
		B[d.ri][d.ci] = '-';
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			BC[i][j] = B[i][j];
		}
	}
	BC[d.ri][d.ci] = '-';
	return false;
}
bool IsPOnTheLastRow(int turn, position d)
{
	if (turn == white)
		return d.ri == 0;
	else
		return d.ri == 8;
}
bool IsKnightOnTheLast2Rows(int turn, position d)
{
	if (turn == white)
		return d.ri == 0 || d.ri == 1;
	else
		return d.ri == 8 || d.ri == 7;
}
bool IsValidDropS(int turn, position s, char* CPW, char* CPB)
{
	int i = s.ci;
	if (s.ri == 2 || s.ri == 5)
		i = i % 10;
	else if (s.ri == 3 || s.ri == 6)
		i--;

	if (turn == white)
	{
		if (s.ri >= 5 && s.ri <= 6 && s.ci >= 10 && s.ci <= 18 && CPB[i] != '-')
			return true;
		return false;
	}
	else
	{
		if (s.ri >= 2 && s.ri <= 3 && s.ci >= 10 && s.ci <= 18 && CPW[i] != '-')
			return true;
		return false;
	}
}
bool IsValidDropD(char** B, position d, position s, char** BC, int turn, int dim, char* CPW, char* CPB)
{
	int i = s.ci;
	if (s.ri == 2 || s.ri == 5)
		i = i % 10;
	else if (s.ri == 3 || s.ri == 6)
		i--;
	if (d.ri < 0 || d.ri >= dim || d.ci < 0 || d.ci >= dim)
		return false;
	if (IsTherePiece(d, BC))
		return false;
	if (turn == white && CPB[i] == 'P' || turn == black && CPW[i] == 'p')
	{
		if (IsPInCol(BC, d, turn, dim))
			return false;
		if (IsPGivingCheckmate(turn, B, BC, dim, d, i, CPW, CPB))
			return false;
		if (IsPOnTheLastRow(turn, d))
			return false;
	}
	if (turn == white && CPB[i] == 'N' || turn == black && CPW[i] == 'n')
	{
		if (IsKnightOnTheLast2Rows(turn, d))
			return false;
	}
	if (turn == white && CPB[i] == 'L' || turn == black && CPW[i] == 'l')
	{
		if (IsPOnTheLastRow(turn, d))
			return false;
	}
	if (IsSelfCheck(turn, BC, dim))
	{
		PlaceMoveCopy(s, d, BC, B, dim);
		if (IsSelfCheck(turn, BC, dim))
		{
			BC[d.ri][d.ci] = '-';
			return false;
		}
	}
	BC[d.ri][d.ci] = '-';
	return true;
}
bool** BoolMapSingle(char* CPW, char* CPB, char** B,char**BC,int dim,position s,int turn)
{
	position d;
	bool** bmap;
	bmap = new bool* [dim];
	for (int i = 0;i < dim;i++)
		bmap[i] = new bool[dim] {};
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			d.ri = i;
			d.ci = j;
			BC[i][j] = B[i][j];
			if (IsValidDropD(B, d, s, BC, turn, dim, CPW, CPB))
				bmap[i][j] = true;

		}
	}
	return bmap;
}
bool IsDrop(int dim, position s)
{
	if (s.ri >= 2 && s.ri <= 3 && s.ci >= 10 && s.ci <= 18)
		return true;
	else if (s.ri >= 5 && s.ri <= 6 && s.ci >= 10 && s.ci <= 18)
		return true;
	return false;
}
void UnPromotion(char* CPW, char* CPB, int i, int turn)
{
	if (turn == white)
	{
		switch (CPB[i])
		{
		case'Q':
			CPB[i] = 'P';
			break;
		case'W':
			CPB[i] = 'L';
			break;
		case'E':
			CPB[i] = 'S';
			break;
		case'T':
			CPB[i] = 'N';
			break;
		case'X':
			CPB[i] = 'B';
			break;
		case'Y':
			CPB[i] = 'R';
			break;
		}
	}
	else
	{
		switch (CPW[i])
		{
		case'q':
			CPW[i] = 'p';
			break;
		case'w':
			CPW[i] = 'l';
			break;
		case'e':
			CPW[i] = 's';
			break;
		case't':
			CPW[i] = 'n';
			break;
		case'x':
			CPW[i] = 'b';
			break;
		case'y':
			CPW[i] = 'r';
			break;
		}
	}
}
void BeforeDrop(char* CPW, char* CPB, undo* u, int ui,int dim)
{
	for (int i = 0;i < dim * 2 + 1;i++)
	{
		u[ui].ucpw[i] = CPW[i];
		u[ui].ucpb[i] = CPB[i];
	}
}
void DropPiece(char** B, char** BC, char* CPW, char* CPB, position& s, position& d, int turn, int dim, int& count,undo*u,int &ui)
{
	bool **bmap;
	if (!IsValidDropS(turn, s, CPW, CPB))
		return;
	bmap = BoolMapSingle(CPW, CPB, B, BC, dim, s, turn);
	ShowSteps(bmap, dim);
	gotoRowCol(dim * Brows + 3, 0);
	SetClr(15);
	cout << "Select where to place" << endl;
	getRowColbyLeftClick(d.ri, d.ci);
	d.ri = d.ri / Brows;
	d.ci = d.ci / Bcolumns;
	if (!IsValidDropD(B,d,s,BC,turn,dim,CPW,CPB))
	{
		HideSteps(bmap, dim);
		count = 0;
		RemoveRowsOnConsole(dim * Brows + 2, 3);
		SetClr(15);
		cout << "Invalid placement" << endl;
		return;
	}
	HideSteps(bmap, dim);
	if (s.ri == 2 || s.ri == 5)
	{
		int i = s.ci % 10;
		ui++;
		BeforeDrop(CPW, CPB, u, ui, dim);
		UnPromotion(CPW, CPB, i, turn);
		if (turn == white)
		{
			B[d.ri][d.ci] = CPB[i];
			CPB[i] = '-';
		}
		else
		{
			B[d.ri][d.ci] = CPW[i];
			CPW[i] = '-';
		}
	}
	else if (s.ri == 3 || s.ri == 6)
	{
		int i = s.ci;
		i--;
		ui++;
		BeforeDrop(CPW, CPB, u, ui, dim);
		UnPromotion(CPW, CPB, i, turn);
		if (turn == white)
		{
			B[d.ri][d.ci] = CPB[i];
			CPB[i] = '-';
		}
		else
		{
			B[d.ri][d.ci] = CPW[i];
			CPW[i] = '-';
		}
	}
	return;
}
void UndoBox(int dim)
{
	int r = 0, c = Bcolumns * dim + 2;
	DrawBox(r, c, 6, 6, 15);

}
void PrintUndo(int dim)
{
	SetClr(11);
	gotoRowCol(3, Bcolumns * dim + 3);
	cout << "undo";
}
bool IsUndo(int dim, int r, int c)
{
	return r >= 0 && r < 6 && c >= Bcolumns * dim + 2 && c < Bcolumns* dim + 8;
}
void RemoveTrailSolo(int s, int r, int clr, int dim)
{
	if (s == 9)
	{
		s = 0;
		r += 1;
	}
	else if (s > 9)
	{
		r += 1;
		s = (s % 10) + 1;
	}
	DrawBox(r * Brows, dim * Bcolumns + 10 + (s * Bcolumns), Brows, Bcolumns, clr);
}
void SaveUndo(char** B, char* CPW, char* CPB, int dim, position d, position s, undo* u, int ui)
{
	u[ui].us.ri = s.ri;
	u[ui].us.ci = s.ci;
	u[ui].ud.ri = d.ri;
	u[ui].ud.ci = d.ci;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			u[ui].U[i][j] = B[i][j];
		}
	}
	for (int i = 0;i < dim * 2 + 1;i++)
	{
		u[ui].ucpw[i] = CPW[i];
		u[ui].ucpb[i] = CPB[i];
	}
}
void UndoMove(char**& B, int dim, char* CPW, char* CPB, undo* u, int ui)
{
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			B[i][j] = u[ui].U[i][j];
		}
	}
	if (u[ui].chb == true)
	{
		int r = 5;
		CPB[u[ui].i] = '-';
		RemoveTrailSolo(u[ui].i, r, 7, dim);
	}
	else if (u[ui].chw == true)
	{
		int r = 2;
		CPW[u[ui].i] = '-';
		RemoveTrailSolo(u[ui].i, r, 8, dim);
	}
	else if (u[ui].chw == false && u[ui].chb == false)
	{
		for (int i = 0;i < dim * 2 + 1;i++)
		{
			CPW[i] = u[ui].ucpw[i];
			CPB[i] = u[ui].ucpb[i];
		}
	}
}
void Promotion(char** B, position p)
{
	switch (B[p.ri][p.ci])
	{

	case'P':
		B[p.ri][p.ci] = 'Q';
		break;

	case'L':
		B[p.ri][p.ci] = 'W';
		break;

	case'S':
		B[p.ri][p.ci] = 'E';
		break;

	case'N':
		B[p.ri][p.ci] = 'T';
		break;
	case'B':
		B[p.ri][p.ci] = 'X';
		break;
	case'R':
		B[p.ri][p.ci] = 'Y';
		break;



	case'p':
		B[p.ri][p.ci] = 'q';
		break;
	case'l':
		B[p.ri][p.ci] = 'w';
		break;
	case's':
		B[p.ri][p.ci] = 'e';
		break;
	case'n':
		B[p.ri][p.ci] = 't';
		break;
	case'b':
		B[p.ri][p.ci] = 'x';
		break;
	case'r':
		B[p.ri][p.ci] = 'y';



	}
}
void PromoBox(int dim)
{
	int r = (dim - 1) * Brows, c = Bcolumns * dim + 2;
	DrawBox(r, c, 7, 7, 15);
}
void PrintPromo(int dim)
{
	SetClr(10);
	gotoRowCol(((dim - 1) * Brows) + 3, Bcolumns * dim + 3);
	cout << "promo";
}
bool UnPromoPiece(char** B, position s)
{
	switch (B[s.ri][s.ci])
	{
	case'p':
	case'P':
	case'B':
	case'b':
	case'R':
	case'r':
	case'L':
	case'l':
	case'N':
	case'n':
	case'S':
	case's':
	case'G':
	case'g':
	case'K':
	case'k':
		return true;
		break;
	}
	return false;
}
bool IsPromo(int r, int c, int dim)
{
	return r >= (dim - 1) * Brows && r <= (dim - 1) * Brows + 7 && c >= Bcolumns * dim + 2 && c <= Bcolumns * dim + 9;
}
void AutoPromo(int turn, char** B, position d)
{
	if (turn == white)
	{
		if (B[d.ri][d.ci] == 'P' || B[d.ri][d.ci] == 'L')
		{
			if (IsPOnTheLastRow(turn, d))
				Promotion(B, d);
			return;
		}
		else if (B[d.ri][d.ci] == 'N')
		{
			if (IsKnightOnTheLast2Rows(turn, d))
				Promotion(B, d);
			return;
		}
		return;
	}
	else
	{
		if (B[d.ri][d.ci] == 'p' || B[d.ri][d.ci] == 'l')
		{
			if (IsPOnTheLastRow(turn, d))
				Promotion(B, d);
			return;
		}
		else if (B[d.ri][d.ci] == 'n')
		{
			if (IsKnightOnTheLast2Rows(turn, d))
				Promotion(B, d);
			return;
		}
		return;
	}
}
bool IsLegalPromo(int turn, int dim, char** B, position d, position s)
{

	if (turn == white)
	{
		if (d.ri >= 0 && d.ri <= 2 && UnPromoPiece(B, s))
			return true;
		return false;
	}
	else
	{
		if (d.ri >= 6 && d.ri <= 8 && UnPromoPiece(B, s))
			return true;
		return false;
	}

}
void CheckPromo(int turn, int dim, char** B, position d, position s)
{
	position d1;
	if (turn == white)
	{
		if (IsLegalPromo(turn, dim, B, d, s))
		{
			SetClr(15);
			RemoveRowsOnConsole(dim * Brows + 2, 4);
			cout << "Do you want to promote the piece?" << endl;
			cout << "Select promo to promote or click anywhere to skip";
			getRowColbyLeftClick(d1.ri, d1.ci);
			if (IsPromo(d1.ri, d1.ci, dim))
				Promotion(B, s);
		}
	}
	else
	{

		if (IsLegalPromo(turn, dim, B, d, s))
		{
			SetClr(15);
			RemoveRowsOnConsole(dim * Brows + 2, 4);
			cout << "Do you want to promote the piece?" << endl;
			cout << "Select promo to promote or click anywhere to skip";
			getRowColbyLeftClick(d1.ri, d1.ci);
			if (IsPromo(d1.ri, d1.ci, dim))
				Promotion(B, s);
		}
	}
}
bool IsCapture(char** B, position d)
{
	return  B[d.ri][d.ci] != '-';
}
void DrawCaptureBox(int dim, string PNames[])
{
	gotoRowCol(2 * Brows - 2, 10 * Bcolumns);
	cout << PNames[1] << "'s captured pieces";
	for (int i = 2;i < 4;i++)
	{
		for (int j = 10;j < 19;j++)
		{
			DrawBox(i * Brows, j * Bcolumns, Brows, Bcolumns, 8);
		}
	}
	SetClr(15);
	gotoRowCol(5 * Brows - 2, 10 * Bcolumns);
	cout << PNames[0] << "'s captured pieces";
	for (int i = 5;i < 7;i++)
	{
		for (int j = 10;j < 19;j++)
		{
			DrawBox(i * Brows, j * Bcolumns, Brows, Bcolumns, 7);
		}
	}
}
int CapturePieces(char* CPW, char* CPB, char** B, position d, int turn, int dim)
{
	char store;
	if (turn == white)
	{
		for (int i = 0;i <= dim * 2;i++)
		{
			if (CPB[i] == '-')
			{
				store = B[d.ri][d.ci];
				Capitalize(store);
				CPB[i] = store;
				return i;
			}
		}
	}
	else
	{
		for (int i = 0;i <= dim * 2;i++)
		{
			if (CPW[i] == '-')
			{
				store = B[d.ri][d.ci];
				DeCapitalize(store);
				CPW[i] = store;
				return i;
			}
		}
	}
}
void ShowCapturedPieces(char* CPW, char* CPB, int dim)
{
	int count = 0;
	int wr = 25, wc = dim * Bcolumns + 15;
	int br = 55, bc = dim * Bcolumns + 15;
	for (int i = 0;i <= dim * 2;i++, wc += 10, count++)
	{
		SetClr(15);
		if (count == 9)
		{
			wc = dim * Bcolumns + 15;
			wr += 10;
		}
		switch (CPW[i])
		{
		case 'b':
			BishopSym2(wr, wc, 6);
			break;
		case 'p':
			PawnSym2(wr, wc, 6);
			break;
		case'r':
			RookSym2(wr, wc, 6);
			break;
		case'l':
			LanceSym2(wr, wc, 6);
			break;
		case'n':
			KnightSym2(wr, wc, 6);
			break;
		case'g':
			GoldGeneralSym2(wr, wc, 6);
			break;
		case's':
			SilverGeneralSym2(wr, wc, 6);
			break;
		case'k':
			KingSym2(wr, wc, 6);
			break;
		case'q':
			PromoPawnSymW(wr, wc, 6);
			break;
		case'w':
			PromoLanceSymW(wr, wc, 6);
			break;
		case'e':
			PromoSilverGeneralW(wr, wc, 6);
			break;
		case't':
			PromoKnightSymW(wr, wc, 6);
			break;
		case'x':
			PromoBishopSymW(wr, wc, 6);
			break;
		case'y':
			PromoRookSymW(wr, wc, 6);
			break;
		}

	}
	count = 0;
	for (int i = 0;i <= dim * 2;i++, count++, bc += 10)
	{
		SetClr(0);
		if (count == 9)
		{
			bc = dim * Bcolumns + 15;
			br += 10;
		}
		switch (CPB[i])
		{
		case 'B':
			BishopSym2(br, bc, 6);
			break;
		case 'P':
			PawnSym2(br, bc, 6);
			break;
		case'R':
			RookSym2(br, bc, 6);
			break;
		case'L':
			LanceSym2(br, bc, 6);
			break;
		case'N':
			KnightSym2(br, bc, 6);
			break;
		case'G':
			GoldGeneralSym2(br, bc, 6);
			break;
		case'S':
			SilverGeneralSym2(br, bc, 6);
			break;
		case'K':
			KingSym2(br, bc, 6);
			break;
		case'Q':
			PromoPawnSymB(br, bc, 6);
			break;
		case'W':
			PromoLanceSymB(br, bc, 6);
			break;
		case'E':
			PromoSilverGeneralB(br, bc, 6);
			break;
		case'T':
			PromoKnightSymB(br, bc, 6);
			break;
		case'X':
			PromoBishopSymB(br, bc, 6);
			break;
		case'Y':
			PromoRookSymB(br, bc, 6);
			break;
		}

	}
}
void PrintReplay(int dim)
{
	SetClr(14);
	gotoRowCol(Brows + (6 / 2), Bcolumns * dim + 4);
	cout << "Re" << endl;
}
void DrawReplayBox(int dim)
{
	int r = Brows, c = Bcolumns * dim + 2;
	DrawBox(r, c, 6, 6, 15);
}
bool IsReplay(int dim, int r, int c)
{
	return r >= Brows && r < Brows + 6 && c >= Bcolumns * dim + 2 && c < Bcolumns* dim + 8;
}
void Replay(undo* u, int ui, int dim,string PNames[])
{
	system("cls");
	char** R;
	R = new char* [dim];
	for (int i = 0;i < dim;i++)
	{
		R[i] = new char[dim];
	}
	DrawBoard(dim, Brows, Bcolumns);
	DrawCaptureBox(dim, PNames);
	for (int i1 = 0;i1 <= ui;i1++)
	{
		for (int i = 0;i < dim;i++)
		{
			for (int j = 0;j < dim;j++)
			{
				R[i][j] = u[i1].U[i][j];
			}
		}
		PrintBoard(R, dim);
		RemoveTrail(u[i1].us, u[i1].ud);
		Delay(0.01);
	}
}
void Click(position& s, position& d, int dim, char** B, int turn, bool**& bmap, char** BC, string PNames[], int& r, int& c, char* CPW, char* CPB, undo* u, int& ui)
{
	int clrr = Brows * dim + 2;
	RemoveRowsOnConsole(clrr, 4);
	int count = 0;
	do {
		if (count != 0)
		{
			RemoveRowsOnConsole(clrr, 4);
			cout << "Check!! select again:" << endl;
		}
		count = 0;
		do
		{
			if (count != 0)
			{
				HideSteps(bmap, dim);
				RemoveRowsOnConsole(clrr, 4);
				SetClr(15);
				cout << "Invalid placement select again from start:" << endl;
			}
			count = 0;
			do
			{
				if (count != 0)
				{
					RemoveRowsOnConsole(clrr, 4);
					cout << "Invalid selection enter again:" << endl;
				}
				cout << "Select a piece : " << endl;
				getRowColbyLeftClick(r, c);
				if (IsUndo(dim, r, c) && ui >= 0)
				{
					RemoveTrail(u[ui].us, u[ui].ud);
					UndoMove(B, dim, CPW, CPB, u, ui);
					ui--;
					return;
				}
				if (IsReplay(dim, r, c))
					Replay(u, ui, dim,PNames);
				s.ri = r / Brows;
				s.ci = c / Bcolumns;
				count++;
				if (IsDrop(dim, s))
				{
					DropPiece(B, BC, CPW, CPB, s, d, turn, dim, count,u,ui);
					if (IsValidDropD(B, d, s, BC, turn, dim, CPW, CPB))
					{
						u[ui].chb = false;
						u[ui].chw = false;
						return;
					}
				}
			} while (!InBoundS(s, dim) || !IsValidMoveS(s, B, turn));
			cout << char(s.ci + 'A') << s.ri << " is selected" << endl;
			bmap = BoolMap(B, s, dim, turn);
			ShowSteps(bmap, dim);
			getRowColbyLeftClick(r, c);
			if (IsUndo(dim, r, c) && ui >= 0)
			{
				HideSteps(bmap, dim);
				UndoMove(B, dim, CPW, CPB, u, ui);
				RemoveTrail(u[ui].us, u[ui].ud);
				ui--;
				return;
			}
			d.ri = r / Brows;
			d.ci = c / Bcolumns;
			count++;
		} while (!InBoundD(d, dim) || !IsValidMoveD(d, B, turn) || !bmap[d.ri][d.ci]);
		HideSteps(bmap, dim);
		PlaceMoveCopy(s, d, BC, B, dim);
		count++;
	} while (IsSelfCheck(turn, BC, dim));
	CheckPromo(turn, dim, B, d, s);
	ui++;
	u[ui].chw = false;
	u[ui].chb = false;
	RemoveRowsOnConsole(clrr, 4);
}
void SelectPosition(position& s, position& d, int dim, char** B, int turn)
{
	int count = 0;
	do
	{
		if (count != 0)
			cout << "Invalid placement enter again from the start:" << endl;
		count = 0;
		do
		{
			if (count != 0)
				cout << "Invalid selection enter again:" << endl;
			char chs;
			cout << "Select a piece from(A1:I9):" << endl;
			cout << "Enter column(A:I):" << endl;
			cin >> chs;
			Capitalize(chs);
			cout << "Enter row(1:9):" << endl;
			cin >> s.ri;
			s.ri--;
			s.ci = int(chs - 'A');
			count++;
		} while (!InBoundS(s, dim) || !IsValidMoveS(s, B, turn));
		char che;
		cout << "Select where to place (A1:I9):" << endl;
		cout << "Enter column(A:I):" << endl;
		cin >> che;
		Capitalize(che);
		cout << "Enter row(1:9):" << endl;
		cin >> d.ri;

		d.ri--;
		d.ci = int(che - 'A');
	} while (!InBoundD(d, dim) || !IsValidMoveD(d, B, turn));


}
void PlaceMove(position s, position d, char**& B)
{

	B[d.ri][d.ci] = B[s.ri][s.ci];
	B[s.ri][s.ci] = '-';
}
void Save(char** B, int dim, string PNames[], int turn, char* CPW, char* CPB)
{
	ofstream cout1("save.txt");
	for (int i = 0;i < 2;i++)
	{
		cout1 << PNames[i] << endl;
	}
	cout1 << turn << endl;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			cout1 << B[i][j] << " ";
		}
		cout1 << endl;
	}
	for (int i = 0;i <= dim * 2;i++)
	{
		cout1 << CPW[i] << " ";
	}
	cout1 << endl;
	for (int i = 0;i <= dim * 2;i++)
	{
		cout1 << CPB[i] << " ";
	}
}
void AfterDrop(position& s, position d, int dim, char** B, int turn, undo* u, int ui)
{
	u[ui].ud.ri = d.ri;
	u[ui].ud.ci = d.ci;
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
		{
			u[ui].U[i][j] = B[i][j];
		}
	}
	u[ui].U[d.ri][d.ci] = '-';
	int clr;
	if (s.ri == 2 || s.ri == 5)
		s.ci = s.ci % 10;
	else if (s.ri == 3 || s.ri == 6)
		s.ci--;
	if (turn == white)
	{
		s.ri = 5;
		clr = 7;
	}
	else
	{
		s.ri = 2;
		clr = 8;
	}
	RemoveTrailSolo(s.ci, s.ri, clr, dim);
}
int main()
{
	undo* u;
	int dim, turn, r, c,ui = -1;
	char** B, ** BC, * CPW, * CPB;
	bool** bmap;
	position s, d;
	string PNames[100];
	init(B, turn, dim, PNames, BC, CPW, CPB, u);
	DrawBoard(dim, Brows, Bcolumns);
	PrintBoard(B, dim);
	UndoBox(dim);
	DrawCaptureBox(dim, PNames);
	PrintUndo(dim);
	ShowCapturedPieces(CPW, CPB, dim);
	PromoBox(dim);
	PrintPromo(dim);
	DrawReplayBox(dim);
	PrintReplay(dim);
	while (true)
	{
		TurnMsg(PNames, turn, dim);
		Click(s, d, dim, B, turn, bmap, BC, PNames, r, c, CPW, CPB, u, ui);
		if (!IsUndo(dim, r, c) && !IsDrop(dim, s))
		{
			if (IsCapture(B, d))
			{
				if (turn == white)
					u[ui].chb = true;
				else
					u[ui].chw = true;
				u[ui].i = CapturePieces(CPW, CPB, B, d, turn, dim);
			}
			SaveUndo(B, CPW, CPB, dim, d, s, u, ui);
			PlaceMove(s, d, B);
			AutoPromo(turn, B, d);
			RemoveTrail(s, d);
		}
		ShowCapturedPieces(CPW, CPB, dim);
		if (!IsUndo(dim,r,c) && IsDrop(dim, s))
			AfterDrop(s, d, dim, B, turn, u, ui);
		PrintBoard(B, dim);
		if (!IsDefend(turn, dim, B, BC))
		{
			if (IsCheckmate(turn, dim, B, BC))
			{
				gotoRowCol(dim * Brows + 2, 0);
				cout << "Checkmate!!!" << PNames[turn] << "'s wins" << endl;
				DeleteMemoryDouble(B, BC, bmap, dim);
				DeleteMemorySingle(CPW, CPB);
				exit(1);
			}
		}
		TurnCh(turn);
		Save(B, dim, PNames, turn, CPW, CPB);
	}

}
