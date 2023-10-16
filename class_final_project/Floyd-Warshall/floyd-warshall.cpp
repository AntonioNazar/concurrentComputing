#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

void floyd_warshall(vector<vector<int>>& dist, const int V)
{
    for (int k=0; k < V; k++)
    {
        for (int i=0; i < V; i++)
        {
            for (int j=0; j < V; j++)
            {   if(k!=i){
                if (dist[i][k] != INT_MAX &&  dist[k][j] != INT_MAX)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
            }
        }
    }
}

void adicionarAresta(vector<vector<int>>& dist,
             int u, int v, int w)
{
    dist[u][v] = w;
}

int main()
{
    int V = 5;
    vector<vector<int>> dist(V, vector<int> (V, INT_MAX));

    adicionarAresta(dist, 0, 1, 3);
    adicionarAresta(dist, 0, 4, -4);
    adicionarAresta(dist, 0, 2, 8);
    adicionarAresta(dist, 1, 3, 1);
    adicionarAresta(dist, 1, 4, 7);
    adicionarAresta(dist, 2, 1, 4);
    adicionarAresta(dist, 3, 0, 2);
    adicionarAresta(dist, 3, 2, -5);
    adicionarAresta(dist, 4, 3, 6);
    
    for (int i=0; i < V; i++)
        dist[i][i] = 0;

    floyd_warshall(dist, V);
    cout << "Shortest distance between all pairs: " << endl;
    for (int i=0; i < V; i++)
        for (int j=0; j < V; j++)
            cout << i << "," << j << " : " << dist[i][j] << endl;
        cout << endl;
        
    return 0;
}