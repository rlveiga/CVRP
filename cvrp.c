#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define N 31 // clientes + depósito
#define V 5 //  veículos
#define CAPACITY 100

struct Cliente
{
	int id;
	int x;
	int y;
	int demanda;
};

struct Cliente c[N]; // Inicializando vetor de clientes

struct Veiculo
{
	int id;

	// Definindo o estado atual do veículo
	int state; // id de algum cliente (ou depósito)
	int capacity;
};

struct Veiculo v[V]; // Inicializando vetor de veículos

void clientsInit() {
	srand(time(NULL));

	for(int i = 0; i < N; i++) {
		c[i].id = i;
		c[i].x = (rand() % 99) + 1;
		c[i].y = (rand() % 99) + 1;

		if(i == 0)
			c[i].demanda = 0;

		else
			c[i].demanda = (rand() % 29) + 1;
	}
}

void vehiclesInit() {

	for(int i = 0; i < V; i++) {
		v[i].id = i;
		v[i].state = 0; // Todos os veículos começam no depósito
		v[i].capacity = CAPACITY; // Todos os veículos começam com a mesma capacidade
	}
}

int main() {
	
	clientsInit();
	vehiclesInit();

	for(int i = 0; i < N; i++) {
		printf("Client no. %d - x: %d y: %d demanda: %d\n", c[i].id, c[i].x, c[i].y, c[i].demanda);
	}

	printf("\n");

	for (int i = 0; i < V; ++i)
	{
		printf("Vehicle no. %d - state: %d capacity: %d\n", v[i].id, v[i].state, v[i].capacity);
	}
}