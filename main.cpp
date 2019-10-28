#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define age 65
#define itens 20
using namespace std;

float caixa=0;
int totalprodutos=0;

string notastxt[]={
    "Péssimo",
    "Ruim",
    "Mediano",
    "Bom",
    "Ótimo"
};
float totalclientes=0;
float notamedia=0;
float notatotal=0;
int filacont=0, listacont=0;

struct ItemCardapio{
    int id;
    char nome[256];
    float preco;
} item[itens];
struct ListaPedido{
    ItemCardapio data;

    ListaPedido *prox=NULL;
    ListaPedido *ante=NULL;
};

struct PedidoData{
    ListaPedido *listapedido=NULL;
    float conta=0;
    int totalitens=0;
    int pedidoid;
};
struct ClienteData{
    char nome[256];
    int idade;
};
struct Cliente{
    PedidoData pedido;
    ClienteData pessoal;

    Cliente *prox=NULL;
    Cliente *ante=NULL;
} *listahead, *listatail, *filahead, *filatail;

struct Nota{
    float nota;
    ClienteData pessoal;
    Nota *prox=NULL;
} *notas;

/*FUNC*/
bool validstr(char str[]){
    if(strlen(str)<=0) return false;

    int space=0;
    for(int i=0; i<strlen(str); i++){
        if(str[i]==' '){
            if(i==0 || i==strlen(str)-1) return false;
            else space++;
        } else space=0;
        if(space>1) return false;
    }
    return true;
}
double pow(double base, double power){
    double num=1;
    if(power>=0)for(int i=0; i<power; i++) num*=base;
    else{
        for(int i=0; i<-power; i++) num*=base;
        num = 1/num;
    }
    return num;
}
int chartoint(char chr){
    switch(chr){
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case '0': return 0;
        case '.': return -2;
        case ',': return -2;
        default: return -1;
    }
}
double strtoint(char str[]){
    double num=0;
    int after=0;

    for(int i=0; i<strlen(str); i++){
        if(after<-2) break;

        if(chartoint(str[i])>=0 && after==0) num+=chartoint(str[i])*pow(10,strlen(str)-i-1);
        else if(chartoint(str[i])==-1){
            num /= pow(10,strlen(str)-i);
            if(num<=0) return -1;
            return num;
        }

        else if(chartoint(str[i])==-2){
            if(after==0) num /= pow(10,strlen(str)-i);
            else if(after<0) return num;
            after--;
        }

        else if(chartoint(str[i]>=0) && after<0){
            num += pow(10,after) * chartoint(str[i]);
            after--;
        }
    }
    return num;
}

/*NOTAS*/
void novanota(ClienteData cliente, int valor){
    Nota *notaadd = new Nota;
    notaadd->pessoal = cliente;
    notaadd->nota = valor;

    notaadd->prox=notas;
    notas=notaadd;

    notatotal += valor;
    notamedia = notatotal/totalclientes;
}
void shownotas(){
    Nota *aux=notas;
    for(int i=0; i<totalclientes; i++){
        printf("[%.1f] - ",aux->nota);
        cout<<aux->pessoal.nome<<", "<<aux->pessoal.idade<<" anos.\n";
        aux=aux->prox;
    }

    printf("\nM�dia: [%.1f] ",notamedia);

    int floormedia = notamedia - 0.5;
    cout<<notastxt[floormedia];

    cout<<endl<<endl;
}

/*CLIENTE*/
void filatolista(){
    Cliente *cliente = filahead;
    if(filacont>1){
        filahead = filahead->prox;
        filahead->ante = NULL;
    } else filahead = NULL;

    if(listacont==0){
        listahead = cliente;
        listatail = listahead;
        listahead->prox = NULL;
    } else{
        listatail->prox = cliente;
        cliente->ante = listatail;
        listatail = cliente;
    }

    filacont--;
    listacont++;
}

Cliente *novocliente(){
    Cliente *cliente = new Cliente;
    char aux[256];

    while(1){
        cout<<"NOVO CLIENTE-----------\n";
        cout<<"Nome Completo: ";
        cin.getline(aux,sizeof(aux));
        if(validstr(aux)){
            strcpy(cliente->pessoal.nome,aux);
            break;
        }
        else system("CLS");
    }

    while(1){
        cout<<"Idade: ";
        cin.getline(aux,sizeof(aux));
        if(validstr(aux) && strtoint(aux)>-1){
            cliente->pessoal.idade=strtoint(aux);
            break;
        } else {
            system("CLS");
            cout<<"NOVO CLIENTE-----------\n";
            cout<<"Nome Completo: "<<cliente->pessoal.nome<<endl;
        }
    }

    system("CLS");

    cout<<cliente->pessoal.nome<<" adicionado a fila de espera.\n\n";

    return cliente;
}
void addcliente(){
    Cliente *cliente = novocliente();
    if(filacont==0){
        filahead = cliente;
        filatail = filahead;
    } else{
        if(cliente->pessoal.idade < age){
            filatail->prox = cliente;
            cliente->ante = filatail;
            filatail = cliente;
        } else{
            if(filahead->pessoal.idade < age){
                cliente->prox=filahead;
                filahead->ante=cliente;
                filahead=cliente;
            } else{
                Cliente *aux=filahead;
                for(int i=0; i<filacont-1; i++){
                    if(aux->prox->pessoal.idade < age) break;
                    aux=aux->prox;
                }
                cliente->prox=aux->prox;
                cliente->ante=aux;
                aux->prox = cliente;
                if(aux==filatail) filatail=cliente;
            }
        }
    }
    filacont++;
}

void showfila(){
    Cliente *aux = filahead;
    for(int i=0; i<filacont; i++){
        cout<<"["<<i+1<<"�] - "<<aux->pessoal.nome<<", "<<aux->pessoal.idade<<" anos.\n";
        aux=aux->prox;
    }
    cout<<endl;
}
void showlista(){
    void showlistapedidos(Cliente *cliente, bool showid, bool showprec);

    Cliente *aux = listahead;
    for(int i=0; i<listacont; i++){
        printf("[#%02d] - ", aux->pedido.pedidoid);
        cout<<aux->pessoal.nome<<", "<<aux->pessoal.idade<<" anos.\n\n";
        showlistapedidos(aux, false, true);
        if(i<listacont-1) cout<<"-----------------------\n";
        aux=aux->prox;
    }
    cout<<endl;
}

void clientedelloc(Cliente *cliente){
    void destroylista(Cliente *cliente);

    if(cliente==listahead){
        listahead=listahead->prox;
        if(listacont>1)listahead->ante=NULL;
    } else if(cliente==listatail){
        listatail=listatail->ante;
        listatail->prox=NULL;
    } else{
        cliente->prox->ante=cliente->ante;
        cliente->ante->prox=cliente->prox;
    }

    cout<<cliente->pessoal.nome<<" removido da lista de pedidos.\n\n";

    destroylista(cliente);
    delete cliente;
    listacont--;
}
void deletecliente(){
    int input;
    char inhold[256];

    if(listacont==1){
        clientedelloc(listahead);
        return;
    }
    Cliente *aux = listahead;

    showlista();
    cout<<"ENTREGAR PEDIDO--------"<<
    endl<<"Digite o ID do pedido: #";
    cin.getline(inhold, sizeof(inhold));

    if(validstr(inhold))input=strtoint(inhold);
    else input=-1;

    system("CLS");

    for(int i=0; i<listacont; i++){
        if(aux->pedido.pedidoid == input){
            clientedelloc(aux);
            break;
        }
        aux=aux->prox;
        if(i==listacont-1)cout<<"ID n�o encontrado.\n\n";
    }
}

void buscaid(Cliente *head, int cont, int id, bool showid){
    void showlistapedidos(Cliente *cliente, bool showid, bool showprec);

    Cliente *aux = head;
    for(int i=0; i<cont; i++){
        if(aux->pedido.pedidoid == id){
            if (showid) printf("[#%02d] - ", aux->pedido.pedidoid);
            cout<<aux->pessoal.nome<<", "<<aux->pessoal.idade<<" anos.\n\n";
            showlistapedidos(aux, false, true);

            break;
        }
        aux=aux->prox;
        if(i==cont-1) cout<<"ID n�o encontrado.\n";
    }

    cout<<endl;
}
void buscanome(Cliente *head, int cont, char nome[], bool showid, bool showpedido){
    void showlistapedidos(Cliente *cliente, bool showid, bool showprec);

    Cliente *aux = head;
    int result=0;
    for(int i=0; i<cont; i++){
        bool finded=false;
        bool word=true;

        if(strlen(nome) <= strlen(aux->pessoal.nome)){
            for(int i2=0, i3=0; i2<strlen(aux->pessoal.nome); i2++, i3++){
                if(nome[i3]==0 || !word) while(1){
                    if(aux->pessoal.nome[i2]==' ' || aux->pessoal.nome[i2]==0 ) break;
                    else i2++;
                }

                if(aux->pessoal.nome[i2]==' ' && nome[i3]!=' '){
                    if(!finded || !word){
                        //cout<<"!finded\n";
                        word = true;
                        i2++;
                        i3=0;
                    } else break;
                }

                //cout<<"["<<aux->pessoal.nome[i2]<<"] == ["<<nome[i3]<<"]\n";
                if(tolower(aux->pessoal.nome[i2])==tolower(nome[i3]))finded = true;
                else{
                    finded = false;
                    word = false;
                }
            }
        }
        //cout<<endl;
        if(finded && word){
            if(result>0 && showpedido) cout<<"-----------------------\n";
            if(showid) printf("[#%02d] - ", aux->pedido.pedidoid);
            else cout<<"["<<i+1<<"�] - ";
            cout<<aux->pessoal.nome<<", "<<aux->pessoal.idade<<" anos.\n";

            if(showpedido){
                cout<<endl;
                showlistapedidos(aux, false, true);
            }
            result++;
        }
        aux=aux->prox;
    }
    if(result==0) cout<<nome<<" n�o encontrado.\n";
    cout<<endl;
}
void buscaemfila(){
    char input[256];
    cout<<"BUSCAR CLIENTE---------"<<
    endl<<"Nome: ";
    cin.getline(input,sizeof(input));

    system("CLS");
    if(validstr(input)) buscanome(filahead, filacont, input, false, false);
}
void buscaemlista(){
    int opt=-1;
    char opthold[256];

    while(opt!=0){
        cout<<"BUSCAR CLIENTE---------"<<
        endl<<"1-Busca por Nome"<<
        endl<<"2-Busca por ID"<<
        endl<<"0-Retornar";

        cout<<"\n\nO que buscar? ";
        cin.getline(opthold, sizeof(opthold));

        if(validstr(opthold))opt=strtoint(opthold);
        else opt=-1;

        system("CLS");

        switch(opt){
            case 0: break;
            case 1:{
                char nomein[256];

                cout<<"BUSCAR CLIENTE---------"<<
                endl<<"Nome: ";
                cin.getline(nomein,sizeof(nomein));

                system("CLS");

                if(validstr(nomein)) buscanome(listahead, listacont, nomein, true, true);
                opt=0;
                break;
            }

            case 2:{
                int idin;
                char idhold[256];

                cout<<"BUSCAR CLIENTE---------"<<
                endl<<"Digite o ID do pedido: #";
                cin.getline(idhold, sizeof(idhold));

                if(validstr(idhold))idin=strtoint(idhold);
                else idin=-1;

                system("CLS");

                buscaid(listahead, listacont, idin, true);
                opt=0;
                break;
            }

            default: cout<<"Valor n�o encontrado.\n\n";
        }
    }
}
void buscacliente(){
    int opt=-1;
    char opthold[256];

    while(opt!=0){
        cout<<"BUSCAR CLIENTE---------"<<
        endl<<"1-Fila de Espera"<<
        endl<<"2-Lista de Pedidos"<<
        endl<<"0-Retornar";

        cout<<"\n\nOnde Buscar? ";
        cin.getline(opthold, sizeof(opthold));

        if(validstr(opthold))opt=strtoint(opthold);
        else opt=-1;

        system("CLS");

        switch(opt){
            case 0: break;
            case 1:
                buscaemfila();
                opt=0;
                break;

            case 2:
                buscaemlista();
                opt=0;
                break;

            default: cout<<"Valor n�o encontrado.\n\n";
        }
    }
}

/*PEDIDO*/
void iniciacardapio(){
    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "BigMac");
    item[totalprodutos].preco = 25.5;
    totalprodutos++;

    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "McLanche Feliz");
    item[totalprodutos].preco = 19;
    totalprodutos++;

    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "DelValle Uva");
    item[totalprodutos].preco = 5.5;
    totalprodutos++;

    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "Coca Cola 2L");
    item[totalprodutos].preco = 7.5;
    totalprodutos++;

    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "Casquinha de Baunilha");
    item[totalprodutos].preco = 1.5;
    totalprodutos++;

    item[totalprodutos].id = totalprodutos+1;
    strcpy(item[totalprodutos].nome, "McFlurry Ovomaltine");
    item[totalprodutos].preco = 6.5;
    totalprodutos++;
}
void showcardapio(bool showid){
    cout<<"CARDAPIO---------------\n";
    for(int i=0; i<itens; i++){
        if(strcmp(item[i].nome,"")!=0){
            if(showid) printf("[#%02d] - R$", item[i].id);
            printf("%05.2f - ",item[i].preco);
            cout<<item[i].nome<<endl;
        }
    }
    cout<<endl;
}
void showlistapedidos(Cliente *cliente, bool showid, bool showprec){
    ListaPedido *aux = cliente->pedido.listapedido;
    for(int i=0; i<cliente->pedido.totalitens; i++){
        if(showid) printf("[#%02d] - ", aux->data.id);
        if(showprec) printf("R$%05.2f - ",aux->data.preco);
        cout<<aux->data.nome<<endl;

        aux=aux->prox;
    }
    printf("\nValor total: R$%.2f\n\n",cliente->pedido.conta);
}

void itemalloc(ListaPedido *produto, ItemCardapio data, Cliente *cliente){
    produto = new ListaPedido;

    if(cliente->pedido.listapedido!=NULL) {
        produto->prox = cliente->pedido.listapedido;
        cliente->pedido.listapedido->ante = produto;
    }
    cliente->pedido.listapedido=produto;

    produto->data=data;

    system("CLS");
    cout<<produto->data.nome<<" adicionado ao pedido.\n\n";

    cliente->pedido.conta+=produto->data.preco;
    cliente->pedido.totalitens++;
}
void novoitem(Cliente *cliente){
    showcardapio(true);
    int idin;
    cout<<"ADICIONAR ITEM---------\n";

    char idhold[256];
    cout<<"Insira o ID: #";
    cin.getline(idhold, sizeof(idhold));
    if(validstr(idhold))idin=strtoint(idhold);
    else idin=-1;

    for(int i=0; i<itens; i++){
        if(item[i].id==idin && idin>0) {
            itemalloc(cliente->pedido.listapedido, item[i], cliente);
            break;
        }
        if(i==itens-1){
            system("CLS");
            cout<<"ID inv�lido.\n\n";
        }
    }
}

void itemdelloc(ListaPedido *produto, Cliente *cliente){
    if(produto!=cliente->pedido.listapedido){
        produto->ante->prox = produto->prox;
        if(produto->prox!=NULL) produto->prox->ante = produto->ante;
    } else{
        if(produto->prox!=NULL){
            produto->prox->ante = NULL;
            cliente->pedido.listapedido = produto->prox;
        }
    }

    system("CLS");
    cout<<produto->data.nome<<" removido do pedido.\n\n";

    cliente->pedido.conta-=produto->data.preco;
    cliente->pedido.totalitens--;
    delete produto;
}
void rmvitem(Cliente *cliente){
    cout<<"PEDIDO-----------------\n";
    showlistapedidos(cliente, true, true);

    cout<<"REMVOVER ITEM----------\n";
    int idin;
    char idhold[256];
    cout<<"Insira o ID: #";
    cin.getline(idhold, sizeof(idhold));
    if(validstr(idhold))idin=strtoint(idhold);
    else idin=-1;

    ListaPedido *aux=cliente->pedido.listapedido;
    for(int i=0; i<cliente->pedido.totalitens; i++){
        if(aux->data.id==idin) {
            itemdelloc(aux, cliente);
            break;
        }
        if(i==cliente->pedido.totalitens-1){
            system("CLS");
            cout<<"ID inv�lido.\n\n";
        }
        aux=aux->prox;
    }
}

void destroylista(Cliente *cliente){
    for(int i=0; i<cliente->pedido.totalitens; i++){
        ListaPedido *aux=cliente->pedido.listapedido;
        cliente->pedido.listapedido=cliente->pedido.listapedido->prox;
        delete aux;
    }
}
void finalizar(Cliente *cliente){
    bool loop=true;
    char auxin[256];

    while(loop){
        cout<<"FINALIZAR PEDIDO-------"<<
        endl<<"Defina o ID do pedido: #";
        cin.getline(auxin, sizeof(auxin));

        if(validstr(auxin) && strtoint(auxin)>-1)cliente->pedido.pedidoid=strtoint(auxin);
        else cliente->pedido.pedidoid=-1;

        if(listacont==1 && cliente->pedido.pedidoid>0) break;
        if(cliente->pedido.pedidoid<=0){
            system("CLS");
            cout<<"ID inv�lido.\n\n";
        } else{
            Cliente *aux = listahead;
            for(int i=0; i<listacont-1; i++){
                if(aux->pedido.pedidoid == cliente->pedido.pedidoid){
                    system("CLS");
                    cout<<"ID existente.\n\n";
                    loop=true;
                    break;
                } else loop=false;
                aux=aux->prox;
            }
        }
    }
    int input;

    cout<<"\nATENDIMENTO------------"<<
    endl<<"1-P�ssimo"<<
    endl<<"2-Ruim"<<
    endl<<"3-Mediano"<<
    endl<<"4-Bom"<<
    endl<<"5-�timo"<<
    endl<<"0-Anular";

    cout<<"\n\nAvalie o atendimento: ";
    cin.getline(auxin, sizeof(auxin));
    if(validstr(auxin) && strtoint(auxin)>-1)input=strtoint(auxin);
    else input=0;

    if(input<0) input=0;
    else if(input>5) input=5;

    caixa+=cliente->pedido.conta;

    if(input>0){
        totalclientes++;
        novanota(cliente->pessoal, input);

        system("CLS");
        cout<<"Avaliac�o registrada.\n\n";
    } else system("CLS");
}
void novopedido(Cliente *cliente){
    filatolista();

    int opt=-1;
    char opthold[256];
    bool loop=true;
    while(loop){
        cout<<"NOVO PEDIDO------------"<<
        endl<<"1-Adicionar Item"<<
        endl<<"2-Remover Item"<<
        endl<<"3-Vizualizar Pedido"<<
        endl<<"4-Finalizar Pedido";

        cout<<"\n\nAtendendo "<<cliente->pessoal.nome<<", "<<cliente->pessoal.idade<<" anos.";
        cout<<"\nMenu: ";
        cin.getline(opthold, sizeof(opthold));
        if(validstr(opthold))opt=strtoint(opthold);
        else opt=-1;

        system("CLS");
        switch(opt){
            case 1:
                novoitem(cliente);
                break;

            case 2:
                if(cliente->pedido.totalitens>0)rmvitem(cliente);
                else cout<<"Sem itens na lista.\n\n";
                break;

            case 3:
                if(cliente->pedido.totalitens>0){
                    cout<<"PEDIDO-----------------\n";
                    showlistapedidos(cliente,false,true);
                }
                else cout<<"Sem itens na lista.\n\n";
                break;

            case 4:
                if(cliente->pedido.totalitens>0){
                    finalizar(cliente);
                    loop=false;
                }
                else cout<<"� necess�rio pelo menos um item.\n\n";
                break;

            default: cout<<"Valor n�o encontrado.\n\n";
        }
    }
}

/*CARD�PIO*/
void addproduto(){
    int i;
    char aux[256];
    for(i=0; i<itens; i++) if(strcmp(item[i].nome,"")==0) break;

    cout<<"NOVO PRODUTO-----------"<<
    endl<<"Nome: ";
    cin.getline(aux,sizeof(aux));

    if(validstr(aux)) strcpy(item[i].nome,aux);
    else {
        system("CLS");
        return;
    }

    while(1){
        char precohold[256];
        cout<<"Pre�o: R$";
        cin.getline(precohold, sizeof(precohold));

        if(validstr(precohold) && strtoint(precohold)>-1){
            item[i].preco=strtoint(precohold);
            break;
        } else{
            system("CLS");
            cout<<"NOVO PRODUTO-----------"<<
            endl<<"Nome: "<<item[i].nome<<endl;
        }
    }

    item[i].id=i+1;

    system("CLS");
    cout<<item[i].nome<<" adicionado ao card�pio.\n\n";

    totalprodutos++;
}
void rmvproduto(){
    showcardapio(true);
    char idhold[256];
    int idin;

    cout<<"REMOVER PRODUTO--------"<<
    endl<<"Insira o ID: #";
    cin.getline(idhold, sizeof(idhold));

    if(validstr(idhold))idin=strtoint(idhold);
    else idin=-1;

    system("CLS");

    for(int i=0; i<itens; i++){
        if(item[i].id==idin && idin>0) {
            cout<<item[i].nome<<" removido do card�pio.\n\n";
            strcpy(item[i].nome,"");
            item[i].id=-1;
            item[i].preco=0;
            totalprodutos--;
            break;
        }
        if(i==itens-1){
            cout<<"ID inv�lido.\n\n";
        }
    }
}
void edtproduto(){
    showcardapio(true);

    char idhold[256];
    int idin;
    cout<<"EDITAR PRODUTO---------"<<
    endl<<"Insira o ID: #";
    cin.getline(idhold, sizeof(idhold));
    if(validstr(idhold))idin=strtoint(idhold);
    else idin=-1;

    system("CLS");

    for(int i=0; i<itens; i++){
        if(item[i].id==idin && idin>0) {
            char aux[256];

            cout<<"Editando "<<item[i].nome<<" - R$";
            printf("%05.2f\n",item[i].preco);
            cout<<"EDITAR PRODUTO---------"<<
            endl<<"Nome: ";
            cin.getline(aux,sizeof(aux));

            if(strcmp(aux,"")!=0) strcpy(item[i].nome,aux);
            else {
                system("CLS");
                return;
            }

            while(1){
                char precohold[256];
                cout<<"Pre�o: R$";
                cin.getline(precohold, sizeof(precohold));

                if(validstr(precohold) && strtoint(precohold)>-1){
                    item[i].preco=strtoint(precohold);
                    break;
                } else cout<<"Valor inv�lido.\n\n";
            }
            system("CLS");
            cout<<"Produto atualizado.\n\n";
            break;
        }
        if(i==itens-1){
            cout<<"ID inv�lido.\n\n";
        }
    }
}
void managecardapio(){
    char opthold[256];
    int opt=-1;
    while(opt!=0){
        cout<<"GERENCIAR CARDAPIO-----"<<
        endl<<"1-Adicionar Produto"<<
        endl<<"2-Remover Produto"<<
        endl<<"3-Editar Produto"<<
        endl<<"4-Exibir Card�pio"<<
        endl<<"0-Retornar";

        cout<<"\n\nMenu: ";
        cin.getline(opthold, sizeof(opthold));

        if(validstr(opthold))opt=strtoint(opthold);
        else opt=-1;

        system("CLS");

        switch(opt){
            case 0: break;
            case 1:
                if(totalprodutos<itens) addproduto();
                else cout<<"Card�pio cheio.\n\n";
                break;

            case 2:
                if(totalprodutos>1) rmvproduto();
                else cout<<"� necess�rio ter pelo menos um produto.\n\n";
                break;

            case 3:
                edtproduto();
                break;

            case 4:
                showcardapio(true);
                break;

            default: cout<<"Valor n�o encontrado.\n\n";
        }
    }
}

void dothething(){
        cout<<
        endl<<"                           888                        888     888"<<
        endl<<"                           888                        888     888"<<
        endl<<"                           888                        888     888"<<
        endl<<" 88888b.d88b.  .d8888b .d88888 .d88b. 88888b.  8888b. 888 .d88888"<<
        endl<<" 888 '888 '88bd88P'   d88' 888d88""88b888888888b  '88b888d888888888"<<
        endl<<" 888  888  888888     888  888888  888888  888.d888888888888  888"<<
        endl<<" 888  888  888Y88b.   Y88b 888Y88..88P888  888888  888888Y88b 888"<<
        endl<<" 888  888  888 'Y8888P 'Y88888 'Y88P' 888  888'Y888888888 'Y88888"<<endl<<endl;
}
int main(){
    setlocale(LC_ALL, "portuguese");
    iniciacardapio();

    int opt=-1;
    char opthold[256];
    while(opt!=0){
        cout<<"INICIO-----------------"<<
        endl<<"1-Novo Cliente"<<
        endl<<"2-Fila de Espera"<<
        endl<<"3-Atender Cliente"<<
        endl<<"4-Pedidos Pendentes"<<
        endl<<"5-Entregar Pedido"<<
        endl<<"6-Buscar Cliente"<<
        endl<<"7-Gerenciar Card�pio"<<
        endl<<"8-Avali��es"<<
        endl<<"9-Caixa"<<
        endl<<"0-Sair";

        cout<<"\n\nMenu: ";
        cin.getline(opthold, sizeof(opthold));
        if(validstr(opthold))opt=strtoint(opthold);
        else opt=-1;

        if(opt!=0)system("CLS");

        switch(opt){
            case 0:{
                cout<<"Sair? 1-Sim | 2-N�o ";
                cin.getline(opthold, sizeof(opthold));
                if(validstr(opthold))opt=strtoint(opthold);
                else opt=-1;

                if(opt==1){
                    opt=0;
                    dothething();
                } else{
                    opt=-1;
                    system("CLS");
                }
                break;
            }
            case 1:
                addcliente();
                break;

            case 2:
                if(filacont>0) showfila();
                else cout<<"Sem clientes na fila.\n\n";
                break;

            case 3:
                if(filacont>0) novopedido(filahead);
                else cout<<"Sem clientes na fila.\n\n";
                break;

            case 4:
                if(listacont>0) showlista();
                else cout<<"Sem pedidos pendentes.\n\n";
                break;

            case 5:
                if(listacont>0) deletecliente();
                else cout<<"Sem pedidos pendentes.\n\n";
                break;

            case 6:
                if(listacont>0 && filacont>0) buscacliente();
                else if(listacont==0 && filacont>0) buscaemfila();
                else if(listacont>0 && filacont==0) buscaemlista();
                else cout<<"Sem clientes no momento.\n\n";
                break;

            case 7:
                 managecardapio();
                 break;

            case 8:
                if(totalclientes>0)shownotas();
                else cout<<"Sem avalia��es registradas.\n\n";
                break;

            case 9:
                printf("Caixa: R$%.2f\n\n",caixa);
                break;

            default: cout<<"Valor n�o encontrado.\n\n";
        }
    }
    return 0;
}
//
