# C++ Module 06 — C++ Casts

Bu modülün amacı bir değeri dönüştürmekten önce, dönüşümün niyetine uygun C++ cast
operatörünü seçmeyi öğrenmektir.

## Nasıl çalıştırılır?

```sh
make -C ex00 && ./ex00/convert 42.0f
make -C ex01 && ./ex01/serializer
make -C ex02 && ./ex02/identify
```

Her egzersizi temizleyip baştan derlemek için:

```sh
make -C ex00 re
```

## Dört C++ cast türü

| Cast | Ne zaman kullanılır? | Güvenlik modeli |
|---|---|---|
| `static_cast` | Sayısal dönüşüm, bilinen kalıtım dönüşümü | Derleme zamanında tür ilişkisi kontrol edilir |
| `reinterpret_cast` | Pointer/adres temsilini başka bir tür olarak yorumlama | Anlamsal güvenlik sağlamaz |
| `dynamic_cast` | Polymorphic hiyerarşide gerçek türü kontrol etme | Çalışma zamanında kontrol edilir |
| `const_cast` | `const`/`volatile` niteliğini değiştirme | Yalnızca cv niteliğini değiştirir |

C-style cast:

```cpp
int value = (int)number;
```

hangi dönüşüm mekanizmasının istendiğini gizler. C++ cast'i niyeti açıklar:

```cpp
int value = static_cast<int>(number);
```

## ex00 — ScalarConverter ve static_cast

Dosyalar: [`ex00/ScalarConverter.hpp`](ex00/ScalarConverter.hpp),
[`ex00/ScalarConverter.cpp`](ex00/ScalarConverter.cpp)

`ScalarConverter` nesne durumu taşımaz. Bu nedenle kullanıcı tarafından
oluşturulamaz; yalnızca static fonksiyonu çağrılır:

```cpp
ScalarConverter::convert("42.0f");
```

### Dönüşüm hattı

```text
string literal
    |
    +-- char mı?
    +-- int/float/double decimal literal mi?
    +-- nan veya infinity pseudo-literal mı?
    |
    v
sayısal değer
    |
    +-- hedef türün sınır ve gösterilebilirlik kontrolü
    |
    v
static_cast + çıktı
```

Desteklenen temel biçimler:

```text
a          char
42         int
42.5f      float
42.5       double
.5f        float
1e3        double
nan/nanf   özel değer
+inf/-inf  double sonsuzluk
+inff/-inff float sonsuzluk
```

`strtod()` bir başlangıç bölümünü başarıyla okuyup geri kalanını bırakabilir. Bu
yüzden yalnızca dönen değeri kontrol etmek yeterli değildir; `end` pointer'ının
girdinin sonuna ulaştığı da doğrulanır. Örneğin `12abc`, `42f` ve boş string geçerli
decimal literal değildir.

### `impossible` ve `Non displayable`

İkisi farklı durumları anlatır:

- `char: impossible`: Değer `char` aralığında değildir veya `nan/inf` gibi anlamsız
  bir dönüşümdür.
- `char: Non displayable`: Dönüşüm mümkündür fakat ortaya çıkan kontrol karakteri
  ekranda görünmez. `0` bunun örneğidir.

`int` dönüşümünden önce `std::numeric_limits<int>` ile sınırlar kontrol edilir.
Kontrol yapılmadan aralık dışındaki bir floating-point değeri integer'a cast etmek
tanımsız davranışa yol açabilir.

### Hassasiyet

Ondalıklı çıktı her zaman tek basamağa zorlanmaz. Aksi halde `42.4242` yanlışlıkla
`42.4` olurdu. Değer tam sayı görünümündeyse okunabilirlik için `.0` eklenir:

```text
42      -> float: 42.0f, double: 42.0
42.4242 -> float: 42.4242f, double: 42.4242
```

Örnek testler:

```sh
./ex00/convert a
./ex00/convert 0
./ex00/convert -42
./ex00/convert 42.4242
./ex00/convert nan
./ex00/convert +inff
./ex00/convert 2147483648
./ex00/convert 12abc
```

## ex01 — Serializer ve reinterpret_cast

Dosyalar: [`ex01/Serializer.hpp`](ex01/Serializer.hpp),
[`ex01/Serializer.cpp`](ex01/Serializer.cpp), [`ex01/Data.hpp`](ex01/Data.hpp)

Egzersizde pointer değeri `uintptr_t` biçimine ve geri dönüştürülür:

```cpp
uintptr_t raw = reinterpret_cast<uintptr_t>(ptr);
Data *restored = reinterpret_cast<Data *>(raw);
```

`uintptr_t`, mevcut platformda bir pointer değerini taşıyabilecek unsigned integer
türüdür. Testin temel koşulu şudur:

```cpp
Data *original = &data;
Data *restored = Serializer::deserialize(
	Serializer::serialize(original)
);

original == restored; // true
```

Bu işlem nesneyi kopyalamaz ve klasik anlamda serialization yapmaz. Yalnızca adresin
sayısal temsilini geçici olarak saklar. Şu sınırlamalar vardır:

- `Data` nesnesi yaşamaya devam etmelidir.
- Adres aynı process içinde anlamlıdır.
- Program kapatılıp açıldıktan sonra eski adres kullanılamaz.
- `deserialize()` sonucunu yanlış pointer değeriyle dereference etmek tehlikelidir.

Header'daki `struct Data;` forward declaration'ı yeterlidir; fonksiyon imzaları
yalnızca `Data*` kullanır. Nesnenin üyelerine erişilen yerde tam `Data.hpp` tanımı
gerekir.

## ex02 — dynamic_cast ve gerçek tür

Dosyalar: [`ex02/Base.hpp`](ex02/Base.hpp), [`ex02/Base.cpp`](ex02/Base.cpp),
[`ex02/A.hpp`](ex02/A.hpp), [`ex02/B.hpp`](ex02/B.hpp), [`ex02/C.hpp`](ex02/C.hpp)

Hiyerarşi şöyledir:

```text
          Base
       /    |    \
      A     B     C
```

`Base` sınıfının public virtual destructor'ı vardır. Bu hem sınıfı polymorphic yapar
hem de `Base*` üzerinden güvenli silme sağlar.

### Pointer sürümü

```cpp
if (dynamic_cast<A *>(p))
	std::cout << "A" << std::endl;
```

Pointer cast başarısız olursa sonuç `NULL` olur. Bu yüzden `if` içinde doğrudan
kontrol edilebilir.

### Reference sürümü

```cpp
try
{
	(void)dynamic_cast<A &>(p);
	std::cout << "A" << std::endl;
}
catch (const std::exception &)
{
}
```

Reference `NULL` olamayacağı için başarısız cast exception fırlatır. Egzersiz reference
fonksiyonunun içinde pointer kullanılmasını ve `<typeinfo>` include edilmesini yasaklar;
bu nedenle reference cast'leri `try/catch` ile denenir.

| İşlem | Başarısızlık sonucu |
|---|---|
| `dynamic_cast<A *>(p)` | `NULL` |
| `dynamic_cast<A &>(p)` | Exception |

`generate()` heap üzerinde rastgele `A`, `B` veya `C` üretir. Dönen `Base*`, kullanım
sonunda `delete` edilmelidir.

## Hangi cast'i seçmeliyim?

```text
Sayısal/bilinen güvenli dönüşüm mü?
    -> static_cast

Pointer bitlerini/adresini farklı temsil etmek mi?
    -> reinterpret_cast

Polymorphic nesnenin gerçek derived türünü kontrol etmek mi?
    -> dynamic_cast

Yalnızca const/volatile niteliğini değiştirmek mi?
    -> const_cast
```

## Test kontrol listesi

- Pozitif/negatif integer ve sınır değerler
- Ondalıklı float/double ve exponent gösterimi
- Tek karakter girdisi
- `nan`, `nanf`, `+inf`, `-inf`, `+inff`, `-inff`
- Boş veya sonu kirli literal (`12abc`)
- Float ve double overflow
- Serialize/deserialize sonrasında pointer eşitliği
- `Data` üyelerinin restored pointer üzerinden okunması
- `A`, `B`, `C` için pointer ve reference identification
- `Base*` üzerinden `delete`

## Kendini sınamak için

1. `static_cast<int>(42.9)` neden `42` olur?
2. `nan` neden `int` veya `char` yapılamaz?
3. `reinterpret_cast` neden gerçek veri serialization'ı değildir?
4. `dynamic_cast` çalışması için `Base` neden polymorphic olmalıdır?
5. Başarısız pointer ve reference `dynamic_cast` neden farklı davranır?

