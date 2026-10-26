**Pametni digitalni sat sa web sinhronizacijom i alarmom**

-	*Opis:*
  Prikaz stanja vremenskih parametara u zatvorenom prostoru: temperature, vlažnosti, osvetljenja i koncentracije CO2. Na ekranu se prikazuje vreme i meni gde možemo da izaberemo parametar o kom želimo da se informišemo. Ukoliko je prostor veoma osvetljen, sija plava dioda, a ako je mračno, sija crvena, u suprotnom je aktivirana zelena. Ako je temperatura, vlažnost ili koncentracija CO2 iznad definisanog praga, buzzer će se aktivirati na nekoliko sekundi i na ekranu će se pojaviti odgovarajuća poruka.

-	*Potrebni delovi:*
  ESP-32, I2C OLED displej ili LCD displej, buzzer, jumper kablovi, breadboard, DHT11, MQ7, led dioda

-	*Web aplikacija:*
  Ona se ogleda u predstavljanju istorije vrednosti parametara koji su predviđeni za merenje ovim satom. Svi parametri su dostupni direktno u aplikaciji tj. ne koristi se meni već grid. Predviđeno je da imaju i preporuke u vidu oblačenja, kao i podaci o trenutnoj temperaturi napolju.
