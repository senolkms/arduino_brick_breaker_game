# Tuğla Kırma Oyunu

Bu projenin amacı, çeşitli araçlar kullanarak mikrodenetleyici tabanlı bir oyun makinesi
geliştirmektir.
Oyuncu, bir fiziksel palet kontrol cihazını kullanarak bir topu yansıtarak OLED
ekrandaki tuğlaları kırmaya çalışacaktır. Oyuncu, fiziksel bir "palet kontrol cihazı" kullanacak.
Bu cihaz, potansiyometre ile kontrol edilecek. OLED ekran, oyunculara oyun alanını gösterir.
Üst kısımda tuğlalar, alt kısımda ise oyuncunun kontrol ettiği palet ve zıplayan bir top bulunur.
Oyuncunun amacı, topu kullanarak üst taraftaki tuğlaları kırmaktır. Top, oyuncunun kontrol
ettiği paleti kullanarak yukarı doğru yansır. Top, tuğlalara çarptığında, tuğla kaybolur ve
oyuncu “1” puan kazanır. Bu puan 7 segment display ile gösterilmelidir. Top, palet veya
duvarlara çarptığında yönü değişir. Ekranın sağ ve sol taraflarıda bir duvar kabul edilmelidir.
Oyun başarılı bir şekilde biterse topun önceki hızına göre yüzde 20 fazla olacak şekilde bir
sonraki oyun başlamalıdır. Bir sonraki yere geçildiğinde skorbord ekranı sıfırlanmamalıdır.
Oyun, topun alt kısmından düşmesi durumunda sona erer. 
İSTERLER
• İlk açılacak ekranda “Başlat” ve “Çıkış” seçenekleri olacak. Kullanıcı bunlara aşağı ve
yukarı tuşlarıyla gelerek 3.bir tuş ile menüde bulunan seçeneklerden birini
seçebilecektir.
• “Başlat” tuşuyla oyun ekranı açılacak ve oyun başlayacaktır.”Çıkış” seçeneğVnde Vse
“Oyunumuza gösterdiğiniz ilgi için teşekkürler” yazısı yazacaktır.
• Oyun başladıktan sonra kullanıcının kontrol ettVğV palet potansiyometre ile sağ ve sol
hareketleri yaptırılacaktır.
• Palet ile yönlendirme yapılan top çarptığı tuğlaları kıracak ve yön değiştirecektir. Ayrıca
top platformun sağ, sol ve platformun bitişinin üst tarafındaki duvara çarpıncada yön
değiştirmelidir. Yön değiştirmeler fizik kuralları çerçevesinde olmalıdır.(Absürt top
hareketleri olmamalıdır.)
• Her tuğla kırıldığında seven segment displayda gösterilecek skor puanı 1 artmalıdır.
• Her kullanıcı oyuna başladığında 3 canı vardır. Bu canlar led ile gösterilmelidir. Top,
palet ile kurtarılamayıp aşağı düştüğünde oyuncunun 1 canı gider. Daha sonra tekrar
top paletin üstünde olarak oyun başlar.
• Her tuğla kırıldığında yüzde 10 şans ile bir obje düşürecektir. Bu obje kırıldığı tuğladan
sabit olarak aşağı yöne doğru hareket edecektir. Oyuncu bu objeyi alırsa can sayısı 1
artacaktır. Bu eşzamanlı olarak led ile gösterilecektir.(Kullanıcının 2 canı varsa aldığı
canla beraber 3 canı led ile gösterilecektir.)
• Yukarıdaki örnek ekranda olduğu gibi oyunun arka planı siyah, tuğlalar beyaz olacaktır.
Bu 2 objenin rengi ışık sensörüne göre tam tersine değişebilecektir. (Işık açılınca
arkaplan siyah, tuğlalar beyaz olacaktır.)
• Oyundaki tüm tuğlalar bitince 5 saniye ara ekran girecek ve daha sonra diğer bölüm
başlayacaktır. Bu bölümdeki tuğla dizaynının aynı olmaması beklenmektedir. Skorun
kaldığı yerden devam etmesi beklenmektedir. Ayrıca her bölüm geçildiğinde topun hızı
bir önceki tura göre yüzde 20 artacaktır.
![1](https://github.com/senolkms/arduino_brick_breaker_game/assets/80278755/e8d09285-3a58-4a26-a8c1-06aa8b3541b8)
