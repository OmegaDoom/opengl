#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <exception>
#include <string>

class Texture
{
public:
    class exception : public std::exception
    {
    public:
        exception(std::string name)
            : m_name(std::move(name))
        {
        }

        virtual const char* what() const noexcept
        {
            return m_name.c_str();
        }

    private:
        std::string m_name;
    };

    Texture() = default;
    Texture(const Texture& rhs) = delete;
    Texture& operator = (const Texture& rhs) = delete;
    virtual ~Texture() = 0;
    virtual void Use(unsigned samplerId) const = 0;
};

#endif //TEXTURE_HPP
