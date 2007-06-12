/* graph.cpp */
/*
    Copyright 2001 Vladimir Kolmogorov (vnk@cs.cornell.edu), Yuri Boykov (yuri@csd.uwo.ca).

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <stdio.h>
#include "graph.h"

Graph::Graph(void (*err_function)(char *))
{
	error_function = err_function;
	node_block = new Block<node>(NODE_BLOCK_SIZE, error_function);
	arc_block  = new Block<arc>(NODE_BLOCK_SIZE, error_function);
	flow = 0;
}

Graph::~Graph()
{
	delete node_block;
	delete arc_block;
}

Graph::node_id Graph::add_node()
{
	node *i = node_block -> New();

	i -> first = NULL;
	i -> tr_cap = 0;

	return (node_id) i;
}

void Graph::add_edge(node_id from, node_id to, captype cap, captype rev_cap)
{
	arc *a, *a_rev;

	a = arc_block -> New(2);
	a_rev = a + 1;

	a -> sister = a_rev;
	a_rev -> sister = a;
	a -> next = ((node*)from) -> first;
	((node*)from) -> first = a;
	a_rev -> next = ((node*)to) -> first;
	((node*)to) -> first = a_rev;
	a -> head = (node*)to;
	a_rev -> head = (node*)from;
	a -> r_cap = cap;
	a_rev -> r_cap = rev_cap;
}

void Graph::set_tweights(node_id i, captype cap_source, captype cap_sink)
{
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	((node*)i) -> tr_cap = cap_source - cap_sink;
}

void Graph::add_tweights(node_id i, captype cap_source, captype cap_sink)
{
	register captype delta = ((node*)i) -> tr_cap;
	if (delta > 0) cap_source += delta;
	else           cap_sink   -= delta;
	flow += (cap_source < cap_sink) ? cap_source : cap_sink;
	((node*)i) -> tr_cap = cap_source - cap_sink;
}
