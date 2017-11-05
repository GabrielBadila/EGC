# Build and Survive (Romanian language)



	Elemente de Grafica pe Calculator
	
	Tema 2 - Build and Survive
	
	Badila Gabriel Alin
	333 CA
	
	
	============================================================================
	
	Utilizarea aplicatie
	
	Tastele W, A, S, D, Q, E ajuta la modificarea pozitiei player-ului: W - se 
	misca in sus, S - se misca in jos, A - se misca la stanga, D - se misca la 
	dreapta, Q - se misca in jos (intra in pamant, axa Oy), E - se misca in sus 
	(se misca in aer, axa Oy).
	Tastele Z si X realizeaza ZoomIn si ZoomOut. 
	Tastele 1, 2, 3 schimba arma cu care trage player-ul: 1 - pistol, 2 - pusca, 
	3 - grenade launcher.
	Butonul stanga al mouse-ului lanseaza proiectilele catre pozitia cursorului 
	mouse-ului.
	Butonul dreapta al mouse-ului tinut apasat ajuta la miscarea camerei 
	Third-Person.
	
	============================================================================
	
	Reguli joc
	
	Imediat cum incepe jocul, player-ul este pozitionat fix in centrul ecranului 
	(pozitia 0,0,0). Inamicii sunt spamati la pozitia (-12, 0, -12) si se vor 
	misca in forma unui Z pana in pozitia (12, 0, 12), trecand si prin origine. 
	Player-ul este ajutat sa omoare inamicii de catre trei turete pozitionate in 
	jurul originii. Acesta la randul sau este echipat cu trei arme(pistol, pusca, 
	grenade launcher) cu care poate elimina inamici. Jocul se termina atunci 
	cand trei inamici parcurg intregul teren (player-ul are trei vieti si de 
	fiecare data cand un inamic parcurge terenul pierde una). Pe parcursul 
	jocului inamicii vor deveni mai puternici (vor avea mai multe vieti) si se 
	vor spama mai repede (dificultatea jocului va creste).
	
	============================================================================
	
	Detalii de implementare
	
	Arhiva temei contine framework-ul de laborator. In folderul Laborator5 se 
	gasesc fisiere temei: Generator.cpp(.h), LabCamera.h, Enemy.cpp(.h), 
	Life.cpp(.h), Projectile.cpp(.h), Turret.cpp(.h).

	Fisierele "Life", "Enemy", "Projectile" si "Turret" sunt clase ce retin 
	informatii despre vietile player-ului, despre inamici, despre proiectile si 
	despre turetele din joc.
	
	"LabCamera.h" contine miscarile si operatiile uilizate de camera Third-Person.
	
	"Generator.cpp" este fisierul de baza al aplicatiei, in el are loc 
	implementarea acestul joc. Acesta contine mai multe metode, dupa cum urmeaza:
	
	Init - initializarea celor doua camere ce vor fi folosite (spatiu jocului si 
		   minimap-ul), setarea unor variabile si incarcarea meshe-lor ce vor fi 
		   folosite in cadrul jocului.
		   
	FrameStart - setarea spatiului logic pentru joc si pentru minimap.
	
	Update - metoda in care, in fiecare frame, se vor updata valorile obiectelor 
			 reprezentate. Daca player-ul nu mai are nicio viata 
			 ecranul va ingheta si se va seta cu culoarea rosie, altfel se 
			 realizeaza update-ul valorilor. Sunt implementate aici functiile: 
			 de miscare a inamicilor in forma de Z; de stergere a inamicilor in 
			 urma coliziunilor, doar dupa ce au realizat rotatia de cazatura; 
			 de rotire a turetelor catre inamicii ce patrund in range-ul lor si 
			 de creare a proiectilelor; update-ul coordinatelor proiectilelor 
			 pentru a ajunge la inamici si de stergere a proiectilelor ce rateaza 
			 inamicii; de verificare a coliziunii dintre inamici si proiectile si 
			 de eliminare a proiectilelor ce lovesc inamicii; de spamare a 
			 inamicilor la o anumita durata de timp.
			 
	DrawScene - sunt realizate diverse translatii, scalari si rotiri prin care 
				vor fi afisate pe ecran: player-ul, turetele, inamicii, 
				proiectilele si vietile player-ului. Aici este desenat intreg 
				jocul.
				
	DrawSceneMini - functia de desenarea a elementelor minimap-ului. Aici se 
					gasesc aceleasi functii ca in DrawScene, doar ca toate 
					obiectele din teren sunt reprezentate prin box-uri.
					
	CheckRange - apelata pentru a verifica daca proiectilele si inamicii se mai 
				 gasesc in range-ul turetelor.
				
	OnInputUpdate - sunt implementate tastele de miscare si de ZoomIn, respectiv
					ZoomOut ale player-ului.
					
	OnKeyPress - sunt implementate tastele de schimbare a armelor.
					
	OnMouseMove - utilitatea de camera Third-Person si calcularea unghiului 
				  pentru care player-ul ramane mereu cu spatele la camera.
	
	============================================================================
	
	Probleme intalnite
	
	- pozitionarea player-ului cu spatele la camera indiferent de cum misc 
	camera, rezolvata prin rotirea player-ului dupa acel unghi calculat in 
	OnMouseMove.
	
	- pastrarea player-ului pe teren (fara a intra in pamant) atunci cand 
	folosesc tastele W, S, rezolvata prin utilizarea functiei MoveForward 
	din LabCamera.h .
