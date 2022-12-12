1) lista inicjująca <- w konstruktorze
2) jeden shared_ptr
3) jeśli operacja się nie uda to nie powinno dojść do copy on write 
4) komentarze czemu noexception
5) nie można trzymać dwóch kopii klucza, trzeba wskaźnik albo referencje