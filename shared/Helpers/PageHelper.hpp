#pragma once
#include <vector>

namespace GorillaUI::PageHelper
{

    /// @brief Gets a page from a vector of a specific type
    /// @param vector the full length vector to get a page from
    /// @param pageSize the size of a page
    /// @param pagenum the page which we want to get
    /// @return vector of size < pageSize with elements from vector at offset pagenum * pageSize
    template<class T>
    std::vector<T> GetPage(std::vector<T>& vector, int pageSize, int pagenum)
    {
        std::vector<T> page = {};
        int size = vector.size();
        
        int start = pageSize * pagenum;

        if (start > size) return page;
        int end = start + pageSize;
        end = end > size ? size : end;
        
        for (int i = start; i < end; i++)
        {
            page.push_back(vector[i]);
        }

        return page;
    }

    /// @brief Gets the amount of pages of pages that would fit in a vector
    /// @param vector the vector to check for
    /// @param pageSize the desired size
    /// @return the amount of pages of size pageSize that would be able to be taken out of vector
    template<class T>
    int GetPageAmount(std::vector<T>& vector, int pageSize)
    {
        int size = vector.size();
        int count = 0;
        while(size > 0)
        {
            size -= pageSize;
            count++;
        }
        return count;
    }
}