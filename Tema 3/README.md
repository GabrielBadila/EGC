# Simularea apei (Romanian language)



	Elemente de Grafica pe Calculator
	
	Tema 3 - Simularea apei
	
	Badila Gabriel Alin
	333 CA
	
	
	============================================================================
	
	Utilizare aplicatie
	
	Tastele W, A, S, D, Q, E ajuta la modificarea pozitiei sferei de lumina.
	Tastele UP, DOWN, LEFT, RIGHT ajuta la modificarea sferei din care cad 
	picaturile de apa.
	Tastele Z si X realizeaza efectele de fast-forward si slow-motion.
	
	============================================================================
	
	Detalii de implementare
	
	Arhiva temei contine framework-ul de laborator. In folderul Laborator7 se 
	gasesc fisiere temei: Laborator7.cpp(.h), Grid.cpp(.h), Drop.cpp(.h) si 
	respectiv folderul Shaders ce contine FragmentShader.glsl si VertexShader.glsl.

	In cadrul fisierului "Grid" este realizata constructia planului de 100x100 
	puncte, precum si umplerea lui cu triunghiuri. Este creat pe baza unei 
	functii din fisierul Object2D.cpp din cadrul laboratorului 3.
	
	Fisierul "Drop" este o clasa ce contine informatii cu privire la starea 
	picaturilor: pozitie, viteza, greutate, dimensiune, numar de vieti (intre 5 
	si 7) si o functie care updateaza starea picaturilor.
	
	Fisierul "Laborator7" este fisierul principal al temei aici realizez 
	desenarea sferei de lumina, containerului de picaturi, a picaturilor si,
	desigur, a planului apei cu ajutorul unei functii din fisierul "Grid". In 
	cadrul functiei "Update", pe langa desenarea celor precizate mai sus, ma 
	ocup si de crearea/stergerea picaturilor si modificarea starii picaturilor, 
	dar si de modificarea aplitudinii si a centrului valurilor rezultate din 
	caderea picaturilor. In functia "RenderSimpleMesh" trimit uniformele, 
	proprietatile de iluminare si proprietatile undelor catre VertexShader. 
	Deoarece desenez atat planul, cat si picaturile cu mesha "ShaderLab7" este 
	nevoie sa trimit si un checker, astfel incat, la desenare, picaturile de apa 
	sa nu fie influentate de valurile planului. Tot aici realizez si cateva 
	calcule(frecventa si faza undelor), date ce vor fi trimise mai departe, 
	restul calculelor fiind realizate in VertexShader. In "OnInputUpdate" si in 
	"OnKeyPress" realizez constructia butoanelor de modificarea a pozitiilor 
	elementelor din plan.
	
	In "VertexShader" realizez calcularea varfurilor valurilor, iar in 
	"FragmentShader" realizez iluminarea planului.
	
	============================================================================
	
	Probleme intalnite
	
	- probleme intalnite la umplerea planului, cauzate de complicarea mea de a 
	desena planul fara a folosi cea mai simpla metoda: matrice ce retine 
	valorile varfurilor.
	
	- crearea unei gauri negre in centrul planului, in procesul de realizare a 
	valurilor atunci cand centrul planului era diferit de (0, 0, 0), problema 
	remediata prin setarea pornirii valurilor din punctul de cadere al 
	picaturilor de apa.
