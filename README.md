# 2player-snake-game-Terminal-Game-
Game ini dibuat dalam bahasa C++ dengan memanfaatkan library ncurses. Sedikit mirip dengan game ular lama namun memiliki 2 pemain.  
  
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
