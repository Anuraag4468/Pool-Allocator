#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

class PoolAllocator{
    private:
        struct Node{
            Node* next;
        };

        std::size_t m_chunkSize;
        std::size_t m_chunksPerBlock;
        void* m_memoryBlockStart;
        Node* m_head;

    public:
        PoolAllocator(std::size_t chunkSize, std::size_t chunksPerBlock) : m_chunkSize(chunkSize), 
        m_chunksPerBlock(chunksPerBlock), m_memoryBlockStart(nullptr), m_head(nullptr){

            if(m_chunkSize < sizeof(uintptr_t)){
                m_chunkSize = sizeof(uintptr_t);
            }

            const std::size_t totalBytes = m_chunkSize * m_chunksPerBlock;
            m_memoryBlockStart = std::malloc(totalBytes);
        
            if(!m_memoryBlockStart){ throw std::bad_alloc();}

            char* current = static_cast<char*>(m_memoryBlockStart);

            for(std::size_t i = 0; i < m_chunksPerBlock - 1; i++){
                char* nextChunk = current + m_chunkSize;
                reinterpret_cast<Node*>(current)->next = reinterpret_cast<Node*>(nextChunk);
                current = nextChunk;
            }

            reinterpret_cast<Node*>(current)->next = nullptr;
            m_head = static_cast<Node*>(m_memoryBlockStart);

            std::cout << "Pre allocated " << totalBytes << " bytes at "
            << m_memoryBlockStart  << " (" << m_chunksPerBlock << " chunks of " << m_chunkSize << " bytes). \n";
        
        }

        ~PoolAllocator(){
            if( m_memoryBlockStart){
                std::free(m_memoryBlockStart);
                std::cout << "Memory pool released.\n";
            }
        }   
        
        void printFreeList() const {
        std::cout << " Free List Trace\n";

        Node* current = m_head;
        int index = 0;
        while(current != nullptr){
            std::cout << "Chunk "<< index++ << " at address: " << current
            << " - > next points to: " << current->next << std::endl;
            current = current->next;
        }
    }
};


int main(){
    try{
        PoolAllocator pool(32, 8);
        pool.printFreeList();
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}