#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Para  X-n101-k25
#define N 101 // clientes + depósito
#define V 25 //  numero minimo de veículos
#define C 206

//Arestas distancia entre pontos
double d[N][N];

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

//Matriz solução
Cliente s[V][N-1];

double bestSolutionCost = 100000;
Cliente *bestSolution;

double getCostOf(Cliente *path) {

	double cost = 0;

	//Custo depósito --> primeiro cliente
	cost = cost + d[0][path[0].id];

	int i = 0;
	//Enquanto o veiculo não volta para o depósito, somar custo entre clientes
	while(path[i].id != 0) {
		cost = cost + d[path[i].id][path[i+1].id]; 
		i++;
	}

	//Custo último cliente --> depósito
	cost = cost + d[path[i].id][0];

	return cost;
}
double bestCost = 100000;

void getNeighborhood(Cliente *corrente, int index) {
	Cliente *aux = corrente;

	int i = 0;
	int j = 0;
	double cost;

	while(corrente[i].id != 0) {
		while(corrente[j].id != 0) {
			if(i != j) {
				cost = getCostOf(corrente);

				if(cost < bestCost) {
					for(int i = 0; i < N-1; i++) {
						s[index][i] = corrente[i];
					}
					bestCost = cost;
				}

				Cliente temp = aux[i];

				corrente[i] = aux[j];
				corrente[j] = temp;
			}
			j++;
		}
		i++;
	}

	printf("Novo caminho do veiculo %d:\n", v[index].id);

	i = 0;

	while(s[index][i].id != 0) {
		printf("%d\n", s[index][i].id);
		i++;
	}
}

void clientsInitFile(FILE* arq) {

	int qntdLeitura, id;
	float x,y,demanda;
	c = malloc(N * sizeof(Cliente));
	
	for(int i = 0; i < N; i++) {
		qntdLeitura = fscanf(arq,"%d\t%f\t%f",&id,&x,&y);
		c[i].id = id;
		c[i].x = x;
		c[i].y = y;	
	}	
	
	for(int i = 0; i < N; i++) {

		qntdLeitura = fscanf(arq,"%d\t%f",&id,&demanda);
		c[i].demanda = demanda;
		 
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


	for(int i = 0; i <= V; i++) {
		v[i].id = i+1;
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
				d[i][j] = 0;
			}

			else {
				d[i][j] = sqrt(pow(abs(clientes[i].x-clientes[j].x),2)+ pow(abs(clientes[i].y-clientes[j].y),2));
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
				//Pega distância entre veículo e potencial cliente pela matriz de arestas
				distancia = d[c[i].id][v1.id];
				if (distancia < distanciaMin) {
					distanciaMin = distancia;
					clienteMin = c[i];
				}
			}
		}
	}

	return clienteMin;
}

int counter = 0;

int geraCaminho(Veiculo v1){
		//Achar cliente disponível mais próximo
		Cliente clienteProx = calcClienteProximo(v1);

		//Veículo volta pro depósito
		if(clienteProx.id == 1) {

			counter = 0;
			return 0;
		}

		s[(v1.id)-1][counter] = clienteProx;
		counter++; 
			
		v1.capacity = v1.capacity - clienteProx.demanda;
		c[clienteProx.id - 1].demanda = 0;
		v1.state = clienteProx.id;

		geraCaminho(v1);
}

int main() {

	double totalCost = 0;

	FILE *arq;
	arq = fopen("X-n101-k25.txt","rt");
	if(arq == NULL){
   		printf("Erro, nao foi possivel abrir o arquivo\n");
 	}

	clientsInitFile(arq);

	fclose(arq);
	
	vehiclesInit();

	geraArestas(c);

	for(int i = 0; i < V; i++)
		geraCaminho(v[i]);

	printf("Solucao inicial:\n");

	int j = 0;
	for(int i = 0; i < V; i++) {
		printf("Rota do  veiculo %d\n", v[i].id);
		while(s[i][j].id != 0) {
			printf("%d\n", s[i][j].id);
			j++;
		}

		j = 0;
	}

	printf("\n");

	//Calculate initial cost, without hill-climbing
	for(int i = 0; i < V; i++) {
		totalCost = totalCost + getCostOf(s[i]);
	}
	printf("Custo inicial: %.2f\n", totalCost);

	for(int i = 0; i < V; i++) {
		getNeighborhood(s[i], i);
	}

	totalCost = 0;

	for(int i = 0; i < V; i++) {
		totalCost = totalCost + getCostOf(s[i]);
	}
	printf("Custo final: %.2f\n", totalCost);

	free(c);
	free(v);
}