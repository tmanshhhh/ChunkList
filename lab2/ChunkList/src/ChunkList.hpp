#pragma once
#include <iterator>
#include <memory>
#include <iostream>

namespace fefu_laboratory_two {
    template <typename T>
    class Allocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using pointer = T*;

        Allocator() noexcept = default;

        Allocator(const Allocator<T>& other) noexcept = default;

        template <class U>
        explicit Allocator(const Allocator<U>& other) noexcept {}

        ~Allocator() = default;

        pointer allocate(size_type n) {
            auto p = static_cast<pointer>(malloc(sizeof(value_type) * n));
            if (p)
                return p;

            throw std::bad_alloc();
        }

        void deallocate(pointer p, size_type n) noexcept {
            (void)n;
            free(p);
        }
    };

    template <typename ValueType>
    class IChunkList {
    public:
        using size_type = std::size_t;
        using reference = ValueType&;

        virtual size_t GetSize() const noexcept = 0;
        virtual reference at(size_type position) = 0;
        virtual reference operator[](std::ptrdiff_t position) = 0;
    };

    template <typename ValueType>
    class ChunkList_iterator {
    public:
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
        using size_type = std::size_t;

    protected:
        pointer value = nullptr;
        IChunkList<value_type>* chunk = nullptr;
        int index = 0;

    public:
        ChunkList_iterator() noexcept = default;

        ChunkList_iterator(pointer current_value, size_type current_index, IChunkList<value_type>* current_chunk) :
                value(current_value), index(current_index), chunk(current_chunk) {}

        ChunkList_iterator(const ChunkList_iterator& other) noexcept = default;

        ChunkList_iterator& operator=(const ChunkList_iterator& other) = default;

        ~ChunkList_iterator() = default;

        int GetIndex() {
            return index;
        }

        friend void swap(ChunkList_iterator<ValueType>& first, ChunkList_iterator<ValueType>& second) {
            std::swap(first.chunk, second.chunk);
            std::swap(first.value, second.value);
            std::swap(first.index, second.index);
        }

        friend bool operator==(const ChunkList_iterator<ValueType>& first,
                               const ChunkList_iterator<ValueType>& second) {
            return first.value == second.value;
        }
        friend bool operator!=(const ChunkList_iterator<ValueType>& first,
                               const ChunkList_iterator<ValueType>& second) {
            return first.value != second.value;
        }

        reference operator*() const {
            return *this->value;
        }

        pointer operator->() const {
            return this->value;
        }

        ChunkList_iterator& operator++() {
            if (index + 1 == chunk->GetSize()) {
                this->chunk = nullptr;
                this->value = nullptr;
                this->index = 0;
                return *this;
            }
            this->value = &chunk->at(++index);
            return *this;
        }

        ChunkList_iterator operator++(int) {
            if (index + 1 == chunk->GetSize()) {
                return ChunkList_iterator();
            }
            this->value = &chunk->at(++index);
            return *this;
        }

        ChunkList_iterator& operator--() {
            if (index - 1 < 0) {
                throw std::exception();
            }
            this->value = &chunk->at(--index);
            return *this;
        }

        ChunkList_iterator operator--(int) {
            if (index - 1 < 0) {
                throw std::exception();
            }
            this->value = &chunk->at(--index);
            return *this;
        }

        ChunkList_iterator operator+(const difference_type& difference) const {
            return ChunkList_iterator(&chunk->at(index + difference), index + difference, chunk);
        }

        ChunkList_iterator& operator+=(const difference_type& difference) {
            this->index += difference;
            this->value = &chunk->at(index);
            return *this;
        }

        ChunkList_iterator operator-(const difference_type& difference) const {
            return ChunkList_iterator(&chunk->at(index - difference), index - difference, chunk);
        }

        ChunkList_iterator& operator-=(const difference_type& difference) {
            this->index -= difference;
            this->value = &chunk->at(index);
            return *this;
        }

        reference operator[](const difference_type& n) {
            return chunk[n];
        }

        friend bool operator<(const ChunkList_iterator<ValueType>& first,
                              const ChunkList_iterator<ValueType>& second) {
            return first.index < second.index;
        }

        friend bool operator<=(const ChunkList_iterator<ValueType>& first,
                               const ChunkList_iterator<ValueType>& second) {
            return first.index <= second.index;
        }

        friend bool operator>(const ChunkList_iterator<ValueType>& first,
                              const ChunkList_iterator<ValueType>& second) {
            return first.index > second.index;
        }
        friend bool operator>=(const ChunkList_iterator<ValueType>& first,
                               const ChunkList_iterator<ValueType>& second) {
            return first.index >= second.index;
        }
    };

    template <typename ValueType>
    class ChunkList_const_iterator : public ChunkList_iterator<ValueType> {
    public:
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const ValueType*;
        using reference = const ValueType&;

        ChunkList_const_iterator() : ChunkList_iterator<value_type>() {};

        ChunkList_const_iterator(value_type* value, int index, IChunkList<value_type>* chunk) :
                ChunkList_iterator<ValueType>(value, index, chunk) {};

        ChunkList_const_iterator(const ChunkList_const_iterator& other) noexcept = default;

        ChunkList_const_iterator(pointer value, int index, const IChunkList<value_type>* chunk) :
                ChunkList_iterator<value_type>(const_cast<value_type*>(value), index, const_cast<IChunkList<value_type>*>(chunk)) {}

        ChunkList_const_iterator& operator=(const ChunkList_const_iterator&) = default;

        ~ChunkList_const_iterator() = default;

        friend void swap(ChunkList_const_iterator<ValueType>& first,
                         ChunkList_const_iterator<ValueType>& second) {
            std::swap(first.value, second.value);
            std::swap(first.chunk, second.chunk);
            std::swap(first.index, second.index);
        }

        int GetIndex() {
            return this->index;
        }

        friend bool operator==(const ChunkList_const_iterator<ValueType>& first,
                               const ChunkList_const_iterator<ValueType>& second) {
            return first.value == second.value;
        }

        friend bool operator!=(const ChunkList_const_iterator<ValueType>& first,
                               const ChunkList_const_iterator<ValueType>& second) {
            return first.value != second.value;
        }

        reference operator*() const {
            return *this->value;
        }

        pointer operator->() const {
            return this->value;
        }

        ChunkList_const_iterator& operator++() {
            if (this->index + 1 == this->chunk->GetSize()) {
                this->chunk = nullptr;
                this->value = nullptr;
                this->index = 0;
                return *this;
            }
            this->value = &this->chunk->at(++this->index);
            return *this;
        }

        ChunkList_const_iterator operator++(int) {
            if (this->index + 1 == this->chunk->GetSize()) {
                return ChunkList_const_iterator();
            }
            this->value = &this->chunk->at(++this->index);
            return *this;
        }

        ChunkList_const_iterator& operator--() {
            if (this->index - 1 < 0) {
                throw std::exception();
            }
            this->value = &this->chunk->at(--this->index);
            return *this;
        }

        ChunkList_const_iterator operator--(int) {
            if (this->index - 1 < 0) {
                throw std::exception();
            }
            this->value = &this->chunk->at(--this->index);
            return *this;
        }

        ChunkList_const_iterator operator+(const difference_type& difference) const {
            return ChunkList_const_iterator(&this->chunk->at(this->index + difference), this->index + difference, this->chunk);
        }

        ChunkList_const_iterator& operator+=(const difference_type& difference) {
            this->index += difference;
            this->value = &this->chunk->at(this->index);
            return *this;
        }

        ChunkList_const_iterator operator-(const difference_type& difference) const {
            return ChunkList_const_iterator(&this->chunk->at(this->index - difference), this->index - difference, this->chunk);
        }

        ChunkList_const_iterator& operator-=(const difference_type& difference) {
            this->index -= difference;
            this->value = &this->chunk->at(this->index);
            return *this;
        }

        reference operator[](const difference_type& n) {
            return this->chunk[n];
        }

        friend bool operator<(const ChunkList_const_iterator<ValueType>& first,
                              const ChunkList_const_iterator<ValueType>& second) {
            return first.index < second.index;
        }

        friend bool operator<=(const ChunkList_const_iterator<ValueType>& first,
                               const ChunkList_const_iterator<ValueType>& second) {
            return first.index <= second.index;
        }

        friend bool operator>(const ChunkList_const_iterator<ValueType>& first,
                              const ChunkList_const_iterator<ValueType>& second) {
            return first.index > second.index;
        }

        friend bool operator>=(const ChunkList_const_iterator<ValueType>& first,
                               const ChunkList_const_iterator<ValueType>& second) {
            return first.index >= second.index;
        }
    };

    template <typename ValueType>
    class Chunk : IChunkList<ValueType> {
    public:
        using reference = ValueType&;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using size_type = std::size_t;
        using value_type = ValueType;

        int size = 0;
        int current_size = 0;
        pointer list = nullptr;
        Allocator<value_type> allocator;
        Chunk* prev = nullptr;
        Chunk* next = nullptr;

        Chunk(int chunk_size) : size(chunk_size)
        {
            list = allocator.allocate(size);
        }

        Chunk(int chunk_size, Allocator<value_type> allocator) : size(chunk_size), allocator(allocator)
        {
            list = allocator.allocate(size);
        }

        size_t GetSize() const noexcept override {
            return current_size;
        }

        reference at(size_type position) override {
            if (position >= size || position < 0) {
                throw std::out_of_range("out of range");
            }
            return list[position];
        }

        reference operator[](difference_type position) override {
            return list[position];
        }

        pointer CopyElements() {
            pointer values = allocator.allocate(size);
            for (int i = 0; i < size; i++) {
                values[i] = list[i];
            }
            return values;
        }
    };

    template <typename T, int N, typename Allocator = Allocator<T>>
    class ChunkList : IChunkList<T> {
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = ChunkList_iterator<value_type>;
        using const_iterator = ChunkList_const_iterator<value_type>;

    private:
        int size = 0;
        Chunk<value_type>* start = nullptr;

    public:

        ChunkList() : start(new Chunk<value_type>(N)) {}

        explicit ChunkList(const Allocator& alloc) : start(new Chunk<value_type>(N, alloc)) {}

        size_t GetSize() const noexcept override {
            return size;
        }

        ChunkList(size_type count, const T& value = T(), const Allocator& alloc = Allocator()) : start(new Chunk<value_type>(N))
        {
            Chunk<value_type>* temp_pointer = start;
            bool flag = false;
            while (size < count) {
                temp_pointer->allocator = alloc;
                for (int j = 0; j < N; j++) {
                    temp_pointer->list[j] = value;
                    temp_pointer->current_size++;
                    size++;
                    if (size >= count) {
                        flag = true;
                        break;
                    }
                }
                if (flag) {
                    return;
                }
                Chunk<value_type>* pointer_prev = temp_pointer;
                temp_pointer->next = new Chunk<value_type>(N, alloc);
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = pointer_prev;
            }
        }

        explicit ChunkList(size_type count, const Allocator& alloc = Allocator()) : start(new Chunk<value_type>(N))
        {
            Chunk<value_type>* temp_pointer = start;
            bool flag = false;
            while (size < count) {
                temp_pointer->allocator = alloc;
                for (int j = 0; j < N; j++) {
                    temp_pointer->list[j] = value_type();
                    temp_pointer->current_size++;
                    size++;
                    if (size >= count) {
                        flag = true;
                        break;
                    }
                }
                if (flag) {
                    return;
                }
                Chunk<value_type>* pointer_prev = temp_pointer;
                temp_pointer->next = new Chunk<value_type>(N, alloc);
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = pointer_prev;
            }
        }

        ChunkList(const ChunkList& other) : start(new Chunk<value_type>(N)) {
            Chunk<value_type>* other_list = other.start;
            Chunk<value_type>* this_list = start;
            size = other.size;
            while (other_list != nullptr) {
                this_list->current_size = other_list->current_size;
                this_list->list = other_list->CopyElements();
                this_list->next = new Chunk<value_type>(N);
                Chunk<value_type>* temp_pointer = this_list;
                this_list = this_list->next;
                this_list->prev = temp_pointer;
                other_list = other_list->next;
            }
        }

        ChunkList(const ChunkList& other, const Allocator& alloc) : start(new Chunk<value_type>(N, alloc)) {
            Chunk<value_type>* other_list = other.start;
            Chunk<value_type>* this_list = start;
            size = other.size;
            while (other_list != nullptr) {
                this_list->current_size = other_list->current_size;
                this_list->list = other_list->CopyElements();
                this_list->next = new Chunk<value_type>(N, alloc);
                Chunk<value_type>* temp_pointer = this_list;
                this_list = this_list->next;
                this_list->prev = temp_pointer;
                other_list = other_list->next;
            }
        }

        ChunkList(ChunkList&& other) {
            size = std::move(other.size);
            start = std::move(other.start);
            other.clear();
        }

        ChunkList(ChunkList&& other, const Allocator& alloc) {
            this = ChunkList(other);
            Chunk<value_type>* temp_pointer = this->start;
            while (temp_pointer != nullptr) {
                temp_pointer->allocator = alloc;
                temp_pointer = temp_pointer->next;
            }
        }

        ~ChunkList() {
            clear();
        }

        ChunkList& operator=(const ChunkList& other) {
            this = new ChunkList(other);
            return *this;
        }

        ChunkList& operator=(ChunkList&& other) {
            size = std::move(other.size);
            start = std::move(other.start);
            other.clear();
            return *this;
        }

        void assign(size_type count, const T& value) {
            if (count > 0) {
                clear();
                for (int i = 0; i < count; i++) {
                    push_back(value);
                }
                size = count;
            }
        }

        allocator_type get_allocator() const noexcept {
            return start->allocator;
        }

        reference at(size_type pos) override {
            if (pos < 0 || pos >= max_size()) {
                throw std::out_of_range("out of range");
            }
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type>* temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->at(value_number);
        }

        const_reference at(size_type pos) const
        {
            if (pos < 0 || pos >= max_size()) {
                throw std::out_of_range("out of range");
            }
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type>* temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->at(value_number));
        }

        reference operator[](difference_type pos) override {
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type>* temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->list[value_number];
        }

        const_reference operator[](difference_type pos) const
        {
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type>* temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->list[value_number]);
        }

        reference front() {
            if (size > 0)
                return start->list[0];
            else
                throw std::runtime_error("empty");
        }

        const_reference front() const {
            if (size > 0) {
                static_cast<reference>(start->list[0]);
            }
            else throw std::runtime_error("empty");
        }

        reference back() {
            if (size == 0) {
                throw std::runtime_error("empty");
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->list[temp_pointer->current_size - 1];
        }

        const_reference back() const {
            if (size == 0) {
                throw std::runtime_error("empty");
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->list[temp_pointer->current_size - 1]);
        }

        iterator begin() noexcept {
            if (size == 0) {
                return end();
            }
            return ChunkList_iterator<value_type>(&at(0), 0, this);
        }

        const_iterator begin() const noexcept {
            if (size == 0) {
                return end();
            }
            return ChunkList_const_iterator<value_type>(&at(0), 0, this);
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        iterator end() noexcept {
            return ChunkList_iterator<value_type>();
        }

        const_iterator end() const noexcept {
            return ChunkList_const_iterator<value_type>();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        bool empty() const noexcept {
            return size == 0;
        }

        int get_size() const noexcept {
            return size;
        }

        size_type max_size() const noexcept {
            size_type value_number = size % N;
            return (value_number == 0 ? size : size + N - value_number);
        }

        void clear() noexcept {
            Chunk<value_type>* current_chunk = start;
            while (current_chunk != nullptr) {
                Chunk<value_type>* temp_pointer = current_chunk;
                current_chunk = current_chunk->next;
                delete temp_pointer;
            }
            start = nullptr;
            size = 0;
        }

        iterator insert(const_iterator pos, const T& value) {
            if (pos == cend())
                return end();
            int index = pos.GetIndex();
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            ChunkList_iterator<value_type> iterator(&temp_pointer->
                    list[temp_pointer->current_size - 1], size - 1, this);
            if (size < max_size()) {
                for (int i = 0; iterator >= pos; i++, iterator--) {
                    at(size - i) = at(size - i - 1);
                }
                size++;
            }
            else {
                temp_pointer->next = new Chunk<value_type>(N);
                Chunk<value_type>* prev_chunk = temp_pointer;
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = prev_chunk;
                size++;
                for (int i = 0; iterator >= pos; i++, iterator--) {
                    at(size - i - 1) = at(size - i - 2);
                }
            }
            at(index) = value;
            temp_pointer->current_size++;
            return ChunkList_iterator<value_type>(&at(index), index, this);
        }

        iterator insert(const_iterator pos, T&& value) {
            if (pos == cend())
                return end();
            int index = pos.GetIndex();
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            ChunkList_iterator<value_type> iterator(&temp_pointer->
                    list[temp_pointer->current_size - 1], size - 1, this);
            if (size < max_size()) {
                for (int i = 0; iterator >= pos; i++, iterator--) {
                    at(size - i) = at(size - i - 1);
                }
                size++;
            }
            else {
                temp_pointer->next = new Chunk<value_type>(N);
                Chunk<value_type>* prev_chunk = temp_pointer;
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = prev_chunk;
                size++;
                for (int i = 0; iterator >= pos; i++, iterator--) {
                    at(size - i - 1) = at(size - i - 2);
                }
            }
            at(index) = std::move(value);
            temp_pointer->current_size++;
            return ChunkList_iterator<value_type>(&at(index), index, this);
        }

        iterator erase(const_iterator pos) {
            int index = pos.GetIndex();
            if (index + 1 == size) {
                size--;
                return ChunkList_iterator<value_type>();
            }
            for (int i = index + 1; index < size; i++) {
                at(i - 1) = at(i);
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            if (size == max_size() - N + 1) {
                temp_pointer = temp_pointer->prev;
                delete temp_pointer->next;
                temp_pointer->next = nullptr;
            }
            else {
                temp_pointer->current_size--;
            }
            size--;
            return ChunkList_iterator<value_type>(&at(index), index, this);
        }

        iterator erase(const_iterator first, const_iterator last) {
            int index = last.GetIndex();
            auto iterator = (*this).begin();
            int end = 1;
            int range_length = 0;
            for (; iterator != first; iterator++, end++);
            for (iterator = first; iterator != last; iterator++, end++, range_length++);
            for (; end < size; end++) {
                at(end - range_length) = at(end);
            }
            size -= range_length;
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            while (range_length > 0) {
                if (temp_pointer->current_size > 0) {
                    temp_pointer->current_size--;
                    range_length--;
                }
                else {
                    temp_pointer = temp_pointer->prev;
                    temp_pointer->next = nullptr;
                }
            }
            return ChunkList_iterator<value_type>(&at(index), index, this);
        }

        void push_back(const T& value) {
            if (start == nullptr) {
                start = new Chunk<value_type>(N);
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            if (temp_pointer->current_size == temp_pointer->size) {
                temp_pointer->next = new Chunk<value_type>(N);
                Chunk<value_type>* prev_pointer = temp_pointer;
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = prev_pointer;
            }
            temp_pointer->list[temp_pointer->current_size] = value;
            temp_pointer->current_size++;
            size++;
        }

        void push_back(T&& value) {
            if (start == nullptr) {
                start = new Chunk<value_type>(N);
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            if (temp_pointer->current_size == temp_pointer->size) {
                temp_pointer->next = new Chunk<value_type>(N);
                Chunk<value_type>* prev_pointer = temp_pointer;
                temp_pointer = temp_pointer->next;
                temp_pointer->prev = prev_pointer;
            }
            temp_pointer->list[temp_pointer->current_size] = std::move(value);
            temp_pointer->current_size++;
            size++;
        }

        void pop_back() {
            if (start == nullptr) {
                throw std::runtime_error("empty");
                return;
            }
            Chunk<value_type>* temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            temp_pointer->current_size--;
            size--;
        }

        void push_front(const T& value) {
            insert(cbegin(), value);
        }

        void push_front(T&& value) {
            insert(cbegin(), std::move(value));
        }

        void pop_front() {
            erase(cbegin());
        }

        void swap(ChunkList& other) {
            Chunk<value_type>* tmp_start;
            int tmp_size;
            tmp_start = other.start;
            tmp_size = other.size;
            other.start = this->start;
            other.size = this->size;
            this->start = tmp_start;
            this->size = tmp_size;
        }

        template <class U, class Alloc>
        friend bool operator==(const ChunkList<U, N, Alloc>& lhs,
                               const ChunkList<U, N, Alloc>& rhs) {
            if (lhs.size != rhs.size) {
                return false;
            }
            for (int i = 0; i < lhs.size; i++) {
                if (lhs.at(i) != rhs.at(i)) {
                    return false;
                }
            }

            return true;
        }

        template <class U, class Alloc>
        friend bool operator!=(const ChunkList<U, N, Alloc>& lhs,
                               const ChunkList<U, N, Alloc>& rhs) {
            return !(lhs == rhs);
        }
    };
}