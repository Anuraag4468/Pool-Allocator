#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

class PoolAllocator{
    private:
        std::size_t m_chunkSize;
        std::size_t m_chunksPerBlock;
        void* m_memoryBlockStart;

    public:
        PoolAllocator(std::size_t chunkSize, std::size_t chunksPerBlock) : m_chunkSize(chunkSize), 
        m_chunksPerBlock(chunksPerBlock), m_memoryBlockStart(nullptr){

            if(m_chunkSize < sizeof(uintptr_t)){
                m_chunkSize = sizeof(uintptr_t);
            }

            const std::size_t totalBytes = m_chunkSize * m_chunksPerBlock;
            m_memoryBlockStart = std::malloc(totalBytes);
        
            if(!m_memoryBlockStart){ throw std::bad_alloc();}

            std::cout << "Pre allocated " << totalBytes << " bytes at "
            << m_memoryBlockStart  << " (" << m_chunksPerBlock << " chunks of " << m_chunkSize << " bytes). \n";
        
        }

        ~PoolAllocator(){
            if( m_memoryBlockStart){
                std::free(m_memoryBlockStart);
                std::cout << "Memory pool released.\n";
            }
        }   
};


int main(){
    try{
        PoolAllocator pool(32, 8);
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}