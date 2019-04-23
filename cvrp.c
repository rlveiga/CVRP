#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Para  X-n101-k25
#define N 22 // clientes + depósito
#define V 4 //  numero minimo de veículos
#define C 6000

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

void clientsInit() {
	srand(time(NULL));

	c = malloc(N * sizeof(Cliente));

	for(int i = 0; i < N; i++) {
		c[i].id = i;
		// c[i].x = (rand() % 99) + 1;
		// c[i].y = (rand() % 99) + 1;

		// if(i == 0)
		// 	c[i].demanda = 0;

		// else
		// 	c[i].demanda = (rand() % 99) + 1;
	}

	c[0].x = 145;
	c[0].y = 215;
	c[0].demanda = 0;

	c[1].x = 151;
	c[1].y = 264;
	c[1].demanda = 1100;

	c[2].x = 159;
	c[2].y = 261;
	c[2].demanda = 700;

	c[3].x = 130;
	c[3].y = 254;
	c[3].demanda = 800;

	c[4].x = 128;
	c[4].y = 252;
	c[4].demanda = 1400;

	c[5].x = 163;
	c[5].y = 247;
	c[5].demanda = 2100;

	c[6].x = 146;
	c[6].y = 246;
	c[6].demanda = 400;

	c[7].x = 161;
	c[7].y = 242;
	c[7].demanda = 800;

	c[8].x = 142;
	c[8].y = 239;
	c[8].demanda = 100;

	c[9].x = 163;
	c[9].y = 236;
	c[9].demanda = 500;

	c[10].x = 148;
	c[10].y = 232;
	c[10].demanda = 600;

	c[11].x = 128;
	c[11].y = 231;
	c[11].demanda = 1200;

	c[12].x = 156;
	c[12].y = 217;
	c[12].demanda = 1300;

	c[13].x = 129;
	c[13].y = 214;
	c[13].demanda = 1300;

	c[14].x = 146;
	c[14].y = 208;
	c[14].demanda = 300;

	c[15].x = 164;
	c[15].y = 208;
	c[15].demanda = 900;

	c[16].x = 141;
	c[16].y = 206;
	c[16].demanda = 2100;

	c[17].x = 147;
	c[17].y = 193;
	c[17].demanda = 1000;

	c[18].x = 164;
	c[18].y = 193;
	c[18].demanda = 900;

	c[19].x = 129;
	c[19].y = 189;
	c[19].demanda = 2500;

	c[20].x = 155;
	c[20].y = 185;
	c[20].demanda = 1800;

	c[21].x = 139;
	c[21].y = 182;
	c[21].demanda = 700;
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

int calcCaminho(Veiculo v1){
		//Achar cliente disponível mais próximo
		Cliente clienteProx = calcClienteProximo(v1);

		//Veículo volta pro depósito
		if(clienteProx.id == 0) {
			printf("Veículo voltou para o depósito\n");
			return 0;
		}

		printf("Capacidade: %d\n", v1.capacity);
			
		v1.capacity = v1.capacity - clienteProx.demanda;
		c[clienteProx.id].demanda = 0;
		v1.state = clienteProx.id;

		printf("Cliente %d atendido\n", clienteProx.id);

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

	printf("\n");

	for(int i = 0; i < V; i++) {
		printf("Rota do veículo %d:\n", v[i].id);
		calcCaminho(v[i]);
		printf("\n");
	}

	printf("Resultado: %d\n", somatorio);

	free(c);
	free(v);
}