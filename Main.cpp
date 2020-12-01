#include<iostream>
#include<Windows.h>
#include <time.h>
#include <list>

// -1 = default staring values.

struct Vertex
{
	std::string m_name;
	bool m_blocked;
	int m_distance;	// -1 if not filled in yet
	std::list<Vertex*> m_neighbours;

	Vertex(std::string name) : m_name(name), m_blocked(false), m_distance(-1) {}
};

void connectVerts(Vertex& v1, Vertex& v2)
{
	v1.m_neighbours.push_back(&v2);
	v2.m_neighbours.push_back(&v1);
}

void display(std::list<Vertex*>& graph)
{
	for (auto v : graph)
	{
		std::cout << v->m_name << ": " << v->m_distance << std::endl;
	}
}

void flood(std::list<Vertex*>& graph)
{
	// Start from starting point
	// flood with cumulative distance from starting point until end
	// Check for blocked

	bool setStartDist = false;
	int currDist = 0;

	for (auto it = graph.begin(); it != graph.end(); ++it)
	{
		Vertex* v = *it;

		if (!setStartDist)
		{
			v->m_distance = currDist;
			setStartDist = true;
		}

		// If our current vertex is blocked move to the next one.
		if (v->m_blocked)
		{
			continue;
		}

		currDist = v->m_distance;

		// Check neighbours of each vertex to see if its blocked
		// If not assign it a distance value of curr distance + 1
		for (auto jit = v->m_neighbours.begin(); jit != v->m_neighbours.end(); ++jit)
		{
			Vertex* neighbourV = *jit;

			if (neighbourV->m_blocked)
			{
				continue;
			}
			else if (neighbourV->m_distance == -1)
			{
				neighbourV->m_distance = currDist + 1;
			}
		}
	}
}

std::list<Vertex*> trace(std::list<Vertex*>& graph)
{
	std::list<Vertex*> trace;
	bool startFound = false;
	bool setStartTrace = false;
	int currDist = 0;

	// Use reverse iterator to move from the last node to the start
	// Checking for a distance 1 less than curr distance
	// Once we find that, save that vertex in a list of vertex.

	for (auto it = graph.rbegin(); it != graph.rend(); ++it)
	{
		Vertex* v = *it;

		// If the vertex we're on is blocked, or has a value of zero other, then move on.
		// The zero check doesn't affect the start vertex for 2 reasons.
		// 1. We're starting at the end and so looking for a number > 0.
		// 2. When we do find the start which has a value of zero, this is checked before we get back to this point in the last iteration
		// i.e. we set start found and then break out of all loops, so we never get back here to check for zero on the start vertex.
		if (v->m_blocked || v->m_distance == 0)
		{
			continue;
		}

		// For the first non-blocked vertex we find, save that as the start of our trace.
		if (!setStartTrace)
		{
			trace.push_back(v);
			currDist = v->m_distance;
			setStartTrace = true;
		}

		// If our current vertex distance is greater than the distance we just saved on the last iteration, continue.
		if (v->m_distance > currDist)
		{
			continue;
		}

		// Check neighbours of each vertex looking for a vertex with a distance of 1 less than our curr distance.
		for (auto jit = v->m_neighbours.rbegin(); jit != v->m_neighbours.rend(); ++jit)
		{
			Vertex* neighbourV = *jit;

			if (neighbourV->m_distance == (currDist - 1))
			{
				// Save vertex. and move here. i.e. dont check anymore neighbours, so break from this loop.
				trace.push_back(neighbourV);
				currDist = neighbourV->m_distance;

				// Check if we have found the start.
				if (neighbourV->m_distance == 0)
				{
					startFound = true;
				}

				break;
			}
		}

		if (startFound)
		{
			break;
		}
	}

	return trace;
}

void test()
{
	// Create Vertices.
	Vertex va("A");
	Vertex vb("B");
	Vertex vc("C");
	Vertex vd("D");
	Vertex ve("E");
	Vertex vf("F");
	Vertex vg("G");
	Vertex vh("H");
	Vertex vi("I");

	// Connect vertices.
	connectVerts(va, vb);
	connectVerts(vb, vc);
	connectVerts(vb, vd);
	connectVerts(vc, ve);
	connectVerts(vd, vg);
	connectVerts(vd, vf);
	connectVerts(ve, vf);
	connectVerts(vf, vh);
	connectVerts(vf, vi);
	connectVerts(vh, vi);

	ve.m_blocked = true;
	std::list<Vertex*> graph{ &va, &vb, &vc, &vd, &ve, &vf, &vg, &vh, &vi };

	display(graph);
	flood(graph);
	std::cout << '\n';
	display(graph);
	std::cout << '\n';
	std::list<Vertex*> backTrace = trace(graph);
	display(backTrace);
}

int main()
{
	// initialize random seed
	//srand(time(NULL));

	test();

	return 0;
}