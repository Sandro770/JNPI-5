TODO
1) wybrać strukturę
2) impl
3) exception safety
4) przetestować wszystkie możliwe ścieżki rzucanych wyjątków
5) jak mamy zapewniać semantykę copy-on-write jeśli nic nie modyfikujemy
6) czy copy-one-write ma być tylko jak modyfikuje się kolejkę
7*) być może warto uniknąć dublowania kluczy 
8) przetestować czy są tworzone 2 kopie klucza 
9) rollback również kopii jeśli dojdzie do wyjątku
10) czy może powstać wyjątek przy kopiowaniu wartości przy returnie?