#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Para  X-n101-k25
#define N 5 // clientes + depósito
#define V 1 //  numero minimo de veículos
#define C 6000

double distancia[N][N];

typedef struct Cliente
{
	int id;
	double x;
	double y;
	double demanda;
} Cliente;

Cliente *c;

typedef struct Veiculo
{
	int id;

	// Definindo o estado atual do veículo
	int state; // id de algum cliente (ou depósito)
	int capacity;
} Veiculo;

Veiculo *v;

void getSizeOf(Cliente *neighbor) {
	printf("\n");

	for(int i = 0; i < 4; i++) {
		printf("%d\n", neighbor[i].id);
	}

	printf("Size: \n");

	double sum = distancia[0][neighbor[0].id] + distancia[neighbor[0].id][neighbor[1].id] + distancia[neighbor[1].id][neighbor[2].id] + distancia[neighbor[2].id][neighbor[3].id] + distancia[neighbor[3].id][0];

	printf("%2f", sum);

	printf("\n");
}

int getNeighborhood(Cliente *corrente) {

	//2 3 1 4
	Cliente *aux = corrente;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(i != j) {
				// 2
				Cliente temp = aux[i];

				//swap 2 for 3
				corrente[i] = aux[j];
				corrente[j] = temp;

				getSizeOf(corrente);
			}
			else {
				getSizeOf(corrente);
			}
		}
	}
}

void clientsInit() {
	srand(time(NULL));

	c = malloc(N * sizeof(Cliente));

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
	v = malloc(V * sizeof(Veiculo));


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

void geraArestas(Cliente *clientes) {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(i == j) {
				distancia[i][j] = 0;
			}

			else {
				distancia[i][j] = sqrt(pow(abs(clientes[i].x-clientes[j].x),2)+ pow(abs(clientes[i].y-clientes[i].y),2));
			}
		}
	}
} 

int isAvailable(Veiculo v, Cliente c) {
	//Veiculo pode atender o cliente
	if(v.capacity >= c.demanda && c.demanda != 0) {
		return 1;
	}
 	
 	else 
		return 0;
}

int somatorio = 0;

//Achar cliente mais proximo
Cliente calcClienteProximo(Veiculo v1){
	double distancia, distanciaMin = 10000;
	Cliente clienteMin;

	//Inicializa cliente como depósito até que seja encontrado um cliente disponível
	clienteMin.id = c[0].id;
	clienteMin.x = c[0].x;
	clienteMin.y = c[0].y;

	for(int i = 0; i < N; i++){
		//Evitar problema dele calculando distancia entre ele mesmo
		if(v1.state != c[i].id) {
			if(isAvailable(v1, c[i])) {
				distancia = calcDistancia(c[v1.state], c[i]);
				if (distancia < distanciaMin) {
					distanciaMin = distancia;
					clienteMin = c[i];
				}
			}
		}
	}

	if(distanciaMin != 10000) {
		somatorio = somatorio + distanciaMin;
	}

	return clienteMin;
}

//Vetor solução
Cliente s[N-1];

int counter = 0;

int calcCaminho(Veiculo v1){
		//Achar cliente disponível mais próximo
		Cliente clienteProx = calcClienteProximo(v1);

		//Veículo volta pro depósito
		if(clienteProx.id == 0) {
			// printf("Veículo voltou para o depósito\n");
			counter = 0;
			return 0;
		}

		s[counter] = clienteProx;
		counter++; 

		// printf("Capacidade: %d\n", v1.capacity);
			
		v1.capacity = v1.capacity - clienteProx.demanda;
		c[clienteProx.id].demanda = 0;
		v1.state = clienteProx.id;

		// printf("Cliente %d atendido\n", clienteProx.id);

		calcCaminho(v1);
}

int main() {
	
	clientsInit();

	for(int i = 0; i < N; i++) {
		printf("Client no. %d - x: %.2f y: %.2f demanda: %.2f\n", c[i].id, c[i].x, c[i].y, c[i].demanda);
	}

	printf("\n");

	vehiclesInit();

	geraArestas(c);

	for(int i = 0; i < V; i++) {
		// printf("Rota do veículo %d:\n", v[i].id);
		calcCaminho(v[i]);
		// printf("\n");
	}

	printf("Solucao inicial:\n");

	for(int i = 0; i < N-1; i++) {
		printf("%d\n", s[i].id);
	}

	printf("Resultado inicial: %d\n", somatorio);

	getNeighborhood(s);

	free(c);
	free(v);
}