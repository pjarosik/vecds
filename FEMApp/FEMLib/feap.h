/* Legenda: (x) - zmienna nieistotna       (w) -- zmienna ustawiana w user element */
/*          (r) - zmienna dla user elmt.   (?) -- ni jestem pewien, chyba (x)     */ 



extern "C" {                /*       character*4     o,head               */
  extern struct{            /* Pierwsza linia wydruku                     */
    char o[4], head[20][4]; /*    o = feap  (r)                           */
  } bdata_;                 /* head = tekst pierwszego wiersza plików (r) */
}                           

extern "C" {                           /*  numnp-Number of mesh nodes    (x)   nen-Maximum nodes/element (r) */
  extern struct{		       /*  numel-Number of mesh elements (x) neq-Number active equations (x) */
    int numnp,numel,nummat,nen,neq,ipr;/* nummat-Number of material sets (x) ipr-Real variable precision (x) */
  } cdata_;			       /*       pmanual.pdf str.33                                    */
}

extern "C" {
  extern struct{	      /*                                                                            */
    int n;		      /* dm-Element proportional load    (x) mct-Print counter                   (w)*/
    double dm;		      /*  n-Current element number       (r) iel-User element number             (x)*/
    int ma,mct,iel,nel,pstyp; /* ma-Current element material set (x) nel-Number nodes on current element (r)*/
  } eldata_;		      /*                                                                            */
}

extern "C" {                
  extern struct{     /*  ior - current input logical unit  (w)     iow - current output logical unit (w) */
    int ior,iow,ilg; /*  ilg - solution log file unit number (x)                                         */  
  } iofile_;      
}              

extern "C" {                
  extern struct{            /*EPL(1) - EPL(3) -- Linear momenta (x)  EPL(8) -- Stored energy          (w) */
    double epl[200];	    /*EPL(4) - EPL(6) -- Angular momenta(x)  EPL(9) -- Work by external loads (x) */
    int iepl[200][2],neplts;/*         EPL(7) -- Kinetic energy (x) EPL(10) -- Total energy           (x) */   
  } ptdat6_;                /*          nepls -- (x)                                                      */ 
}              
									    
extern "C" {                
  extern struct{
    int istv, iste, istp;  /*   iste --  liczba map wyprowadzana z elementu   (w) */
  } strnum_;      
}              

extern "C" {                
  extern struct{                /* dla dyfuzji na razie obie zmienne typu (x) potem druga (r)     */ 
    double bpr[3],ctan[3],psil; /* bpr -- Principal stretch (x)   ctan -- Element multipliers (r) */
  } eltran_;      
}              

/*
      integer*8       nph,ner                                      ! int8
      real*8                  erav,j_int   ,jshft
      common /prstrs/ nph,ner,erav,j_int(3),jshft
*/

extern "C" {                
  extern struct{                /* dla dyfuzji na razie obie zmienne typu (x) potem druga (r)     */ 
    long int nph,ner; 	/* bpr -- Principal stretch (x)   ctan -- Element multipliers (r) */
     double erav, j_int[3], jshft;
  } prstrs_;      
}  

/*
	 real*8          hr
      integer               mr
      common          hr(1),mr(1)
*/

extern "C" {                
  extern struct{                /* dla dyfuzji na razie obie zmienne typu (x) potem druga (r)     */ 
	double hr[1];
	int mr[1];
  } comblk_;      
}  
