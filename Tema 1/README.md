# Geometry Wars (Romanian language)



	Elemente de Grafica pe Calculator
	
	Tema 1 - Geometry Wars
	
	Badila Gabriel Alin
	333 CA
	
	
	============================================================================
	
	Utilizarea aplicatie
	
	Tastele W, A, S, D ajuta la modificarea pozitiei navei utilizatorului.
	Tastele Q si E realizeaza ZoomIn si ZoomOut. Nu recomand folosirea lor 
	deoarece actionarea lor duce la modificarea spatiului logic si astfel la un 
	unghi eronat, astfel player-ul nemaifiind capabil a se roti si a se deplasa 
	corespunzator. Se pot folosi insa pentru testarea aplicatiei.
	Butonul stanga al mouse-ului lanseaza proiectilele catre pozitia cursorului 
	mouse-ului.
	
	============================================================================
	
	Reguli joc
	
	Player-ul este reprezentat de nava de culoare verde, pozitionata la inceput 
	in centrul ecranului.
	Inamicii sunt de trei feluri:
	- inamic albastru: o singura viata, dimensiune mai mare decat a player-ului
	- inamic galben: doua vieti, dimensiune mai mare decat a player-ului
	- inamic rosu: o singura viata, dimensiune mai mica decat a player-ului
	Cand un inamic galben este lovit de proiectilele player-ului isi pierde o 
	viata si devine inamic rosu.
	Inamicii se spameaza random, in pozitie circulara, fata de centrul ecranului, 
	in afara spatiului de joc.
	Player-ul are trei vieti, pozitionate pe ecran in coltul din dreapta, sus.
	Atunci cand player-ul isi pierde cele trei vieti jocul se termina, ecranul 
	inghetata si se coloreaza in rosu.
	
	============================================================================
	
	Detalii de implementare
	
	Arhiva temei contine framework-ul de laborator. In folderul Laborator3 se 
	gasesc fisiere temei: Generator.cpp(.h), Transform2D.h, Ship.cpp(.h), 
	Projectile.cpp(.h).

	Fisierele "Ship" si "Projectile" sunt de fapt doua clase. "Ship" este o clasa 
	ce retine informatii despre coordonatele x, y, unghiul, viteza, tipul si 
	numarul de vieti atat ale player-ului, cat si ale inamicilor. "Projectile" 
	este de asemenea o clasa, ce contine informatii despre coordonatele x si y, 
	unghiul si viteza (constanta, egala cu 4 pentru toate proiectilele) ale 
	fiecarui proiectilele
	
	"Transform2D.h" contine matricele de translatie, scalare si rotatie.
	
	"Generator.cpp" este fisierul de baza al aplicatiei, in el are loc 
	implementarea acestul joc. Acesta contine mai multe metode, dupa cum urmeaza:
	
	Init - initializarea spatiului logic, a caracteristicilor navei player-ului, 
		   a catorva constante de control, dar si a celor doua figuri prin care 
		   vor fi reprezentate: nava player-ului, navele inamice, proiectilele 
		   si nivelul de viata al player-ului.
		   
	CreateMesh - functia de creare a meshelor (obiectelor) din spatiul logic.
	
	VisualizationsTransf2DUnif - crearea matricei de visualizare a spatiului logic.
	
	SetViewportArea - setarea spatiului de vizualizare.
	
	Update - metoda in care, in fiecare frame, se vor updata valorile obiectelor 
			 reprezentate. Se seteaza intai aria de desenare si se compune 
			 matricea de vizualizare. Daca player-ul nu mai are nicio viata 
			 ecranul va ingheta si se va seta cu culoarea rosie, altfel se 
			 realizeaza update-ul valorilor. Nava player-ului se orienteaza dupa 
			 pozitia cursorului mouse-ului, iar inamicii si proiectilele se 
			 orienteaza dupa pozitia player-ului. Inamicii se genereaza random, 
			 la un interval de 2 secunde, interval ce va scadea pana la 0.5 
			 secunde. Este verificata coliziunea player <-> inamic, inamic <-> 
			 proiectil, si se trateaza corespunzator (eliminarea inamicilor si 
			 a proiectilelor implicate, scaderea numarului de vieti al 
			 player-ului).
			 
	DrawScene - sunt realizate diverse translatii, scalari si rotiri prin care 
				vor fi afisate pe ecran: nava player-ului, navele inamicilor, 
				proiectilele si vietile playerului, colorate corespunzatoar.
				
	OnInputUpdate - sunt implementate tastele de miscare si de ZoomIn, respectiv
					ZoomOut ale player-ului.
					
	OnMouseMove - calcularea unui unghi, fara de care nu ar fi posibila rotirea 
				  navei player-ului dupa pozitia cursorului mouse-ului.
				  
	OnMouseBtnPress - este realizata crearea unui proiectil.
	
	============================================================================
	
	Probleme intalnite
	
	- calcularea unghiului de rotatie a navei dupa pozitia cursorului mouse-ului, 
	rezolvata prin setarea unui spatiu logic simetric cu rezolutia ferestrei
	(logicSpace.width = 12.8, logicSpace.height = 7.2).
	
	- realizarea animatiei de scalare de la inamic galben la inamic rosu, care 
	sa dureze circa 250 ns, rezolvata prin utilizarea variabilei deltaTimeSecons.
