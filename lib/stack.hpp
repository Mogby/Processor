#ifndef NCPU_STACK_HPP
#define NCPU_STACK_HPP

#include "util.hpp"

#include <functional>
#include <sstream>

namespace NCPU {

template<typename TData, typename THash = std::hash<TData>>
class TStack {
  private:
    using TSelf = TStack<TData, THash>;

    using TParrot = uint32_t;
    using THashSum = size_t;

  private:
    static const TParrot NORWEGIAN_BLUE = 0xDEADBEEF;

  public:
    explicit TStack(TSize capacity);
    ~TStack();

    template <typename TValue>
    void Push(TValue&& data);
    void Pop();

    const TData& GetFrontElement() const;

    TSize GetSize() const;
    bool IsEmpty() const;

    bool IsValid() const;
    TString GetDump(size_t buffer_dump_max_size = 10, bool safe = false) const;

  private:
    THashSum GetHashSum() const;

    TString GetGuardDump(TParrot guard) const;

  private:
    TParrot front_guard_ = NORWEGIAN_BLUE;

    TData* buffer_;
    TSize capacity_;
    TSize size_ = 0;

    THash hash_{};
    THashSum hash_sum_ = 0;

    TParrot rear_guard_ = NORWEGIAN_BLUE;
};

template <typename TData, typename THash>
TStack<TData, THash>::TStack(NCPU::TSize capacity)
        : capacity_(capacity) {
    buffer_ = new TData[capacity_];
}

template <typename TData, typename THash>
TStack<TData, THash>::~TStack() {
    delete[] buffer_;
}

template <typename TData, typename THash>
template <typename TValue>
void TStack<TData, THash>::Push(TValue&& data) {
    if (size_ == capacity_) {
        throw TException("Attempt to push to a full stack");
    }
    hash_sum_ += (size_ + 1) * hash_(data);
    buffer_[size_++] = std::forward<TValue>(data);
}

template <typename TData, typename THash>
void TStack<TData, THash>::Pop() {
    if (size_ == 0) {
        throw TException("Attempt to pop from empty stack");
    }
    hash_sum_ -= size_ * hash_(buffer_[size_ - 1]);
    --size_;
}

template <typename TData, typename THash>
const TData& TStack<TData, THash>::GetFrontElement() const {
    if (IsEmpty()) {
        throw TException("Attempt to read from empty stack");
    }
    return buffer_[size_ - 1];
}

template <typename TData, typename THash>
TSize TStack<TData, THash>::GetSize() const {
    return size_;
}

template <typename TData, typename THash>
bool TStack<TData, THash>::IsEmpty() const {
    return size_ == 0;
}

template <typename TData, typename THash>
bool TStack<TData, THash>::IsValid() const {
    if (front_guard_ != NORWEGIAN_BLUE || rear_guard_ != NORWEGIAN_BLUE) {
        return false;
    }
    if (buffer_ == NULL) {
        return false;
    }
    if (capacity_ < size_) {
        return false;
    }
    if (capacity_ == 0) {
        return false;
    }
    if (hash_sum_ != GetHashSum()) {
        return false;
    }
    return true;
}

template <typename TData, typename THash>
TString TStack<TData, THash>::GetDump(size_t buffer_dump_max_size, bool safe) const {
    size_t buffer_dump_size = std::min(capacity_, buffer_dump_max_size);

    std::ostringstream dump;
    dump << "TStack@0x" << this << " (" << (IsValid() ? "ok" : "error") << ") {\n";
    dump << "\t// front guard is " << GetGuardDump(front_guard_) << '\n';
    dump << "\t// rear guard is " << GetGuardDump(rear_guard_) << '\n';
    dump << "\t// actual hash sum is " << GetHashSum() << '\n';
    dump << "\n";
    dump << "\thash_sum_ = " << hash_sum_ << ";\n";
    dump << "\tsize = " << size_ << ";\n";
    dump << "\tcapacity = " << capacity_ << ";\n";
    dump << "\n";
    dump << "\tbuffer_[" << capacity_ << "] = {\n";
    for (size_t i = 0; i < buffer_dump_size; ++i) {
        dump << "\t\t";
        if (i < size_) {
            if (safe) {
                dump << "@0x" << buffer_ + i;
            } else {
                dump << TDumper<TData>::GetDump(buffer_[i]);
            }
        } else {
            dump << "POISON";
        }
        if (i + 1 < capacity_) {
            dump << ",";
        }
        dump << "\n";
    }
    if (buffer_dump_max_size < capacity_) {
        dump << "\t\t... // truncated\n";
    }
    dump << "\t}\n";
    dump << "}";

    return dump.str();
}

template <typename TData, typename THash>
typename TStack<TData, THash>::THashSum TStack<TData, THash>::GetHashSum() const {
    THashSum sum = 0;
    for (TSize i = 0; i < size_; ++i) {
        sum += (i + 1) * hash_(buffer_[i]);
    }
    return sum;
}

template <typename TData, typename THash>
TString TStack<TData, THash>::GetGuardDump(typename NCPU::TStack<TData, THash>::TParrot guard) const {
    return guard == NORWEGIAN_BLUE
            ? "pining for the fjords"
            : "gone to meet his maker";
}

} // namespace NCPU

#endif // NCPU_STACK_HPP