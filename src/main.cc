#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <boost/shared_ptr.hpp>
#include <boost/timer/timer.hpp>

#include "Rectangle.hh"
#include "RTree.hh"

using boost::shared_ptr;
using std::vector;
using boost::uint64_t;

unsigned int tile_size = 1000;
unsigned int max_elements = 100000;

vector<shared_ptr<Rectangle> > generateRectangles(uint64_t items, uint64_t edgeSize, uint64_t dimensionNo)
{
    vector<shared_ptr<Rectangle> > rectangles;

    switch (dimensionNo)
    {
    case 1:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            std::vector<uint64_t> min;
            std::vector<uint64_t> max;
            min.push_back(i * tile_size + 1);
            max.push_back((i + 1)*tile_size);

            shared_ptr<Rectangle> r(new Rectangle(min, max));
            rectangles.push_back(r);
        }
    }
    break;
    case 2:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            for (uint64_t j = 0; j < edgeSize; ++j)
            {
                std::vector<uint64_t> min;
                std::vector<uint64_t> max;
                min.push_back(i * tile_size + 1);
                min.push_back(j * tile_size + 1);
                max.push_back((i + 1)*tile_size);
                max.push_back((j + 1)*tile_size);

                shared_ptr<Rectangle> r(new Rectangle(min, max));
                rectangles.push_back(r);
            }
        }
    }
    break;
    case 3:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            for (uint64_t j = 0; j < edgeSize; ++j)
            {
                for (uint64_t k = 0; k < edgeSize; ++k)
                {
                    std::vector<uint64_t> min;
                    std::vector<uint64_t> max;
                    min.push_back(i * tile_size + 1);
                    min.push_back(j * tile_size + 1);
                    min.push_back(k * tile_size + 1);

                    max.push_back((i + 1)*tile_size);
                    max.push_back((j + 1)*tile_size);
                    max.push_back((k + 1)*tile_size);


                    shared_ptr<Rectangle> r(new Rectangle(min, max));
                    rectangles.push_back(r);
                }
            }
        }
    }
    break;

    case 4:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            for (uint64_t j = 0; j < edgeSize; ++j)
            {
                for (uint64_t k = 0; k < edgeSize; ++k)
                {
                    for (uint64_t m = 0; m < edgeSize; ++m)
                    {
                        std::vector<uint64_t> min;
                        std::vector<uint64_t> max;
                        min.push_back(i * tile_size + 1);
                        min.push_back(j * tile_size + 1);
                        min.push_back(k * tile_size + 1);
                        min.push_back(m * tile_size + 1);

                        max.push_back((i + 1)*tile_size);
                        max.push_back((j + 1)*tile_size);
                        max.push_back((k + 1)*tile_size);
                        max.push_back((m + 1)*tile_size);

                        shared_ptr<Rectangle> r(new Rectangle(min, max));
                        rectangles.push_back(r);
                    }
                }
            }
        }
    }
    break;

    case 5:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            for (uint64_t j = 0; j < edgeSize; ++j)
            {
                for (uint64_t k = 0; k < edgeSize; ++k)
                {
                    for (uint64_t m = 0; m < edgeSize; ++m)
                    {
                        for (uint64_t n = 0; n < edgeSize; ++n)
                        {
                            std::vector<uint64_t> min;
                            std::vector<uint64_t> max;
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

                            shared_ptr<Rectangle> r(new Rectangle(min, max));
                            rectangles.push_back(r);
                        }
                    }
                }
            }
        }
    }
    break;

    case 6:
    {
        for (uint64_t i = 0; i < edgeSize; ++i)
        {
            for (uint64_t j = 0; j < edgeSize; ++j)
            {
                for (uint64_t k = 0; k < edgeSize; ++k)
                {
                    for (uint64_t m = 0; m < edgeSize; ++m)
                    {
                        for (uint64_t n = 0; n < edgeSize; ++n)
                        {
                            for (uint64_t p = 0; p < edgeSize; ++p)
                            {
                                std::vector<uint64_t> min;
                                std::vector<uint64_t> max;
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

                                shared_ptr<Rectangle> r(new Rectangle(min, max));
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

    return rectangles;

}

void insert_ND(uint64_t itemsNo, uint64_t size)
{
    boost::timer::cpu_timer timer;
    int no_iterations = max_elements / itemsNo;
    uint64_t edge_size = (uint64_t)ceil(pow((double)itemsNo, 1.0 / (double)size));
    std::vector<  shared_ptr<Rectangle> > rectangles = generateRectangles(itemsNo, edge_size, size);

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < no_iterations; i++)
    {
        RTree tree;
        for (size_t j = 0; j < rectangles.size(); ++j)
        {
            tree.insert(rectangles[j]);
        }
    }

    timer.stop();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Inserting " << rectangles.size() << " " << size << "D items:"
              << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ms" << std::endl;
}

void search_ND(uint64_t itemsNo, uint64_t size)
{
    int no_iterations = max_elements / itemsNo;
    uint64_t edge_size = (unsigned int)ceil(pow((double)itemsNo, 1.0 / (double)size));
    std::vector<  shared_ptr<Rectangle> > rectangles = generateRectangles(itemsNo, edge_size, size);

    RTree tree;
    for (size_t i = 0; i < rectangles.size(); ++i)
    {
        tree.insert(rectangles[i]);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < no_iterations; i++)
    {
        std::vector<uint64_t> min(size, 0);

        for (unsigned int j = 0; j < edge_size; ++j)
        {
            std::vector<uint64_t> max(size, (j + 1)*tile_size);
            boost::shared_ptr<Rectangle> search_rect(new Rectangle(min,max));
            tree.search(search_rect);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Running  " << edge_size << " queries " << size << "D items:"
              << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ms" << std::endl;
}


void search_ND_All(uint64_t itemsNo, uint64_t size)
{
    int no_iterations = max_elements / itemsNo;
    uint64_t edge_size = (unsigned int)ceil(pow((double)itemsNo, 1.0 / (double)size));
    std::vector<  shared_ptr<Rectangle> > rectangles = generateRectangles(itemsNo, edge_size, size);

    RTree tree;
    for (size_t i = 0; i < rectangles.size(); ++i)
    {
        tree.insert(rectangles[i]);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < no_iterations; i++)
    {
        for (size_t j = 0; j < rectangles.size(); j++)
        {
            tree.search(rectangles[j]);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Running " << rectangles.size() << " tiles using "<<itemsNo<<" queries over "
              << size << "D items:" << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ms" << std::endl;
}

void pointQueries_ND(uint64_t itemsNo, uint64_t size)
{
    int no_iterations = max_elements / itemsNo;
    uint64_t edge_size = (unsigned int)ceil(pow((double)itemsNo, 1.0 / (double)size));
    std::vector<  shared_ptr<Rectangle> > rectangles = generateRectangles(itemsNo, edge_size, size);
    std::vector<  shared_ptr<Rectangle> > queries;

    RTree tree;
    for (size_t i = 0; i < rectangles.size(); ++i)
    {
        tree.insert(rectangles[i]);
        std::vector<uint64_t> min(size, i*tile_size);
        std::vector<uint64_t> max(size, i*tile_size);
        shared_ptr<Rectangle>  pointQuery(new Rectangle(min, max));
        queries.push_back(pointQuery);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < no_iterations; i++)
    {
        for (size_t j = 0; j < queries.size(); j++)
        {
            tree.search(queries[j]);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Running" << queries.size() << " point queries on " << size << "D items:"
              << (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / (double)no_iterations << "ms" << std::endl;
}

void benchmark()
{
    for(uint64_t j=2; j<=6; j++)
    {
        for (int i = 1; i <= max_elements; i = i * 10)
        {
            pointQueries_ND(i,j);
        }
        std::cout <<std::endl;
    }

    for(uint64_t j=2; j<=6; j++)
    {
        for (int i = 1; i <= max_elements; i = i * 10)
        {
            search_ND_All(i,j);
        }
        std::cout <<std::endl;
    }

    for(uint64_t j=2; j<=6; j++)
    {
        for (int i = 1; i <= max_elements; i = i * 10)
        {
            search_ND(i,j);
        }
        std::cout <<std::endl;
    }

    for(uint64_t j=2; j<=6; j++)
    {
        for (int i = 1; i <= max_elements; i = i * 10)
        {
            insert_ND(i,j);
        }
        std::cout <<std::endl;
    }
}

int main()
{
    benchmark();
    return 0;
}
