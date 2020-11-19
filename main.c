#include<stdio.h>
#include<stddef.h>

// TAMANHO MÁXIMO DA MEMÓRIA
char memoria[20000];

// ESTRUTURA GERAL PARA BLOCOS ALOCADOS E DESALOCADOS
struct bloco{
    size_t tamanho; // tamanho do bloco
    int libera; // sinaliza se o bloco está livre ou não
     struct bloco *prox; // ponteiro para o bloco prox
};


// declaração de bloco inicial
struct bloco *memoLivre=(void*)memoria;

// FUNÇÕES DO AJUDANTE - DEFINIDAS MAIS TARDE NO CÓDIGO
void  inicializar();
void quebra(struct bloco *encaixe_de_memo,size_t tamanho);
void *aloca(size_t nDeBytes);
void misturar();
void libera(void* ptr);

// inicializando o bloco de memória com tamanho máximo (memoLivre é o bloco principal)
void  inicializar(){
    memoLivre->tamanho=20000-sizeof(struct bloco);
    memoLivre->libera=1;
    memoLivre->prox=NULL;
}

// Abrindo caminho para uma nova alocação de bloco dividindo um bloco livre, ou seja, memoLivre usando o First Fit
void quebra(struct bloco *encaixe_de_memo,size_t tamanho){
    struct bloco *new=(void*)((void*)encaixe_de_memo+tamanho+sizeof(struct bloco));
    new->tamanho=(encaixe_de_memo->tamanho)-tamanho-sizeof(struct bloco);
    new->libera=1;
    new->prox=encaixe_de_memo->prox;
    encaixe_de_memo->tamanho=tamanho;
    encaixe_de_memo->libera=0;
    encaixe_de_memo->prox=new;
}

// basicamente divide a memória necessária da memoLivre e dos nós de links (como uma lista vinculada)
void *aloca(size_t nDeBytes){
    struct bloco *atual,*prev;
    void *resultado;
    if(!(memoLivre->tamanho)){
        inicializar();
        printf("Memória inicializada\n");
    }
    atual=memoLivre;
    while((((atual->tamanho)<nDeBytes)||((atual->libera)==0))&&(atual->prox!=NULL)){
        prev=atual;
        atual=atual->prox;
        printf("Um bloco foi verificado\n");
    }
    if((atual->tamanho)==nDeBytes){
        atual->libera=0;
        resultado=(void*)(++atual);
        printf("Bloco de encaixe exato alocado\n");
        return resultado;
    }
    else if((atual->tamanho)>(nDeBytes+sizeof(struct bloco))){
        quebra(atual,nDeBytes);
        resultado=(void*)(++atual);
        printf("Bloco de encaixe alocado com uma divisão\n");
        return resultado;
    }
    else{
        resultado=NULL;
        printf("Desculpe. Não há memória suficiente para alocar\n");
        return resultado;
    }
}

// função auxiliar para liberar memória ao desalocar
void misturar(){
    struct bloco *atual,*prev;
    atual=memoLivre;
    while((atual->prox)!=NULL){
        if((atual->libera) && (atual->prox->libera)){
            atual->tamanho+=(atual->prox->tamanho)+sizeof(struct bloco);
            atual->prox=atual->prox->prox;
            }
        prev=atual;
        atual=atual->prox;
    }
}

//libera, ou seja, retornando memória para memoLivre e excluindo o bloco
void libera(void* ptr){
    if(((void*)memoria<=ptr)&&(ptr<=(void*)(memoria+20000))){
        struct bloco* atual=ptr;
        --atual;
        atual->libera=1;
        misturar();
    }
    else printf("Forneça um ponteiro válido alocado por aloca\n");
}

//TESTE
int main(){
    int *p=(int)aloca(100*sizeof(int));
    char *q=(char)aloca(250*sizeof(char));
    int *r=(int)aloca(100*sizeof(int));
    libera(p);
    libera(p);
    char *w=(char)aloca(700);
    libera(r);
    int *k=(int)aloca(500*sizeof(int));
    libera(q);
}