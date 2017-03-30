#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define key(A) (A) 
#define less(A, B) (key(A) < key(B)) 

typedef struct node *link;

typedef struct node1 *link1;

typedef struct Cheque *cheque;

typedef struct Cliente *cliente;

struct Cheque {
	int valor;
	long int refe;
	long int refb;
	long int refc;
};

struct Cliente {
	long int ref;
	int nche;
	int vche;
	int nchb;
	int vchb;
};

struct node {
	cheque cq;
	link next;
};

struct node1 {
	cliente cl;
	link1 left;
	link1 right;
	int profund;
};

static link head, tail;
static link1 raiz;
int num_cheques = 0;
int valor_total = 0;
int num_clientes = 0;

void init();
int empty();
void adiciona_cheque(cheque cq);
link novo_elemento(cheque cq, link next);
link remove_cheque(long int refc);
link encontra_cheque(link head, long int refc);
link1 cria_cliente(cliente cl, link1 left, link1 right);
link1 encontra_cliente(link1 raiz, long int ref);
link1 adiciona_cliente(link1 raiz, cliente cl);
link1 remove_cliente(link1 raiz, cliente clt);
link1 maior(link1 raiz);
int num_elementos(link1 raiz);
int profundidade(link1 raiz);
link1 roda_left(link1 raiz);
link1 roda_right(link1 raiz);
link1 roda_left_right(link1 raiz);
link1 roda_right_left(link1 raiz);
int balance_factor(link1 raiz);
link1 equilibra(link1 raiz);
link1 atualiza_cliente(link1 raiz, long int ref,int a_r, int e_b, int valor);
void escreve_clientes_ordenados(link1 ramo);


int main()
{
	int valor;
	int num_cheques = 0;
	int valor_total = 0;
	int num_clientes = 0;
	long int refc, refe, refb;
	link c;
	link1 cli;
	char comando[15];	
	cheque cq;
	cliente cl, cl2;
	init();

	while (1)	{
		cq = NULL;
		cl = NULL;
		cl2 = NULL;
		scanf("%s", comando);
				if(strcmp(comando, "cheque") == 0){
					num_cheques += 1;
					scanf("%d %ld %ld %ld", &valor, &refe, &refb, &refc);
					valor_total += valor;
					cq = (cheque) malloc(sizeof(struct Cheque));
					cq->valor = valor;
					cq->refe = refe;
					cq->refb = refb;
					cq->refc = refc;
					adiciona_cheque(cq);
					cli = encontra_cliente(raiz, refe);
					if(cli == NULL) {
						cl = (cliente) malloc(sizeof(struct Cliente));
						cl->ref = refe;
						cl->vche = valor;
						cl->nche = 1;
						cl->vchb = 0;
						cl->nchb = 0;
						raiz = adiciona_cliente(raiz, cl);
					}
					else {
						raiz = atualiza_cliente(raiz, refe, 1, 0, valor);
					}
					cli = encontra_cliente(raiz, refb);
					if(cli == NULL) {
						cl2 = (cliente) malloc(sizeof(struct Cliente));
						cl2->ref = refb;
						cl2->vchb = valor;
						cl2->nchb = 1;
						cl2->vche = 0;
						cl2->nche = 0;
						raiz = adiciona_cliente(raiz, cl2);
					}
					else {
						raiz = atualiza_cliente(raiz, refb, 1, 1, valor);
					}
				}
				else if(strcmp(comando, "processa") == 0){
					if(head != NULL){
						num_cheques -= 1;
						refc = head->cq->refc;
						refe = head->cq->refe;
						refb = head->cq->refb;
						valor = head->cq->valor;
						valor_total -= valor;
						remove_cheque(refc);
						raiz = atualiza_cliente(raiz, refe, 0, 0, valor);
						cli = encontra_cliente(raiz, refe);
						if(cli->cl->nche == 0 && cli->cl->nchb == 0) {
							raiz = remove_cliente(raiz, cli->cl);
						} 
						raiz = atualiza_cliente(raiz, refb, 0, 1, valor);
						cli = encontra_cliente(raiz, refb);
						if(cli->cl->nche == 0 && cli->cl->nchb == 0) {
							raiz = remove_cliente(raiz, cli->cl);
						} 
					}
					else
						printf("Nothing to process\n");
				}
				else if(strcmp(comando, "processaR") == 0){
					scanf("%ld", &refc);
					c = encontra_cheque(head, refc);
					if(c != NULL) {
						num_cheques -= 1;
						refe = c->cq->refe;
						refb = c->cq->refb;
						valor = c->cq->valor;
						valor_total -= valor;
						remove_cheque(refc);
						raiz = atualiza_cliente(raiz, refe, 0, 0, valor);
						cli = encontra_cliente(raiz, refe);
						if(cli->cl->nche == 0 && cli->cl->nchb == 0) {
							raiz = remove_cliente(raiz, cli->cl);
						} 
						raiz = atualiza_cliente(raiz, refb, 0, 1, valor);
						cli = encontra_cliente(raiz, refb);
						if(cli->cl->nche == 0 && cli->cl->nchb == 0) {
							raiz = remove_cliente(raiz, cli->cl);
						}
					}
					else {
						printf("Cheque %ld does not exist\n", refc);
					} 
				}
				else if(strcmp(comando, "infocheque") == 0){
					scanf("%ld", &refc);
					c = encontra_cheque(head, refc);
					if(c != NULL)
						printf("Cheque-info: %ld %d %ld --> %ld\n", c->cq->refc, c->cq->valor, c->cq->refe, c->cq->refb);
				}
				else if(strcmp(comando, "infocliente") == 0){
					scanf("%ld", &refc);
					cli = encontra_cliente(raiz, refc);
					if(cli != NULL)
						printf("Cliente-info: %ld %d %d %d %d\n", cli->cl->ref, cli->cl->nche, cli->cl->vche, cli->cl->nchb, cli->cl->vchb);
				}
				else if(strcmp(comando, "info") == 0){
					if(num_elementos(raiz) != 0) {
						escreve_clientes_ordenados(raiz);
					}
					else
						printf("No active clients\n");
                }
				else if(strcmp(comando, "sair") == 0){
					while(head != NULL) {
						refc = head->cq->refc;
						remove_cheque(refc);
					}
					num_clientes = num_elementos(raiz);
					printf("%d %d %d\n",num_clientes, num_cheques, valor_total);
					return 0;
				}
				else{
					printf("Comando nao conhecido introduza novamente.\n");
				}

		}
	return EXIT_FAILURE;
}


void init() {
	head = NULL;
	tail = NULL;
	raiz = NULL;
}

int empty() {
	return head == NULL;
}

void adiciona_cheque(cheque cq) {
	if(head == NULL) {
		head = (tail = novo_elemento(cq, head));
		return;
	}
	tail->next = novo_elemento(cq, NULL);
	tail = tail->next;
	return;
}

link novo_elemento(cheque cq, link next) {
	link novo_c = (link) malloc(sizeof(struct node));
	novo_c->cq = cq;
	novo_c->next = next;
	return novo_c;
}

link remove_cheque(long int refc) {
	link c, prev;
		for(c = head, prev = NULL; c != NULL; prev = c, c = c->next) {
			if(c->cq->refc == refc) {
				if(c == head)
					head = c->next;
				else
					prev->next = c->next;
				if (c->next==NULL)
					tail=prev;
				free(c->cq);
				free(c);
				return head;
			}
		}
		printf("Cheque %ld does not exist\n", refc);
		return head;
}

link encontra_cheque(link head, long int refc) {
	link c;
	for(c = head; c != NULL; c = c->next) {
		if(refc == c->cq->refc) {
			return c;
		}
	}
	return NULL;
}

link1 novo_cliente(cliente cl, link1 left, link1 right) {
	link1 novo_cl = (link1) malloc(sizeof(struct node1));
	novo_cl->cl = cl;
	novo_cl->left = left;
	novo_cl->right = right;
	novo_cl->profund = 0;
	return novo_cl;
}

link1 encontra_cliente(link1 ramo, long int ref) {
	if(ramo == NULL) {
		return NULL;
	}
	else if(ref == ramo->cl->ref) {
		return ramo;
	}
	else if (less(ref, ramo->cl->ref)) {
		return encontra_cliente(ramo->left, ref);
	}
	else {
		return encontra_cliente(ramo->right, ref);

	}
}

link1 adiciona_cliente(link1 ramo, cliente cl) {
	if(ramo == NULL) {
		return novo_cliente(cl, NULL, NULL);;
	}
	else if(less(cl->ref, ramo->cl->ref)) 
		ramo->left = adiciona_cliente(ramo->left, cl);
	else
		ramo->right = adiciona_cliente(ramo->right, cl);
	ramo = equilibra(ramo);
	return ramo;
}

link1 remove_cliente(link1 ramo, cliente clt) {
    link1 cli;
    cliente x;
    if(ramo == NULL) {
    	return ramo;
    }
    else if (less(clt->ref, ramo->cl->ref)) {
    	ramo->left = remove_cliente(ramo->left, clt);
    }
    else if (less(ramo->cl->ref, clt->ref)) {
    	ramo->right = remove_cliente(ramo->right, clt);
    }
    else {
		if (ramo->left != NULL && ramo->right != NULL) {
            cli = maior(ramo->left);
            {x = ramo->cl; ramo->cl = cli->cl; cli->cl = x;}
            ramo->left = remove_cliente(ramo->left, cli->cl);
        }
		else {                           
			cli = ramo;
			if (ramo->left == NULL && ramo->right == NULL)
				ramo = NULL;
			else if (ramo->left == NULL)
				ramo = ramo->right; 
			else 
				ramo = ramo->left;
			free(cli->cl);
			free(cli); 
        }
  }
  ramo = equilibra(ramo);
  return ramo;
}



link1 maior(link1 ramo) {
	if(ramo == NULL || ramo->right == NULL){
		return ramo;
	}
	else 
		return maior(ramo->right);
}

int num_elementos(link1 ramo) {
	if(ramo == NULL)
		return 0;
	else
		return 1 + num_elementos(ramo->left) + num_elementos(ramo->right);
}

int profundidade(link1 ramo) {
	if(ramo == NULL)
		return -1;
	return ramo->profund;
}

link1 roda_left(link1 ramo) {
	int profund_left, profund_right;
	link1 cli = ramo->right;
	ramo->right = cli->left;
	cli->left = ramo;

	profund_left = profundidade(ramo->left);
	profund_right = profundidade(ramo->right);
	ramo->profund = profund_left > profund_right ? profund_left + 1 : profund_right +1;

	profund_left = profundidade(ramo->left);
	profund_right = profundidade(ramo->right);
	cli->profund = profund_left > profund_right ? profund_left + 1 : profund_right +1;
	return cli;
}

link1 roda_right(link1 ramo) {
	int profund_left, profund_right;
	link1 cli = ramo->left;
	ramo->left = cli->right;
	cli->right = ramo;

	profund_left = profundidade(ramo->left);
	profund_right = profundidade(ramo->right);
	ramo->profund = profund_left > profund_right ? profund_left + 1 : profund_right +1;

	profund_left = profundidade(ramo->left);
	profund_right = profundidade(ramo->right);
	cli->profund = profund_left > profund_right ? profund_left + 1 : profund_right +1;
	return cli;
}

link1 roda_left_right(link1 ramo) {
	if(ramo == NULL)
		return ramo;
	ramo->left = roda_left(ramo->left);
	return roda_right(ramo);
}

link1 roda_right_left(link1 ramo) {
	if(ramo == NULL)
		return ramo;
	ramo->right = roda_right(ramo->right);
	return roda_left(ramo);
}

int balance_factor(link1 ramo) {
	if(ramo == NULL)
		return 0;
	return profundidade(ramo->left) - profundidade(ramo->right);
}

link1 equilibra(link1 ramo) {
	int bf;
	if(ramo == NULL)
		return ramo;
	bf = balance_factor(ramo);
	if(bf > 1) {
		if(balance_factor(ramo->left) > 0)
			ramo = roda_right(ramo);
		else
			ramo = roda_left_right(ramo);
	}
	else if(bf < -1) {
		if(balance_factor(ramo->right) < 0)
			ramo = roda_left(ramo);
		else
			ramo = roda_right_left(ramo);
	}
	else {
		int profund_left = profundidade(ramo->left);
		int profund_right = profundidade(ramo->right);
		if(less(profund_right, profund_left))
			ramo->profund = profund_left + 1;
		else
			ramo->profund = profund_right + 1;
	}
	return ramo;
}

link1 atualiza_cliente(link1 ramo, long int ref,int a_r, int e_b, int valor){ /*verifica-s é emissor ou benificiario*/
	link1 cli;
	if (a_r == 1) {
		if(e_b == 0) {
			cli = encontra_cliente(ramo, ref);
			cli->cl->vche += valor;
			cli->cl->nche += 1;
			return ramo;
		}
		else {
			cli = encontra_cliente(ramo, ref);
			cli->cl->vchb += valor;
			cli->cl->nchb += 1;
			return ramo;
		}
	}
	else {
		if(e_b == 0) {
			cli = encontra_cliente(ramo, ref);
			cli->cl->vche -= valor;
			cli->cl->nche -= 1;
			return ramo;
		}
		else {
			cli = encontra_cliente(ramo, ref);
			cli->cl->vchb -= valor;
			cli->cl->nchb -= 1;
			return ramo;
		}
	}
}

void escreve_clientes_ordenados(link1 ramo) {
	if(ramo == NULL)
		return;
	escreve_clientes_ordenados(ramo->left);

		printf("*%ld %d %d %d %d\n", ramo->cl->ref, ramo->cl->nche, ramo->cl->vche, ramo->cl->nchb, ramo->cl->vchb);
	escreve_clientes_ordenados(ramo->right);
}
