//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include "RTree.h"

unsigned int tile_size = 1000;
int max_elements = 100000;

std::ofstream of;

template <int size>
struct Rect{
	Rect(std::vector<unsigned int> min, std::vector<unsigned int> max){
		assert(min.size() == size);
		assert(max.size() == size);

		for (int i = 0; i < size; i++){
			this->min[i] = min[i];
			this->max[i] = max[i];
		}
	}

	unsigned int min[size];
	unsigned int max[size];
};

bool MySearchCallback(int id, void* arg)
{
	//printf("Hit data rect %d\n", id);
	return true; // keep going
}

template <int size>
void generateRectangles(int items, int edge_size, std::vector<Rect<size> >& rectangles){
	rectangles.clear();
	switch (size){
	case 1:{
		for (unsigned int i = 0; i < edge_size; ++i){
			std::vector<unsigned int> min;
			std::vector<unsigned int> max;
			min.push_back(i * tile_size + 1);
			max.push_back((i + 1)*tile_size);

			Rect<size> r(min, max);
			rectangles.push_back(r);
		}
	}
		break;
	case 2:{
		for (unsigned int i = 0; i < edge_size; ++i){
			for (unsigned int j = 0; j < edge_size; ++j){
				std::vector<unsigned int> min;
				std::vector<unsigned int> max;
				min.push_back(i * tile_size + 1);
				min.push_back(j * tile_size + 1);
				max.push_back((i + 1)*tile_size);
				max.push_back((j + 1)*tile_size);

				Rect<size> r(min, max);
				rectangles.push_back(r);
			}
		}
	}
		break;
	case 3:{
		for (unsigned int i = 0; i < edge_size; ++i){
			for (unsigned int j = 0; j < edge_size; ++j){
				for (unsigned int k = 0; k < edge_size; ++k){
					std::vector<unsigned int> min;
					std::vector<unsigned int> max;
					min.push_back(i * tile_size + 1);
					min.push_back(j * tile_size + 1);
					min.push_back(k * tile_size + 1);

					max.push_back((i + 1)*tile_size);
					max.push_back((j + 1)*tile_size);
					max.push_back((k + 1)*tile_size);

					Rect<size> r(min, max);
					rectangles.push_back(r);
				}
			}
		}
	}
		break;

	case 4:{
		for (unsigned int i = 0; i < edge_size; ++i){
			for (unsigned int j = 0; j < edge_size; ++j){
				for (unsigned int k = 0; k < edge_size; ++k){
					for (unsigned int m = 0; m < edge_size; ++m){
						std::vector<unsigned int> min;
						std::vector<unsigned int> max;
						min.push_back(i * tile_size + 1);
						min.push_back(j * tile_size + 1);
						min.push_back(k * tile_size + 1);
						min.push_back(m * tile_size + 1);

						max.push_back((i + 1)*tile_size);
						max.push_back((j + 1)*tile_size);
						max.push_back((k + 1)*tile_size);
						max.push_back((m + 1)*tile_size);

						Rect<size> r(min, max);
						rectangles.push_back(r);
					}
				}
			}
		}
	}
		break;

	case 5:{
		for (unsigned int i = 0; i < edge_size; ++i){
			for (unsigned int j = 0; j < edge_size; ++j){
				for (unsigned int k = 0; k < edge_size; ++k){
					for (unsigned int m = 0; m < edge_size; ++m){
						for (unsigned int n = 0; n < edge_size; ++n){
							std::vector<unsigned int> min;
							std::vector<unsigned int> max;
							min.push_back(i * tile_size + 1);
							min.push_back(j * tile_size + 1);
							min.push_back(k * tile_size + 1);
							min.push_back(m * tile_size + 1);
							min.push_back(n * tile_size + 1);

							max.push_back((i + 1)*tile_size);
							max.push_back((j + 1)*tile_size);
							max.push_back((k + 1)*tile_size);
							max.push_back((m + 1)*tile_size);
							max.push_back((n + 1)*tile_size);

							Rect<size> r(min, max);
							rectangles.push_back(r);
						}
					}
				}
			}
		}
	}
		break;

	case 6:{
		for (unsigned int i = 0; i < edge_size; ++i){
			for (unsigned int j = 0; j < edge_size; ++j){
				for (unsigned int k = 0; k < edge_size; ++k){
					for (unsigned int m = 0; m < edge_size; ++m){
						for (unsigned int n = 0; n < edge_size; ++n){
							for (unsigned int p = 0; p < edge_size; ++p){
								std::vector<unsigned int> min;
								std::vector<unsigned int> max;
								min.push_back(i * tile_size + 1);
								min.push_back(j * tile_size + 1);
								min.push_back(k * tile_size + 1);
								min.push_back(m * tile_size + 1);
								min.push_back(n * tile_size + 1);
								min.push_back(p * tile_size + 1);

								max.push_back((i + 1)*tile_size);
								max.push_back((j + 1)*tile_size);
								max.push_back((k + 1)*tile_size);
								max.push_back((m + 1)*tile_size);
								max.push_back((n + 1)*tile_size);
								max.push_back((p + 1)*tile_size);

								Rect<size> r(min, max);
								rectangles.push_back(r);
							}
						}
					}
				}
			}
		}
	}
		break;
	}
}

template <int size>
void insert_ND(int items){
	std::vector< Rect<size> > rectangles;
	int no_iterations = std::max(max_elements / items, 10);
	unsigned int edge_size = (unsigned int)ceil(pow((double)items, 1.0 / (double)size));
	generateRectangles(items, edge_size, rectangles);

	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < no_iterations; i++){
		RTree<unsigned int, unsigned int, size, float> tree;

		for (size_t j = 0; j < rectangles.size(); ++j){
			tree.Insert(rectangles[j].min, rectangles[j].max, j);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	of << "Inserting " << rectangles.size() << " " << size << "D items:" << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ns" << std::endl;
}

template <int size>
void search_ND(int items){
	std::vector< Rect<size> > rectangles;
	unsigned int edge_size = (unsigned int)ceil(pow((double)items, 1.0 / (double)size));
	int no_iterations = std::max(max_elements / items, 10);
	generateRectangles(items, edge_size, rectangles);

	RTree<int, unsigned int, size, float> tree;

	for (size_t i = 0; i < rectangles.size(); ++i){
		tree.Insert(rectangles[i].min, rectangles[i].max, i);
	}

	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < no_iterations; i++){
		std::vector<unsigned int> min(size, 0);

		for (unsigned int j = 0; j < edge_size; ++j){
			std::vector<unsigned int> max(size, (j + 1)*tile_size);

			Rect<size> search_rect(min, max);
			int nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
		}

	}

	auto end = std::chrono::high_resolution_clock::now();

	of << "Running " << edge_size << "queries on " << size << "D items:" << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ns" << std::endl;
}

template <int size>
void search_ND_All(int items){
	std::vector< Rect<size> > rectangles;
	unsigned int edge_size = (unsigned int)ceil(pow((double)items, 1.0 / (double)size));
	int no_iterations = std::max(max_elements / items, 10);
	generateRectangles(items, edge_size, rectangles);

	RTree<int, unsigned int, size, float> tree;

	for (size_t i = 0; i < rectangles.size(); ++i){
		tree.Insert(rectangles[i].min, rectangles[i].max, i);
	}

	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < no_iterations; i++){
		for (size_t j = 0; j < rectangles.size(); j++){
			tree.Search(rectangles[j].min, rectangles[j].max, MySearchCallback, NULL);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	of << "Querying tree with " << rectangles.size() << " tiles using " << items << " queries over " << size << "D items:" << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ns" << std::endl;
}

template <int size>
void pointQueries_ND(int items){
	std::vector< Rect<size> > rectangles;
	std::vector< Rect<size> > queries;
	unsigned int edge_size = (unsigned int)ceil(pow((double)items, 1.0 / (double)size));
	int no_iterations = std::max(max_elements / items, 10);
	generateRectangles(items, edge_size, rectangles);

	RTree<int, unsigned int, size, float> tree;

	for (size_t i = 0; i < rectangles.size(); ++i){
		tree.Insert(rectangles[i].min, rectangles[i].max, i);
		std::vector<unsigned int> min(size, i*tile_size+1);
		std::vector<unsigned int> max(size, i*tile_size+1);
		Rect<size> pointQuery(min, max);
		queries.push_back(pointQuery);
	}

	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < no_iterations; i++){
		for (size_t j = 0; j < queries.size(); j++){
			tree.Search(queries[j].min, queries[j].max, MySearchCallback, NULL);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	of << "Running " << queries.size() << " point queries " << size << "D items:" << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ns" << std::endl;
}

void measureInsertion(){

	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<1>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<2>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<3>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<4>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<5>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		insert_ND<6>(i);
	}
	of << std::endl;
}

void measureSearch(){

	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<1>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<2>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<3>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<4>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<5>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND<6>(i);
	}
	of << std::endl;
}

void measureSearchAll(){
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<1>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<2>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<3>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<4>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<5>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		search_ND_All<6>(i);
	}
	of << std::endl;
}

void measurePointQuery(){
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<1>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<2>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<3>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<4>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<5>(i);
	}
	of << std::endl;
	for (int i = 1; i <= max_elements; i = i * 10){
		pointQueries_ND<6>(i);
	}
	of << std::endl;
}

void benchmark(){
	measurePointQuery();
	of << std::endl << std::endl;
	measureSearchAll();
	of << std::endl << std::endl;
	measureSearch();
	of << std::endl << std::endl;
	measureInsertion();
	of << std::endl << std::endl;
}

void main()
{
	of.open("results.txt");
	benchmark();
	std::cout << "Done." << std::endl;
	getchar(); // Wait for keypress on exit so we can read console output
	of.close();
}