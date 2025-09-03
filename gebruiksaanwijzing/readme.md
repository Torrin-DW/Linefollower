# Gebruiksaanwijzing

### opladen / vervangen batterijen
De Li-Ion batterijen zijn simpel op te laden met een oplaadstation, zoals dit voorbeeld: www.amazon.com.be/-/en/Charger-Li-Ion-Rechargeable-Battery-Lithium/dp/B09CK9KV4D/ref=sr_1_5?adgrpid=143962164122&dib=eyJ2IjoiMSJ9.rPrL_1NQjMf_DiQY8e9RQrQmwuGmKCZo4jmKOc_8ByYiwKy0O_bH7n4O2uHXVa07f0ysyqSGx3ymyvQtLVoGUHZ71zSnryeAok3YPoXdsB54KjVfugKXmqjLFot6JlOUNmSrCOGmQfqKjbGLqLl49LwUwoI4dF5tNI1U5N5bieBsk5qWVfh8jUEqcJ6reXjdbg38OWwQAsdjaPcyDJ2m2QyoWQQc9mkWuKfIvgbojpen8iY1VwBKCKRrAd_2IalhUkQtbAmJRAsBZezaOsYUmvJka_XxigM2VQkCo4QX98M.Qn-__tCTLv8oK1AzXcQI-_diJ32xAVBahbvmXkvmiuw&dib_tag=se&hvadid=628039321330&hvdev=c&hvlocphy=9197508&hvnetw=g&hvqmt=e&hvrand=18212040421412782190&hvtargid=kwd-296191063336&hydadcr=9065_2270883&keywords=18650+oplader&mcid=fb4490431ac43cf8b5389bd95ecf37c1&qid=1756941072&sr=8-5 
Het is ook mogelijk 4 AA-batterijen te gebruiken maar deze zullen rapper leeg zijn.

### draadloze communicatie
#### verbinding maken
Voor de draadloze communicatie word er gebruik gemaakt van een HC-05 bluetooth module. Op de smartphone wordt de app "Serial Bluetooth Terminal" gebruikt om verbinding te maken met de module. Hierin kunnen er commando's gestuurd worden naar de Arduino met behulp van de "SerialCommand" bibliotheek.

#### commando's
debug:  opvragen van de huidige parameters 
start:  de robot begint te rijden
stop:  de robot stopt met rijden
set cycle [µs]: de tijd dat de "loop" functie uitgevoert word.
set power [0..255]:  instellen van de kracht van de motoren
set diff [0..1]:  Door de diff aan te passen zal de auto vertragen of versnellen in de bochten. Hoe hoger diff hoe meer de auto zal versnellen in de bochten.
set kp [0..]:  Dit is de correctie van de fout. Hoe hoger kp is, hoe sterker de auto zich corrigeert om de zwarte lijn in het midden van de sensor te hebben. Als de auto schokt van links naar rechts over de zwarte lijn, is kp te hoog.
set ki [0..]:  hoe langer de fout duurt, hoe sterker de robot zal bijsturen
set kd [0..]:  Dit is de foutverandering. Deze kijkt naar de fout uit de vorige cyclus en gaat dan de fout bijregelen. Was deze fout groot dan wordt er meer gecorrigeerd.
calibrate black:  Kalibreert de waarde van de zwarte lijn. Moet uitgevoerd worden wanneer de sensors over zwart zijn.
calibrate white:  Kalibreert de waarde van het witte papier. Moet uitgevoerd worden wanneer de sensors over wit zijn.

### kalibratie
Om de waardes van de zwarte lijn te kalibreren moet sensor volledig over een zwart deel hangen en moet het commando "calibrate black" uitgevoerd worden. Om de waardes van het witte papier te kalibreren moet dit boven een stuk wit en met het commando "calibrate white".

### settings
De robot rijdt stabiel met volgende parameters:  cycle : 2000; Diff : 0.68; kp : 5; power 50

### start/stop button
Er is een start/stop knop naast de arduino geplaatst. Deze is verbonden met pin 7 die is verbonden met een pull-down weerstand aan de ground. De knop start of stopt de auto aan de hand van een interrupt in het programma.
