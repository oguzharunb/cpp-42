# C++ Module 08 — Templated Containers, Iterators, Algorithms

Bu modül STL container'ları, iterator'ları ve algoritmaları üzerinde çalışır.

## Derleme ve çalıştırma

```sh
make -C ex00 && ./ex00/easyfind
make -C ex01 && ./ex01/span
make -C ex02 && ./ex02/mutantstack
```

Her egzersiz C++98 ile ve `-Wall -Wextra -Werror` bayraklarıyla derlenir.

## ex00 — easyfind

`easyfind`, bir integer container'ında verilen değerin ilk eşleşmesini
`std::find` ile arar. Başarılı olduğunda iterator döndürür; değer bulunamazsa
`EasyFindNotFound` fırlatır. Const ve değiştirilebilir container overload'ları vardır.

## ex01 — Span

`Span`, constructor'da verilen kapasite kadar integer saklar. Tek değer
`addNumber(int)` ile, bir aralık ise iterator alan `addNumber(first, last)` ile eklenir.
Kapasite aşımı `FullSpanException`, ikiden az elemanla span hesabı ise
`NoSpanException` fırlatır.

`shortestSpan()` sıralanmış bir kopyadaki komşuları karşılaştırır;
`longestSpan()` minimum ile maksimum arasındaki farkı hesaplar. Fark hesapları,
`INT_MIN` ve `INT_MAX` birlikte kullanıldığında signed integer overflow oluşturmaz.

## ex02 — MutantStack

`MutantStack`, `std::stack`'ten türeyerek stack API'sini korur ve korunan alttaki
container'a iterator erişimi açar. `begin/end`, const iterator ve reverse iterator
destekleri bulunur. İkinci template parametresiyle `std::deque` dışında uygun bir
container da seçilebilir.
