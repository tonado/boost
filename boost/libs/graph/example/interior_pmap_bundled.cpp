//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Copyright 2004 Trustees of Indiana University
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek, Douglas Gregor
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================

#include <boost/config.hpp>
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map.hpp>
#include <string>

using namespace std;
using namespace boost;

/*
  Interior Property Map Basics

  An interior property map is a way of associating properties
  with the vertices or edges of a graph. The "interior" part means
  that the properties are stored inside the graph object. This can be
  convenient when the need for the properties is somewhat permanent,
  and when the properties will be with a graph for the duration of its
  lifetime. A "distance from source vertex" property is often of this
  kind.

  Sample Output

  Jeremy owes Rich some money
  Jeremy owes Andrew some money
  Jeremy owes Jeff some money
  Jeremy owes Kinis some money
  Andrew owes Jeremy some money
  Andrew owes Kinis some money
  Jeff owes Jeremy some money
  Jeff owes Rich some money
  Jeff owes Kinis some money
  Kinis owes Jeremy some money
  Kinis owes Rich some money

 */

template <class EdgeIter, class Graph>
void who_owes_who(EdgeIter first, EdgeIter last, const Graph& G)
{
  while (first != last) {
    cout << G[source(*first, G)].first_name << " owes " 
         << G[target(*first, G)].first_name << " some money" << endl;
    ++first;
  }
}

struct VertexData
{
  string first_name;
};

int
main()
{
  {
    // Create the graph, and specify that we will use std::string to
    // store the first name's.
    typedef adjacency_list<vecS, vecS, directedS, VertexData> MyGraphType;
    
    typedef pair<int,int> Pair;
    Pair edge_array[11] = { Pair(0,1), Pair(0,2), Pair(0,3), Pair(0,4), 
                            Pair(2,0), Pair(3,0), Pair(2,4), Pair(3,1), 
                            Pair(3,4), Pair(4,0), Pair(4,1) };
    
    MyGraphType G(5);
    for (int i=0; i<11; ++i)
      add_edge(edge_array[i].first, edge_array[i].second, G);

    G[0].first_name = "Jeremy";
    G[1].first_name = "Rich";
    G[2].first_name = "Andrew";
    G[3].first_name = "Jeff";
    G[4].first_name = "Doug";
    
    who_owes_who(edges(G).first, edges(G).second, G);
  }

  cout << endl;

  return 0;
}
