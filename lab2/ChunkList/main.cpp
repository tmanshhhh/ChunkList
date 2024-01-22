#include "src/ChunkList.hpp"
#include <cassert>
#include <iostream>

using namespace fefu_laboratory_two;

int main() {
    {
        ChunkList<int, 3> list;
        assert(list.get_size() == 0);
        assert(list.empty() == true);
    }
    {
        Allocator<int> allocator;
        ChunkList<int, 3> list(5, 4, allocator);

        assert(list.get_size() == 5);

        for (int i = 0; i < 5; i++) {
            assert(list[i] == 4);
        }
    }
    {
        Allocator<int> allocator;
        ChunkList<int, 3> list(5, int(), allocator);

        assert(list.get_size() == 5);

        for (int i = 0; i < 5; i++) {
            assert(list[i] == int());
        }
    }
    {
        ChunkList<int, 3> first_list;
        for (int i = 0; i < 5; i++) {
            first_list.push_back(i);
        }

        auto second_list = first_list;

        for (int i = 0; i < 5; i++) {
            assert(first_list[i] == second_list[i]);
        }

        assert(first_list.get_size() == second_list.get_size());
    }
    {
        Allocator<int> allocator;
        ChunkList<int, 3> first_list;

        for (int i = 0; i < 5; i++) {
            first_list.push_back(i);
        }

        ChunkList<int, 3> second_list(first_list, allocator);

        for (int i = 0; i < 5; i++) {
            assert(first_list[i] == second_list[i]);
        }

        assert(first_list.get_size() == second_list.get_size());
    }
    {
        Allocator<int> allocator;
        ChunkList<int, 3> list(5, 4, allocator);

        list.assign(3, 3);

        assert(list.get_size() == 3);

        for (int i = 0; i < list.get_size(); i++) {
            assert(list[i] == 3);
        }
    }
    {
        ChunkList<int, 3> list;

        for (int i = 0; i < 5; i++) {
            list.push_back(i);
        }

        assert(list.at(2) == 2);
        assert(list.at(4) == 4);
    }
    {
        ChunkList<int, 3> list;

        for (int i = 0; i < 30; i++) {
            list.push_back(i);
        }

        for (int i = 0; i < 30; i++) {
            assert(list[i] == i);
        }
    }
    {
        ChunkList<int, 5> list;

        for (int i = 4; i < 42; i++) {
            list.push_back(i);
        }

        list.clear();

        assert(list.empty() == true);
    }
    {
        ChunkList<int, 5> list;

        for (int i = 4; i < 42; i++) {
            list.push_back(i);
        }

        assert(list.front() == 4);
    }
    {
        ChunkList<int, 5> list;

        for (int i = 4; i < 42; i++) {
            list.push_back(i);
        }

        assert(list.back() == 41);
    }
    {
        ChunkList<int, 8> list;
        int* test_arr = new int[10];

        for (int i = 0; i < 10; i++) {
            list.push_back(i);
            test_arr[i] = i;
        }

        int j = 0;
        for (auto iter : list)
            assert(test_arr[j++] == iter);

        auto it1 = list.begin();
        auto it2 = list.begin();
        assert(it1 == it2);
        assert(it1 >= it2);

        it2++;
        assert(it2 > it1);

        it1 += 2;
        it2 -= 1;
        assert(it1 > it2);

        auto it3 = list.back();
        assert(it3 == 9);
    }
    {
        ChunkList<int, 8> list;

        for (int i = 0; i < 10; i++)
            list.push_back(i);

        auto it = list.cbegin();
        it += 2;
        list.insert(it, 2);

        assert(list[2] == 2);
    }
    {
        ChunkList<int, 3> list;

        for (int i = 0; i < 10; i++)
            list.push_back(i);

        list.pop_back();
        assert(list[list.get_size() - 1] == 8);
    }


    std::cout << "All tests passed." << std::endl;

    return 0;
}