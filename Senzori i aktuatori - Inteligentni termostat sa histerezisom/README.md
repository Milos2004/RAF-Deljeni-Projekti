**Analiza problema (Ground Bounce):**

Tokom testiranja sistema (implementacije1) uočeno je da aktivacija relejnog modula izaziva trenutan i lažan skok očitanih vrednosti sa temperaturnog senzora LM35 za otprilike 7–8 °C. Problem je lociran u deljenoj masi (GND) unutar breadboard-a. Relejni modul pri aktivaciji povlači znatnu struju (~100 mA) kroz unutrašnje bakarne kontakte breadboard-a koji poseduju parazitni otpor. Usled Ohmovog zakona ($U = I \times R$), ovaj prolaz struje podiže potencijal mase na breadboard-u sa idealnih 0V na približno 0.1V. Pošto senzor LM35 referencira svoj izlazni signal u odnosu na tu istu masu, njegov izlazni napon raste za 100 mV, što Arduino interpretira kao stvarni skok temperature.

**Rešenje:**

Problem bi se otklonio razdvajanjem linija mase. Uzemljenje senzora LM35 se uklanja sa breadboard-a i povezuje direktno na ugrađeni GND pin na Arduino ploči, koji ima masivnije vodove i stabilan nivo od 0V. Na ovaj način, strujna petlja releja više ne utiče na referentnu tačku senzora, čime je obezbeđeno potpuno stabilno i tačno očitavanje temperature bez obzira na stanje releja i motora.
