#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
/****************************Прототипы функций****************************/
short search(FILE* IN,FILE* OUT_C,unsigned char Str[256],unsigned char Word[20]);//функция поиска повторяющихся слов
short coder(FILE* IN, FILE* OUT_C, FILE* OUT_S, unsigned char Str[256], unsigned char Word[20],short Code);//функция кодирования слов и сжатия текста
short decoder(FILE* OUT_C, FILE* OUT_S, unsigned char Str[256], unsigned char Word[20], short Code);//функция восстановления текста
/****************************Основная программа****************************/
int main()
{
	setlocale(LC_ALL, "RUSSIAN");
	FILE* IN;//указатель для входного файла
	FILE* OUT_C;//указатель для файла с таблицей
	FILE* OUT_S;//указатель для файла с сжатым текстом
	unsigned char Word[20];//массив для сохранения слова
	unsigned char Str[256];//массив для сохранения строки
	short Code;//переменная, содержащая значения кода
	short Error;//код ошибки
	IN = fopen("Input.txt", "rt");//входной файл
	if (IN == NULL)
	{
		printf("Ошибка! Не удалось открыть входной файл\nНажмите Enter");
		getchar();
		return 1;
	}
	OUT_C = fopen("Table.txt", "wt+");//файл для таблицы кодов
	if (OUT_C == NULL)
	{
		printf("Ошибка! Не удалось открыть файл для таблицы кодов\nНажмите Enter");
		getchar();
		fclose(IN);
		return 2;
	}
	OUT_S = fopen("Output.txt", "wt+");//файл для сжатого текста
	if (OUT_S == NULL)
	{
		printf("Ошибка! Не удалось открыть файл для сжатого текста\nНажмите Enter");
		getchar();
		fclose(IN);
		fclose(OUT_C);
		return 3;
	}
	Code = search(IN, OUT_C, Str, Word);//поиск повторяющихся слов
	if (Code > 0)
	{
		fclose(IN);
		fclose(OUT_C);
		fclose(OUT_S);
		switch (Code)
		{
		case 1:
		{
			printf("Ошибка! Не удалось открыть вспомогательный файл\nНажмите Enter");;
			getchar();
			return 4;
		}
		case 2:
		{
			printf("Произошла файловая ошибка в входном файле\nНажмите Enter");
			getchar();
			return 5;
		}
		case 3:
		{
			printf("Входной файл пуст\nНажмите Enter");
			getchar();
			return 6;
		}
		case 4:
		{
			printf("Ошибка! Не удалось установить указатель положения на заданную позицию в файле для таблицы\nНажмите Enter");
			getchar();
			return 7;
		}
		case 5:
		{
			printf("Ошибка! Не удалось установить указатель положения на заданную позицию в вспомогательном файле\nНажмите Enter");
			getchar();
			return 8;
		}
		case 6:
		{
			printf("Ошибка! Не удалось вернуть считанный символ в входной файл\nНажмите Enter");
			getchar();
			return 9;
		}
		case 7:
		{
			printf("Ошибка записи в вспомогательный файл\nНажмите Enter");
			getchar();
			return 10;
		}
		case 8:
		{
			printf("Ошибка записи в файл с таблицей\nНажмите Enter");
			getchar();
			return 11;
		}
		}
	}
	if (fseek(OUT_C, 22, SEEK_SET) != 0)
	{
		printf("Ошибка! Не удалось установить указатель положения на заданную позицию в файле c таблицей\nНажмите Enter");
		getchar();
		fclose(IN);
		fclose(OUT_C);
		fclose(OUT_S);
		return 12;
	}
	//если в файл с таблицей не было записи слов
	if (fgetc(OUT_C) == EOF)//т.к. fgetc возвращает значение символа в типе int, буква "я" будет обрабатываться под кодом 255 с подключенным русским языком
	{
		printf("Повторов нет\nНажмите Enter");
		getchar();
		fclose(IN);
		fclose(OUT_C);
		fclose(OUT_S);
		return 13;
	}
	Error=coder(IN, OUT_C, OUT_S, Str, Word, Code);//сжатие текста
	if (Error != 0)
	{
		fclose(IN);
		fclose(OUT_C);
		fclose(OUT_S);
		switch (Error)
		{
		case 1:
		{
			printf("Ошибка!Не удалось установить указатель положения на заданную позицию в файле с таблицей\nНажмите Enter");
			getchar();
			return 14;
		}
		case 2:
		{
			printf("Произошла файловая ошибка в входном файле\nНажмите Enter");
			getchar();
			return 15;
		}
		case 3:
		{
			printf("Ошибка записи в файл для сжатого текста\nНажмите Enter");
			getchar();
			return 16;
		}
		}
	}
	Error = decoder(OUT_C, OUT_S, Str, Word, Code);//восстановление текста
	if (Error != 0)
	{
		fclose(IN);
		fclose(OUT_C);
		fclose(OUT_S);
		switch (Error)
		{
		case 1:
		{
			printf("Ошибка! Не удалось открыть файл для восстановленного текста\nНажмите Enter");
			getchar();
			return 17;
		}
		case 2:
		{
			printf("Ошибка!Не удалось установить указатель положения на заданную позицию в файле для восстановленного текста\nНажмите Enter");
			getchar();
			return 18;
		}
		case 3:
		{
			printf("Ошибка!Не удалось установить указатель положения на заданную позицию в файле с таблицей\nНажмите Enter");
			getchar();
			return 19;
		}
		case 4:
		{
			printf("Произошла файловая ошибка в файле с сжатым текстом\nНажмите Enter");
			getchar();
			return 20;
		}
		case 5:
		{
			printf("Ошибка записи в файл для восстановленного текста\nНажмите Enter");
			getchar();
			return 21;
		}
		}
	}
	printf("Программа завершила свою работу\nНажмите Enter");
	getchar();
	fclose(IN);
	fclose(OUT_C);
	fclose(OUT_S);
	return 0;
}

short search(FILE* IN, FILE* OUT_C, unsigned char Str[256], unsigned char Word[20])
{
	FILE* Pos;
	Pos = fopen("Pos.txt", "wt+");//вспомогательный файл
	short lenghtword = 0;//длина слова
	short lengthstr = 0;//длина строки
	short count = 0;//счетчик
	short countstring = 0;//счетчик строк
	short countword = 1;//счетчик повторяющихся слов
	short countallword = 1;//счетчик всех слов
	short counttemp = 0;//счетчик просмотренных слов
	short flag = 0;//флаг
	short flags = 0;//флаг полного обхода текста
	//флаги проверок на совпадение
	short flaga = 0;
	short flagb = 0;
	short symbol;//переменная для возврата символа в поток
	unsigned char symbolc;//переменная для поиска символа в тексте
	short Code = -1;//переменная, содержащая значения кода
	short bw = 0;//координата начала слова
	short ew_temp = 0;//координата конца сохраненного слова
	short ew = 0;//координата конца слова
	//вспомогательные переменные
	short i = 0;
	short j = 0;
	short k = 0;
	fprintf(OUT_C, "%s", "Слово\t\t\tКод\t\t\tСимвол\n");
	if (Pos == NULL)
		return 1;
	while (fgets(Str, 256, IN) != NULL)
	{
		//Проверка на выход за пределы числа кодов
		if (abs(Code) == 255)
		{
			printf("Все возможные коды были заполнены. Невозможно закодировать оставшиеся слова.\nПрограмма продолжит работу\nНажмите Enter");
			getchar();
			break;
		}
		countstring++;//Подсчет строк
		lengthstr = strlen(Str);
		if (countstring == k)//Ставим координату предыдущео слова,если дошли до нужной строки
		{
			i = ew_temp + 1;
			counttemp++;
		}
		while (flag == 0)//флаг переключается на 1 после того,как встретится новое слово
		{
			while ((isspace(Str[i])) || (ispunct(Str[i])))//пропуск разделителей
				i++;
			if ((Str[i] == '\0'))
				break;
			bw = i;
			ew = bw;
			while (!(isspace(Str[i])) && !(ispunct(Str[i])) && (i < lengthstr))//цикл по слову
			{
				ew++;
				i++;
			}
			ew--;
			lenghtword = ew - bw + 1;//определение длины слова
			for (j = 0; j < lenghtword; j++)//сохранение этого слова в массив Word
				Word[j] = Str[bw + j];
			Word[j] = '\0';
			if (fseek(OUT_C, 22, SEEK_SET) != 0)
			{
				fclose(Pos);
				remove("Pos.txt");//удаление файла
				return 4;
			}
			//проверка-было ли рассмотрено слово ранее(было ли оно записано как повторяющееся,либо как с 2мя и менее повторами)
			if (fgetc(OUT_C) != EOF)
			{
				j = 0;
				if (fseek(OUT_C, 21, SEEK_SET) != 0)
				{
					fclose(Pos);
					remove("Pos.txt");//удаление файла
					return 4;
				}
				symbolc = fgetc(OUT_C);
				while (!feof(OUT_C))
				{
					while ((symbolc != '\n') && (!feof(OUT_C)))
					{
						symbolc = fgetc(OUT_C);
					}
					symbolc = fgetc(OUT_C);
					while ((symbolc != '\t') && (!feof(OUT_C)))
					{
						if (Word[j] == symbolc)
							count++;
						j++;
						symbolc = fgetc(OUT_C);
					}
					if ((count == strlen(Word)) && (j == strlen(Word)))
					{
						flaga = 0;
						count = 0;
						break;
					}
					else
						flaga = 1;
					j = 0;
					count = 0;
				}
			}
			else
				flaga = 1;
			if (fseek(Pos, 0, SEEK_SET) != 0)
			{
				fclose(Pos);
				remove("Pos.txt");//удаление файла
				return 5;
			}
			if (fgetc(Pos) != EOF)
			{
				j = 0;
				if (fseek(Pos, 0, SEEK_SET) != 0)
				{
					fclose(Pos);
					remove("Pos.txt");//удаление файла
					return 5;
				}
				while (!feof(Pos))
				{
					symbolc = fgetc(Pos);
					while ((symbolc != '\n') && (!feof(Pos)))
					{
						if (Word[j] == symbolc)
							count++;
						j++;
						symbolc = fgetc(Pos);
					}
					if ((count == strlen(Word)) && (j == strlen(Word)))
					{
						flagb = 0;
						count = 0;
						break;
					}
					else
						flagb = 1;
					j = 0;
					count = 0;
				}
			}
			else
				flagb = 1;
			if (flaga && flagb)//если слово раньше не встречалось, флаги переключаются в 1, координаты слова и номер строки запоминаются
			{
				flag = 1;
				k = countstring;
				ew_temp = ew;
			}
			else//иначе очистка строки и поиск слова
			{
				memset(Word, 0, 20);
			}
		}
		if (fseek(Pos, 0, SEEK_END) != 0)
		{
			fclose(Pos);
			remove("Pos.txt");//удаление файла
			return 5;
		}
		if (fseek(OUT_C, 0, SEEK_END) != 0)
		{
			fclose(Pos);
			remove("Pos.txt");//удаление файла
			return 4;
		}
		count = 0;
		while (i < lengthstr)//поиск повторяющихся слов
		{
			while ((isspace(Str[i])) || (ispunct(Str[i])))//пропуск разделителей
				i++;
			if ((Str[i] == '\0'))
				break;
			bw = i;
			ew = bw;
			while (!(isspace(Str[i])) && !(ispunct(Str[i])) && (i < lengthstr))//цикл по слову
			{
				ew++;
				i++;
			}
			ew--;
			if (flags == 0)
				countallword++;
			lenghtword = ew - bw + 1;
			if (lenghtword == strlen(Word))//если слова совпали,инкремент счетчика
			{
				for (j = 0; j < lenghtword; j++)
					if (Str[bw + j] == Word[j])
						count++;
				if (count == lenghtword)
					countword++;
				count = 0;
			}

			i++;
		}
		i = 0;
		//проверка перед перемещением каретки в начало входного файла
		if (((symbol = fgetc(IN)) == EOF) && ((countword >= 3) || (counttemp < countallword)))
		{
			rewind(IN);//возврат каретки
			if (countword >= 3)
			{
				j = 0;
				countword = abs(Code);
				while (countword!= 0)//определение кол-ва разрядов в числе Code
				{
					countword = countword / 10;
					j++;
				}
				if (strlen(Word) < 8)
				{
					if (fprintf(OUT_C, "%s\t\t\t%d\t\t\t%c%c\n", Word, Code, Code, 159) != strlen(Word) + j + 10)
					{
						fclose(Pos);
						remove("Pos.txt");//удаление файла
						return 8;
					}
				}
				else
				{
					if (fprintf(OUT_C, "%s\t\t%d\t\t\t%c%c\n", Word, Code, Code, 159) != strlen(Word) + j + 9)
					{
						fclose(Pos);
						remove("Pos.txt");//удаление файла
						return 8;
					}
				}
				Code--;
			}
			else
				if (Word[0] != '\0')
					if (fprintf(Pos, "%s\n", Word) != strlen(Word)+1)
					{
						fclose(Pos);
						remove("Pos.txt");//удаление файла
						return 7;
					}
			countword = 1;
			memset(Word, 0, 20);
			countstring = 0;
			flag = 0;
			flags = 1;
			j = 0;
		}
		else
			if ((ungetc(symbol, IN) == -1)&&(counttemp<countallword))
			{
				fclose(Pos);
				remove("Pos.txt");//удаление файла
				return 6;
			}
		memset(Str, 0, 256);
	}
	
	if (ferror(IN) == 1)
	{
		fclose(Pos);
		remove("Pos.txt");//удаление файла
		return 2;
	}
	if (feof(IN) && (flags == 0))
	{
		fclose(Pos);
		remove("Pos.txt");//удаление файла
		return 3;
	}
	fclose(Pos);
	remove ("Pos.txt");//удаление файла
	return Code;
}

short coder(FILE* IN, FILE* OUT_C, FILE* OUT_S, unsigned char Str[256], unsigned char Word[20], short Code)
{
	short lenghtword = 0;//длина слова
	short lengthstr = 0;//длина строки
	short count = 0;//счетчик
	short flag = 0;//флаг
	unsigned char symbolc;//переменная для поиска символа в тексте
	short CodeTemp=abs(Code)-1;//переменная для сохранения текущего кода
	short bw = 0;//координата начала слова
	short ew = 0;//координата конца слова
	//вспомогательные переменные
	short i = 0;
	short j = 0;
	Code = 0;
	rewind(IN);
	fprintf(OUT_S, "Сжатый текст:\n");
	if (fseek(OUT_C, 21, SEEK_SET) != 0)
		return 1;
	symbolc = fgetc(OUT_C);
	while (fgets(Str, 256, IN) != NULL)
	{
		lengthstr = strlen(Str);
		while (CodeTemp != 0)//пока не буду рассмотрены все коды
		{
			if (flag == 0)//выбор слова по текущему коду
			{
				memset(Word, 0, 20);
				j = 0;
				while ((symbolc != '\n') && (!feof(OUT_C)))
				{
					symbolc = fgetc(OUT_C);
				}
				symbolc = fgetc(OUT_C);
				while ((symbolc != '\t') && (!feof(OUT_C)))
				{
					Word[j] = symbolc;
					j++;
					symbolc = fgetc(OUT_C);
				}
				Code--;//выбор следующего слова
				CodeTemp--;
				flag = 1;
			}
			while (i < lengthstr)
			{
				while ((isspace(Str[i])) || (ispunct(Str[i])))//пропуск разделителей
					i++;
				if ((Str[i] == '\0'))
					break;
				bw = i;
				ew = bw;
				while (!(isspace(Str[i])) && !(ispunct(Str[i])) && (i < lengthstr))//цикл по слову
				{
					ew++;
					i++;
				}
				ew--;
				lenghtword = ew - bw + 1;
				if (lenghtword == strlen(Word))//поиск совпадающих слов, и их замена на коды
				{
					for (j = 0; j < lenghtword; j++)
						if (Str[bw + j] == Word[j])
							count++;
					if (count == lenghtword)
					{
						for (j = bw + 1; j < ew + 1; j++)
							Str[j] = ' ';//удаление слова
						Str[bw] = Code;//присвоение кода
						for (j = lengthstr; j >= bw + 1; j--)
							Str[j] = Str[j - 1];//расширение строки под вспомогательный символ
						Str[bw + 1] = 159;//вставка вспомогательного символа
						for (short q = 0; q < lenghtword - 1; q++)//сжатие строки
							for (j = bw + 2; j < lengthstr + 1 - q; j++)
							{
								Str[j] = Str[j + 1];
							}
						i = i - (lenghtword - 1);
						lengthstr = strlen(Str);
					}
				}
				count = 0;
			}
			i = 0;
			flag = 0;
		}
		CodeTemp = abs(Code);
		Code = 0;
		if (fseek(OUT_C, 21, SEEK_SET) != 0)
			return 1;
	
		if (fprintf(OUT_S, "%s", Str) != strlen(Str))
			return 3;
		memset(Str, 0, 256);
	}
	if (ferror(IN) == 1)
		return 2;
	return 0;
}

short decoder(FILE* OUT_C, FILE* OUT_S, unsigned char Str[256], unsigned char Word[20], short Code)
{
	FILE* OUT_T;
	short lenghtword = 0;//длина слова
	short lengthstr = 0;//длина строки
	short flag = 0;//флаг
	unsigned char symbolc;//переменная для поиска символа в тексте
	short CodeTemp;//переменная для сохранения текущего кода
	short bw = 0;//координата начала слова
	short ew = 0;//координата конца слова
	//вспомогательные переменные
	short i = 0;
	short j = 0;
	OUT_T = fopen("OutputText.txt", "wt+");//файл для восстановленного текста
	if (OUT_T == NULL)
		return 1;
	CodeTemp = abs(Code);
	Code = 0;
	fprintf(OUT_T, "Восстановленный текст:\n");
	if (fseek(OUT_S, 15, SEEK_SET) != 0)
	{
		fclose(OUT_T);
		return 2;
	}
	if (fseek(OUT_C, 21, SEEK_SET) != 0)
	{
		fclose(OUT_T);
		return 3;
	}
	symbolc = fgetc(OUT_C);
	while (fgets(Str, 256, OUT_S) != NULL)
	{
		lengthstr = strlen(Str);
		while (CodeTemp != 0)//пока не будут обработаны все коды
		{
			if (flag == 0)//выбор слова по текущему коду
			{
				memset(Word, 0, 20);
				j = 0;
				while ((symbolc != '\n') && (!feof(OUT_C)))
				{
					symbolc = fgetc(OUT_C);
				}
				symbolc = fgetc(OUT_C);
				while ((symbolc != '\t') && (!feof(OUT_C)))
				{
					Word[j] = symbolc;
					j++;
					symbolc = fgetc(OUT_C);
				}
				Code--;
				CodeTemp--;
				flag = 1;
			}
			while (i < lengthstr)
			{
				while ((isspace(Str[i])) || (ispunct(Str[i])))//пропуск разделителей
					i++;
				if ((Str[i] == '\0'))
					break;
				bw = i;
				ew = bw;
				while (!(isspace(Str[i])) && !(ispunct(Str[i])) && (i < lengthstr))//цикл по слову
				{
					ew++;
					i++;
				}
				ew--;
				lenghtword = strlen(Word);
				symbolc = Code;
				if ((Str[bw] == symbolc) && (Str[bw + 1] == 159))//поиск кода и вспомогательного символа
				{
					//растягивание строки для вставки слова, замена кода на слово
					if (strlen(Word) == 1)//если была закодирована 1 буква
					{
						Str[bw + 1] = ' ';
						for (j = bw + 1; j < lengthstr; j++)
						{
							Str[j] = Str[j + 1];
						}
						i = i + lenghtword - 2;
					}
					else
					{
						for (short q = 0; q < lenghtword - 2; q++)
							for (j = lengthstr + q; j > bw + 2; j--)
							{
								Str[j] = Str[j - 1];
							}
						i = i + (lenghtword - 2);
					}
					lengthstr = strlen(Str);
					for (j = 0; j < lenghtword; j++)
					{
						Str[bw + j] = Word[j];
					}
				}
			}
			i = 0;
			flag = 0;
		}
		CodeTemp = abs(Code);
		Code = 0;
		if (fseek(OUT_C, 21, SEEK_SET) != 0)
		{
			fclose(OUT_T);
			return 3;
		}
		if (fprintf(OUT_T, "%s", Str) != strlen(Str))
		{
			fclose(OUT_T);
			return 5;
		}
		memset(Str, 0, 256);
	}
	if (ferror(OUT_S) == 1)
	{
		fclose(OUT_T);
		return 4;
	}
	fclose(OUT_T);
	return 0;
}