<h2> Potoki nazwane i nienazwane </h2>
<h1> Zadanie 1 (40%) </h1>

Napisz interpreter poleceń przechowywanych w pliku. Ścieżka do pliku to pierwszy argument wywołania programu.

Polecenia w pliku przechowywane  w następującej postaci: 

Definicja składników:

składnik1 = prog1 arg1 ... argn1 | prog2 arg1 ... argn2 | ... | progN arg1 ... argnN
składnik2 = prog1 arg1 ... argn1 | prog2 arg1 ... argn2 | ... | progM arg1 ... argnM
...
składnikK = prog1 arg1 ... argn1 | prog2 arg1 ... argn2 | ... | progZ arg1 ... argnZ

Wykonanie potoku:

składnik1 | składnik2 | składnikK

Przykład
składnik1 = cat /etc/passwd | wc -l
składnik2 = ls | grep '^a'
składnik3 = grep 11

składnik1 | składnik3 # ⇔ cat /etc/passwd | wc -l | grep 11 
składnik1             # ⇔ cat /etc/passwd | wc -l
składnik2             # ⇔ ls | grep '^a'

- Interpreter powinien uruchomić wszystkie polecenia w osobnych procesach, zapewniając przy użyciu potoków nienazwanych oraz funkcji dup2(),
by wyjście standardowe procesu k było przekierowane do wejścia standardowego procesu (k+1)
- Można założyć ograniczenie górne na ilość obsługiwanych argumentów oraz ilość połączonych komend w pojedynczym poleceniu (co najmniej 3)
- Po uruchomieniu ciągu programów składających się na pojedyncze polecenie (linijkę) interpreter powinien oczekiwać na zakończenie wszystkich tych programów.
Program należy zaimplementować, korzystając z funkcji: pipe(), fork() oraz exec().

<h1> Zadanie 2 (20%) </h1>

Napisać program przyjmujący jeden (nadawca lub data) lub trzy argumenty (<adresEmail> <tytuł> <treść>):
- W przypadku wywołania z jednym argumentem uruchamiany jest (za pomocą popen()) program mail. Program użytkownika ma wypisywać listę e-maili posortowaną alfabetycznie
wg. adresów e-mail (argument nadawca) lub wg. daty otrzymania e-maili (argument data)
- Jeżeli program zostanie wywołany z trzema argumentami, to (za pomocą popen()) uruchamiany jest program mail i za jego pomocą wysyłany jest e-mail do określonego
nadawcy z określonym tematem i treścią\

