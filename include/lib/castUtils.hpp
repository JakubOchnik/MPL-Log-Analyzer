#include <memory>
#include <line_types/baseLine.hpp>
#include <line_types/internalEntry.hpp>
#include <line_types/logLine.hpp>


class CastUtils
{
public:
    template<typename T>
    static std::shared_ptr<T> downcast(const std::shared_ptr<BaseLine>& entry)
    {
        return std::dynamic_pointer_cast<T>(entry);
    }
};