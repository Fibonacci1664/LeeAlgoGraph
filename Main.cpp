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

bool flood(std::list<Vertex*>& graph, std::string& start, std::string& target)
{
	// Start from starting point
	// flood with cumulative distance from starting point until end
	// Check for blocked

	bool targetFound = false;
	bool setStartDist = false;
	bool updatedValue = false;
	int currDist = 0;
	int count = 0;

	while (!targetFound || updatedValue)
	{
		updatedValue = false;

		for (auto it = graph.begin(); it != graph.end(); ++it)
		{
			++count;
			Vertex* v = *it;

			if (!setStartDist)
			{
				if (v->m_name == start)
				{
					v->m_distance = currDist;
					setStartDist = true;
				}
				else
				{
					continue;
				}
			}

			// If our current vertex is blocked or its current distance is < 0, move to the next vertex.
			// This is because if it has not been assigned a value then it is connected to a blocker vertex.
			if (v->m_blocked || v->m_distance < 0)
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
					updatedValue = true;

					if (neighbourV->m_name == target && neighbourV->m_distance > 0)
					{
						targetFound = true;
						break;
					}
				}
			}

			// If we've found our target vertex, stop flooding.
			if (targetFound)
			{
				std::cout << "\nThere was a path!\n";
				updatedValue = false;
				break;
			}
		}

		if (!updatedValue)
		{
			break;
		}
	}

	return targetFound;
}

std::list<Vertex*> trace(std::list<Vertex*>& graph, std::string& start)
{
	std::list<Vertex*> trace;
	bool startFound = false;
	bool setStartTrace = false;
	int currDist = 0;

	// Use reverse iterator to move from the last node to the start
	// Checking for a distance 1 less than curr distance
	// Once we find that, save that vertex in a list of vertex.

	while (!startFound)
	{
		for (auto it = graph.begin(); it != graph.end(); ++it)
		{
			Vertex* v = *it;

			// If the vertex we're on is blocked, or has a value < zero, then move on.
			// i.e. our current vertex was never assigned a distance value, more than likely because it was connected to a blocker.
			if (v->m_blocked || v->m_distance < 0)
			{
				continue;
			}

			// For the first non-blocked, positive value vertex we find, save that as the start of our trace.
			if (!setStartTrace)
			{
				// Keep looping through until we find the place where we want to start, namely the end position of our flood path.
				if (v->m_name == start)
				{
					trace.push_back(v);
					currDist = v->m_distance;
					setStartTrace = true;
				}
				else
				{
					continue;
				}
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

	vd.m_blocked = true;
	//vf.m_blocked = true;
	std::list<Vertex*> graph{ &va, &vb, &vc, &vd, &ve, &vf, &vg, &vh, &vi };

	display(graph);
	std::string start = "A";
	std::string target = "I";
	bool isPath = flood(graph, start, target);
	std::cout << '\n';
	display(graph);
	std::cout << '\n';

	if (isPath)
	{
		std::list<Vertex*> backTrace = trace(graph, target);
		display(backTrace);
	}
	else
	{
		std::cout << "There was not a path!\n";
	}	
}

int main()
{
	// initialize random seed
	//srand(time(NULL));

	test();

	return 0;
}