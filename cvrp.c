#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Para  X-n101-k25
#define N 4 // clientes + depósito
#define V 1 //  numero minimo de veículos
#define C 206

typedef struct Cliente
{
	int id;
	double x;
	double y;
	double demanda;
} Cliente;

Cliente c[N];

typedef struct Veiculo
{
	int id;

	// Definindo o estado atual do veículo
	int state; // id de algum cliente (ou depósito)
	int capacity;
} Veiculo;

Veiculo v[V]; // Inicializando vetor de veículos

void clientsInit() {
	srand(time(NULL));

	for(int i = 0; i < N; i++) {
		c[i].id = i;
		c[i].x = (rand() % 99) + 1;
		c[i].y = (rand() % 99) + 1;

		if(i == 0)
			c[i].demanda = 0;

		else
			c[i].demanda = (rand() % 99) + 1;
	}
}

void vehiclesInit() {
	//PROBLEMA AQUI NUMERO FIXO DE VEICULOS
	for(int i = 0; i < V; i++) {
		v[i].id = i;
		v[i].state = 0; // Todos os veículos começam no depósito
		v[i].capacity = C; // Todos os veículos começam com a mesma capacidade
	}
}

//Distacia entre cliente1 e clienet2
double calcDistancia(Cliente cliente1,Cliente cliente2){
	return sqrt(pow(abs(cliente1.x-cliente2.x),2)+ pow(abs(cliente1.y-cliente2.y),2));
}

int isAvailable(Veiculo v, Cliente c) {
	//Veiculo pode atender o cliente
	if(v.capacity >= c.demanda && c.demanda != 0) {
		return 1;
	}
 	
 	else 
		return 0;
}

//Achar cliente mais proximo
Cliente calcClienteProximo(Veiculo v1){
	double distancia, distanciaMin = 10000;
	Cliente clienteMin;

	for(int i = 0; i < N; i++){
		//Evitar problema dele calculando distancia entre ele mesmo
		if(v1.state != c[i].id) {
			if(isAvailable(v1, c[i])) {
				distancia = calcDistancia(c[v1.state], c[i]);
				if (distancia < distanciaMin) {
					distanciaMin= distancia;
					clienteMin = c[i];
				}
			}
		}
	}

	return clienteMin;
}

void calcCaminho(Veiculo v1){
	printf("Veiculo está em %d", v1.state);

	//Achar cliente disponível mais próximo
	Cliente clienteProx = calcClienteProximo(v1);

	printf("Cliente %d é o disponível mais proximo\n", clienteProx.id);
		
	v1.capacity = v1.capacity - clienteProx.demanda;
	c[clienteProx.id].demanda = 0;
	v1.state = clienteProx.id;

	calcCaminho(v1);
}

int main() {
	
	clientsInit();
	vehiclesInit();

	for(int i = 0; i < N; i++) {
		printf("Client no. %d - x: %.2f y: %.2f demanda: %.2f\n", c[i].id, c[i].x, c[i].y, c[i].demanda);
	}

	printf("\n");

	for (int i = 0; i < V; ++i)
	{
		printf("Vehicle no. %d - state: %d capacity: %d\n", v[i].id, v[i].state, v[i].capacity);
	}

	calcCaminho(v[0]);
}