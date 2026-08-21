# C++ Module 05 — Repetition and Exceptions

Bu modülün ana konusu C++98'de exception kullanımıdır. Egzersizler ilerledikçe tek
bir sınıftan soyut sınıfa, kalıtıma, polymorphism'e ve factory tasarımına geçilir.

## Nasıl çalıştırılır?

Her egzersiz bağımsız bir programdır:

```sh
make -C ex00 && ./ex00/bureaucrat
make -C ex01 && ./ex01/bureaucrat
make -C ex02 && ./ex02/bureaucrat
make -C ex03 && ./ex03/bureaucrat
```

Temizlemek veya baştan derlemek için:

```sh
make -C ex00 fclean
make -C ex00 re
```

Diğer egzersizler için yalnızca dizin adını değiştirmen yeterlidir.

## 1. Exception mantığı

Bir fonksiyon sözleşmesini yerine getiremiyorsa bir exception `throw` eder. Çağıran
kod bu exception'ı `catch` edebilir:

```cpp
try
{
	Bureaucrat bureaucrat("Alice", 0);
}
catch (const std::exception &e)
{
	std::cout << e.what() << std::endl;
}
```

Burada üç önemli fikir vardır:

- `throw` normal kontrol akışını hemen durdurur.
- Exception, kendisini karşılayan ilk `catch` bloğuna kadar yukarı taşınır.
- `std::exception` taban sınıfı sayesinde farklı exception türleri aynı arayüzle
  yakalanabilir. Hata mesajı `what()` ile alınır.

C++98'de kullanılan imza şöyledir:

```cpp
virtual const char *what() const throw();
```

Sondaki `throw()`, bu fonksiyonun exception fırlatmayacağını söyler.

## ex00 — Bureaucrat

Dosyalar: [`ex00/Bureaucrat.hpp`](ex00/Bureaucrat.hpp),
[`ex00/Bureaucrat.cpp`](ex00/Bureaucrat.cpp)

Bir `Bureaucrat` iki veri taşır:

```cpp
const std::string _name;
int _grade;
```

Grade aralığı `1..150` şeklindedir:

| Grade | Anlamı |
|---:|---|
| 1 | En yüksek yetki |
| 150 | En düşük yetki |
| 0 veya daha küçük | `GradeTooHighException` |
| 151 veya daha büyük | `GradeTooLowException` |

İsim `const` olduğu için nesne oluşturulduktan sonra değiştirilemez. Atama operatörü
bu nedenle yalnızca `_grade` alanını kopyalar.

### Increment neden sayıyı azaltıyor?

Grade yükseldikçe sayısal değer küçülür:

```text
grade 42 --incrementGrade()--> grade 41
grade 42 --decrementGrade()--> grade 43
```

Fonksiyonlar değeri değiştirmeden önce sınırı kontrol eder. Böylece exception
fırlatıldığında nesne hâlâ geçerli durumdadır.

## ex01 — Form

Dosyalar: [`ex01/Form.hpp`](ex01/Form.hpp),
[`ex01/Form.cpp`](ex01/Form.cpp)

`Form` şu bilgileri taşır:

- Değişmeyen form adı
- İmzalanıp imzalanmadığını tutan boolean
- İmzalamak için gereken grade
- Çalıştırmak için gereken grade

Form ilk oluşturulduğunda imzasızdır. `beSigned()` yalnızca bureaucrat yeterli
yetkiye sahipse `_isSigned` değerini `true` yapar:

```text
Bureaucrat::signForm(form)
          |
          v
Form::beSigned(bureaucrat)
          |
          +-- grade yeterli  -> form imzalanır
          +-- grade yetersiz -> GradeTooLowException
```

`Bureaucrat` ve `Form` birbirlerini kullandıkları için header dosyalarında forward
declaration bulunur:

```cpp
class Form;
```

Bir header'da yalnızca `Form&` veya `Form*` kullanılacaksa sınıfın tam tanımına
ihtiyaç yoktur. Üyelerine erişen `.cpp` dosyasında ise `Form.hpp` include edilir.

## ex02 — Abstract AForm ve concrete formlar

Dosyalar: [`ex02/AForm.hpp`](ex02/AForm.hpp),
[`ex02/AForm.cpp`](ex02/AForm.cpp)

`AForm` içindeki saf sanal fonksiyon sınıfı abstract yapar:

```cpp
virtual void executeAction() const = 0;
```

Bu nedenle doğrudan `AForm` nesnesi oluşturulamaz. Ancak `AForm*` ve `AForm&`,
türetilmiş bütün form türlerini ortak bir arayüz üzerinden kullanabilir.

`AForm::execute()` ortak kontrolleri tek yerde yapar:

```text
execute(executor)
    |
    +-- form imzalı mı? -------- hayır -> FormNotSignedException
    |
    +-- executor grade yeterli mi? hayır -> GradeTooLowException
    |
    +-- executeAction() -> gerçek formun polymorphic davranışı
```

Bu yapı bir "template method" yaklaşımıdır: algoritmanın sabit kısmı taban sınıfta,
değişen eylem türetilmiş sınıflardadır.

| Form | İmza grade | Çalıştırma grade | Eylem |
|---|---:|---:|---|
| `ShrubberyCreationForm` | 145 | 137 | `<target>_shrubbery` dosyasına ağaç yazar |
| `RobotomyRequestForm` | 72 | 45 | Delme sesi çıkarır, %50 başarı bildirir |
| `PresidentialPardonForm` | 25 | 5 | Zaphod Beeblebrox'un affını bildirir |

`AForm` destructor'ı `virtual` olmak zorundadır. Böylece aşağıdaki silme işlemi
önce gerçek formun, sonra `AForm`un destructor'ını çağırır:

```cpp
AForm *form = new RobotomyRequestForm("Bender");
delete form;
```

## ex03 — Intern ve factory yaklaşımı

Dosyalar: [`ex03/Intern.hpp`](ex03/Intern.hpp),
[`ex03/Intern.cpp`](ex03/Intern.cpp)

`Intern::makeForm()` bir form adını doğru constructor davranışıyla eşleştirir:

```text
"shrubbery creation"  -> new ShrubberyCreationForm(target)
"robotomy request"    -> new RobotomyRequestForm(target)
"presidential pardon" -> new PresidentialPardonForm(target)
```

Kod uzun bir `if/else if` zinciri yerine ad ve pointer-to-member-function çiftleri
içeren küçük bir tablo kullanır. Eşleşen creator şu şekilde çağrılır:

```cpp
(this->*forms[i].creator)(target)
```

Fonksiyon heap üzerinde bir `AForm` oluşturduğu için sahiplik çağırana geçer:

```cpp
AForm *form = intern.makeForm("robotomy request", "Bender");
// form kullanılır
delete form;
```

Bilinmeyen form adında `NULL` döner; pointer dereference edilmeden önce kontrol
edilmelidir.

## Orthodox Canonical Form

Exception sınıfları dışında sınıflarda şu dört parça bulunur:

```cpp
ClassName();
ClassName(const ClassName &other);
ClassName &operator=(const ClassName &other);
~ClassName();
```

`const` üyeler constructor initializer list ile kurulur ve daha sonra atanamaz.
Bu yüzden `Bureaucrat::operator=` ismi değil grade'i; `AForm::operator=` ise yalnızca
imza durumunu kopyalar.

## Test kontrol listesi

- Grade `1` ve `150` ile nesne oluşturma
- Grade `0` ve `151` için doğru exception
- Grade `1` üzerinde increment ve `150` üzerinde decrement
- Tam sınır grade'iyle form imzalama/çalıştırma
- Bir puan yetersiz grade ile başarısızlık
- İmzalanmamış formu çalıştırma
- Üç concrete formun doğru eylemi
- Shrubbery dosyasının gerçekten oluşması
- Geçerli ve geçersiz `Intern::makeForm()` çağrıları
- `AForm*` üzerinden `delete` sırasında bellek kaçağı olmaması

## Kendini sınamak için

1. `Bureaucrat` constructor'ı grade'i üyeye atadıktan sonra neden kontrol edebilir?
2. `signForm()` neden exception'ı kendi içinde yakalıyor?
3. `executeAction()` neden public çağrılmak yerine normalde `execute()` üzerinden
   kullanılmalıdır?
4. `AForm` destructor'ından `virtual` kaldırılırsa hangi kullanım tehlikeli olur?
5. `Intern::makeForm()` tarafından dönen pointer'ı kim silmelidir?

