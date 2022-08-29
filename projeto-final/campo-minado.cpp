#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <locale.h>

using namespace std;

int analisa(char [40][40],int, int );
void geraJogo(char [40][40], char [40][40], char [40][40]);
void imprimirmatriz(char [40][40]);
void mininhas(char [40][40],char [40][40],char, int, int );
void descobrir(char [40][40],char [40][40],bool&,int, int );
void cafifosa(char [40][40],char [40][40],int,int );
void revela(char matriz1[40][40],char matriz2[40][40], int linhas, int colunas);
bool parada(char matriz2[40][40],int linhas,int colunas,int contminas);
bool testarmina();
void menu();
void menufinal();
void exibirranking();
void memory();
void atualizaranking(int dt,int m);

int linhas,colunas,minas,contminas,dif,tempo;
char matriz1[40][40],matriz2[40][40],matriz3[40][40];
bool jogando=false;

struct dados{
    char name[80];
    int time;
};

dados p1i,p2i,p3i,p1d,p2d,p3d;

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int o,dt;

    srand(time(NULL));
    memory();
    menu();

    while(jogando)
    {
        dt=time(NULL)-tempo;

        if(parada(matriz1,linhas,colunas,contminas))
        {
            revela(matriz1,matriz3,linhas,colunas);
            if(dif==1)
                atualizaranking(dt,dif);
            else
                atualizaranking(dt,dif);
            imprimirmatriz(matriz1);
            cout<<endl<<endl<<"VOCÊ É TOP, CONSEGUIU DESCOBRIR TODAS AS MINAS, CARA, MEUS PARABÉNS!!!"<<endl;
            jogando=false;
            menufinal();
            break;
        }

        int col, lin;
        char op;

        imprimirmatriz(matriz1);

        cout<<"Minas a marcar: "<<contminas<<endl;
        cout<<"D --> Descobrir quadrado\nM --> Marcar Mina\nT --> Talvez Mina \nL --> Limpar Marcação\nS --> Sair"<<endl;
        cout<<"Digite a opção seguida das coordenadas do quadrado: ";
        cin>>op>>lin>>col;

        if(col<=colunas && lin<=linhas)
        {
            switch(op)
            {
            case 'D':
                cout<<endl<<"Tempo de Jogo: "<<dt<<" segundo(s)"<<endl;
                descobrir(matriz1,matriz2,jogando,lin-1,col-1);
                break;
            case 'M':
                cout<<endl<<"Tempo de Jogo: "<<dt<<" segundo(s)"<<endl;
                mininhas(matriz1,matriz2,op,lin-1,col-1);
                //contminas--;
                break;
            case 'T':
                cout<<endl<<"Tempo de Jogo: "<<dt<<" segundo(s)"<<endl;
                mininhas(matriz1,matriz2,op,lin-1,col-1);
                break;
            case 'L':
                cout<<endl<<"Tempo de Jogo: "<<dt<<" segundo(s)"<<endl;
                mininhas(matriz1,matriz2,op,lin-1,col-1);
                break;
            case 'S':
                jogando=false;
                break;
            default:
                cout<<"Opção incorreta"<<endl;
            }
        }
        else
        {
            imprimirmatriz(matriz1);
            cout<<endl<<"NÚMERO DE LINHAS OU COLUNAS INCORRETO, DIGITE NOVAMENTE."<<endl<<endl;
        }
    }
}

void menu()
{
    int o;
    cout<<"Bem-Vindo ao Campo Minado, digite um número:"<<endl<<"1 - Novo Jogo"<<endl<<"2 - Melhor Tempo"<<endl<<"3 - Sair"<<endl;
    cin>>o;
    switch(o)
    {
    case 1:
        cout<<"Digite a dificuldade do Jogo desejada:\n1-Fácil\n2-Difícil\n3-Personalizado"<<endl;
        cin>>dif;
        switch(dif)
        {
        case 1:
            linhas=colunas=8;
            minas=10;
            contminas=minas;
            jogando=true;
            break;
        case 2:
            linhas=colunas=16;
            minas=40;
            contminas=minas;
            jogando=true;
            break;
        case 3:
            cout<<"Digite o número de Linhas do jogo(máximo 40): "<<endl;
            cin>>linhas;
            cout<<"Digite o número de Colunas do jogo(máximo 40): "<<endl;
            cin>>colunas;
            cout<<"Digite o número de Minas do jogo: "<<endl;
            cin>>minas;

            while(linhas>40 && colunas>40 && minas<=((colunas*linhas)-10))
            {
                if(linhas>40)
                {
                    cout<<"Numero de Linhas Inválido, digite novamente: "<<endl;
                    cin>>linhas;
                }

                else if(colunas>40)
                {
                    cout<<"Numero de Colunas Inválido, digite novamente: "<<endl;
                    cin>>colunas;
                }

                else if(((linhas*colunas)-10)>=minas)
                {
                    cout<<"Numero de Minas Inválido, digite novamente: "<<endl;
                    cin>>minas;
                }
            }
            contminas=minas;
            jogando=true;
            break;

        }
        break;
    case 2:
        exibirranking();
        break;
    case 3:
        break;
    default:
        cout<<"Número inválido, inicie o jogo novamente."<<endl;
    }
    geraJogo(matriz1,matriz2,matriz3);
}

void menufinal()
{   int e;
    cout<<"Deseja jogar Novamente?\n1-Sim\n2-Qualquer tecla"<<endl;
    cin>>e;

    if(e==1)
        menu();
}

void geraJogo(char matriz1[40][40], char matriz2[40][40], char matriz3[40][40])
{
    int geraLinha,geraColuna;
    tempo=time(NULL);

    for(int n=0; n<minas; n++)
    {
        geraLinha=rand()%linhas;
        geraColuna=rand()%colunas;
        if(matriz2[geraLinha][geraColuna]=='*')
        {
            n--;
        }
        else
            matriz2[geraLinha][geraColuna]='*';
    }

    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
        {
            matriz1[i][j]='.';

            if(matriz2[i][j]!='*')
                matriz2[i][j]='0'+analisa(matriz2,i,j);

        }
    }

    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
            matriz3[i][j]=matriz2[i][j];
    }
}

void imprimirmatriz(char matriz[40][40])
{
    int v[50];
    for(int i=0; i<50; i++)
    {
        v[i]=i+1;
    }
    cout<<endl<<"   ";

    for(int i=0; i<colunas; i++)
        if(i<9)
            cout<<"0"<<v[i]<<" ";
        else
            cout<<v[i]<<" ";

    cout<<endl<<endl;

    for(int i=0; i<linhas; i++)
    {
        if(i<9)
            cout<<"0"<<v[i]<<"  ";
        else
            cout<<v[i]<<"  ";

        for(int j=0; j<colunas; j++)
        {
            cout<< matriz[i][j]<< "  ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int analisa(char matriz2[40][40],int m, int n)
{
    int cont=0;

    for(int i=m-1; i<=m+1; i++)
    {
        for(int j=n-1; j<=n+1; j++)
        {
            if(matriz2[i][j]=='*')
            {
                cont++;
            }
        }
    }
    return cont;
}

void descobrir(char matriz1[40][40],char matriz2[40][40],bool& jogando,int lin, int col)
{

    if(matriz2[lin][col]=='*')
    {
        matriz1[lin][col]=matriz2[lin][col];
        imprimirmatriz(matriz1);
        cout<<"VOCÊ ESCOLHEU UMA MINA E PERDEU O JOGO"<<endl;
        jogando=false;
        menufinal();
    }

    else if(matriz2[lin][col]=='0')
    {
        matriz1[lin][col]=matriz2[lin][col];
        cafifosa(matriz1,matriz2,lin,col);
    }
    else
    {
        matriz1[lin][col]=matriz2[lin][col];
    }
}

void cafifosa(char matriz1[40][40],char matriz2[40][40],int lin,int col)
{
    for(int i=lin-1; i<=lin+1; i++)
    {
        for(int j=col-1; j<=col+1; j++)
        {
            if(matriz1[i][j]!='M' && matriz1[i][j]!='T')
            {
                matriz1[i][j]=matriz2[i][j];

                if(matriz2[i][j]=='0')
                {
                    matriz2[i][j]='_';
                    cafifosa(matriz1,matriz2,i,j);
                }

                else if(matriz2[lin][col]>'0' && matriz2[lin][col]<='8')
                    matriz1[lin][col]=matriz2[lin][col];
            }
        }
    }
}

void mininhas(char matriz1[40][40],char matriz2[40][40],char op, int lin, int col)
{
    if(op=='M' && matriz1[lin][col]!='M')
    {
        matriz1[lin][col]='M';
        contminas--;
    }
    else if(op=='T' && matriz1[lin][col]!='T')
    {
        matriz1[lin][col]='?';
    }
    else if(op=='L' && matriz1[lin][col]=='?' || matriz1[lin][col]=='M')
    {
        matriz1[lin][col]='.';
        contminas++;
    }
    else
    {
        cout<<"Opção inválida, digite novamente."<<endl;
    }
}

void revela(char matriz1[40][40],char matriz2[40][40], int linhas, int colunas)
{
    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
            matriz1[i][j]=matriz2[i][j];
    }
}

bool parada(char matriz1[40][40],int linhas,int colunas,int contminas)
{
    int cont=0;

    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
        {
            if(!(matriz1[i][j]=='.' && matriz1[i][j]=='?'))
                cont++;
        }
    }

    if(cont==(linhas*colunas-minas))
        return true;

    if(testarmina())
        return true;

    else
        return false;
}

bool testarmina()
{
    int cont=0;

    for(int i=0; i<linhas; i++)
    {
        for(int j=0; j<colunas; j++)
        {
            if(matriz1[i][j]=='M' && matriz2[i][j]=='*')
                cont++;

        }
    }

    if(cont==10)
        return true;
    else
        return false;
}

void exibirranking()
{
    cout<<endl<<endl;
    cout<<"Ranking do Modo Iniciante: "<<endl;
    cout<<"Nomes:            Tempos:"<<endl;
    cout <<p1i.name <<" --------> "<<p1i.time<<" segundo(s)"<<endl;
    cout <<p2i.name <<" --------> "<<p2i.time<<" segundo(s)"<<endl;
    cout <<p3i.name <<" --------> "<<p3i.time<<" segundo(s)"<<endl;
    cout<<endl<<endl;
    cout<<"Ranking do Modo Difícil: "<<endl;
    cout<<"Nomes:            Tempos:"<<endl;
    cout <<p1d.name <<" --------> "<<p1d.time<<" segundo(s)"<< endl;
    cout <<p2d.name <<" --------> "<<p2d.time<<" segundo(s)"<<endl;
    cout <<p3d.name <<" --------> "<<p3d.time<<" segundo(s)"<<endl;

    cout<<endl<<endl;

    menu();
}

void memory()
{
    ifstream salvamento1,salvamento2;
    salvamento1.open("arquivoiniciante.txt");
    salvamento2.open("arquivodificil.txt");

    if(!salvamento1.is_open())
    {
        cout<<"Erro, arquivo não aberto"<<endl;
        exit(1);
    }
    if(!salvamento2.is_open())
    {
        cout<<"Erro, arquivo não aberto"<<endl;
        exit(1);
    }

    salvamento1>>p1i.name;
    salvamento1>>p1i.time;
    salvamento1>>p2i.name;
    salvamento1>>p2i.time;
    salvamento1>>p3i.name;
    salvamento1>>p3i.time;

    salvamento2>>p1d.name;
    salvamento2>>p1d.time;
    salvamento2>>p2d.name;
    salvamento2>>p2d.time;
    salvamento2>>p3d.name;
    salvamento2>>p3d.time;

    salvamento1.close();
    salvamento2.close();
}

void atualizaranking(int dt,int m)
{
    char nome[80];
    ofstream ranking1,ranking2;
    cin.ignore();
    cout<<"Digite seu PRIMEIRO NOME: "<<endl;
    cin.getline(nome,80);

    if(m==1)
    {
        ranking1.open("arquivoiniciante.txt");
        if(dt<p3i.time)
        {
            strcpy(p3i.name,nome);
            p3i.time=dt;
        }
        else if(dt<p2i.time)
        {
            strcpy(p3i.name,p2i.name);
            p3i.time=p2i.time;
            strcpy(p2i.name,nome);
            p2i.time=dt;
        }
        else if(dt<p1i.time)
        {
            strcpy(p3i.name,p2i.name);
            p3i.time=p2i.time;
            strcpy(p2i.name,p1i.name);
            p2i.time=p1i.time;
            strcpy(p1i.name,nome);
            p1i.time=dt;
        }

        ranking1<<p1i.name<<endl;
        ranking1<<p1i.time<<endl;
        ranking1<<p2i.name<<endl;
        ranking1<<p2i.time<<endl;
        ranking1<<p3i.name<<endl;
        ranking1<<p3i.time<<endl;

        ranking1.close();
    }
    else if(m==2)
    {
        ranking2.open("arquivodificil.txt");

        if(dt<p3d.time)
        {
            strcpy(p3d.name,nome);
            p3d.time=dt;
        }
        else if(dt<p2d.time)
        {
            strcpy(p3d.name,p2d.name);
            p3d.time=p2d.time;
            strcpy(p2d.name,nome);
            p2d.time=dt;
        }
        else if(dt<p1i.time)
        {
            strcpy(p3d.name,p2d.name);
            p3d.time=p2d.time;
            strcpy(p2d.name,p1d.name);
            p2d.time=p1d.time;
            strcpy(p1d.name,nome);
            p1d.time=dt;
        }

        ranking2<<p1d.name<<endl;
        ranking2<<p1d.time<<endl;
        ranking2<<p2d.name<<endl;
        ranking2<<p2d.time<<endl;
        ranking2<<p3d.name<<endl;
        ranking2<<p3d.time<<endl;

        ranking2.close();
    }

}

