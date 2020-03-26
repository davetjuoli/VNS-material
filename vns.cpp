#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <set>
#define vb vector<bool>
#define TT 350
#define INF 987654321
using namespace std;

time_t ini, fim, tempo_max;
int qtde_vertices;
bool aresta[TT][TT];
vb g_original;


vb s_aleatoria2(int tam);
vb s_aleatoria3(int tam);
//vb s_aleatoria(int tam);
bool garras(const vb &s);
vb melhor_v(const vector<vb> &vizinhos, const vb &s);
int f_objetivo(const vb &s);
vb bagunca(const vb &s, int n);
vb BL1(const vb &s);
vb BL2(const vb &s);
vb VNS(const vb &s);

int main(){
  srand(time(NULL));
  vb s_inicial, s_final;
  string aux;
  int edge;
  //lê a entrada
  getline(cin,aux);
  cin >> qtde_vertices;
  for(int i=0; i<qtde_vertices; i++)
    cin >> aux;
  for(int i=0; i<qtde_vertices; i++){
    for(int j=i+1; j<qtde_vertices; j++){
      cin >> edge;
      if(edge > 0){
        aresta[i][j] = aresta[j][i] = 1;
      }
    }
  }
  if(qtde_vertices<=50)tempo_max=10;
  else if(qtde_vertices<=100)tempo_max=30;
  else if(qtde_vertices<=200)tempo_max=60;
  else tempo_max=120;
/*
vb s_aux = {0,1,1,1,0};
s_aux=BL1(s_aux);
for(auto v:s_aux) cout<<v<<" ";
cout<<endl<<f_objetivo(s_aux)<<endl;
exit(10);
*/
  s_inicial = s_aleatoria3(qtde_vertices);
	for(bool v : s_inicial)
    		cout << v << " ";
	cout<<"solucao inicial: "<<f_objetivo(s_inicial)<<endl;

  /*
  int cont=0;
  do{ 
    s_inicial = s_aleatoria(qtde_vertices);
    cont++;
    if(cont>20000){
      s_inicial= vb(qtde_vertices,0);
    }
  }while(garras(s_inicial));
  cout<<"uhu"<<endl;
  */
  
  //s_inicial= vb(qtde_vertices,0);
  /*
  cout << endl << f_objetivo(s_inicial) << endl;
  for(bool v : s_inicial)
    cout << v << " ";
  cout<<endl;
  */
  s_final = VNS(s_inicial);
  cout<<endl<<endl;
  for(bool v : s_final)
    cout << v << " ";
  cout<<endl<<"função objetivo = "<<f_objetivo(s_final);
  cout<<endl<<"tempo = "<<difftime(fim,ini);
  
  return 0;
}

vb s_aleatoria2(int tam){
  vb v_disponiveis(tam,1), s(tam,0), s_linha(tam,0);
  while(!v_disponiveis.empty()){
    int u = rand()%v_disponiveis.size();
    s_linha[u] = 1;
    if(!garras(s_linha))
      s = s_linha;
    v_disponiveis.erase(v_disponiveis.begin()+u);
  }
  return s;
}

vb s_aleatoria3(int tam){
  vb s = s_aleatoria2(tam), s_linha;
  for(int i=1; i<=9; i++){
    s_linha = s_aleatoria2(tam);
    if(f_objetivo(s_linha) < f_objetivo(s))
      s = s_linha;
  }
  return s;
}

/*
vb s_aleatoria(int tam){
  vb s;
  while(tam--)
    s.push_back(rand()%2);
  return s;
}
*/

int f_objetivo(const vb &s){
  int penalidade = 0;
  for(int i=0; i<qtde_vertices; i++)
    if(!s[i])
      penalidade++;
  if(garras(s)){
    penalidade += qtde_vertices;
  }
  return penalidade;
}

vb bagunca(const vb &s, int n){
  vb s_nova;
  set<int> indices;
  set<int>::iterator it;
  int x;
  for(int i=0; i<n; i++){
    do{
      x = rand()%s.size();
      it = indices.find(x);
    }while(it!=indices.end());
    indices.insert(x);
  }
  it = indices.begin();
  for(int i=0; i<s.size(); i++){
    if(i == *it){
      s_nova.push_back(!s[i]);
      it++;
    }
    else s_nova.push_back(s[i]);
  }
  cout<<n<<"   ";
  for(auto x:s_nova) cout<<x;
  cout<<endl;
  return s_nova;
}

vb melhor_v(const vector<vb> &vizinhos, const vb &s){
  int menor = INF, melhor_v_v;
  for(int i=0; i<vizinhos.size(); i++){
    int aux = f_objetivo(vizinhos[i]);
    if(aux <= menor){
      menor = aux;
      melhor_v_v = i;
    }
  }
  if(menor < f_objetivo(s))
    return vizinhos[melhor_v_v];
  return s;
}

vb BL1(const vb &s){
  vector<vb> vizinhos;
  for(int i=0; i<qtde_vertices; i++){
    vb aux;
    for(int j=0; j<qtde_vertices; j++){
      if(j==i)
        aux.push_back(!s[j]);
      else aux.push_back(s[j]);
    }
    vizinhos.push_back(aux);
  }
  return melhor_v(vizinhos, s);  
}

vb BL2(const vb &s){
  vector<vb> vizinhos;
  for(int i=0; i<qtde_vertices; i++){
    for(int j=0; j<qtde_vertices; j++){
      vb aux;
      for(int x=0; x<qtde_vertices; x++){
        if(x==i or x==j)
          aux.push_back(!s[x]);
        else aux.push_back(s[x]);
      }
      vizinhos.push_back(aux);
    }
  }
  return melhor_v(vizinhos,s);
}

vb VNS(const vb &s){
  time(&ini);
  vb melhor_final = s, s_linha, s_atual = s;
  int b = 1, k = 1, max_b = s.size()/2;
  bool melhorou;

  while(difftime(fim,ini) < tempo_max){
    melhorou = false;

    if(k == 1) s_linha = BL1(s_atual);
    else s_linha = BL2(s_atual);

    int aa = f_objetivo(s_linha), bb = f_objetivo(s_atual);
    if(aa < bb){
      cout<<'.';
      k = 1; //b = 1;

      s_atual = s_linha;
      melhorou = true;

      if(aa < f_objetivo(melhor_final)){
        melhor_final = s_linha;
        cout<<"m_pontuaç: "<<aa<<" ";
        for(auto x:s_linha) cout<<x;
        cout<<endl;   
      }
    }else{
      k++;
    }
    if(melhorou == false and k == 3){
      vb aux = s_atual;
      do{
        aux = bagunca(s_atual, b);
      }while(garras(aux));
      s_atual = aux;
      k = 1;
      b++;
      if(b > max_b)
        b = max_b;
    }
    time(&fim);
  }
  return melhor_final;
}

bool garras(const vb &s){
  int tam = qtde_vertices;
  for(int v1=0; v1<tam; v1++){
    if(s[v1]){
      for(int v2=v1+1; v2<tam; v2++){
        if(s[v2] and aresta[v1][v2]){
          for(int v3=0; v3<tam; v3++){
            bool caso1=false, caso2=false;
            if(s[v3] and v3!=v1 and v3!=v2){
              if(aresta[v1][v3] and !aresta[v2][v3])
                caso1=true;
              else if(aresta[v2][v3] and !aresta[v1][v3])
                caso2=true;
              for(int v4=0; v4<tam; v4++){
                if(s[v4] and v4!=v1 and v4!=v2 and v4!=v3 and !aresta[v3][v4]){
                  if(caso1 and aresta[v1][v4] and !aresta[v2][v4]) 
                    return true;
                  if(caso2 and aresta[v2][v4] and !aresta[v1][v4])
                    return true;
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}
