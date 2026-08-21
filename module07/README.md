# C++ Module 07 — Templates

Bu modül generic programming'e giriş yapar. Aynı algoritmayı `int`, `double`,
`std::string` veya kendi sınıfların için tekrar yazmak yerine türü template parametresi
haline getirirsin.

## Nasıl çalıştırılır?

```sh
make -C ex00 && ./ex00/templates
make -C ex01 && ./ex01/iter
make -C ex02 && ./ex02/array
```

Temiz derleme için:

```sh
make -C ex02 re
```

## Template nasıl derlenir?

Normal bir `.cpp` fonksiyonu ayrı olarak derlenebilir. Template ise tek bir fonksiyon
değil, gerektiğinde fonksiyon veya sınıf üreten bir kalıptır:

```cpp
template <typename T>
void swap(T &a, T &b);
```

Compiler çağrıyı gördüğünde gereken specialization'ı üretir:

```cpp
swap(firstInt, secondInt);       // swap<int>
swap(firstString, secondString); // swap<std::string>
```

Bu yüzden template tanımının yalnızca bildirimi değil, gövdesi de kullanıldığı
translation unit tarafından görülmelidir. Template implementasyonları header'da veya
header'ın include ettiği `.tpp` dosyasında bulunur.

## ex00 — Function templates

Dosya: [`ex00/whatever.hpp`](ex00/whatever.hpp)

Üç generic fonksiyon bulunur:

```cpp
template <typename T>
void swap(T &a, T &b);

template <typename T>
const T &min(const T &a, const T &b);

template <typename T>
const T &max(const T &a, const T &b);
```

### Türden beklenen yetenekler

Template her türle otomatik olarak çalışmaz; gövdede kullanılan işlemleri türün
desteklemesi gerekir:

- `swap`: copy construction ve assignment
- `min`: `<` karşılaştırması
- `max`: `>` karşılaştırması

Örneğin `std::string` bu işlemleri desteklediği için aynı fonksiyonlar string ile de
çalışır.

### Eşit değerlerde ikinci argüman

`min` mantığı şöyledir:

```cpp
if (a < b)
	return (a);
return (b);
```

Değerler eşitse `a < b` false olur ve ikinci argüman döner. `max` aynı davranışı
`a > b` kontrolüyle sağlar.

Parametreler `const T&` olduğu için hem const nesneler hem geçici olmayan normal
nesneler gereksiz kopya olmadan kullanılabilir. Sonuç da referans olarak döner.

## ex01 — iter

Dosya: [`ex01/iter.hpp`](ex01/iter.hpp)

`iter`, bir dizinin her elemanı için verilen fonksiyonu çağıran function template'tir:

```cpp
template <typename T>
void iter(T *array, std::size_t length, void (*func)(T &));
```

Kontrol akışı:

```text
array + length + function
          |
          v
for i = 0 .. length - 1
          |
          v
function(array[i])
```

Değiştirilebilir dizi örneği:

```cpp
template <typename T>
void doubleValue(T &value)
{
	value *= 2;
}

iter(numbers, 5, doubleValue<int>);
```

Const dizi için ayrı overload, callback'e `const T&` verir:

```cpp
template <typename T>
void iter(const T *array, std::size_t length,
		  void (*func)(const T &));
```

Bu overload const-correctness'i korur: callback const dizinin elemanlarını
değiştiremez.

## ex02 — Array class template

Dosya: [`ex02/Array.hpp`](ex02/Array.hpp)

`Array<T>` çalışma anında belirlenen uzunlukta, heap üzerinde bir `T` dizisi yönetir:

```cpp
T *_array;
unsigned int _size;
```

### Boş ve boyutlu construction

```cpp
Array<int> empty;      // size = 0, pointer = NULL
Array<int> numbers(5); // beş adet default-initialized int
```

Allocation şu şekilde yapılır:

```cpp
new T[_size]();
```

Sondaki `()` value-initialization sağlar. Temel türlerde `Array<int>(5)` elemanları
başlangıçta `0` olur; sınıf türlerinde default constructor çağrılır.

### Rule of Three ve deep copy

Sınıf heap belleği yönettiği için üç operasyon birlikte düşünülür:

- Copy constructor
- Copy assignment operator
- Destructor

Copy işlemleri yeni bir dizi ayırır ve elemanları tek tek kopyalar:

```text
original._array ---> [10][20][30]

copy._array     ---> [10][20][30]
```

İki pointer farklı allocation'ları gösterir. Bu nedenle `copy[0]` değiştiğinde
`original[0]` değişmez.

Atama operatörü önce yeni diziyi hazırlayıp kopyalar, başarıdan sonra eski diziyi
siler. Allocation veya eleman kopyası exception fırlatırsa hedef nesnenin eski değeri
korunur ve geçici allocation temizlenir.

Self-assignment da kontrol edilir:

```cpp
if (this != &other)
```

### Bounds checking

Her iki subscript overload'u da sınırı kontrol eder:

```cpp
T &operator[](unsigned int index);
const T &operator[](unsigned int index) const;
```

`index >= _size` olduğunda `OutOfBoundsException` fırlatılır. Böylece boş bir array'in
`[0]` elemanına veya beş elemanlı array'in `[5]` elemanına erişilemez.

Const overload sayesinde şu kullanım güvenlidir:

```cpp
const Array<int> values(3);
std::cout << values[0] << std::endl; // okunabilir
// values[0] = 42;                   // derleme hatası
```

## Template deduction

Function template'te compiler çoğu zaman `T` türünü argümandan çıkarır:

```cpp
int a = 1;
int b = 2;
swap(a, b); // T = int
```

Class template'te ise C++98'de tür açıkça yazılır:

```cpp
Array<int> numbers(5);
Array<std::string> words(3);
```

Farklı türde iki argüman tek `T` bekleyen fonksiyona doğrudan verilemez:

```cpp
int a = 1;
double b = 2.0;
// min(a, b); // T hem int hem double olamaz
```

## Test kontrol listesi

- `swap`, `min`, `max` ile integer ve string
- Eşit değerlerde ikinci argümanın dönmesi
- Const değerlerle `min` ve `max`
- `iter` ile eleman okuma ve değiştirme
- Const array üzerinde const callback
- `Array<int>(n)` elemanlarının başlangıç değeri
- Boş array ve `size()`
- Copy constructor sonrası bağımsızlık
- Assignment sonrası bağımsızlık
- Self-assignment
- İlk ve son geçerli index
- `index == size()` ve negatif değerin unsigned'a dönüşmesiyle sınır exception'ı
- `const Array<T>` üzerinden okuma
- Destructor sırasında `delete[]` ve bellek kaçağı kontrolü

## Kendini sınamak için

1. Template implementasyonu neden yalnızca `.cpp` dosyasında bırakılamaz?
2. `min` eşit değerlerde ikinci argümanı nasıl seçiyor?
3. `iter` callback'i neden referans alıyor?
4. Shallow copy yapılırsa iki `Array` nesnesi yok edilirken ne olur?
5. `new T[n]` ile `new T[n]()` arasında temel türler açısından ne fark vardır?
6. Assignment sırasında önce eski belleği silmek exception safety açısından neden
   risklidir?

