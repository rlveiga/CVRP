#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>

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

void exibeRota(Cliente *path) {
	int i = 0;
	while(path[i].id != 1) {
		printf("%d\n", path[i].id);
		i++;
	}

	printf("\n");
}

void copyArray(Cliente *source, Cliente *dest) {
	int i = 0;

	while(source[i-1].id != 1) {
		dest[i] = source[i];
		i++;
	}
}

double getCostOf(Cliente *path) {

	double cost = 0;

	//Custo depósito --> primeiro cliente
	cost = cost + d[1][path[0].id];
	// printf("Custo deposito - %d: %.2f\n", path[0].id, d[1][path[0].id]);

	int i = 0;
	//Enquanto o veiculo não volta para o depósito, somar custo entre clientes
	while(path[i+1].id != 1) {
		cost = cost + d[path[i].id][path[i+1].id]; 
		// printf("Custo %d - %d: %.2f\n", path[i].id, path[i+1].id, d[path[i].id][path[i+1].id]);
		i++;
	}

	//Custo último cliente --> depósito
	cost = cost + d[path[i].id][1];

	return cost;
}

void getNeighborhood(Cliente *corrente, int index) {

	// exibeRota(corrente);

	double bestCost = getCostOf(corrente);

	Cliente aux[N-1];

	copyArray(corrente, aux);

	int i = 0;
	int j = 0;
	double cost;

	// printf("Custo antes da manipulação: %.2f\n", getCostOf(corrente));

	while(corrente[i].id != 1) {
		while(corrente[j].id != 1) {
			if(i != j) {

				copyArray(corrente, aux);

				Cliente temp = corrente[i];
				aux[i] = corrente[j];
				aux[j] = temp;

				cost = getCostOf(aux);

				if(cost < bestCost) {
					// printf("Achou melhor custo para veiculo %d: %.2f\n", index, cost);
					for(int i = 0; i < N-1; i++) {
						s[index][i] = aux[i];
					}
					bestCost = cost;
				}
			}
			j++;
		}
		j = 0;
		i++;
	}

	// printf("Custo após a manipulação: %.2f\n", getCostOf(s[index]));
}

void clientsInit(FILE* arq) {

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

void vehiclesInit() {
	v = malloc(V * sizeof(Veiculo));


	for(int i = 0; i <= V; i++) {
		v[i].id = i+1;
		v[i].state = 1; // Todos os veículos começam no depósito
		v[i].capacity = C; // Todos os veículos começam com a mesma capacidade
	}
}

void matrixInit() {
	for(int i = 0; i < V; i++) {
		for(int j = 0; j < N-1; j++) {
			s[i][j].id = 0;
			s[i][j].x = 0;
			s[i][j].y = 0;
			s[i][j].demanda = 0;
		}
	}
}

void geraArestas(Cliente *clientes) {
	for(int i = 1; i <= N; i++) {
		for(int j = 1; j <= N; j++) {
			if(i == j) {
				d[i][j] = 0;
			}

			else {
				d[i][j] = sqrt(pow(fabs(clientes[i-1].x-clientes[j-1].x),2) + pow(fabs(clientes[i-1].y-clientes[j-1].y),2));
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
				distancia = d[c[i].id][v1.state];
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
		s[(v1.id)-1][counter] = clienteProx;
		counter = 0;
		return 0;
	}

	else {
		s[(v1.id)-1][counter] = clienteProx;
		counter++; 
			
		v1.capacity = v1.capacity - clienteProx.demanda;
		c[clienteProx.id - 1].demanda = 0;
		v1.state = clienteProx.id;

		geraCaminho(v1);
	}
}

int main() {

	double custoInicial = 0, custoFinal = 0;
	clock_t start, end;
	double cpu_time_used;

	FILE *arq;
	arq = fopen("X-n101-k25.txt","rt");

	if(arq == NULL) {
   		printf("Erro na leitura do arquivo\n");
 	}

	clientsInit(arq);

	fclose(arq);
	
	vehiclesInit();

	geraArestas(c);

	//Gerando solução inicial para os veículos por meio de heurísticas
	for(int i = 0; i < V; i++)
		geraCaminho(v[i]);

	printf("Solução inicial:\n\n");

	for(int i = 0; i < V; i++) {
		printf("Rota do veiculo %d:\n", v[i].id);
		exibeRota(s[i]);
	}

	for(int i = 0; i < V; i++) {
		custoInicial = custoInicial + getCostOf(s[i]);
	}

	printf("Custo inicial: %.2f\n", custoInicial);

	//Começando a calcular tempo de execução
	start = clock();

	Cliente *newArray;

	for(int i = 0; i < V; i++) {
		copyArray(s[i], newArray);
		getNeighborhood(newArray, i);
	}

	printf("\n");

	printf("Solução após hill-climbing:\n\n");

	for(int i = 0; i < V; i++) {
		printf("Rota do veiculo %d:\n", v[i].id);
		exibeRota(s[i]);
	}

	for(int i = 0; i < V; i++) {
		custoFinal = custoFinal + getCostOf(s[i]);
	}

	printf("Custo final: %.2f\n", custoFinal);

	end  = clock();
	cpu_time_used =((double)(end-start))/CLOCKS_PER_SEC;


	printf("Tempo(in seconds): %.6f\n", cpu_time_used);

	free(c);
	free(v);
}