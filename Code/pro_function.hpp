#ifndef PRO_FUNCTION_H
#define PRO_FUNCTION_H

#include<vector>
#include<iostream>
#include <typeinfo>
#include<math.h>
#include "ThreadPool.hpp"
#include "rasterizer.hpp"

namespace pro_function
{
    template<typename T>
    void print_vector(std::vector<T> v)
    {
        for(auto i=v.begin();i!=v.end();++i)
        {
            std::cout << *i << " ";
        }
        std::cout<<std::endl;
	};

    template<typename T>
    int ceil_integer(T t)
    {
        int integer = (int)t;
        if(t-integer>0)
        {
            return integer+1;
        }
        else if(t-integer==0)
        {
            return integer;
        }
        else
        {
            throw "error";
        }
        
	};

    template<typename T>
    std::vector<std::vector<T> > Vector_Divide(const std::vector<T> v, int size)
    {
        int vector_size,block_size;
        vector_size = v.size();
        block_size = ceil_integer((double)vector_size/size);
        std::vector<std::vector<T> > vectors;
        std::vector<T> tmp;
        for(int i=0;i<size;i++)
        {
            if(i!=size-1)
            {
                tmp.resize(block_size);
                std::copy(v.begin()+i*block_size,(v.begin()+(i+1)*block_size),tmp.begin());
                vectors.push_back(tmp);
            }
            else
            {
                int lastblock_size = v.size()-i*block_size;
                tmp.resize(lastblock_size);
                copy(v.begin()+i*block_size, v.end(), tmp.begin());
                vectors.push_back(tmp);
            }
        }
        return vectors;

	};

	void draw(rst::rasterizer *r, std::vector<Triangle*> &divide)
	{
		r->draw(divide);
	}

	void allocate_ThreadPool(int Pool_size, std::vector<std::vector<Triangle*>> divide_lists, Thread::ThreadPool *pools, rst::rasterizer *r)
	{
		//r.draw(TriangleList);
		std::vector<Triangle*> divide;
		for (int i = 0; i < Pool_size; i++)
		{
			divide = divide_lists[i];
			pools->enqueue(pro_function::draw,r,divide);
		}
		pools->wait_for_tasks();    //等待所有线程完成
	}
}

#endif