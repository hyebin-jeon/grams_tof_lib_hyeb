#include <functional>
#include <iostream>
#include <span>

#include <experimental/reflect>
namespace std_refl = std::experimental::reflect;

template<std_refl::Typed... Ms>
using tuple_from_seq_t = std::tuple<Ms...>;

template<std_refl::Record T>
using MakeMembersReflectionTuple = std_refl::unpack_sequence_t<tuple_from_seq_t, std_refl::get_data_members_t<T>>;

template<std_refl::Typed... Ms>
constexpr auto getReflectionNames(std::tuple<Ms...>)
{
    return std::array{
        std_refl::get_name_v<Ms>...
    };
}

template<std_refl::Typed... Ms>
constexpr auto getReflectionPointers(std::tuple<Ms...>)
{
    return std::tuple{
        std_refl::get_pointer_v<Ms>...
    };
}

template<class S>
struct Printer
{
  const S& obj;
  std::span<const char* const> names;

  template<class... T>
  void operator()(T S::*... members) const
  {
    size_t i = 0;
    ((std::cout << names[i++] << " = " << obj.*members << std::endl), ...);
  }
};

template<class Obj>
void print(const Obj& obj)
{
     using Refl = reflexpr(Obj);
     static constexpr auto kNames          = getReflectionNames(MakeMembersReflectionTuple<Refl>());
     static constexpr auto kMemberPointers = getReflectionPointers(MakeMembersReflectionTuple<Refl>());
     std::apply(Printer<Obj>{ obj, kNames }, kMemberPointers);
}

int main()
{
  struct S {
    int b;
    const char* s;
    const void* v;
  };

  const S obj{ 5, "Hello, World!", "pointer" };
  print(obj);
}