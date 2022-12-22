1) lista inicjująca <- w konstruktorze
2) jeden shared_ptr
3) jeśli operacja się nie uda to nie powinno dojść do copy on write 
4) komentarze czemu noexception
5) nie można trzymać dwóch kopii klucza, trzeba wskaźnik albo referencje
6) #include <memory>, #include <map>, #include <queue>
7) zrobić guarda
8) jeśli damy referencję we front to powinno dojść do kopii
9) using dać jako prywatne
10) zrobić te metody na funkcji implementującej
11)   kvfifo(kvfifo const &other) : data(other.data) { } /// robić kopię w pewnych przypadkach
12) nie używać new i delete
13) k_iterator ma pozwalac przegladac zbior kluczy, nie listę 