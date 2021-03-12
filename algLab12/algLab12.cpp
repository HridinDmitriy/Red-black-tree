#include "pch.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include "RB_Tree.h"
using std::cout;
using std::cin;
using std::endl;

int main()
{
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << " 1 - Добавить пару ключ-значение\n";
	std::cout << " 2 - Удалить пару ключ-значение\n";
	std::cout << " 3 - Получить значение по ключу\n";
	std::cout << " 4 - Проверить, содержит ли дерево данный ключ\n";
	std::cout << " 5 - Получить количество узлов в дереве\n";
	std::cout << " 6 - Вывести пары ключ-значение на экран\n";
	std::cout << " 7 - Выход с программы\n";

	RB_Tree<int, int> rbt; 
	/*rbt.Add(20, 0);
	rbt.Add(40, 0);
	rbt.Add(32, 0);
	rbt.Add(28, 0);
	rbt.Add(30, 0);
	rbt.Add(11, 0);
	rbt.Add(5, 0);
	rbt.Add(35, 0);
	rbt.Add(7, 0);
	rbt.Add(3, 0);
	rbt.Add(6, 0);
	rbt.Add(50, 0);
	rbt.Add(34, 0);
	rbt.Add(25, 0);
	rbt.Add(45, 0);
	rbt.Add(84, 0);
	rbt.Add(1, 0);
	rbt.Add(85, 0);
	rbt.Add(43, 0);
	rbt.Add(47, 0);*/
/*	rbt.Delete(11);
	rbt.Delete(7);
	rbt.Delete(34);
	rbt.Delete(32);
	rbt.Delete(84);
	rbt.Delete(47);
	rbt.Delete(40);
	rbt.Delete(30);
	rbt.Delete(50);
	rbt.Delete(45);
	rbt.Delete(6);
	rbt.Delete(5);
	rbt.Delete(1);
	rbt.Delete(43);
	rbt.Delete(35);
	rbt.Delete(3);
	rbt.Delete(25);
	rbt.Delete(20);
	rbt.Delete(85);
	rbt.Delete(28);*/

	int key;
	int value;

	while (true)
	{
		std::cout << "\nВыберите пункт...\n";
		switch (_getch())
		{
		case 49:
			std::cout << "\nПункт 1\n\nВведите пару ключ-значение: ";
			cin >> key >> value;
			rbt.Add(key, value);
			std::cout << "\nУзел добавлен!\n";
			break;
		case 50:
			std::cout << "\nПункт 2\n\nВведите ключ: ";
			cin >> key;
			rbt.Delete(key);
			std::cout << "\nУзел удален!\n";
			break;
		case 51:
			std::cout << "\nПункт 3\n\nВведите ключ: ";
			cin >> key;
			try
			{
				value = rbt.TryGetValue(key);
				cout << "\nЗначение, хранящееся по ключу " << key << ": " << value << endl;
			}
			catch (KeyNotFoundException)
			{
				cout << "\nНе удалось найти указанный ключ!\n";
			}
			break;
		case 52:
			std::cout << "\nПункт 4\n\nВведите ключ: ";
			cin >> key;
			if (rbt.ContainsKey(key))
			{
				cout << "\nДерево содержит указанный ключ!\n";
			}
			else
			{
				cout << "\nДерево не содержит указанного ключа!\n";
			}
			break;
		case 53:
			std::cout << "\nПункт 5\n\n";
			std::cout << "\nКоличество узлов в дереве: " << rbt.get_size() << endl;
			break;
		case 54:
			std::cout << "\nПункт 6\n\n";
			rbt.print();
			std::cout << "\nГотово!\n";
			break;
		case 55:
			exit(0);
			break;
		}
	}
}