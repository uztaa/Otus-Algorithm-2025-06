#include <vector>
#include <mutex>

/**
 * Потокобезопасный контейнер (для пополнения данными из разных потоков)
 */
template <typename T>
class ThreadSafeVector
{
public:
    ThreadSafeVector() = default;
    
    /**
     * @brief добавление элемента в контейнер
     * @param value элемент для добавления в контейнер
     */
    void push_back(const T &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push_back(value);
    }

    /**
     * @brief получение копии вектора
     * @return копия всех данных в контейнере
     */
    std::vector<T> getCopy() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_;
    }

private:
    mutable std::mutex mutex_;
    std::vector<T> data_;
};
