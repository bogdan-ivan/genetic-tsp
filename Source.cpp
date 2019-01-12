#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

int** allocateMatrix(int row, int col)
{

	int** matrix = new int*[row];

	for (int i = 0; i < row; ++i)
	{
		matrix[i] = new int[col];
	}
	return matrix;
}

void deleteMatrix(int** matrix, int row)
{
	for (int i = 0; i < row; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

int random(int start, int end)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(start, end);
	return distribution(rng);
}

double random_real(int start, int end)
{
	std::mt19937 rng2;
	rng2.seed(std::random_device()());
	std::uniform_real_distribution<double> dis(start, end);
	return dis(rng2);
}

void printSquareMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printVector(std::vector<std::vector<int>> table)
{
	for (int i = 0; i < table.size(); i++)
	{
		std::cout << "Elementul " << i+1 << " = ";
		for (int j = 0; j < table[i].size(); j++)
		{
			std::cout << table[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printVector(std::vector<int> vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		std::cout << vector[i] << " ";
	}
	//std::cout << std::endl;
}

void multiplepop(int element, std::vector<std::vector<int>> &table)
{
	std::vector<int>::iterator it;
	for (int i = 0; i < table.size(); i++)
	{
		it = std::find(table[i].begin(), table[i].end(), element);
		if (it != table[i].end())
		{
			table[i].erase(it);
		}
		it = std::find(table[i].begin(), table[i].end(), -element);
		if (it != table[i].end())
		{
			table[i].erase(it);
		}
	}
}

int positie(int elem, std::vector<int> parent1)
{
	for (int i = 0; i < parent1.size(); i++)
	{
		if (parent1[i] == elem)
		{
			return i;
		}
	}
	return 0;
}

struct individ
{
	std::vector<int> cromozom;
	int fitness;
};

int calculate_fitness(individ individ, int** adiacenta);

individ erx(int numar_orase, std::vector<int> parent1, std::vector<int> parent2, int** adiacenta)
{
	std::vector<std::vector<int>> table(numar_orase, std::vector<int>(0));
	int size = parent1.size();
	individ child;
	for (int i = 0; i < size; i++)
	{
		int element = i + 1;

		// pozitia elementului in fiecare parinte
		int parent1pos = positie(element, parent1);
		int parent2pos = positie(element, parent2);

		// vecini

		int parent1stanga = parent1[((size + parent1pos - 1) % size)]; // stanga
		int parent1dreapta = parent1[((parent1pos + 1) % size)]; // dreapta
		int parent2stanga = parent2[((size + parent2pos - 1) % size)];
		int parent2dreapta = parent2[((parent2pos + 1) % size)]; 

		// cazuri de egalitate a vecinilor

		if (parent1stanga == parent2stanga)
		{
			table[i].push_back(parent1dreapta);
			table[i].push_back(parent2dreapta);
			table[i].push_back(-parent1stanga);
		}
		else if (parent1dreapta == parent2dreapta)
		{
			table[i].push_back(parent1stanga);
			table[i].push_back(parent2stanga);
			table[i].push_back(-parent1dreapta);
		}
		else if ((parent1stanga == parent2dreapta) && (parent1dreapta == parent2stanga))
		{
			table[i].push_back(-parent1stanga);
			table[i].push_back(-parent1dreapta);
		}
		else if (parent1stanga == parent2dreapta)
		{
			table[i].push_back(parent1dreapta);
			table[i].push_back(parent2stanga);
			table[i].push_back(-parent2dreapta);
		}
		else if (parent1dreapta == parent2stanga)
		{
			table[i].push_back(parent1stanga);
			table[i].push_back(parent2dreapta);
			table[i].push_back(-parent2stanga);
		}
		else if ((parent1stanga == parent2stanga) && (parent1dreapta == parent2dreapta))
		{
			table[i].push_back(-parent1stanga);
			table[i].push_back(-parent1dreapta);
		}
		else
		{
			table[i].push_back(parent1stanga);
			table[i].push_back(parent1dreapta);
			table[i].push_back(parent2stanga);
			table[i].push_back(parent2dreapta);
		}
	}
	int reruntable;
	int start = random(1, parent1.size());
	child.cromozom.push_back(start);
	multiplepop(start,table);

	while (child.cromozom.size() != parent1.size())
	{
			int min = 99;
			int minholder = 0;
			int exam;

			if (table[start - 1].size() != 0)
			{
				exam = start - 1;
			}
			else
			{
				exam = child.cromozom[0]-1;
			}
				for (int i = 0; i < table[exam].size(); i++)
				{
					if (table[exam][i] < 0) // oras cu -
					{
						minholder = table[exam][i];
						break;
					}
					if (table[std::abs(table[exam][i]) - 1].size() < min)
					{
						min = table[std::abs(table[exam][i]) - 1].size();
						minholder = table[exam][i];
					}
				}
				if (minholder != 0)
				{
					child.cromozom.push_back(std::abs(minholder));
					multiplepop(minholder, table);
					start = std::abs(minholder);
				}
				else
				{ 
					for (int i = 0; i < size; i++)
					{
						if (table[i].size() > 0)
						{
							reruntable = i;
							break;
						}
					}
					start = reruntable + 1;
				}
	}
	
		child.fitness = calculate_fitness(child, adiacenta);
		return child;
}

void matriceAdiacenta(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i][i] = 0;
		for (int j = i + 1; j < size; j++)
		{
			matrix[i][j] = random(100, 900);
			matrix[j][i] = matrix[i][j];
		}
	}
}

std::vector<int> genePosibile(int numar_orase)
{
	std::vector<int> genep(numar_orase);
	for (int i = 0; i < numar_orase; i++)
	{
		genep[i] = i+1;
	}
	return genep;
}

int calculate_fitness(individ individ, int** adiacenta)
{
	int fitness = 0;
	int size = individ.cromozom.size();
	for (int i = 0; i < size; i++)
	{
		int dreapta = individ.cromozom[((i + 1) % size)];
		fitness += adiacenta[individ.cromozom[i]][individ.cromozom[((i + 1) % size)]];
	}
	return fitness;
}

individ genereaza_individ(std::vector<int> gene_posibile, int** adiacenta)
{
	individ individ_nou;
	std::vector<int>::iterator it;

	while (individ_nou.cromozom.size() != gene_posibile.size())
	{
		int gene = gene_posibile[random(0,gene_posibile.size()-1)];
		it = std::find(individ_nou.cromozom.begin(), individ_nou.cromozom.end(), gene);
		if (it == individ_nou.cromozom.end())
		{
			individ_nou.cromozom.push_back(gene);
		}
	}

	individ_nou.fitness = calculate_fitness(individ_nou, adiacenta);
	return individ_nou;
}

bool comparatie(individ array1, individ array2)
{
	return (array1.fitness < array2.fitness);
}

float genereaza_populatie_intiala(std::vector<individ> &populatie_veche, int POPULATIE, std::vector<int> GENES, int** adiacenta)
{
	float average_fitness = 0;
	for (int i = 0; i < POPULATIE; i++)
	{
		populatie_veche.push_back(genereaza_individ(GENES,adiacenta));
		average_fitness += populatie_veche[i].fitness;
	}
	average_fitness = average_fitness / POPULATIE;
	std::sort(populatie_veche.begin(), populatie_veche.end(), comparatie);
	return average_fitness;
}

individ turnament(std::vector<individ> &populatie_veche, int index)
{
	int i = index;
	int size = populatie_veche.size();
	int rand = 0;
	if (index + 1 < size)
	{
		if (populatie_veche[i].fitness > populatie_veche[i + 1].fitness)
		{
			return populatie_veche[i];
		}
		else
		{
			return populatie_veche[i + 1];
		}
	}
	else
	{
		int rand1 = random(0, size - 1);
		int rand2 = random(0, size - 1);
		if (populatie_veche[rand1].fitness > populatie_veche[rand2].fitness) 
		{
			return populatie_veche[rand1];
		}
		else
		{
			return populatie_veche[rand2];
		}
	}
}

void mutatie(std::vector<individ> &populatie_veche, double mutate)
{
	double sansa = 0;
	for (int i = 0; i < populatie_veche.size(); i++)
	{
		sansa = random_real(0, 1);
		if (sansa >= mutate)
		{
			int firstgene = random(0, (populatie_veche[i].cromozom.size() - 1));
			int secondgene = random(0, (populatie_veche[i].cromozom.size() - 1));
			std::swap(populatie_veche[i].cromozom[firstgene], populatie_veche[i].cromozom[secondgene]);
		}
	}
}

float genereaza_populatie_noua(std::vector<individ> &populatie_veche, std::vector<individ> &populatie_noua, int POPULATIE, float elitism, int numar_orase, int** adiacenta, double procent_mutatie)
{
	float average_fitness = 0;
	double mutate = (100 - procent_mutatie) / 100;

	for (int i = 0; i < POPULATIE; i++)
	{
		if (populatie_veche[i].fitness < elitism)
		{
			populatie_noua.push_back(populatie_veche[i]);
			average_fitness += populatie_veche[i].fitness;
		}
	}

	int index = 0;
	while (populatie_noua.size() < POPULATIE)
	{
		individ individ_nou = erx(numar_orase, turnament(populatie_veche, index).cromozom, turnament(populatie_veche, index + 1).cromozom, adiacenta);
		populatie_noua.push_back(individ_nou);
		average_fitness += individ_nou.fitness;
		index++;
	}

	std::sort(populatie_noua.begin(), populatie_noua.end(), comparatie);

	std::copy(populatie_noua.begin(), populatie_noua.end(), populatie_veche.begin());

	average_fitness = average_fitness / populatie_noua.size();
	
	populatie_noua.clear();

	mutatie(populatie_veche, mutate);

	return average_fitness;
}

void main()
{
	int numar_orase;
	int POPULATIE;
	int generatii;
	double procent_mutatie;

	std::cout << "Introduceti marimea populatiei : ";
	std::cin >> POPULATIE;
	std::cout << "Introduceti numarul de orase: ";
	std::cin >> numar_orase;
	std::cout << "Introduceti numarul de generatii: ";
	std::cin >> generatii;
	std::cout << "Introduceti sansa de mutatie %: ";
	std::cin >> procent_mutatie;

	std::vector<int> GENES = genePosibile(numar_orase);
	std::vector<individ> populatie_veche;
	std::vector<individ> populatie_noua;

	int** adiacenta = allocateMatrix(numar_orase+1, numar_orase+1);
	matriceAdiacenta(adiacenta, numar_orase+1);
	std::cout << "Orasele au fost generate random !" << std::endl;
	std::cout << "Distantele dintre orase sunt reprezentate prin urmatoarea matrice de adiacenta: " << std::endl;

	std::cout << "Incepem cautarea solutiei" << std::endl;

	float elitism;
	elitism = genereaza_populatie_intiala(populatie_veche, POPULATIE, GENES, adiacenta);
	std::vector<int> best(numar_orase);

	for (int i = 0; i < generatii; i++)
	{
		elitism = genereaza_populatie_noua(populatie_veche, populatie_noua, POPULATIE, elitism, numar_orase, adiacenta, procent_mutatie);
		std::cout << " FIT : " << populatie_veche[0].fitness << std::endl;
		std::copy(populatie_veche[0].cromozom.begin(), populatie_veche[0].cromozom.end(), best.begin());
	}

	std::cout << "Cel mai bun traseu dupa " << generatii << " de generatii este :" << std::endl;
	printVector(best);
	std::cout << std::endl;
	
}