
#include <cstddef>
#include <cstdlib>
#include <new>
#include <stdexcept>
class Array {
public:
    size_t size; // index of last element
    size_t lastIndex;
    int *ptr;
    Array(size_t initial = 0) {
        this->lastIndex = 0;
        this->ptr = nullptr;
        realloc(initial);
    }

    size_t push(int i) {
        if (size == lastIndex) {
            realloc((size + 1) * 2);
        }
        ptr[lastIndex++] = i;
        return lastIndex - 1;
    }

    int& operator[](size_t index) {
        if (!ptr || index >= lastIndex) throw std::range_error("Out of range");
        return ptr[index];
    }

    void sort() {
        std::qsort(ptr, lastIndex, sizeof(int), [](const void* p, const void *q) -> int {
            return (*((int*) p)) - (*((int*) q));
        });
    }

    void* operator new(size_t s) {
        return ::operator new(s);
    }

    void operator delete(void* p) {
        ::operator delete(p);
    }

    void* operator new[](size_t s) {
        return ::operator new[](s);
    }

    void operator delete[](void* p) {
        ::operator delete[](p);
    }

private:
    void realloc(size_t newSize) {
        int *newPtr = new int[newSize];
        if (ptr) {
            for (int i = 0; i <= lastIndex && i < newSize; i++) {
                newPtr[i] = ptr[i];
            }
        } else {
            for (int i = 0; i < newSize; i++) newPtr[i] = 0;
        }
        delete[] ptr;
        this->ptr = newPtr;
        this->size = newSize;
        printf("new size: %lu\n", newSize);
    }
};