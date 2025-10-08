// 
// CSE3081-1/AIE3051-1 Design and Analysis of Algorithm
// (c) 2025 Soang University
//
// Example: Using a min-heap-based priority queue to manage clients with priority
// CLIENTS25.cpp
//

#include "CLIENTS25.h"

CLIENTS::CLIENTS() {
	n_max_clients = DEFAULT_MAX_CLIENTS;
	clients = new CLIENT_RECORD[n_max_clients];
	PQ_initialize();
}

CLIENTS::CLIENTS(int n_max_clients) {
	this->n_max_clients = n_max_clients;
	clients = new CLIENT_RECORD[n_max_clients];
	PQ_initialize();
}

CLIENTS::~CLIENTS() {
	if (!clients)
		delete[] clients;
}

//
// client management
//
void CLIENTS::reset() {
	for (int i = 0; i < n_max_clients; i++) {
		clients[i].priority = NULL_PRIORITY;
		clients[i].index_to_pq = -1;
		clients[i].client_data = nullptr;
	}
	PQ_initialize();
}

void CLIENTS::reset(int n_max_clients) {
	if (!clients)
		delete[] clients;
	this->n_max_clients = n_max_clients;
	clients = new CLIENT_RECORD[n_max_clients];
	PQ_initialize();
}

bool CLIENTS::active_client_ID(unsigned int client_ID) {
	if ((client_ID < n_max_clients) && (clients[client_ID].priority != NULL_PRIORITY))
		return true;
	else
		return false;
}

void CLIENTS::add_client(unsigned int client_ID, int priority, CLIENT_DATA* client_data) {
	if (active_client_ID(client_ID)) 
		return;
	clients[client_ID].priority = priority;
	clients[client_ID].index_to_pq = -1;
	clients[client_ID].client_data = client_data;
	PQ_insert(client_ID);
}

unsigned int CLIENTS::delete_client(int* priority, CLIENT_DATA** client_data) {
	unsigned int client_ID = PQ_delete();
	*priority = clients[client_ID].priority;
	*client_data = clients[client_ID].client_data;

	clients[client_ID].priority = NULL_PRIORITY; // no more in the system
	clients[client_ID].index_to_pq = -1;
	clients[client_ID].client_data = nullptr;
	return client_ID;
}

void CLIENTS::adjust_client_priority(unsigned int client_ID, int priority_increment) {
	if (active_client_ID(client_ID) && (priority_increment != 0)) {
		int index_to_pq = clients[client_ID].index_to_pq;
		PQ_adjust_priority(index_to_pq, priority_increment);
	}
}

void CLIENTS::print_clients(const char* header) {
	fprintf(stdout, "\n/****************** %s ******************/\n", header);
	fprintf(stdout, "[CLIENTS]\n");
	fprintf(stdout, "  The number of current clients = %d\n",
		_pq.size() - 1);
	for (int i = 0; i < n_max_clients; i++)
		if (clients[i].priority != NULL_PRIORITY) {
			fprintf(stdout, "  [%5d] priority = %5d, index_to_pq = %5d\n",
				i, clients[i].priority, clients[i].index_to_pq);
		}
}

void CLIENTS::print_clients_into_file(const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "*** Error: cannot open the file %s...\n", filename);
		exit(-1);
	}
	fprintf(fp, "%d\n", (int) _pq.size() - 1);
	for (int i = 0; i < n_max_clients; i++)
		if (clients[i].priority != NULL_PRIORITY) {
			fprintf(fp, "%7d %7d %7d\n", i, clients[i].priority, clients[i].index_to_pq);
		}
	fclose(fp);
}

void CLIENTS::print_PQ(const char* header) {
	fprintf(stdout, "\n/****************** %s ******************/\n", header);
	fprintf(stdout, "[Priority Queue]\n");
	fprintf(stdout, "  The number of elements in priority queue = %d\n",
		_pq.size() - 1);
	for (int i = 1; i < _pq.size(); i++) {
		fprintf(stdout, "  [%3d] priority = %5d, client_ID = %5d\n", i,
			clients[_pq[i].client_ID].priority,
			_pq[i].client_ID);
	}
	fprintf(stdout, "\n");
}

void CLIENTS::print_PQ_into_file(const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		fprintf(stderr, "*** Error: cannot open the file %s...\n", filename);
		exit(-1);
	}
	fprintf(fp, "%d\n", (int) _pq.size() - 1);
	for (int i = 1; i < _pq.size(); i++) {
		fprintf(fp, "%7d %7d %7d\n", i, clients[_pq[i].client_ID].priority,
			_pq[i].client_ID);
	}
	fclose(fp);
}

// 
// priority queue management
//
void CLIENTS::PQ_initialize() {
	_pq.clear();
	_pq.emplace_back(-1);
}

void CLIENTS::PQ_insert(unsigned int client_ID) {
	int cur_priority = clients[client_ID].priority;
	_pq.emplace_back(client_ID);
	int cur_index = _pq.size() - 1;

	while (1) {
		if ((cur_index == 1) || (cur_priority >= clients[_pq[cur_index / 2].client_ID].priority))
			/* terminate when the root is reached or the element
			is in its correct place */
			break;
		else {
			/* check the next lower level of the _pq */
			_pq[cur_index] = _pq[cur_index / 2];
			clients[_pq[cur_index / 2].client_ID].index_to_pq = cur_index;
			cur_index /= 2;
		}
	}
	_pq[cur_index] = (PQ_ELEMENT) client_ID;
	clients[client_ID].index_to_pq = cur_index;
}

unsigned int CLIENTS::PQ_delete() {
	if (_pq.size() <= 1) return NULL_CLIENT_ID;

	int client_ID_first = _pq[1].client_ID;
	PQ_ELEMENT _pq_element_last = _pq[_pq.size() - 1];
	int priority_last = clients[_pq_element_last.client_ID].priority;
	_pq.pop_back();

	int parent_index = 1;
	int child_index = 2;
	int last_index = _pq.size() - 1;
	while (child_index <= last_index) {
		if ((child_index < last_index)
			&& (clients[_pq[child_index].client_ID].priority >
				clients[_pq[child_index + 1].client_ID].priority))
			child_index++;

		if (priority_last <= clients[_pq[child_index].client_ID].priority)
			/*correct position has been found */
			break;
		else {
			/* move to the next lower level */
			_pq[parent_index] = _pq[child_index];
			clients[_pq[child_index].client_ID].index_to_pq = parent_index;
			parent_index = child_index;
			child_index *= 2;
		}
	}
	_pq[parent_index] = _pq_element_last;
	clients[_pq_element_last.client_ID].index_to_pq = parent_index;

	return client_ID_first;
}

void CLIENTS::PQ_adjust_priority(int index_to_pq, int priority_increment) {
	// priority_increment could be zero, positive, or negative
	if (priority_increment == 0) return;
	
	int client_ID = _pq[index_to_pq].client_ID;
	int priority_client = clients[client_ID].priority += priority_increment;

	if (priority_increment > 0) {
		// move down the heap
		int parent_index = index_to_pq;
		int child_index = 2 * parent_index;
		int last_index = _pq.size() - 1;

		// Fill this part!
		while (child_index <= last_index) {
			if ((child_index < last_index)
				&& (clients[_pq[child_index].client_ID].priority >
					clients[_pq[child_index + 1].client_ID].priority))
				child_index++;

			if (priority_client <= clients[_pq[child_index].client_ID].priority)
				/*correct position has been found */
				break;
			else {
				/* move to the next lower level */
				_pq[parent_index] = _pq[child_index];
				clients[_pq[child_index].client_ID].index_to_pq = parent_index;
				parent_index = child_index;
				child_index *= 2;
			}
		}
		_pq[parent_index] = (PQ_ELEMENT) client_ID;
		clients[client_ID].index_to_pq = parent_index;
	}
	else if (priority_increment < 0) {
		// Fill this part!

		// move up the heap
		int cur_index = index_to_pq;

		while(cur_index > 1) {
			int parent_index = cur_index / 2;
			
			// parent priority <= client priority
			if(priority_client >= clients[_pq[parent_index].client_ID].priority)
				break;

			_pq[cur_index] = _pq[parent_index];
			clients[_pq[parent_index].client_ID].index_to_pq = cur_index;

			cur_index = parent_index;
		}
		_pq[cur_index] = (PQ_ELEMENT) client_ID;
		clients[client_ID].index_to_pq = cur_index;
	}
}
