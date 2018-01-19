#include <iostream>
#include <fstream>
#include <vector>
#include <iterator> 
#include <string> 
#include <sstream>
#include <random>
#include <math.h>

template <typename T>
std::string toString(T val)//преобразование int к типу string - это хитрый выверт С++,так как тут нет
						   // человеческих функций для работы со строками и их приведения
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

template<typename T>
T fromString(const std::string& s)
{
	std::istringstream iss(s);
	T res;
	iss >> res;
	return res;
}

void preparation_images(std::vector<std::vector <std::string>> &images, std::string &suffix) {
	char buff[50];
	//сперва думал bmp дёргать, а не txt файл, но потратив 3 часа на изучение структуры

	//bmp идея была отложена до лучших времён, по этому каждая картинка будет просто вектором строк
	for (int i = 0; i <= 9; ++i) {//считаем все изображения, что есть
		std::vector <std::string> image = {};
		std::ifstream cock;
		std::string prefix = toString(i);
		std::string file_name = prefix + suffix;
		cock.open(file_name);
		int j = 0;
		while (cock.getline(buff, 50)) {
			image.push_back(std::string(buff));
			j++;
		}
		images.push_back(image);
		cock.close();
	}
}

double comape_two_images_1v(std::vector <std::string> &first, std::vector <std::string> &second){
	int summator=0;
	for (size_t i = 0; i < first.size(); ++i) {
		for (size_t j = 0; j < first[i].size(); ++j) {
			summator += (first[i][j] - second[i][j]) ^ 2;
		}
	}
	return sqrt(summator);
}

double comape_two_images_2v(std::vector <std::string> &first, std::vector <std::string> &second) {
	int summator = 0;
	for (size_t i = 0; i < first.size(); ++i) {
		for (size_t j = 0; j < first[i].size(); ++j) {
			summator += abs(first[i][j] - second[i][j]);
		}
	}
	return summator;
}

double comape_two_images_3v(std::vector <std::string> &first, std::vector <std::string> &second) {
	int summator = 0;
	for (size_t i = 0; i < first.size(); ++i) {
		for (size_t j = 0; j < first[i].size(); ++j) {
			int x = first[i][j] - second[i][j];
			summator += (x > 0) ? 1 : ((x < 0) ? -1 : 0);
		}
	}
	return summator;
}

void create_noisy_digits(std::vector<std::vector <std::string>> images, int noise) {
	for (int i = 0; i <= 9; ++i) {//считаем все изображения, что есть
		std::ofstream noisy_cock;
		std::string prefix = toString(i);
		std::string file_name = prefix + "noisy\.txt";
		noisy_cock.open(file_name);
		std::random_device rd;
		std::mt19937 gen(rd());
		int new_digit = 0;
		std::uniform_int_distribution<int> changer(1, 100);
		for(int j =0;j<images[i].size();j++){
			for (int k = 0; k < images[i][j].size(); k++) {
				if (changer(gen) < noise) { // c noise% вероятностью меняем пиксель с черного на белый и наоборот
					new_digit = abs(int(images[i][j][k])-1-48);
					noisy_cock << new_digit;
				}
				else {
					noisy_cock << images[i][j][k];
				}
			}
			noisy_cock <<  std::endl;
		}
		noisy_cock.close();
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	std::vector<std::vector <std::string>> images;
	std::vector<std::vector <std::string>> noisy_images;
	std::string suffix = "\.txt";
	std::string noisy_suffix = "noisy\.txt";
	int noise = 42;
	preparation_images(images, suffix);//создаём набор базовых изображений цифр
	create_noisy_digits(images, noise);//делаем зашумление с соответстующим процентом
	preparation_images(noisy_images, noisy_suffix);//заполняем набор зашумлённых цифр
	
	//std::vector<double> camparations_1, camparations_2;
	std::cout << "Таблицs со сравнениями зашумлённых цифр с изначальными"<<std::endl;
	//по минимальному значению в строке можно определить на какое число больше всего похоже изображение
	for (size_t i = 0; i <= 9; ++i) {
		std::cout << i << ") ";
		for (size_t j = 0; j <= 9; ++j) {
			//camparations_1.push_back(comape_two_images_1v(images[i], noisy_images[j]));//считаем меру разности между двумя изображениями
			std::cout << comape_two_images_1v(images[i], images[j])<< " ";
			//camparations_2.push_back(comape_two_images_2v(images[j], images[j]));
		}
		std::cout<<std::endl;
	}

	std::cout << std::endl;
	for (size_t i = 0; i <= 9; ++i) {
		std::cout << i << ") ";
		for (size_t j = 0; j <= 9; ++j) {

			std::cout << comape_two_images_2v(images[i], noisy_images[j])<< " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (size_t i = 0; i <= 9; ++i) {
		std::cout << i << ") ";
		for (size_t j = 0; j <= 9; ++j) {

			std::cout << comape_two_images_3v(images[i], noisy_images[j]) << " ";
		}
		std::cout << std::endl;
	}


	system("pause");
	return 0;
}
