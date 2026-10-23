#pragma once

#include <array>
#include <cstddef>
#include <memory>

namespace exchange::core{

template<typename T, std::size_t N>

class MemoryPool{
    //static assert evaluates boolean expression during compile-time and return an error message if the condition is false
    static_assert(N > 0, "MemoryPool capacity must be positive.");
    static_assert(sizeof(T) >= sizeof(void*), "Object storage must fit an intrusive free-list pointer.");

    //keep 64B alignment by default
    struct alignas((alignof(T) > 64U) ? alignof(T) : 64U) Node{
        std::array<std::byte, sizeof(T)> storage{};
    };

    struct FreeNode {
        FreeNode* next;
    };

private:
    std::unique_ptr<Node[]> nodes_;

    FreeNode* free_list_head_{nullptr};

    std::size_t allocated_count_ = 0;

    void initialize_free_list() noexcept{
        free_list_head_ = nullptr;
        for(std::size_t index = 0; index < N; ++index){
            auto* node = reinterpret_cast<FreeNode*>(nodes_[index].storage.data());
            node->next = free_list_head_; 
            free_list_head_ = node;
        }

    }

    T* allocate() noexcept {
        if (free_list_head_ == nullptr) [[unlikely]]{
            return nullptr;
        }
        FreeNode* node = free_list_head_;
        free_list_head_ = free_list_head_->next; 
        ++allocated_count_;
        return reinterpret_cast<T*>(node);
    }

    void deallocate(T* object) noexcept{
        if(object == nullptr) [[unlikely]]{
            return nullptr;
        }
        
        std::destroy_at(object);
        auto* node = reinterpret_cast<FreeNode*>(object);
        node->next = free_list_head_;
        free_list_head_ = node;
        --allocated_count_;
    }

public:
    MemoryPool(): nodes_(std::make_unique<Node[]>(N)){initialize_free_list(); }

    // i need to read more about these move statments
    MemoryPool(const MemoryPool& ) = delete;
    MemoryPool &operator=(const MemoryPool& ) = delete;
    MemoryPool(MemoryPool&& ) = delete;
    MemoryPool &operator=( MemoryPool&& ) = delete;

};
    
}
