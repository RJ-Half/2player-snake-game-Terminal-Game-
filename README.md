# Snatch The Number-
Game ini dibuat dalam bahasa C++ dengan memanfaatkan library ncurses. Sedikit mirip dengan game ular lama namun memiliki 2 pemain dimana para pemain saling bersaing untuk berebut makanan dan mencuri bagian tubuh lawan.  
  
Arsitektur Modular:  
  
Game/  
-->Main.cpp > file utama untuk memanggil semua modul yang tersedia  
-->Makefile > Berisi command untuk mengcompile semua modul sekaligus  

--/Object/  
---->Snake.hpp > Berisi struktur dan mekanisme utama dari ular (objek utama yang dimainkan)   
---->Food.hpp  > Berisi fungsi dan aturan yang dimiliki oleh objek Food (makanan)  

--/Mechanics/  
---->Collision.hpp > Mekanisme tabrakan (ketika menabrak tembok dan segmen sendiri)  
---->Snatch.hpp    > Mekanisme pencurian segmen lawan  
---->Growth.hpp    > Mekanisme pertumbuhan (Growth Point dan Pending Growth)  
---->Controls.hpp  > Mekanisme input kedua player terhadap objek snake  
---->Render.hpp    > Mekanisme pengaturan tick dan rendering permainan  
  
--/Platform/  
---->Arena.hpp     > Layout Arena  
---->Menu.hpp      > Menampilkan notifikasi siapa yang menang, menu quit/restart, dan beberapa informasi lainnya(head value, body length, growth Point)  
