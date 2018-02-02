/*
Copyright 2018 Pierre-Edouard Portier
peportier.me

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
)
*/

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <list>
#include <functional>
#include <utility>
#include <limits>

using namespace std;

struct
node
{
  int val;
  vector< pair< int, node* > > nei;
  node() {};
  node(int i) : val(i) {}
};

void
dijkstra( node* src, node* target, list<node*>& path )
{
  map< node*, node* > parent;
  parent[src] = src;

  map< node*, int> d;
  d[src] = 0;

  set<node*> x;

  priority_queue< pair< int, node* > ,
                  vector< pair< int, node* > >,
                  greater< pair< int, node* > > > y;

  y.push({0, src});
  //x.insert(src);

  function<int(node*)>
  dist = [&]( node* v )
  {
    map< node*, int >::const_iterator it = d.find(v);
    if( d.end() == it )
    {
      return numeric_limits<int>::max();
    }
    return it->second;
  };

  while( !y.empty() )
  {
    node* u = y.top().second;

    if( target == u ) // once the target is found, we rebuild the path to it
    {
      path.clear();
      while( u != src )
      {
        path.push_front(u);
        u = parent[u];
      }
      path.push_front(src);
      return;
    }

    y.pop(); x.insert(u);  // u, the 'smallest' grey node, becomes black

    for( pair< int, node* >& n : u->nei )
    {
      if( x.end() != x.find(n.second) ) continue; // if n is black, do nothing...

      // We don't make a special case if n is already an element of y.
      // Indeed, the STL priority queue doesn't have an increaseKey operation.
      // Therefore, we tolerate duplicates.

      if( dist(n.second) > d[u] + n.first )
      {
        d[n.second] = d[u] + n.first;
        parent[n.second] = u;
        y.push( { d[n.second], n.second } );
      }
    }
  }
}
int
main()
{
  node n1(1); node n2(2); node n3(3);
  node n4(4); node n5(5); node n6(6);
  n1.nei.push_back( { 1, &n2 } );
  n1.nei.push_back( { 4, &n3 } );
  n2.nei.push_back( { 1, &n4 } );
  n2.nei.push_back( { 2, &n5 } );
  n4.nei.push_back( { 1, &n3 } );
  n4.nei.push_back( { 2, &n5 } );
  n5.nei.push_back( { 1, &n6 } );
  n6.nei.push_back( { 1, &n2 } );
  
  list<node*> path;
  dijkstra(&n1, &n3, path);
  
  for( node* n : path )
  {
    cout << n->val << " ; ";
  }
  cout << endl;
  return 0;
}
